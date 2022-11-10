
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>

void* threadFunc(void* arg)
{
	while(1)
	{
		sleep(1);
		printf("I am child thread\n");
	}
	return NULL;
}

int main()
{
  pthread_t threadId;
  pthread_create(&threadId, NULL, threadFunc, NULL);

  while(1)
  {
  }
  return 0;
}
