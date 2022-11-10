#include<pthread.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>

void* threadFunc(void* arg)
{
  pthread_t* tid1 = (pthread_t*) arg;
  //通过系统调用获取到的线程id为LWP的id，全局唯一，而pthread_t获取的其实是一块内存地址
  int tid2 = syscall(SYS_gettid);
  pthread_t tid3 = pthread_self();

  while(1)
  {
    printf("tid1: %lu, tid2: %d, tid3: %lu\n", *tid1, tid2, tid3);
    sleep(1);
  }
}

int main()
{
  pthread_t tid;
  pthread_create(&tid, NULL, threadFunc, &tid);

  pthread_join(tid, NULL);

  return 0;
}
