/* banker.c */
/**
 * 演示防止死锁方法： 锁定等级。 Locking hierarchy
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N_ACCOUNTS 10
#define N_THREADS 20
#define N_ROUNDS 10000

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))

/**
 * 每个账号初始金额 固定
 */
#define INIT_BALANCE 100


struct account
{
    long balance;
    pthread_mutex_t mtx; // 一个互斥量保护balance变量
} accounts[N_ACCOUNTS];


/**
 * 自定范围随机数发生器 
 */
int rand_range(int N)
{
    return (int) ((double)rand() / ((double)RAND_MAX + 1) * N);
}

/**
 * 交易过程, 并发执行
 */ 
void* disburse(void* arg) {
    size_t i, from, to;
    long payment;

    (void)arg; // 可以指示编译器 arg 变量将不被使用

    for ( i = 0; i < N_ROUNDS; i++)
    {
        // printf(".");
        // 随机选择交易双方账户
        from = rand_range(N_ACCOUNTS);
        do
        {
            to = rand_range(N_ACCOUNTS);
        } while (to == from);
        
        // 检测余额并交易
        // 此处为临界区，容易发生数据不同步问题,  此处使用独占锁来保护数据
        // 仅仅是直接为from和to 上锁，将导致死锁问题
        /**
         * 锁定等级法防止死锁，即获取锁的顺序要有一定规则，
         * 比如此例中的账户 from 和 to 本身就有顺序（下标），可以借助此顺序
         * 获取锁时总是先获取下标大的账户的锁 
         */
        pthread_mutex_lock(&accounts[MAX(from, to)].mtx);
        pthread_mutex_lock(&accounts[MIN(from, to)].mtx);
        if (accounts[from].balance > 0)
        {
            payment = 1 + rand_range(accounts[from].balance);
            accounts[from].balance -= payment;
            accounts[to].balance += payment;
        }
        // 释放锁和获取锁顺序相反
        pthread_mutex_unlock(&accounts[MIN(from, to)].mtx);
        pthread_mutex_unlock(&accounts[MAX(from, to)].mtx);
    }

    return NULL;
}

int main(void)
{
    size_t i;
    long total = 0;
    pthread_t ts[N_THREADS];
    
    srand(time(NULL));

    for ( i = 0; i < N_ACCOUNTS; i++)
    {
        // accounts[i].balance = INIT_BALANCE;
        accounts[i] = (struct account){100, PTHREAD_MUTEX_INITIALIZER};
    }

    printf("银行初始总金额: %d \n", N_ACCOUNTS * INIT_BALANCE);
    
    /**
     * 创建多个线程并开始执行； pthread_create() 是唯一一个引起并发的函数
     */ 
    for ( i = 0; i < N_THREADS; i++)
    {
        pthread_create(&ts[i], NULL, disburse, NULL);
    }

    // 等待所有线程结束
    for ( i = 0; i < N_THREADS; i++)
    {
        pthread_join(ts[i], NULL);
    }
    
    // 计算最终银行总额
    for ( i = 0; i < N_ACCOUNTS; i++)
    {
        total += accounts[i].balance;
    }
    
    printf("最终总金额: %ld \n", total);

    return 0;
}
