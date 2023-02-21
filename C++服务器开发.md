
# 1 必知必会
## 1.1 C++新特性

### 1.1.1 C++11统一的类成员初始化语法与std::initializer_list<T>

- C++11中可以使用大括号语法初始化数组类型的成员变量

```C++
class A{
    public:
    A():arr{0,1,2,3}
    {

    }

    public:
    int arr[4];
}
```
- C++11中可以使用花括号对任意类型的变量初始化，而且不用是static类型

```C++
class A{
    public:
        bool ma{true};
        int mb{1000};
        std::string mc{"hello world"};
}
```

使用C++11新引入的对象std::initializer_list\<T\>在自定义类中支持花括号语法:
```C++

#include <initializer_list>

class A{
public:
    A(std::initializer_list<int> integers){
        m_vecIntegers.insert(m_vecIntegers.end(), integers.begin(), integers.end());
    }
    void append(std::initializer_list<int> integers){
        m_vecIntegers.insert(m_vecIntegers.end(), integers.begin(), integers.end());
    }
private:
    std::vector<int> m_vecIntegers;
}

int main(){
    A a{1,2,3,4};
    a.append({5,6,7});
}
```

std::initializer_list\<T\>还提供了三个成员函数:
- size_type size() const;   //返回列表中的元素个数
- const T* begin() const;   //返回第一个元素指针
- const T* end() const; //返回最后一个元素的下一个元素位置，代表结束

### 1.1.2 C++17注解标签(attributes)

 使用注解标签的语法:
 > \[[attribute]] types/functions/enums/etc

 在C++17之前不能用于修饰namespace和enumerator，在C++17标准中这个限制被取消。

 **C++98/03的enumeration和C++11的enumerator**
 > 前者是不限定作用域的枚举，后者为限定作用域的枚举

 ```c++
//enumeration
enum Color{
    black;
    white;
    blue
};

bool white = true; //无法编译通过

// enumerator
enum class Color{
    black;
    white;
    blue
};

bool white = true; //可以通过编译
//enumerator的枚举值对外部不可见，需通过Color::white引用。
 ```

 - \[[noreturn]] 标签， C++11引入，告诉编译器某函数没有返回值

```C++
[[noreturn]] void terminate();
```
- \[[deprecated("message")]]标签，C++14引入，表示一个函数或类型已经被弃用，与Java@Deprecated注解相同作用
- \[[fallthrough]]标签，C++17引入，用于switch-case语句，在某个case分支执行完毕后没有break会引发警告。

```C++
switch(type){
    case 1:
        func1();
        //没有break会引发编译器警告
    case 2:
        func2();
        [[fallthrough]]; //说明是故意不加break，不会引发警告
}
        
```

- \[[nodiscard]]标签，C++17引入，一般用于修饰函数，告诉调用者必须关注返回值，不能丢弃该函数返回值。

```C++
[[nodiscard]] int func(int a, int b){
    //...
    return a+b;
}
```
- \[[maybe_unused]]标签，C++17引入，消除未使用的函数或变量的警告。

```C++
int func(int a, [[maybe_unused]] int b){
    //...
}
```
### 1.1.3 final/override关键字和=default/=delete语法(C++11)
- final修饰类，这个类不允许被继承。需要写在类名后
```C++
class A final{

};
class B:A{  //编译错误，类A不允许被继承

};
```

- override:子类重写父类的方法

```C++
class A{

protected:
    virtual void func(int a, int b);
};

class B:A{
protected:
    virtual void func(int a, int b) override
    {

    }
};
```

- =default语法:让编译器自动生成构造/析构/拷贝构造函数和operator=这几类函数

```C++
class A{
public:
    A() = default;
    ~A() = default;
}
```
- =delete语法:禁止编译器生成构造/析构/拷贝构造函数和operator=这几类函数
```C++
class A{
public:
    A() = default;
    ~A() = default;
    // 禁止拷贝构造函数
    A(const A& a) = delete; 
    A& operator=(const A& a) = delete;
}
```

### 1.1.4 语法糖（C++11）
- auto自动类型推导
- for-each循环

> 1. for-each中的迭代器类型与数组或集合中元素类型完全一致，老式迭代器类型为指针，所以以上代码可以直接iter.second引用键值。
> 2. 在for-each语法中，对于复杂类型，迭代器是原始数据的拷贝，而不是引用，为减少不必要的拷贝构造函数的调用开销，要尽量使用引用
> 
```C++
std::map<std::string, std::string> seasons;
seasons["spring"] = "111";
//...
for(auto iter: seasons){        //拷贝
    std::cout<< iter.second << std::endl;
}
for(auto& iter: seasons){       //引用
    std::cout<< iter.second << std::endl;
}
```

**如何让自定义对象支持for-each语法**
对象至少实现如下两个方法:
- Iterator begin();
- Iterator end();

以上的Iterator是自定义数据类型的迭代子类型，必须支持如下三种操作:
- operator++,在自增之后返回下一个迭代子位置
- operator!=，判断不等
- operator*，解引用

### 1.1.5 C++17结构化绑定
```C++
auto [iterator, inserted] = someMap.insert(...);
double myArray[3] = {1,2,3};
auto [a, b, c] = myarray;
struct Point{
    double x;
    double y;
};
Point p (10.0, 20.0);
auto [myX, myY] = p;
//以下语法无法编译通过
// constexpr auto [myX, myY] = p;
// static auto [myX, myY] = p;
```
> 绑定名称是绑定目标的一份拷贝，建议使用引用，如不需要修改建议使用const引用。无法使用constexpr或static修饰。

### 1.1.5 stl容器新增方法
- 原位构造与容器的emplace系列函数(C++11)
- std::map的try_emplace与insert_or_assigh方法(C++17)

### 1.1.6 C++智能指针

- auto_ptr:C++98，在C++11被弃用，当复制一个auto_ptr对象时，原auto_ptr对象所持有的堆空间会被转移给新的auto_ptr对象。**应该避免在容器中使用。**

- unique_ptr:禁止了复制语义，对持有的堆内存具有唯一拥有权。
  - 自定义资源释放函数:std::unique_ptr\<T, DeletorFuncPtr>，DeletorFuncPtr是一个函数指针。

```C++
auto deletor = [](Socket * psocket){
    psocket->close();
    delete psocket;
}
std::unique_ptr<Socket, void(*)(Socket * psocket)> spSocket(new Socket(), deletor);
//or
std::unique_ptr<Socket, decltype(deletor)> spSocket(new Socket(), deletor);
```
- shared_ptr:可以在多个shared_ptr之间共享，内存管理方法同Java的引用计数相似。
  - 不应该共享栈对象的this指针给智能指针对象，**智能指针设计的最初目的就是管理堆对象**
  - 注意循环引用问题，一个资源的生命周期可以交给一个智能指针对象管理，但是该智能指针的生命周期不可以在交给该资源管理。

```C++
//在实际开发中有时需要在类中包裹返回当前对象的一个shared_ptr给外部使用，
//有此要求的类只需要继承自std::enable_shared_from_this<T>模版对象即可
#include<memory>
class A:public std::enbale_shared_from_this<A>{
public:
    A()=default;
    ~A()=default;
    std::shared_ptr<A> getSelf(){
        return shared_from_this();
    }
}
```
- weak_ptr:不控制资源生命周期的智能指针，是对对对象的一种弱引用，只提供了对其管理的资源的一个访问手段，引入他的目的是为了协助shared_ptr工作。**他的构造和析构不会引起引用计数的变化。** 经典例子为订阅者或观察者模式。

```C++
std::shared_ptr<int> sp1(new int(123));

std::weak_ptr<int> sp2(sp1);

//可以通过expired方法来判断引用的资源是否有效，false代表有效
if(sp2.expired())
    return;
//在有效时可以通过lock方法得到一个shared_ptr对象
std::shared_ptr<int> s = sp2.lock();
//不可以通过weak_ptr直接对资源进行操作，因为没有重写opreator->和operator*，也不能通过weak_ptr直接判断资源是否存在，因为没有重写operator bool()
```

# 第二章 GDB调试知识

## 2.1 gdb常用指令汇总

- 启动调试:
  - gdb filename
  - gdb attach pid
  - gdb filename corename
- run(r):启动程序
- continue(c):继续运行程序
- break(b):添加断点，
  - break functionName
  - break lineNo
  - break filename:lineNo
- tbreak:添加临时断点，触发一次后失效
- backtrace(bt):用于查看当前所在线程调用堆栈
- frame(f):切换堆栈
  - frame 堆栈编号
- info break(info b):查看断点表
  - disable 断点编号:禁用某个断点，不加编号禁用所有
  - enable 断点编号:启用某个断点，不加编号启用所有
  - delete 断点编号:删除某个断点，不加编号删除素有断点
- list(l) [+|-]:查看当前断点附近代码
- print(p):打印变量值，还可以修改变量值
  - print /format variable 格式化输出
  - /o 八进制
  - /x 十六进制
  - /d 十进制
  - /u 无符号十进制
  - /t 二进制
  - /f 浮点值
  - /a 内存地址格式
  - /i 指令格式
  - /s 字符串格式
  - /z 十六进制左侧补零
- ptype:打印变量类型，可以打印结构体变量的详细信息
- infothreads:查看进程当前的所有线程信息
  - thread 线程编号:切换到某个线程
- info args:查看当前函数参数值
- next(n):单步跳过
- step(s):单步进入
- until(u):让程序运行到指定的行停下
- return:结束当前函数调用并返回指定值，未执行的代码不会执行
- finish:执行完当前函数并返回
- jump(j) \<location>:跳转到程序指定行号或函数的地址继续执行
- disassemble:反汇编命令，默认格式为AT&T格式
- set args:传递命令行参数，如果参数之间有空格，可以用双引号引起来
- show args:显示命令行参数
- watch [变量名|内存地址]:监视变量或一段内存，当值发生变化时会中断
- display:监视变量或内存的值，中断时自动打印
- dir:重新定向程序与源码的位置关系

## 2.2 多线程程序调试

- set scheduler-locking [on|step|off]
  - on:锁定当前线程，执行(next,step,until,finish,return)暂停其他线程
  - step:仅当使用next/step时锁定当前线程，使用其他命令时其他线程仍有机会执行
  - off:释放当前锁定线程

## 2.3 多进程程序调试

1. 先开一个shell调试父进程，在父进程fork出子进程后再用其他shell使用gdb attach命令调试子进程
2. 使用**set follow-fork [child|parent]** 设置一个进程fork出新的子进程时，gdb是继续调试父进程还是子进程，默认为父进程。可以通过show follow-fork mode 查看当前值。

## 2.4 其他

**将print输出的字符串或字符数组完成显示**
- set print element 0:

**让被gdb调试的程序接收信号**
- 在gdb中使用signal函数手动向程序发送信号(signal SIGINT)
- 改变gdb信号处理的设置，通过handle SIGINT nostop print告诉gdb在接收到SIGINT时不要停止，并把该信号传递给目标调试程序。

**条件断点**
- break [lineNo] if [condition]
- 先使用普通断点，然后通过**condition 断点编号 断点触发条件**来添加

**自定义gdb调试命令**

在/root/.gdbinit或~/.gdbinit。

**gdb tui图形化界面**
- gdbtui -q 需要调试的程序名
- Ctrl+X+A

**gdb升级版———cgdb**

**VisualGDB:远程调试Linux程序**

# 第三章 多线程编程与资源同步

## 3.1 基本原理

> 在windows系统中，主线程退出时，支线程即使没执行完逻辑也会退出。在（某些）Linux系统中，如果主线程退出，支线程一般不会收到影响，但是会变成僵尸进程。在实际开发中应该避免这类情况。

> 一个线程崩溃是否会对其他线程产生影响？一般来说，每个线程都是独立执行的单位，有自己的上下文堆栈，一个线程崩溃不会影响其他线程。但在通常情况下一个线程崩溃也会导致整个进程退出，例如在Linux系统中会产生一个Segment Fault的段错误，操作系统对这个信号的默认处理就是结束进程。

## 3.2 线程基本操作

### 3.2.1 创建线程

**Linux平台**
```C
#include <pthread.h>
//thread为输出参数，可以得到线程id；
//attr指定线程属性，NULL表示使用默认属性；
//start_routine指定线程函数，调用方式必须是_cdecl，这是在C/C++中定义全局函数时默认的调用方式
//arg用于在创建线程是将某个参数传入线程函数中
//成功返回0，失败返回错误码
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

//不指定函数调用方式，默认为__cdecl
void* start_routine(void* args){
    //..
}

//显示指定函数调用方式
void* __cdecl start_routine(void* args)
{
    //..
}
```

例子：编译时需要加-pthread
```C
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
```


**windows平台**
> windows中使用句柄(HANDLE类型)来管理线程对象，本质上是内核句柄表中的索引值。
```C
HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes,  // pointer to security attributes, usually NULL
  DWORD dwStackSize,                         // initial thread stack size, usually 0 to use default value
  LPTHREAD_START_ROUTINE lpStartAddress,     // pointer to thread function, 调用方式必须是__stdcall, WINAPI和CALLBACK两个宏值为__stdcall
  LPVOID lpParameter,                        // argument for new thread, typedef void* LPVOID;

  //对于dwCreationFlags，一般设置为0，代表创建好线程后立即启动；
  //也可以将其设置为CREATE_SUSPENDED(4)，在需要时通过ResumeThread这个API运行。
  DWORD dwCreationFlags,                     // creation flags
  LPDWORD lpThreadId                         // pointer to receive thread ID
);
```

例子：
```C
#include<Windows.h>
#include<stdio.h>

DWORD WINAPI ThreadProc(LPVOID lpParameters){
    while(TRUE){
        Sleep(1000);
        printf("I am child thread\n");
    }
    return 0;
}

int main()
{
    DWORD dwThreadId;
    HANDLE hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwThreadId);
    if(hThread == NULL){
        printf("Failed to CreateThread.\n");
        return -1;
    }

    while(1){}
    return 0;
}
```

在实际开发中推荐使用CRT提供的线程创建函数：_beginthreadex，位于process.h头文件。
```C
uintptr_t _beginthreadex( // NATIVE CODE
   void *security,
   unsigned stack_size,
   unsigned ( __stdcall *start_address )( void * ),
   void *arglist,
   unsigned initflag,
   unsigned *thrdaddr
);
```

**C++11提供的std::thread类**
C++11新标准引入了std::thread(头文件thread)，使用这个类可以将任意签名形式的函数作为线程函数。std::thread对象在线程函数运行期间必须是有效的，也可以通过detach方法让线程与线程对象脱离关系(**不推荐这么做**)。

### 3.2.2 获取线程id

获取当前线程id：
- phtread_t pthread_self();     //Linux
- DWORD GetCurrentThreadId();   //win
- std::this_thread.get_id();    //C++11

查看一个进程的线程数量：
- pstack pid    //Linux，程序必须有调试符号
- 任务管理器    //win
- syscall(SYS_gettid)   //Linux,系统调用

### 3.2.3 等待线程结束

**Linux**
- int pthread_join(pthread_t thread, void **retval);

```C++
#include<stdio.h>
#include<string.h>
#include<pthread.h>

#define TIME_FILENAME "time.txt"

void* fileThreadFunc(void* arg)
{
  time_t now = time(NULL);
  struct tm* t = localtime(&now);
  char timeStr[32] = {0};
  snprintf(timeStr, 32, "%04d/%02d/%02d %02d:%02d:%02d",
      t->tm_year+1900,
      t->tm_mon+1,
      t->tm_mday,
      t->tm_hour,
      t->tm_min,
      t->tm_sec);
  FILE* fp = fopen(TIME_FILENAME, "w");
  if(fp == NULL)
  {
    printf("Failed to create time.txt.\n");
    return NULL;
  }
  size_t sizeToWrite = strlen(timeStr)+1;
  size_t ret = fwrite(timeStr, 1, sizeToWrite, fp);
  if(ret != sizeToWrite)
    printf("Wirte file error.\n");
  fclose(fp);
  return NULL;
}

int main()
{
  pthread_t fileThreadID;
  int ret = pthread_create(&fileThreadID, NULL, fileThreadFunc, NULL);
  if(ret != 0)
  {
    printf("Failed to create fileThread.\n");
    return -1;
  }
  int* retval;
  pthread_join(fileThreadID, (void**) &retval);
  FILE* fp = fopen(TIME_FILENAME, "r");
  if( fp == NULL)
  {
    printf("open file error.\n");
    return -2;
  }
  char buf[32] = {0};
  int sizeRead = fread(buf, 1, 32, fp);
  if(sizeRead == 0)
  {
    printf("Read file error.\n");
    fclose(fp);
    return -3;
  }
  printf("Current Time is: %s.\n", buf);
  fclose(fp);
  return 0;
}
```

**windows**
- WaitForSingleObject
- WaitForMultipleObjects

```C++
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#define TIME_FILENAME "time.txt"

DWORD WINAPI FileThreadFunc(LPVOID lpParameters)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timeStr[32] = {0};
    snprintf(timeStr, 32, "%04d/%02d/%02d %02d:%02d:%02d",
             t->tm_year + 1900,
             t->tm_mon + 1,
             t->tm_mday,
             t->tm_hour,
             t->tm_min,
             t->tm_sec);
    FILE *fp = fopen(TIME_FILENAME, "w");
    if (fp == NULL)
    {
        printf("Failed to create time.txt.\n");
        return NULL;
    }
    size_t sizeToWrite = strlen(timeStr) + 1;
    size_t ret = fwrite(timeStr, 1, sizeToWrite, fp);
    if (ret != sizeToWrite)
        printf("Wirte file error.\n");
    fclose(fp);
    return NULL;
}

int main()
{
    DWORD dwFileThreadID;
    HANDLE hFileThread = CreateThread(NULL, 0, FileThreadFunc, NULL, 0, 
    &dwFileThreadID);
    if (hFileThread == NULL)
    {
        printf("Failed to create fileThread.\n");
        return -1;
    }
    WaitForSingleObject(hFileThread, INFINITE);
    
    FILE *fp = fopen(TIME_FILENAME, "r");
    if (fp == NULL)
    {
        printf("open file error.\n");
        return -2;
    }
    char buf[32] = {0};
    int sizeRead = fread(buf, 1, 32, fp);
    if (sizeRead == 0)
    {
        printf("Read file error.\n");
        fclose(fp);
        return -3;
    }
    printf("Current Time is: %s.\n", buf);
    fclose(fp);
    return 0;
}
```

**C++11的线程等待函数**
- std::thread的join方法，目标线程必须是可join的，可以通过joinable方法判断

```C++
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<thread>

#define TIME_FILENAME "time.txt"

void fileThreadFunc()
{
  time_t now = time(NULL);
  struct tm* t = localtime(&now);
  char timeStr[32] = {0};
  snprintf(timeStr, 32, "%04d/%02d/%02d %02d:%02d:%02d",
      t->tm_year+1900,
      t->tm_mon+1,
      t->tm_mday,
      t->tm_hour,
      t->tm_min,
      t->tm_sec);
  FILE* fp = fopen(TIME_FILENAME, "w");
  if(fp == NULL)
  {
    printf("Failed to create time.txt.\n");
    return;
  }
  size_t sizeToWrite = strlen(timeStr)+1;
  size_t ret = fwrite(timeStr, 1, sizeToWrite, fp);
  if(ret != sizeToWrite)
    printf("Wirte file error.\n");
  fclose(fp);
  return;
}

int main()
{
  std::thread t(fileThreadFunc);
  if(t.joinable())
  {
    t.join();
  }
  
  FILE* fp = fopen(TIME_FILENAME, "r");
  if( fp == NULL)
  {
    printf("open file error.\n");
    return -2;
  }
  char buf[32] = {0};
  int sizeRead = fread(buf, 1, 32, fp);
  if(sizeRead == 0)
  {
    printf("Read file error.\n");
    fclose(fp);
    return -3;
  }
  printf("Current Time is: %s.\n", buf);
  fclose(fp);
  return 0;
}
```

> 如果使用C++面向对象的方法对线程函数进行封装，线程函数就不能是类的实例方法，只能是静态方法，因为线程函数签名必须是指定格式，对于类实例方法，在翻译时编译器会将对象地址作为第一个参数传递给方法，因此编译后的方法签名就不符合指定格式了。
> 如果使用静态方法，就无法访问类的实例方法了，为了解决这个问题，通常会在创建线程时将当前对象的地址（this指针）传递给线程函数，然后在线程函数中将该指针转换为原来的类实例，再通过这个实例访问类的所有方法。

## 3.4 整型变量的原子操作

### 3.4.1 不是原子类型的整形变量操作
- 给整形变量赋值：int a = 1;[编译器优化策略]
- 自增/自减：a++;--a; [3条汇编]
- 将一个变量赋值给另一个变量：int b = a;  [2条汇编]

### 3.4.2 windows对整型变量的原子操作
Interlocked系列函数（部分）：
```C++
LONG InterlockedAdd(
	LONG volatile *Addend,
	LONG          Value
);
LONG InterlockedIncrement(
	LONG volatile *Addend
);
LONG InterlockedAnd(
	LONG volatile *Destination,
	LONG          Value
);
LONG InterlockedExchangeAdd(
	LONG volatile *Addend,
	LONG          Value
);
LONG InterlockedExchange(
	LONG volatile *Target,
	LONG 		  Value
);
LONG InterlockedCompareExchange(
	LONG volatile *Destination,
	LONG          ExChange,
	LONG          Comperand
);
```

### 3.4.3 C++11对整型变量原子操作的支持

**std::atomic**，这是一个模板类型，可以传入具体整型（bool, char, short, int, uint等）https://en.cppreference.com/w/cpp/atomic/atomic
```C++
#include<atomic>
#include<stdio.h>

int main()
{
  std::atomic<int> value;
  value = 99;
  printf("%d\n", (int)value);

  value++;
  printf("%d\n", (int)value);
  return 0;
}
```

## 3.5 Linux线程同步对象

### 3.5.1 Linux互斥体
http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/pthread_mutex_init.html
```C++
#include <pthread.h>
pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t recmutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

pthread_mutex_t errchkmutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

int pthread_mutex_lock(pthread_mutex_t *mutex);

int pthread_mutex_trylock(pthread_mutex_t *mutex);

int pthread_mutex_unlock(pthread_mutex_t *mutex);

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```
- 无需销毁用PTHREAD_MUTEX_INITIALIZER初始化的互斥体
- 不要销毁一个已经加锁或正在被条件变量使用的互斥体对象

```C++
  int pthread_mutexattr_init(pthread_mutexattr_t *attr);
  int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
  int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
  int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
```
type类型：
- PTHREAD_MUTEX_NORMAL(普通锁)，默认属性，加锁后其他线程会阻塞在lock处。一个线程如果对一个已经加锁的普通锁再次使用lock，则会阻塞在第二次调用lock代码处。
- PTHREAD_MUTEX_ERRORCHECK(检错锁)，如果一个线程使用lock对已经加锁的互斥体对象再次加锁，则会返回EDEADLK。不影响其他线程调用后阻塞。
- PTHREAD_MUTEX_RECURSIVE(可重入锁)。

### 3.5.2 信号量

```C++
#include <semaphore.h>

//pshared=0表示只能在同一个进程之间共享，非0表示可以在多个进程间共享
//value用于设置信号量初始状态下的资源数量
int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destory(sem_t *sem);

//将信号量的资源计数加1，并解锁该信号量对象，会唤醒阻塞的其他线程
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem， const struct timespec* abs_timeout);
```

### 3.5.3 Linux条件变量
pthread_cond_t
```C++
#include <pthread.h>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);

int pthread_cond_signal(pthread_cond_t *cond);

int pthread_cond_broadcast(pthread_cond_t *cond);

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

int pthread_cond_destroy(pthread_cond_t *cond);
```

- pthread_cond_wait在阻塞时，会释放其绑定的互斥体并阻塞线程。**因此在调用前需要对互斥体进行加锁**
- pthread_cond_wait在收到信号时，会返回并对其绑定的互斥体进行加锁，**因此在调用后需要对互斥体进行解锁。**

**虚假唤醒**，条件变量唤醒之后再次测试条件是否满足就可以解决虚假唤醒问题。因此使用while循环而不是if。

> 如果一个条件变量信号在产生时，没有相关线程调用pthread_cond_wait捕获该信号，那么该信号就会永久丢失，再次调用pthread_cond_wait就用导致永久阻塞。**要确保在发送条件信号之前调用pthread_cond_wait。**

### 3.5.4 Linux读写锁
读锁共享，写锁独占。

```C++
#include <pthread.h>
int pthread_rwlock_init(pthread_rwlock_t * restrict lock,
    const pthread_rwlockattr_t * restrict attr);

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

int pthread_rwlock_destroy(pthread_rwlock_t *lock);

int pthread_rwlock_rdlock(pthread_rwlock_t *lock);

int pthread_rwlock_timedrdlock(pthread_rwlock_t * restrict lock,
    const struct timespec * restrict abstime);

int pthread_rwlock_tryrdlock(pthread_rwlock_t *lock);

int pthread_rwlock_wrlock(pthread_rwlock_t *lock);

int pthread_rwlock_timedwrlock(pthread_rwlock_t * restrict lock,
    const struct timespec * restrict abstime);

int pthread_rwlock_trywrlock(pthread_rwlock_t *lock);

int pthread_rwlock_unlock(pthread_rwlock_t *lock);
```

读写锁的属性：
```C++
//pthread_rwlockattr_t
enum
{
  PTHREAD_RWLOCK_PREFER_READER_NP,  //读者优先
  PTHREAD_RWLOCK_PREFER_WRITER_NP,  //读者优先
  PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP, //写者优先
  PTHREAD_RWLOCK_DEFAULT_NP = PTHREAD_RWLOCK_PREFER_READER_NP
};
int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr,
                      int pref);
int pthread_rwlockattr_getkind_np(
                      const pthread_rwlockattr_t *restrict attr,
                      int *restrict pref);
```

## 3.6 [windows线程同步对象](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/)
等待单个对象：
```C++
DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
```
hHandle可能的类型：
- 线程：等待线程结束
- 进程：等待进程结束
- Event(事件):等待Event有信号
- Mutex(互斥体):等待Mutex释放，成功后持有该Mutex
- Semaphore(信号量):等待该Semaphore对象有信号

返回值：
- WAIT_FAILED
- WAIT_OBJECT_0
- WAIT_TIMEOUT
- WAIT_ABANDONED:该mutex处于废弃状态

等待多个对象：
```C++
DWORD WaitForMultipleObjects(
  DWORD nCount,
  const HANDLE *lpHandles,
  BOOL bWaitAll,
  DOWRD dwMilliseconds
);
```

### 3.6.2 Windows临界区对象CriticalSection

临界区的代码在某一时刻只允许一个线程执行。

- InitializeCriticalSection
- InitializeCriticalSectionAndSpinCount:多加了自旋
- EnterCriticalSection
- TryEnterCriticalSection
- LeaveCriticalSection
- DeleteCriticalSection

位于EnterCriticalSection和LeaveCriticalSection之间的代码即临界区代码。TryEnterCriticalSection会尝试进入临界区，如果进入不了不会阻塞线程，而是立即返回False。

```C++
#include <Windows.h>
#include <list>
#include <iostream>
#include <string>


CRITICAL_SECTION g_cs;
int              g_number = 0;

DWORD __stdcall WOrkerThreadProc(LPVOID lpThreadParameter)
{
    DWORD dwThreadId = GetCurrentThreadId();
    while (true)
    {
        EnterCriticalSection(&g_cs);
        std::cout << "EnterCriticalSection, ThreadId:" << dwThreadId << std::endl;
        g_number++;
        SYSTEMTIME st;
        GetLocalTime(&st);
        char szMsg[64] = {0};
        sprintf(szMsg,
        "[%04d-%02d-%02d %02d:%02d:%02d:%03d]NO.%d, ThreadId:%d.",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
        g_number, dwThreadId);
        std::cout << szMsg << std::endl;
        std::cout << "Leave critical section, ThreadId:"
        << dwThreadId << std::endl;
        LeaveCriticalSection(&g_cs);
        Sleep(1000);
    }
    
}

int main()
{
    InitializeCriticalSection(&g_cs);
    HANDLE hWorkerThread1 = CreateThread(NULL, 0, WOrkerThreadProc, NULL, 0, NULL);
    HANDLE hWorkerThread2 = CreateThread(NULL, 0, WOrkerThreadProc, NULL, 0, NULL);

    WaitForSingleObject(hWorkerThread1, INFINITE);
    WaitForSingleObject(hWorkerThread2, INFINITE);

    CloseHandle(hWorkerThread1);
    CloseHandle(hWorkerThread2);

    DeleteCriticalSection(&g_cs);
    return 0;
}
```

封装CriticalSection:
```C++
class CCriticalSection
{
  public:
  CCriticalSection(CRITICAL_SECTION& cs): m_CS(cs)
  {
    EnterCriticalSection(&m_CS);
  }
  ~CCriticalSection()
  {
    LeaveCriticalSection(&m_CS);
  }

private:
  CRITICAL_SECTION& m_CS;
}
```

### 3.6.3 WindowsEvent对象

```C++
HANDLE CreateEventA(
  [in, optional] LPSECURITY_ATTRIBUTES lpEventAttributes,
  [in]           BOOL                  bManualReset,
  [in]           BOOL                  bInitialState,
  [in, optional] LPCSTR                lpName
);
```
- lpEventAttributes设置Event对象安全性，一般为NULL
- bManualReset，设置Event对象受信行为，当设置为Ture时需要手动调用ResetEvent重置为无信号状态
- bInitialState：Event初始状态是否受信
- lpName,可以设置Event对象名称，也可以为NULl，Event对象可以通过名称在不同的进程之间共享。
- 成功返回对象句柄，失败返回NULL

```C++
#include<Windows.h>
#include<string>
#include<iostream>

bool        g_bTaskCompleted = false;
std::string g_TaskResult;
HANDLE      g_hTaskEvent = NULL;

DWORD __stdcall WorkerThreadProc(LPVOID lpThreadParameter)
{
    Sleep(3000);
    g_TaskResult = "Task completed";
    g_bTaskCompleted = true;
    SetEvent(g_hTaskEvent);
    return 0;
}

int main()
{
    g_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE hWorkerThread = CreateThread(NULL, 0, WorkerThreadProc, NULL, 0, NULL);
    DWORD dwResult = WaitForSingleObject(g_hTaskEvent, INFINITE);
    if(dwResult == WAIT_OBJECT_0)
        std::cout << g_TaskResult << std::endl;
    CloseHandle(hWorkerThread);
    CloseHandle(g_hTaskEvent);
    return 0;
}
```

> 手动重置的Event对象一旦变成受信状态，其信号就不会丢失。而条件变量则可能丢失信号。

### 3.6.4 [Windows Mutex对象](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createmutexa)
```C++
HANDLE CreateMutexA(
  [in, optional] LPSECURITY_ATTRIBUTES lpMutexAttributes,
  [in]           BOOL                  bInitialOwner,
  [in, optional] LPCSTR                lpName
);
```
- lpMutexAttributes设置Mutex对象安全性，一般为NULL
- bInitialOwner设置调用者的线程是否立即拥有该对象
- lpName,可以设置Mutex对象名称，也可以为NULl，Mutex对象可以通过名称在不同的进程之间共享。

```C++
#include<Windows.h>
#include<string>
#include<iostream>

int         g_iResource = 0;
HANDLE      g_hMutex = NULL;

DWORD __stdcall WorkerThreadProc(LPVOID lpThreadParameter)
{
    DWORD dwThreadId = GetCurrentThreadId();
    while(true)
    {
        if(WaitForSingleObject(g_hMutex, 1000) == WAIT_OBJECT_0)
        {
            g_iResource++;
            std::cout << "Thread:" << dwThreadId << "becomes mutex owner,ResourceNo:" 
            << g_iResource << std::endl;
            ReleaseMutex(g_hMutex);
        }   
        Sleep(1000);
    }
    return 0;
}

int main()
{
    g_hMutex = CreateMutex(NULL, false, NULL);
    HANDLE hWorkerThreads[5];
    for(int i = 0; i < 5; ++i)
    {
        hWorkerThreads[i] = CreateThread(NULL, 0, WorkerThreadProc, NULL, 0, NULL);
    }
    for(int i = 0; i < 5; ++i)
    {
        WaitForSingleObject(hWorkerThreads[i], INFINITE);
        CloseHandle(hWorkerThreads[i]);
    }
    CloseHandle(g_hMutex);
    return 0;
}
```

### 3.6.5 [Windows Semaphore对象](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createsemaphoreexw)
Semaphore能够精确控制同时唤醒指定数量的线程。
```C++
HANDLE CreateSemaphoreW(
  [in, optional] LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  [in]           LONG                  lInitialCount,
  [in]           LONG                  lMaximumCount,
  [in, optional] LPCWSTR               lpName
);
```

```C++
#include<Windows.h>
#include<iostream>
#include<time.h>
#include<list>
#include<string>

HANDLE                  g_hMsgSemaphore = NULL;
std::list<std::string>  g_listChatMsg;
CRITICAL_SECTION        g_csMsg;

DWORD __stdcall NetThreadProc(LPVOID lpThreadParam)
{
    int nMsgIndex = 0;
    while(true)
    {
        EnterCriticalSection(&g_csMsg);
        int count = rand() % 4 + 1;
        for(int i = 0; i < count; ++i)
        {
            nMsgIndex++;
            SYSTEMTIME st;
            GetLocalTime(&st);
            char szMsg[64] = {0};
            sprintf(szMsg,
            "[%04d-%02d-%02d %02d:%02d:%02d:%03d] A new Msg, No.%d.",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
            nMsgIndex);
            g_listChatMsg.emplace_back(szMsg);
        }
        LeaveCriticalSection(&g_csMsg);
        ReleaseSemaphore(g_hMsgSemaphore, count, NULL);
    }
    return 0;
}

DWORD __stdcall ParseThreadProc(LPVOID lpThreadParam)
{
    DWORD dwThreadId = GetCurrentThreadId();
    std::string current;
    while(true)
    {
        if(WaitForSingleObject(g_hMsgSemaphore, INFINITE) == WAIT_OBJECT_0)
        {
            EnterCriticalSection(&g_csMsg);
            if(!g_listChatMsg.empty())
            {
                current = g_listChatMsg.front();
                g_listChatMsg.pop_front();
                std::cout << "Thread:" << dwThreadId << " Parse msg: " << 
                current << std::endl;
            }
            LeaveCriticalSection(&g_csMsg);
        }
    }
    return 0;
}

int main()
{
    srand(time(NULL));
    InitializeCriticalSection(&g_csMsg);

    g_hMsgSemaphore = CreateSemaphore(NULL, 0, INT_MAX, NULL);

    HANDLE hNetThread = CreateThread(NULL, 0, NetThreadProc, NULL, 0, NULL);
    HANDLE hWorkerThreads[4];
    for(int i = 0; i < 4; ++i)
    {
        hWorkerThreads[i] = CreateThread(NULL, 0, ParseThreadProc, NULL, 0, NULL);
    }
    for(int i = 0;i < 4; ++i)
    {
        WaitForSingleObject(hWorkerThreads[i], INFINITE);
        CloseHandle(hWorkerThreads[i]);
    }
    WaitForSingleObject(hNetThread, INFINITE);
    CloseHandle(hNetThread);

    CloseHandle(g_hMsgSemaphore);

    DeleteCriticalSection(&g_csMsg);
    return 0;
}
```

### 3.6.6 [Windows读写锁](https://learn.microsoft.com/en-us/windows/win32/sync/slim-reader-writer--srw--locks)

- InitializeSRWLock:Initialize an SRW lock.
- AcquireSRWLockShared:以共享方式获得读写锁
- ReleaseSRWLockShared:释放共享读写锁
- AcquireSRWLockExclusive：获得排他读写锁
- ReleaseSRWLockExclusive：释放排他读写锁

不需要显示销毁一个读写锁。

### 3.6.7 [Windows条件变量](https://learn.microsoft.com/en-us/windows/win32/sync/condition-variables)

在XP版本以后支持，user-mode对象，不支持跨进程共享。使用时需要配合一个临界区或读写锁，在调用SleepCondition相关函数前，调用线程必须持有对应临界区或读写锁对象。

- InitializeConditionVariable：Initializes a condition variable.
- SleepConditionVariableCS：Sleeps on the specified condition variable and releases the specified critical section as an atomic operation.
- SleepConditionVariableSRW：Sleeps on the specified condition variable and releases the specified SRW lock as an atomic operation.
- WakeAllConditionVariable：Wakes all threads waiting on the specified condition variable.
- WakeConditionVariable：Wakes a single thread waiting on the specified condition variable.

```C++
CRITICAL_SECTION CritSection;
CONDITION_VARIABLE ConditionVar;

void PerformOperationOnSharedData()
{ 
   EnterCriticalSection(&CritSection);

   // Wait until the predicate is TRUE

   while( TestPredicate() == FALSE )
   {
      //线程无限等待，直到资源可用，由于可能存在操作系统虚假唤醒，所有在其外
      //加了一个用于判断资源是否可用的循环
      SleepConditionVariableCS(&ConditionVar, &CritSection, INFINITE);
   }

   // The data can be changed safely because we own the critical 
   // section and the predicate is TRUE

   ChangeSharedData();

   LeaveCriticalSection(&CritSection);

   // If necessary, signal the condition variable by calling
   // WakeConditionVariable or WakeAllConditionVariable so other
   // threads can wake
}
```

### 3.6.8 在多进程间共享线程同步对象

> Windows Event, Mutex, Semaphore对象在创建时指定名称后就可以在不同的进程之间共享。以下用CreateMutex来说明：
> 
> If the function succeeds, the return value is a handle to the newly created mutex object.
> If the function fails, the return value is NULL. To get extended error information, call GetLastError.
> 
> **If the mutex is a named mutex and the object existed before this function call, the return value is a handle to the existing object, and the GetLastError function returns ERROR_ALREADY_EXISTS.**

## 3.7 C++11/14/17线程同步对象

C++11标准中新增了用于线程同步的std::mutex和std::condition_variable。

- mutex:C++11,基本的互斥量
- timed_mutex:C++11,超时机制互斥量
- recursive_mutex:C++11，可重入的互斥量
- recursive_timed_mutex:C++11
- shared_timed_mutex:C++14
- shared_mutex:C++17

并且提供了如下封装：

- lock_gurad:C++11，基于作用域的互斥量管理
- unique_lock:C++11,更加灵活的互斥量管理
- shared_lock:C++14，共享互斥量管理
- scoped_lock:C++17,多互斥量避免死锁管理

**要避免同一个线程对一个互斥量多次加锁，如果确实需要，则应该使用recursive_mutex，并且加锁多少次就需要解锁多少次。** std::mutex和std::shared_mutex分别对应Java中的ReentrantLock和ReentrantReadWriteLock。

C++11还提供了std::condition_variable来表示条件变量，使用时需要绑定一个std::unique_lock或std::lock_guard对象，并且不需要显示的初始化和销毁。

```C++
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
```

## 3.9 多线程使用锁总结

### 减少锁的使用次数

使用锁会造成以下影响：

- 加解锁本身有开销
- 临界区代码不能并发执行
- 线程竞争激烈的话会造成上下切换开销的消耗

### 明确锁范围

### 减少锁的使用粒度

尽量减少锁作用的临界区代码范围。

### 避免死锁

- 加锁后需要在函数的每个出口都有解锁操作；
- 线程退出时要及时释放其持有的锁
- 多线程请求锁的方向要一致，如有锁A、B需要加锁，所有线程的加锁顺序要么都是先A后B，要么都是先B后A

### 避免活锁

- 避免让过多的线程使用trylock请求锁，以免出现活锁浪费资源

## 3.10 线程局部存储(Thread Local Storage, TLS)

### 3.10.1 [Windows 线程局部存储]((https://learn.microsoft.com/en-us/windows/win32/procthread/thread-local-storage))

windows将线程局部存储区分为TLS_MINIMUM_AVAILABLE个块，默认为64，其定义在winnt.h中。

> Thread local storage (TLS) enables multiple threads of the same process to use an index allocated by the **TlsAlloc** function to store and retrieve a value that is local to the thread. In this example, an index is allocated when the process starts. When each thread starts, it allocates a block of dynamic memory and stores a pointer to this memory in the TLS slot using the **TlsSetValue** function. The CommonFunc function uses the **TlsGetValue** function to access the data associated with the index that is local to the calling thread. Before each thread terminates, it releases its dynamic memory. Before the process terminates, it calls **TlsFree** to release the index.

### 3.10.2 Linux的线程局部存储

```C++
__thread int g_mydata = 99;   //gcc编译器提供的用于定义线程局部变量的方式
//因为进程中的所有线程都可以使用key，所以key应该指向一个全局变量
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int pthread_key_delete(pthread_key_t key);
void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void *value);
void* destructor(void* value)
{
  //多是为了释放value指针指向的资源
}
```

### 3.10.3 C++11的thread_local关键字

使用thread_local来定义一个线程变量。

### 总结
  
- 对于线程变量，每个线程都有一个这个变量的拷贝，互不影响，在线程未结束前一直存在
- 系统的线程局部存储区并不大，尽量不要使用这个空间存储大数据块，如不得不使用，可以将大数据块存储在堆内存中，再将堆内存的地址指针存储在TLS中。

## 3.11 C库的非线程安全函数

这种函数大多采用了全局变量或者静态变量

## 3.12 线程池与队列系统设计

## 3.13 纤程(Fiber)和协程(Routine)

### 3.13.1 [纤程(Fiber)](https://learn.microsoft.com/en-us/windows/win32/procthread/fibers)

纤程是Windows中的概念，使用线程有如下不足：

- 由于线程的调度是由操作系统内核控制的，所以我们无法确定操作系统会何时运行或挂起该线程
- 对于一些轻量级的任务，创建一个线程做的消耗太大

在Windows中，一个线程可以有多个纤程，用户可以根据需要在各个纤程之间自由切换。**ConverThreadToFiber**可以将纤程切换成纤程模式，但在默认情况下x86系统的CPU浮点状态信息不属于CPU寄存器，不会为每个纤程都维护一份，因此如果在纤程中执行浮点操作就会导致数据被破坏，为了禁用这种行为可以使用**ConverThreadToFiberEx(, FIBER_FLAG_FLOAT_SWITCH)**。纤程也有自己的局部存储(FLS)。

```C++
#include<windows.h>
#include<string>

char g_szTime[64] = { "time not set..."};
LPVOID mainWorkerFiber = NULL;
LPVOID pWorkerFiber = NULL;

void WINAPI workerFiberProc(LPVOID lpFiberParameter)
{
    while(true)
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        wsprintfA(g_szTime, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth,
        st.wDay, st.wHour, st.wMinute, st.wSecond);
        printf("%s\n", g_szTime);

        SwitchToFiber(mainWorkerFiber);
    }
}

int main()
{
    mainWorkerFiber = ConvertThreadToFiber(NULL);
    int index = 0;
    while(index < 100)
    {
        ++index;
        pWorkerFiber = CreateFiber(0, workerFiberProc, NULL);
        if(pWorkerFiber == NULL)
            return -1;
        SwitchToFiber(pWorkerFiber);

        memset(g_szTime, 0, sizeof(g_szTime));
        strncpy(g_szTime, "time not set...", strlen("time not set..."));
        printf("%s\n", g_szTime);
        Sleep(1000);
    }
    DeleteFiber(pWorkerFiber);
    ConvertFiberToThread();
    return 0;
}
```

### 3.13.2 协程

线程是操作系统内核对象，线程过多时会导致上下文切换消耗太大，协程可以被认为是应用层模拟的线程，避免了线程上下文切换的部分额外损耗，同时具有并发运行的优点，降低了编写并发程序的复杂度。