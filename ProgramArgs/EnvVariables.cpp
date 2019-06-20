#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    // 读取环境变量
    char* env_var = getenv("ENV_VAR");
    if(!env_var) {
        env_var = "env_var unset";
    }
    cout << "env_var: " << env_var << endl;
    return 0;
}