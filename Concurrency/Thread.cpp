/**
 *This code file shows an example of C++11 Thread
 * to run threads.
 * To compile : 
 * g++ -std=c++11 Thread.cpp -lpthread
*/

#include <iostream>
#include <thread>
#include <chrono>

/**
 * NumberPrinter will keep printing the int data which the pointer pointing to 
 *  until it reach some threshold. Runs as a thread.
 * id: specify the thread id
 * data: the pointer of data
 * threshold: the largest data to print (then exit)
 * 
*/
void NumberPrinter(int id, int* data, int threshold) {
    while (*data < 1000)
    {
        std::cout<<"Thread ID: "<< id <<" Data: "<< *data << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    // Create two threads, read and print the number asynchronously.
    int data_number = 0;

    // Thread runs as it is created.
    std::thread t1(NumberPrinter, 1, &data_number, 1000);
    std::thread t2(NumberPrinter, 2, &data_number, 1500);

    while (data_number < 1600)
    {
        data_number++;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // Wait threads to exit.
    t1.join();
    t2.join();
    
    return 0;
}
