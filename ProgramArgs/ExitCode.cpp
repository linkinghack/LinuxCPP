#include <iostream>
#include <cstdlib>

using namespace std;

void no_args() {
    exit(100);
}

int main(int argc, char* argv[]) {
    // main() 返回值作为退出码给操作系统
    // 其他函数也可以使用cstdlib/exit()函数返回退出码
    // 执行程序后只用 >echo $? 来获取返回码
    if (argc < 2)
    {
        no_args();
    }
    return 0;
}
