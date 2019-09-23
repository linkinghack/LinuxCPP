#include <iostream>

int test_func1(int);
int test_func2(int);


/**
 * Dynamically Linked Library
 *  gcc -Wall -fPIC -c *.c
 *  gcc -shared -Wl,-soname,libctest.so.1 -o libctest.so.1.0   *.o
 * 
 * Use DLL
 *  g++  -Wall -I/path/to/include-files  -L/path/to/dll main.cpp -lcppfunc -o main
 * 
 * Install Dynamically Linked Library
 *  mv libctest.so.1.0 /opt/lib
 *  ln -sf /opt/lib/libcppfunc.so.1.0 /opt/lib/libcppfunc.so.1  
 *  ln -sf /opt/lib/libcppfunc.so.1 /opt/lib/libcppfunc.so   
 *  Soft link of so.1 and so.1.0 both have to be created.
 * 
 * Or use Environment Variable
 *  export LD_LIBRARY_PATH=/path/to/dlls:$LD_LIBRARY_PATH
 * */
int main() {
    int raw = 123;
    std::cout<<"Raw: "<<raw<<std::endl;
    std::cout<<"func1: "<<test_func1(raw)<<std::endl;
    std::cout<<"func2: "<<test_func2(raw)<<std::endl;
    return 0;
}