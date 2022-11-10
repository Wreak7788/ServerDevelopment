#include<stdio.h>
#include <thread>

void threadFunc1()
{
    while(1)
    {
        printf("Func1 Thread\n");
    }
}

void threadFunc2(int a, int b)
{
    while(true){
        printf("Func2 Thread\n");
    }
}

void threadFunc3()
{
  while(true)
    printf("Func3 Thread\n");
}

void func()
{
  std::thread t(threadFunc3);
  //使用detach将线程对象与线程分离，否则局部变量t在func结束后释放将导致程序崩溃
  t.detach();
}
int main()
{
    std::thread t1(threadFunc1);
    std::thread t2(threadFunc2, 1, 2);
    func();
    while(1){}
    return 0;
}
