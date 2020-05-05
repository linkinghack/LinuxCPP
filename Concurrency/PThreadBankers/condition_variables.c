/* banker.c */
/**
 * 演示Pthread 事件机制(Condition Variable)
 * 
 * pthread_cond_broadcast(*cond) 将通知所有等待cond的线程开始运行(一个cond可能被多个线程监听)
 * pthread_cond_signal(*cond) 将只通知一个正在等待cond的线程，在共享队列数据的时候可能有用
 * 
 * 等待模板：
pthread_mutex_lock(&mutex);   // 加锁保护 PREDICATE 变量的同步
while (!PREDICATE)
	pthread_cond_wait(&cond_var, &mutex);  // 需要一个互斥锁保证wait的原子性, 但是wait没有自己维护互斥锁，需要传递一个（第二参数）;
                                            //  比如在监听一个数据的多个事件的时候，多个cond可以共用一个mutex
pthread_mutex_unlock(&mutex);
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N_ACCOUNTS 50
#define N_THREADS 100
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
 * 为状态变量 stats_xx 维护一个锁和condition variable(条件变量)
 */ 
pthread_mutex_t stats_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t stats_cnd = PTHREAD_COND_INITIALIZER;
int stats_curr = 0, stats_best = 0;

void stats_change(int delta)
{
    pthread_mutex_lock(&stats_mtx);
    stats_curr += delta;
    if (stats_curr > stats_best)
    {
        stats_best = stats_curr;
        // 出现新的最高纪录
        pthread_cond_broadcast(&stats_cnd);
        
    }
    pthread_mutex_unlock(&stats_mtx);
}

/**
 * 用于更新计分板的专用线程
 */ 
void* stats_print(void* arg)
{
    int prev_best;
    (void)arg;

    /**
     * 指示此线程不会被其他线程 JOIN, 
     * 当此线程运行结束后资源会被直接回收
     */ 
    pthread_detach(pthread_self());

    while (1)
    {
        pthread_mutex_lock(&stats_mtx);

        prev_best = stats_best;

        while (prev_best == stats_best)
        {
            pthread_cond_wait(&stats_cnd, &stats_mtx);
        }

        printf("%2d\n", stats_best);
        pthread_mutex_unlock(&stats_mtx);
    }
    fflush(stdout);
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

        stats_change(1); // 正式进入交易，统计交易数量

        if (accounts[from].balance > 0)
        {
            payment = 1 + rand_range(accounts[from].balance);
            accounts[from].balance -= payment;
            accounts[to].balance += payment;
        }

        stats_change(-1); // 交易完成，更新交易数量
        
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
    clock_t start, end;
    
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
    start = clock();
    for ( i = 0; i < N_THREADS; i++)
    {
        pthread_create(&ts[i], NULL, disburse, NULL);
    }

    /**
     * 开启一个统计线程，统计当前正在进行交易的线程数量
     * 即 正在执行的disburse()数量
     */ 
    pthread_t stats;
    pthread_create(&stats, NULL, stats_print, NULL);

    // 等待所有线程结束
    for ( i = 0; i < N_THREADS; i++)
    {
        pthread_join(ts[i], NULL);
    }
    end = clock();
    printf("耗时：%lf \n", (double)(end - start));
    // 计算最终银行总额
    for ( i = 0; i < N_ACCOUNTS; i++)
    {
        total += accounts[i].balance;
    }
    
    printf("最终总金额: %ld \n", total);

    return 0;
}
