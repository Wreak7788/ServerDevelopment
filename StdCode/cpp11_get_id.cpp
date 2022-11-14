#include<thread>
#include<iostream>
#include<sstream>

void worker_thread_func()
{
  while(true)
  {}
}

int main()
{
  std::thread t(worker_thread_func);
  std::thread::id worker_id = t.get_id();
  std::cout<<"worker thread id:" << worker_id << std::endl;

  //get main thread id
  std::thread::id main_id = std::this_thread::get_id();
  //convert thread::id to ostringstream
  std::ostringstream oss;
  oss << main_id;
  //convert oss to string
  std::string str = oss.str();
  std::cout << "main thread id:" << str << std::endl;
  //convert string to long
  unsigned long long threadid = std::stoull(str);
  std::cout << "main thread id:" << threadid << std::endl;

  while(true)
  {}
  return 0;
}
