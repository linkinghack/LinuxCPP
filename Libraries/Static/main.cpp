#include <iostream>

int test_func1(int);
int test_func2(int);


/**
 * Static Library
 * g++ -c func1.cpp func2.cpp (compile library->func1.o, func2.o)
 * ar -cvq libcppfunc.a func1.o func2.o  (create static library libcppfunc.a)
 * ar -t libcppfunc.a (list files in library)
 * g++ -o mainexe main.cpp -L$(pwd) -lcppfunc (compile with static library   automatically find 'libcppfunc')
 * 
 * */
int main() {
    int raw = 123;
    std::cout<<"Raw: "<<raw<<std::endl;
    std::cout<<"func1: "<<test_func1(raw)<<std::endl;
    std::cout<<"func2: "<<test_func2(raw)<<std::endl;
    return 0;
}