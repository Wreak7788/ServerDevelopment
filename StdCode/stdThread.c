#include<stdio.h>
#include <thread>

void threadFunc1()
{
    while(1)
    {
        printf("Func1 Thread\n");
        // Sleep(1000);
    }
}

void threadFunc2(int a, int b)
{
    while(true){
        printf("Func2 Thread\n");
    }
}

int main()
{
    std::thread t1(threadFunc1);
    std::thread t2(threadFunc2, 1, 2);
    while(1){}
    return 0;
}
