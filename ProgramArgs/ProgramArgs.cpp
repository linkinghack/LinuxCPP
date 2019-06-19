#include <iostream>
#include <getopt.h> // linux 提供的分析参数列表工具
using namespace std;

void print_all_args(int argc, char *argv[]);
void analyse_args_with_getopt(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    // 1. 测试两个参数列表相关参数基本用法
    print_all_args(argc, argv);

    // 2. 测试getopt.h 提供的参数分析工具
    analyse_args_with_getopt(argc, argv);
    return 0;
}

void print_all_args(int argc, char *argv[])
{
    cout << "program name: " << argv[0] << endl;
    if (argc > 1)
    {
        cout << "args: " << endl;
        for (int i = 1; i < argc; i++)
        {
            cout << argv[i] << endl;
        }
    }
    else
    {
        cout << "There is no argument." << endl;
    }
}

void analyse_args_with_getopt(int argc, char *argv[])
{
    // 短参数名定义列表，每个参数一个字符，使用冒号: 添加附加参数定义
    const char *const shrot_opts = "ho:v";
    const struct option long_opts[] = 
    {
        {"help", 0, NULL, 'h'},
        {"output", 1, NULL, 'o'},
        {"verbose", 0, NULL, 'v'},
        {NULL, 0, NULL, 0}  // 最后封装一个空选项来表示长选项数组结束
    };

    const char* output_filename = NULL; // 输出文件名
    int verbose = 0; // 是否显示复杂信息
    char* program_name = argv[0]; // 保存程序名


    // 循环调用getopt_long(), 读取所有参数
    // 最后一个参数传一个整型指针，用来保存本次读取的参数在argv[]中的下标
    int opt = getopt_long(argc, argv, shrot_opts, long_opts, NULL);
    while (opt != -1)
    {
        switch (opt)
        {
        case 'h':
            cout << "这是帮助信息" << endl;
            break;
        case 'o':
            cout << "outputfile: " << optarg << endl;; // optarg将保存附加参数内容
            break;
        case 'v':
            cout << "verbose set" << endl;
            break;
        case -1:
            break;
        default:
            break;
        }
        opt = getopt_long(argc, argv, shrot_opts, long_opts, NULL);
    }
}
