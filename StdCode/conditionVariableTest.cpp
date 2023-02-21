#include<thread>
#include<mutex>
#include<condition_variable>
#include<list>
#include<iostream>

class Task
{
public:
    Task(int taskId)
    {
        this->taskId = taskId;
    };
    void doTask()
    {
        std::cout << "handle a task, taskId: " << taskId << ", ThreadId:"
        << std::this_thread::get_id() << std::endl;
    }

private:
    int taskId;
};

std::mutex      mymutex;
std::list<Task*> tasks;

std::condition_variable myCv;

void* consumer_thread()
{
    Task* pTask = NULL;
    while(true)
    {
        //减小guard锁范围
        {
            std::unique_lock<std::mutex> gurad(mymutex);
            while(tasks.empty())
            {
                myCv.wait(gurad);
            }
            pTask = tasks.front();
            tasks.pop_front();
        }
        if(pTask == NULL)
            continue;
        pTask->doTask();
        delete pTask;
        pTask = NULL;
    }
}

void* producer_thread()
{
    int taskId = 0;
    Task* pTask = NULL;
    while(true)
    {
        pTask = new Task(taskId);
        {
            std::lock_guard<std::mutex> guard(mymutex);
            tasks.push_back(pTask);
            std::cout << "produce a task, taskId:" << taskId << ", threadId:"
            <<std::this_thread::get_id() << std::endl;
        }
        myCv.notify_one();
        taskId++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return NULL;
}

int main()
{
    std::thread consumer1(consumer_thread);
    std::thread consumer2(consumer_thread);
    std::thread consumer3(consumer_thread);
    std::thread consumer4(consumer_thread);
    std::thread consumer5(consumer_thread);

    std::thread producer(producer_thread);
    producer.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();
    consumer5.join();

    return 0;
}