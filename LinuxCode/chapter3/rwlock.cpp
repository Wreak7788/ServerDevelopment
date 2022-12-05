#include<pthread.h>
#include<unistd.h>
#include<iostream>

int resourceId = 0;
pthread_rwlock_t rwlock;

//读线程
void* ReadFunc(void* arg)
{
  while(1)
  {
    pthread_rwlock_rdlock(&rwlock);
    std::cout << pthread_self() << "\tread data" << std::endl;
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }
  return NULL;
}

void* WriteFunc(void* arg)
{
  while(1)
  {
    pthread_rwlock_wrlock(&rwlock);
    std::cout << pthread_self() << "\twrite data" << std::endl;
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_rwlockattr_t rwattr;
  pthread_rwlockattr_init(&rwattr);
  pthread_rwlock_init(&rwlock, &rwattr);
  pthread_t read, write;
  pthread_create(&read, NULL, &ReadFunc, NULL);
  pthread_create(&write, NULL, &WriteFunc, NULL);
  pthread_join(read, NULL);
  pthread_join(write, NULL);
  pthread_rwlock_destroy(&rwlock);
  return 0;
}
