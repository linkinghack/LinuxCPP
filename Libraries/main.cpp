#include <iostream>

int test_func1(int);
int test_func2(int);


/**
 * Static Library
 * g++ -c func1.cpp func2.cpp (compile library->func1.o, func2.o)
 * ar -cvq libcppfunc.a func1.o func2.o  (create static library libcppfunc.a)
 * ar -t libcppfunc.a (list files in library)
 * g++ -o mainexe main.cpp -L$(pwd) -lcppfunc (compile with static library)
 * 
 * Dynamically Linked Library
 *  gcc -Wall -fPIC -c *.c
 *  gcc -shared -Wl,-soname,libctest.so.1 -o libctest.so.1.0   *.o
 * 
 * Install Dynamically Linked Library
 *  mv libctest.so.1.0 /opt/lib
 *  ln -sf /opt/lib/libcppfunc.so.1.0 /opt/lib/libcppfunc.so.1
 *  ln -sf /opt/lib/libcppfunc.so.1 /opt/lib/libcppfunc.so
 * 
 * Use DLL
 *  g++  -Wall -I/path/to/include-files  -L/path/to/dll main.cpp -lcppfunc -o main
 * */
int main() {
    int raw = 123;
    std::cout<<"Raw: "<<raw<<std::endl;
    std::cout<<"func1: "<<test_func1(raw)<<std::endl;
    std::cout<<"func2: "<<test_func2(raw)<<std::endl;
    return 0;
}