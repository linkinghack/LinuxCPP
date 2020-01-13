#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
int main() {
    int fd1 = open("test_file.txt", O_RDONLY, 0);
    printf("fd1: %d\n", fd1);
    int close1 = close (fd1);
    printf("fd1 closed: %d\n", close1);

    /**
     * 返回的文件描述符总是从当前没有在使用的最小描述符开始
     * 0，1，2分别时 标准输入、标准输出、标准错误，
     * 所以一般从 3 开始
     * 
     * close fd1 之后， fd2 重新获得了'3' 描述符
     */
    int fd2 = open("hello.txt", O_RDONLY, 0);
    printf("fd2: %d\n", fd2);
    int close2 = close (fd2);
    printf("fd2 closed: %d\n", close2);
    
    return 0;
}