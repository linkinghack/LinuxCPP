#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

int main() {

    char c = 0;
    /**
     * unistd.h 中定义的 STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO分别是 0, 1, 2
     * 
     * unistd.h 中定义了UNIX I/O 函数 read(), write()
     * 
     * read() 返回实际读取到的字节数，-1代表EOF
     * 
     * read() 读取到的字节数比要求的少，则可能：
     *  * 遇到了EOF
     *  * 从终端读取的一行文本（每次只能按行读取）不足要求大小
     *  * 从socket中读取时延迟较高或者内部缓冲约束无法满足要求的大小
     */
    while (read(STDIN_FILENO, &c, sizeof(char)))
    {
        write(STDOUT_FILENO, &c, sizeof(char));
    }
    
    printf("EOF reached.\n");
    
    return 0;
}