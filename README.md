
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

# 第四章 网络编程

## 4.1 socket函数

|函数|说明|
|---|---|
|socket|创建套接字|
|bind|将socket绑定到IP和端口|
|listen|让socket监听|
|connect|建立TCP链接，一般用于客户端|
|accept|尝试接收一个链接，用于服务端|
|send|发送数据|
|recv|接收数据|
|select|判断一组socket上的读写和异常事件|
|gethostbyname|通过域名获取机器地址|
|close|关闭套接字|
|shutdown|关闭socket收发通道|
|setsocket|设置套接字选项|
|getsocket|获取套接字选项|

## 4.3 涉及跨平台网络通信库的socket函数用法

### 4.3.1 socket数据类型

```C++
#ifdef WIN32
typedef SOCKET SOCKET_TYPE
#else
typedef int SOCKET_TYPE
#endif
```

### 4.3.2 windows上调用socket函数

对于windows平台，想要使用socket函数，必须先调用WSAStartup函数将与socket函数相关的dll文件加载到进程地址空间，退出时使用WSACleanup函数卸载相关dll文件。WSAStartup和WSACleanup是进程相关的任何一个线程都可以调用，因此，一般在进程退出时才调用WSACleanup函数。

```C++
bool InitSocket()
{
  WORD wVersionRequested = MAKEWORD(2,2);
  WSADATA wsaData;
  int nErrorId = ::WSAStartup(wVersionRequested, &wsaData);
  if(nErrorId != 0)
    return false;
  if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)
  {
    UninitSocket();
    return false;
  }
  return true;
}

void UninitSocket()
{
  ::WSACleanup();
}
```

### 关闭socket函数

Linux中使用close，windows中使用closesocket函数，windows中也定了一个close函数，但不能用于关闭套接字，否则会导致程序崩溃。

```C++
#ifdef WIN32
#define closesocket(s) close(s)
#endif
```

### 获取socket函数的错误码

在某个socket函数调用失败时，Windows需要调用WSAGetLastError()获取错误代码，Linux直接使用errno获取。

```C++
#ifdef WIN32
#define GetSocketError() WSAGetLastError()
#else
#define GetSocketError() errno
#endif
```

### 套接字函数返回值

大多数socket函数在调用失败后会返回-1，windows为此定义了一个宏SOCKET_ERROR，为此在Linux上也可以定义一个：

```C++
#ifndef WIN32
#define SOCKET_ERROR (-1)
#endif
```

### 错误码WSAEWOULDBLOCK和EWOULDBLOCK

```C++
#ifdef WIN32
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
```

对于IO复用技术，两者都支持select模型，Linux特有的poll、epoll模型；windows特有的WSAPOOL和完成端口模型（IOCP）。

## 4.4 bind函数

### 4.4.1 bind函数如何选择绑定地址

使用INADDR_ANY，底层的协议栈会自动选择一个合适的IP地址，在多网卡机器上选择IP地址会变得简单。相当于地址0.0.0.0。

### 4.4.2 bind函数端口号问题

端口号指定0系统会随机分配可用端口号，客户端的socket也可以调用bind函数绑定端口号。

## 4.5 select函数

### 4.5.1 Linux上的select函数

select函数用于检测在一组socket中是否有事件就绪，返回值为0时代表超时，返回-1代表出错，若监视的事件中有任意一个事件发生，则返回值为就绪文件描述符数量，也就是说，即使只有一个文件描述符就绪，返回值也是 1，一般分如下三类：

- 读事件就绪
- 写事件就绪
- 异常事件就绪

```C++
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
          fd_set *exceptfds, struct timeval *timeout);

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);

#include <sys/select.h>

int pselect(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask);
```

- nfds：所有需要使用select函数检测事件的fd中最大值加1
- readfds:需要监听可读事件的fd集合

> 在Linux上，向fd_set添加fd时用的是位图法，一个可以添加1024个fd。

select用法示例：

```C++
//
// Created by ljx on 23-2-22.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <errno.h>
#include <string.h>

#define INVALID_FD -1

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == INVALID_FD)
    {
        std::cout << "create listen_fd error." << std::endl;
        return -1;
    }

    struct sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port = htons(3000);
    if(bind(listen_fd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listen_fd);
        return -1;
    }

    if(listen(listen_fd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listen_fd);
        return -1;
    }

    std::vector<int> client_fds;
    int maxfd;

    while(true)
    {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(listen_fd, &readSet);
        maxfd = listen_fd;

        int clientFdsLength = client_fds.size();
        for(int i = 0; i < clientFdsLength; ++i)
        {
            if(client_fds[i] != INVALID_FD)
            {
                FD_SET(client_fds[i], &readSet);
                if(maxfd < client_fds[i])
                    maxfd = client_fds[i];
            }
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;
        int ret = select(maxfd+1, &readSet, NULL, NULL, &tm);
        if(ret == -1)
        {
            if(errno != EINTR)
                break;
        }
        else if(ret == 0)
        {
            //select 函数超时
            continue;
        }
        else
        {
            if(FD_ISSET(listen_fd, &readSet))
            {
                //如果是监听socket的可读事件，说明有新连接
                struct sockaddr_in clientAddr;
                socklen_t  clientAddrLen = sizeof(clientAddr);
                int clientFd = accept(listen_fd, (struct  sockaddr*)&clientAddr, &clientAddrLen);
                if(clientFd == INVALID_FD)
                {
                    break;
                }
                std::cout << "accept a client connection, fd:" << clientFd << std::endl;
                client_fds.push_back(clientFd);
            }
            else
            {
                char recvBuf[64];
                int clientFdsLength = client_fds.size();
                for(int i = 0;i < clientFdsLength; ++i)
                {
                    if(client_fds[i] != INVALID_FD && FD_ISSET(client_fds[i], &readSet))
                    {
                        //如果其他socket的读事件则是客户端发来的消息
                        memset(recvBuf, 0, sizeof(recvBuf));
                        int length = recv(client_fds[i], recvBuf, 64, 0);
                        //recv函数返回0说明对端关闭了连接
                        if(length <= 0)
                        {
                            std::cout << "recv data error, clientfd:" << client_fds[i] << std::endl;
                            close(client_fds[i]);
                            client_fds[i] = INVALID_FD;
                            continue;
                        }
                        std::cout << "clientfd: " << client_fds[i] << ", recv data:" << recvBuf << std::endl;
                    }
                }
            }
        }
    }
    int clientFdsLength = client_fds.size();
    for(int i = 0; i < clientFdsLength; ++i)
    {
        if(client_fds[i] == INVALID_FD)
        {
            close(client_fds[i]);
        }
    }
    close(listen_fd);
    return 0;
}
```

注意事项：

- select函数在调用前后可能会修改readfds,writefds,exceptfds三个集合的内容，所以想要复用这三个变量时需要小心；
- select函数也会修改timeval结构体的值，在Linux系统上是这样，Windows不会；
- select函数的timeval结构体的两个字段如果都为0， 其行为时select检测相关集合中的fd，如果没有需要的事件则立即返回。如果这个值设置为NULL，select函数则会一直阻塞，知道满足要求的事件触发；
- 在Linux上，select函数的第一个参数必须是待检测事件中最大的fd加1，在Windows上第一个参数传入任意值都可以，本身并不使用这个值

缺点：

- 每次调用select函数，都需要把fd集合从用户态复制到内核态并且在内核态中遍历传递进来的fd集合，这个开销在fd较多时很大；
- 单个进程能够监视的文件描述符数量存在最大限制，Linux上一般为1024；
- select函数在每次调用之前都要对传入的参数进行重新设定；
- 在Linux上，select函数实现原理是其底层使用了poll函数

## 4.6 socket的阻塞模式和非阻塞模式

在阻塞和非阻塞模式下，具有不同行为表现的socket函数一般有connect,accept,send和recv。在Linux上还有write和read。默认创建的socket是阻塞模式的。

### 设置非阻塞模式

1. 创建时指定非阻塞（Linux)

```C++
int s = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
```

2. 使用ioctl或者fcntl

```C++
int oldSocketFlag = fcntl(sockfd, F_GETFL, 0);
int newSocketFlag = oldSocketFlag | O_NONBLOCK;
fcntl(sockfd, F_SETFL, newSocketFlag);
```

3. Linux上的accept4可以将返回的socket设置为非阻塞的

```C++
int clientFd = accept4(listenFd, &clientAddr, &clientAddrLen, SOCK_NONBLOCK);
```

4. 在windows上通过ioctlsocket函数将socket设置为非阻塞的

```C++
int ioctlsocket(SOCKET s, long cmd, u_long *argp);
//cmd为FIONBIO， argp为0时阻塞，argp非0则为非阻塞
```

### 4.6.2 send和recv在阻塞和非阻塞模式下的表现

send函数本质上不是像网络上发送数据，而是将应用层发送缓冲区的数据拷贝到内核缓冲区（TCP窗口），至于什么时候会从网卡缓冲区发送到网络要看TCP/IP协议栈的行为来确定，如果socket设置了TCP_NODELAY(禁用nagel算法)，存放到内核缓冲区的数据就会被立即发送。recv的行为类似。

如果内核缓冲区被填满后会有以下表现；

- 如果socket是阻塞模式，继续调用send/recv，程序会阻塞在send/recv处；
- 如果是非阻塞模式，send/recv会立即出错并返回，会得到一个相关错误码，Linux上为EWOULDBLOCK或EAGAIN，Windows上为WSAEWOULDBLOCK。

### 4.6.3 send/recv小结

非阻塞模式下返回值总结

|返回值n|含义|
|---|---|
|>0|成功发送/接受n字节，并不代表数据全部发送，有可能只发送了部分|
|=0|对端关闭连接，或者主动发送了0字节的数据|
|-1|出错/信号中断/缓冲区满/空|

返回-1时的情况

|错误码|send函数|recv函数|操作系统|
|---|---|---|---|
|EWOULDBLOCK/EAGAIN|TCP窗口太小，数据暂时无法发送|当前内核缓冲区无可读数据|Linux|
|EINTR|信号中断|信号中断|Linux|
|非以上|出错|出错|Linux|
|WSAEWOULDBLOCK|TCP窗口太小，数据暂时无法发送|当前内核缓冲区无可读数据|Windows|
|不是WSAEWOULDBLOCK|出错|出错|Windows|

**使用场景**

非阻塞模式一般用于需要支持高并发多QPS的场景（如服务器程序），阻塞模式逻辑简单，程序明了。阻塞模式可用于下面的场景：

简单的网络应用：对于简单的网络应用，如传输小型文件或消息，阻塞模式的 socket 可以满足需求，因为这些操作不需要长时间的等待和处理，而且相对于非阻塞模式的 socket，阻塞模式更加易于使用和实现。

小规模网络应用：对于小规模的网络应用，阻塞模式的 socket 可以提供足够的性能和可靠性。此外，阻塞模式也更容易实现和调试。

低负载网络：对于低负载的网络，阻塞模式的 socket 可以提供良好的性能和可靠性。例如，对于个人电脑或小型服务器，阻塞模式的 socket 可以满足需求，因为它们通常不需要处理大量的并发连接或高速数据传输。

传输较小数据量：对于传输较小数据量的网络应用，阻塞模式的 socket 可以比非阻塞模式的 socket 更加高效。这是因为在阻塞模式下，数据可以一次性传输完毕，而非阻塞模式下需要多次传输，增加了额外的开销。

总之，阻塞模式的 socket 适用于简单、小规模、低负载、传输较小数据量的网络应用，它更加易于使用和实现，但是在高并发、高速传输和大数据量传输等场景下，可能会影响网络应用的性能和可靠性。

### 主动发送0字节数据的情况

send函数发送0字节的数据时，操作系统协议栈并不会把这些数据发送出去，因此server端的recv函数不会接收到这个0字节的数据包。开发时应该避免写出可能调用send函数发送0字节的代码。

## 4.8 connect函数在阻塞和非阻塞模式下的行为

一般步骤：

- 创建socket，设置非阻塞模式
- 调用connect函数，此时，无论是否连接成功都会立即返回，如果返回-1，并且错误码是EINPROGRASS则表示正在尝试连接
- 调用select函数，在指定时间内判断该socket是否可写，如果可写则说明连接成功，否则认为连接失败

**在Linux上，一个socket没有建立连接之前用select函数检测也会得到可写的结果。因此需要调用getsocketopt检测socket是否出错，并通过错误码检测socket是否连接上。Windows上不会出现此问题**

## 4.9 连接时顺便接受第一组数据

Linux提供了TCP_DEFER_ACCEPT的socket选项，设置该选项之后只有连接成功且接收到第一组对端数据时accept才返回。Windows则可以通过[AcceptEx](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-acceptex)函数来设置。

发送时同样可以设置是否顺便发送第一组数据。Windows通过ConnectEx函数设置，

## 4.10 获取当前socket对应的接受缓存区中的可读数据量

### 函数

Windows提供了[ioctlsocket](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket)这个函数，cmd设置为FIONREAD即可，argp存储操作结果。Linux可以使用ioctl函数，用法基本相同。但是Linux的第三个参数必须初始化为0才能得到正确的结果。

## 4.12 Linux SIGPIPE信号

当A关闭连接时，若B继续向A发送数据，根据TCP的规定，B会收到一个RST报文应答，若B继续向这个服务发送数据，系统就会产生一个SIGPIPE信号给B进程，**并且系统对SIGPIPE信号的默认处理方式是结束进程。** 为了避免这个信号对程序产生影响，可以忽略该信号；

```C++
signal(SIGPIPE, SIG_IGN)
```

这样设置后，第二次调用write/send方法时会返回-1，同时errno错误码被置为EPIPE。

## 4.13 Linux poll函数

poll函数用于检测一组文件描述符上的可读可写和出错事件：

```C++
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd {
    int   fd;         /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};
```

优点：

- poll不要求计算最大fd+1
- 与select相比，poll在处理大数量文件描述符时速度更快
- poll没有最大连接数的限制
- 调用poll时，只需对参数进行一次设置

缺点：

- 在调用epoll时，不管有没有意义，大量fd的数组在用户态和内核地址空间之间被整体复制
- 与select一样，poll返回后需要遍历集合fd来获取就绪的fd
- 同时连接的大量客户端在某一时刻可能只有很少的就绪态，因此随着监视描述符数量增长，其效率也会线性下降

## 4.14 epoll模型

epoll 是一种高效的 I/O 多路复用机制，它比传统的 select 和 poll 函数更加灵活和高效。epoll 主要包括三个函数：

int epoll_create(int size)：用于创建一个 epoll 对象，size 参数指定 epoll 可以监视的文件描述符的最大数量，但实际上这个值并不是一个硬性限制。

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)：用于向 epoll 中添加或修改或删除一个文件描述符，epfd 参数是 epoll 对象的文件描述符，op 参数是操作类型，可以是 EPOLL_CTL_ADD、EPOLL_CTL_MOD 或 EPOLL_CTL_DEL，分别用于添加、修改和删除文件描述符，fd 参数是需要添加、修改或删除的文件描述符，event 参数是一个指向 epoll_event 结构体的指针，用于指定事件类型和其他参数。

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)：用于等待 epoll 监视的文件描述符上发生事件，epfd 参数是 epoll 对象的文件描述符，events 参数是一个指向 epoll_event 数组的指针，用于存储发生事件的文件描述符和事件类型，maxevents 参数指定 events 数组的最大大小，timeout 参数指定等待时间的毫秒数，如果 timeout 参数为 -1，则表示等待时间无限长。

```C++
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_EVENTS 10

int main() {
    int epfd, nfds;
    struct epoll_event event, events[MAX_EVENTS];

    // 创建 epoll 对象
    epfd = epoll_create(1);
    if (epfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    // 监视 stdin 文件描述符上是否有数据可读
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // 等待事件
        nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // 处理事件
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("stdin is readable\n");
            }
        }
    }

    return 0;
}
```

优点：

- epoll_wait调用完成后，可通过参数event拿到所有的有事件就绪的fd
- 在socket连接数量较大而活跃的连接较少时，epoll模型更高效

### 4.14.3 LT模式和ET模式

epoll模式新增了一个事件宏EPOLLET，即边缘触发模式，默认的模式为水平触发模式（LT）。

- 对于水平触发模式，一个事件只要有，就会一直触发
- 对于边缘触发模式，一个事件从无到有才触发

注意事项：

- 对于一个非阻塞socket，如果使用epoll边缘模式检测事件是否可读，则触发可读事件后，一定要一次性把socket上的数据收取干净，方法一般为循环调用recv，直到出错，错误码为EWOULDBLOCK。
- 在LT模式下，不需要写事件时一定要及时移除，避免不必要的触发且浪费CPU资源；在ET模式下，写事件触发后，如果还需要下一次的写事件触发来驱动任务，则需要继续注册一次检测可写事件
- 使用LT可以由我们决定每次收取多少字节或者合适连接，但是可能会导致多次触发；使用ET模式，我们每次必须将数据收完或立即调用accept接受连接，但其优点时触发次数少。

### 4.14.4 EPOLLONESHOT

epoll模型还有一个EPOLLONESHOT选项，如果某个socket注册了该标志，则其注册监听的事件在触发一次后再也不会触发，除非重新监听该事件类型。

## 4.15 高效的readv和writev函数

在实际开发中，高性能服务有一个原则：尽量减少系统调用。有时需要将一个文件描述符对应的数据读到多个缓冲区，或者将多个缓冲区的数据同时写入一个文件描述符对应的文件或者套接字中，为此Linux提供了一系列readv和writev函数。

## 4.16 主机字节序和网络字节序

主机字节序分为大端存储和小端存储：

1) little-endian(LE,小端)。对于一个整数值，整数高位被存储在内存高地址位置，低位存在内存低地址位置。Intel x86系列使用的都是小端存储。
2) big-endian(BE,大端)。对于一个整数值，整数高位被存储在内存低地址位置，低位存在内存高地址位置。Java程序、Mac机器上一般都为大端编码。

网络字节序采用big-endian排序方式。

## 4.17 域名解析API

使用gethostbyname函数将域名转换为IP地址。

- gethostbyname是不可重入函数，在Linux上建议使用gethostbyname_r替代
- gethostbyname在解析域名时会阻塞当前线程
- 在使用gethostbyname出错时，不能使用errno和perror，应该使用h_errno和herror。
- 在新的Linux上，gethostbyname和gethostbyaddr一样被标记为废弃，应该使用新的函数getaddrinfo替代。

```C++
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "iostream"

bool connectToServer(const char* server, short port)
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1)
        return false;
    struct sockaddr_in addr = {0};
    struct hostent* pHostEnt = NULL;

    if(addr.sin_addr.s_addr = inet_addr(server) == INADDR_NONE)
    {
        pHostEnt = gethostbyname(server);
        if(pHostEnt == NULL)
            return false;
        addr.sin_addr.s_addr = *((unsigned long*)pHostEnt->h_addr_list[0]);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int ret = connect(socketFd, (struct sockaddr*)&addr, sizeof addr);
    if(ret == -1)
        return false;
    return true;
}
```

# 第五章 网络通信故障排查命令

## ifconfig

ifconfig是Linux系统下的一个网络配置命令，用于显示、设置以及管理系统中的网络接口（Network Interface Card，NIC）。ifconfig命令可以查询和设置网络设备的状态，比如IP地址、子网掩码、广播地址、MAC地址、MTU等。通常需要使用root权限来执行ifconfig命令。

以下是ifconfig命令的一些常用选项和用法：

```bash
ifconfig：显示所有网络接口及其配置信息
ifconfig eth0：显示eth0网络接口的配置信息
ifconfig eth0 up：启用eth0网络接口
ifconfig eth0 down：禁用eth0网络接口
ifconfig eth0 192.168.1.100 netmask 255.255.255.0：设置eth0网络接口的IP地址为192.168.1.100，子网掩码为255.255.255.0
ifconfig eth0 hw ether 00:11:22:33:44:55：设置eth0网络接口的MAC地址为00:11:22:33:44:55
ifconfig -a:显示所有网卡信息，包括未启用的网卡
```

## ping

ping命令是一个用于测试与另一个网络设备之间连接是否正常的工具。通常情况下，ping命令用于检测另一台计算机是否在线、网络是否正常工作以及网络的连接速度等。在执行ping命令时，它会向目标设备发送一个ICMP（Internet控制消息协议）回声请求，并等待目标设备响应。如果目标设备能够收到并响应这个请求，那么ping命令就会显示响应时间和一些其他统计信息。如果目标设备不能响应请求，则ping命令将显示超时错误。ping命令通常用于调试网络连接问题和排除故障。

## telnet

Telnet是一种用于远程登录的协议和工具。它通过网络连接到远程服务器，并允许用户在本地计算机上以终端模式运行远程命令。以下是一些常见的telnet命令：

```bash
telnet <hostname> <port>：连接到指定主机和端口。
open <hostname> <port>：连接到指定主机和端口。
quit：关闭当前telnet会话。
Ctrl + ]：进入telnet命令行模式。
Ctrl + Shift + ]，然后输入quit：强制退出telnet。
```

## netstat

netstat 命令常见的选项包括：

- -a：显示所有套接字（包括正在监听的和被连接的）。  
- -t：只显示 TCP 套接字。  
- -u：只显示 UDP 套接字。  
- -n：不使用域名，而是以 IP 地址形式显示所有套接字。
- -p：显示与套接字相关联的进程信息。
- -s：显示套接字统计信息，如总连接数、错误数等。
- -l：只显示监听状态的套接字。
- -r：显示路由表信息。

## lsof

lsof (List Open Files) 命令是一个列出当前系统打开的所有文件（包括网络连接）的工具。该命令可以帮助系统管理员和用户查找哪些进程正在使用哪些文件或套接字。以下是一些常用的 lsof 命令选项：

```non
-p: 指定要查看的进程 ID。
-i: 显示所有打开的网络连接。
-u: 指定要查看的用户。
-c: 指定要查看的进程名称。
-t: 只显示进程 ID。
-a: 指定多个条件，需要同时满足。
-F: 指定输出格式，可选的格式包括 f（文件名）、p（进程 ID）、u（用户）、c（进程名称）等。
```

示例用法：

```bash
lsof: 列出系统中所有打开的文件和套接字。
lsof /path/to/file: 列出使用指定文件的进程。
lsof -i :80: 列出所有使用端口 80 的网络连接。
lsof -c nginx: 列出所有名为 nginx 的进程打开的文件和套接字。
lsof -p 1234: 列出进程 ID 为 1234 的进程打开的文件和套接字。
lsof -u user: 列出指定用户打开的文件和套接字。
lsof -a -u user -i: 列出指定用户打开的所有网络连接。
```

## nc

nc是一个网络工具，也称为netcat。它允许用户创建TCP/UDP连接、发送数据包以及监听网络端口。以下是nc的一些常用选项和用法：

```bash
nc -l <port>: 在指定端口上启动nc作为监听器。
nc <host> <port>: 连接到指定的主机和端口。
nc -u <host> <port>: 使用UDP协议连接到指定的主机和端口。
nc -v: 显示详细信息，包括传输的数据量和连接状态。
nc -z <host> <port>: 检查指定的主机和端口是否可用。
nc -e <command>: 执行指定的命令并将其输出发送到连接的另一端。
nc -c <command>: 执行指定的命令并将连接的输入发送到该命令的标准输入。
nc -h: 显示nc的帮助信息。
```

## tcpdump

tcpdump命令是一种常用的网络数据包分析工具，可用于捕获、分析和显示网络流量。它可以捕获经过网络接口的数据包，并将其展示在命令行终端中，以便用户可以对其进行分析和调试。

以下是一些常见的tcpdump选项：

```non
-i：指定要监视的网络接口
-n：禁止进行DNS解析
-v：输出详细的信息，包括协议头和数据包内容
-c：指定捕获的数据包数量
-s：指定要捕获的数据包的最大大小
-w：将捕获的数据包写入文件，以便后续分析
host：指定要捕获的数据包的源或目标主机
port：指定要捕获的数据包的源或目标端口
```

# 第六章 网络通信协议设计

## 6.2 如何解决粘包问题

如果使用TCP协议进行通信，多数情况下是不会出现丢包和乱序问题的。

粘包就是连续向对端发送两个或以上的数据包，对端在一次收取中收到的数据包数量可能大于一个，当大于1个时，可能时几个包加上某个包的部分，或者几个完整的包在一起。当然，也可能收到的只是一个包的部分，这种情况叫半包。

网络编程中的粘包问题是指发送端发送的多个数据包在接收端被当作一个数据包处理的现象。粘包问题通常是由于数据发送端发送数据速度过快，数据接收端没有及时接收数据或处理数据，导致数据累积在缓冲区中，从而形成粘包。

---

以下是几种解决粘包问题的常见方法：

1. 使用固定长度的消息格式：发送方每次发送消息时，把消息分成固定长度的数据块，接收方每次接收固定长度的数据块，并且按照固定长度解析数据块，这样即使发生粘包，也不会影响消息的正确解析。**灵活度差**

2. 在消息头部添加消息长度：发送方在发送消息前，先把消息的长度添加到消息头部，接收方先接收消息头部并解析出消息长度，再按照消息长度接收和处理数据。

3. 使用特殊字符分隔消息：发送方在消息结尾添加特殊字符，接收方根据特殊字符切分消息，这种方法需要保证特殊字符不会出现在消息中。**出现特殊字符需要转义，一般用于控制命令传输**

4. 使用时间间隔：在发送方发送数据时，可以设置发送间隔时间，接收方根据时间间隔判断消息的结束位置。

## 6.4 协议设计

### 6.4.1 TLV（type-length-value)

TLV全称是Type-Length-Value，它是一种数据格式，在网络通信中被广泛应用。它将数据分成三个部分：

- Type：表示数据的类型，通常使用一个字节来表示，例如0x01表示整数类型，0x02表示字符串类型等等。
- Length：表示数据的长度，通常使用一个字节或多个字节来表示，具体长度的表示方式可以根据实际情况来定义。
- Value：表示数据的值，也就是实际的数据内容。
通过TLV格式，数据发送方可以将数据按照一定的格式进行打包，接收方可以按照这个格式进行解包，从而实现数据的可靠传输。TLV格式的优点是灵活、易于扩展，可以根据需要自定义数据类型和长度，同时也方便了通信双方的数据解析。

### 6.4.2 协议分类

根据协议内容是否是文本格式，分为文本协议和二进制协议。像http的包头和FTP等都是文本协议。

### 6.4.3 协议设计工具

为了避免每一套新协议都需要重头编码调试的问题，出现了一种叫做IDL(interface description language)的语言规范，将协议的使用类型规范化，提供跨语言特性。可以定义一个描述协议格式的IDL文件，然后通过IDL工具分析IDL文件，就可以生成各种语言版本的协议代码。Google Protobuf库自带的工具protoc就是一个IDL工具。

### 6.4.4 整型数值的压缩

一个uint32的表示范围为0~4294967295，占4字节，在实际应用中可能用不到太大的值。

---

一字节共有8bit位，使用最高位作为标志位，说明一个整型数值是否到此字节结束，如果最高位为1，说明下一字节仍然是该整型的值。经过这样压缩后，一个整型的长度可能为1~5字节，但是因为实际数据中基本很少使用超过3字节的整数，因此还是比较实用。

## 6.6 设计通信协议的注意事项

- 字节对齐
- 显示定义整型字段长度，如：int32, int64
- 涉及浮点数时存在精度取舍不准确的问题，如果这个浮点数的业务单位较大（如亿），会造成很大的影响。建议在网络传输时将浮点数值放大相应倍数，变成整数或者转换为字符串来传输。
- 大小端问题
- 协议和自动升级功能。一定要保证自动升级协议的新旧兼容

> 对于测试不完善或者快速迭代的产品，只要保证自动升级功能正常，旧版本的任何Bug和瑕疵都可以通过升级新版本解决。

## 6.7 包分片

解决思路：

- 设置分片标志
- 在每个包分片的包头部分都有该包的总分片数量和当前分片编号。

```C++
#pragma pack(push, 1)
typedef struct tagNtPkgHead
{
    unsigned char bStartFlag;       //协议包起始标志 0xFF
    unsigned char bVer;             //版本号
    unsigned char bEncryptFlag;     //加密标志
    unsigned char bFrag;            //分片标志
    unsigned short wLen;            //包总长
    unsigned short wCmd;            //命令号
    unsigned short wSeq;            //包序列号
    unsigned short wCrc;            //Crc校验码
    unsigned int dwSID;             //会话ID
    unsigned short wTotal;          //有分片时的总分片数
    unsigned short wCurSeq;         //有分片时的分片序号，从0开始
} NtPkgHead, *PNtPkgHead;
#pragma pack(pop)
```

## 6.10 HTTP

### 6.10.1 HTTP格式

HTTP由包头和包体组成，包头与包体之间使用一个\r\n（空行）分割。包头的每一行均以\r\n结束。所以HTTP包头以\r\n\r\n结束，可以通过它来界定包头的结束位置和包体的起始位置。

> html文档中的\<head>和\<body>标签是http包body(包体的一部分)。

### 6.10.3 HTTP chunk编码

Http在传输过程中如果包体过大，比如上传大文件、传输动态产生内容时，传输放无法预先知道传输内容有多大，这时候可以使用http chunk编码技术。

> HTTP chunk技术的原理是将HTTP包体分成多个小块，每一块都有自己的字段来说明自身长度，对端收到这些块后，去除说明部分，将多个小块合并在一起得到完成的包体内容。传输方在**HTTP包头中设置Transfer-Encoding:chunked"告诉对方这个数据时分块传输的。**

## 6.11 SMTP与POP3

### 6.11.1 相关协议简介

- SMTP (Simple Mail Transfer Protocol)：SMTP是用于发送电子邮件的协议，它负责将邮件从发件人的电子邮件服务器发送到收件人的电子邮件服务器。SMTP使用TCP端口25进行通信。

- POP3 (Post Office Protocol version 3)：POP3是用于接收电子邮件的协议，它负责从电子邮件服务器下载电子邮件到收件人的电子邮件客户端。当邮件被下载到收件人的电脑上后，通常会从服务器上删除。POP3使用TCP端口110进行通信。

- IMAP (Internet Message Access Protocol)：IMAP也是用于接收电子邮件的协议，与POP3不同的是，IMAP允许用户在服务器上管理邮件。这意味着用户可以在多个设备上查看和管理相同的电子邮件，而不仅仅是在一个客户端上。IMAP使用TCP端口143进行通信。

总之，SMTP用于发送电子邮件，POP3用于下载电子邮件，而IMAP允许用户在多个设备上查看和管理相同的电子邮件。

## 6.12 WebSocket协议

WebSocket协议是用于解决HTTP通信的无状态、短链接和服务端无法主动向客户端推送数据等问题而开发的新协议，其通讯基础也基于TCP。WebSocket协议的通信双方在进行TCP三次握手后，还需要额外进行一次握手，以确定浏览器支持WebSocket协议。这次握手的报文格式是基于HTTP改造的。

# 第7章 单个服务的基本结构

## 7.1 网络通信组件的效率问题

### 7.1.1 高校网络通信框架的设计原则

#### 1. 尽量少等待

1. 如何检测有新的客户端连接到来
2. 如何接受客户端的连接请求
3. 如何检测客户端是否有数据发送过来
4. 如何收取客户端发送的数据
5. 如何检测异常的客户端连接？检测到后如何处理
6. 如何向客户端发送数据
7. 如何在客户端发送完数据后关闭连接

减少等待的关键技术就是IO复用技术。

#### 2. 尽量减少做无用功的时间

IO复用函数可以分为以下两个级别：

1. select和poll
2. WSAAsyncSelect、WSAEventSelect、IOCP、epoll

尽量不要主动查询各个socket事件，而是等待操作系统通知我们。

#### 3. 检测网络事件的高效做法

在高性能服务器设计中一般将socket设置成非阻塞模式，利用IO复用函数检测各个socket上的事件。

对于问题1、2，默认情况下，如果没有新的客户端连接请求，则对监听socket调用accept函数会阻塞调用线程，使用IO复用函数以后，如果epoll_wait检测到监听socket有EPOLLIN事件，或者WSAAsyncSelect检测到有FD_ACCEPT事件，就表明此时有新连接到来，在调用accept函数就不会阻塞调用线程。

对于问题3、4，调用accept函数返回的新socket也应该设置为非阻塞模式，而且应该在epoll_wait或WSAAsyncSelect函数报告这个socket有可读事件时收取数据。

对于问题5，同样使用IO复用函数注册异常事件监听，异常处理一般是关闭socket。TCP连接是状态机，IO复用函数一般无法检测出两个端点之间路由错误导致的链路问题，对于这种情况需要通过定时器结合心跳包来检测。

对于问题6，对于epoll模型的水平出发模式（LT），我们首先不能像检测读事件一样一开始就注册检测写事件标志，否则会导致频繁触发写事件通知。正确做法：如果有数据需要发送，先调用send、write函数尝试直接发送；如果发送不了或者只发送了部分数据，则将剩余数据先缓存起来，再为这个socket注册写事件标志，等待下次写事件触发通知后继续发送数据；数据发送完后，及时为socket移除检测写事件标志，避免触发无用的写事件通知。

### 7.1.3 长连接和短链接

长连接通常操作为：连接-->数据传输-->保持连接-->...->关闭连接。再没有数据交换时需要定时发送保活心跳包，以维持连接状态。长连接主要用于通信双方频繁通信的场景，缺点时通信双方要增加相应的逻辑维护连接的状态，连接信息本身也需要一定的系统消耗；优点是可以进行实时数据交换。

短链接一般用于数据传输完成后即可关闭或者对通信双方的状态信息实时性要求不高的场景。优点时通信双方无需长时间维持连接状态信息；缺点是如果传输数据频率高，则需要频繁建立和关闭连接，也无法实时推送消息。

## 7.2 原始的服务器结构

```C++
int main()
{
    //1.初始化
    while(true)
    {
        //2.利用accept函数接受连接，产生客户端fd
        //3.利用客户端fd进行通信
    }
    //4.资源清理
    return 0;
}
```

每轮循环都只能处理一个客户端连接请求，不支持并发。

## 7.3 一个连接对应一个线程模型

为每个新的连接都创建一个线程去处理。当连接数达到一定量时，会创建很多处理线程，CPU在线程间切换的开销也十分巨大。

## 7.4 Reactor模式

Reactor模式（反应器模式）是一种事件处理设计模式，在IO请求到达后，服务处理程序使用IO复用技术同步的将这些请求派发给相关的请求处理程序。

一个Reactor模式结构一般包括以下模块；

1. 资源请求事件(Resource Request)
2. 多路复用器与事件分发器(IO Demultiplexer & Event Dispatcher)
3. 事件处理器(EventHandler)

## 7.5 one thread on loop思想

### 7.5.1 基本结构

```C++
void* thread_func(void* thread_arg)
{
    //初始化工作
    while(线程退出标志)
    {
        //1. 利用IO复用技术分理处读写事件
        //2. 处理读写事件
        //3. 其他事情
    }
    //清理工作
}
```

### 7.5.2 线程的分工

```C++
while(!m_bQuitFlag)
{
    epoll_or_select_func();
    handle_io_events();
    handle_other_things();
}
```

在上述while循环里面，eopll_or_select_func()中的函数一般设置了一个超时时间。如果超时时间设置为0，则在没有网络IO事件和其他任务处理的情况下，这些工作线程实际上会空转；而如果超时时间设置大于0，则在没有网络IO事件但handle_other_things()中有任务需要处理时，IO复用函数需要挂起指定时间后才能返回，导致handle_other_things()不能立即执行。

解决方案是，仍然会给select,poll,epoll设置一定超时时间，但是对于handle_other_things()采取一种特殊的唤醒策略。以Linux的epoll_wait()为例，不管epollfd上有没有socket fd，我们都会为epollfd挂载一个特殊的fd，被称为wakeup fd（唤醒fd）。当有其他任务需要立即处理时，向这个唤醒fd随便写入1字节的内容，这个唤醒fd就变为可读的了，epoll_wait()函数会被立即唤醒并返回，接下来就可以马上执行handle_other_things()函数，这样其他任务就可以立即处理。

### 7.5.3 唤醒机制实现

#### Linux实现

1. 使用管道
2. 使用Linux2.6新增的eventfd
3. 使用socketpair

#### Windows实现

在windows上使用select函数作为IO复用函数时，由于windows的select函数只支持绑定套接字这种类型的句柄，因此一般只能模范Linux创建socketpair的思路。

### 7.5.6 one thread one loop的效率保障

除了epoll_or_select_func步骤中的IO服用函数可能会造成登台，其他步骤中不能有阻塞整个流程或者耗时的操作。

## 7.6 收发数据的正确做法

对于epoll的LT模式，可以一次收取部分数据，但是对于ET模式，必须在读事件出发后将fd上的数据全部读完。收取完数据的标志时recv/read函数，返回-1，错误码为EWOULDBLOCK.

除ET模式外，其他IO复用函数发送数据时，一般都是先调用send或write函数直接发送，如果发送过程中send函数返回-1，并且错误码时EWOULDBLOCK，则表明此时TCP窗口太小，数据无法再发送。如果仍然有剩下数据未发送，才会为clientfd注册监听可写事件标志，并将剩余的数据存入自定义的socket发送缓冲区，等到写事件触发后将剩余数据发送。当没有需要发送的数据时，需要立即移除对应的写事件监听。这是目前主流网络库的做法。

### 7.6.3 不要多个线程同时利用一个socket收发数据

不要多个线程同时使用一个socket发送数据，或者多个线程同时使用一个socket收取数据。这是因为TCP数据是有序的，如果多个线程同时对一个socket调用send函数，最终对端接收到的数据顺序就无法保证了。

## 7.7 发送、接受缓冲区的设计要点

无论是发送还是接受缓冲区，一般都建议将其设计成一个内存连续的存储容器，并且可以动态增加容量。

对于服务端程序来说，由于要同时服务多个客户端，而每一路连接都会有一个接收缓存区和发送缓存区。所以缓冲区的容量上限一般是有限制的，初始容量一般设置的不会很大。

## 7.8 网络库的分层设计

### 7.8.1 网络库设计中的各个层

常见的网络通信库根据功能可以分为：Session层、Connection层、Channel层、Socket层。其中Session层属于业务层，其他属于技术层。

#### Session层

Session层属于顶层，在设计上不属于网络框架本身，用于记录各种业务状态数据和处理各种业务逻辑。在业务逻辑处理完毕后，则依赖Connection层进行数据收发。

Session对象并不拥有Connection对象，不控制其生命周期，因为虽然Session对象的主动销毁会引起Connection对象的销毁，但是Connection对象本身也可能因为网络出错等原因被销毁，进而引起Session对象被销毁。因此会使用std::weak_ptr来引用TCPConnection对象。

> std::weak_ptr是C++11中的一个智能指针（smart pointer），用于管理动态内存的分配和释放。与std::shared_ptr不同的是，std::weak_ptr不会增加指向对象的引用计数，因此它并不拥有对象的所有权，也不会防止对象被释放。相反，它只是提供了一种访问由std::shared_ptr管理的对象的非拥有性（non-owning）的方式。

下面是std::weak_ptr的几个特性：

- 非拥有性：std::weak_ptr并不拥有对象的所有权，它只是提供了一种访问由std::shared_ptr管理的对象的非拥有性的方式。这意味着当最后一个std::shared_ptr被销毁时，对象可能已经被释放，std::weak_ptr也不能访问对象。

- 安全转换：std::weak_ptr可以安全地转换为std::shared_ptr，以访问对象。如果对象已经被释放，则返回一个空的std::shared_ptr。转换可以通过lock()成员函数实现。

- 软连接：std::weak_ptr可以用作指向由std::shared_ptr管理的对象的软连接（weak reference）。软连接不会增加对象的引用计数，因此它可以用于避免循环引用（circular reference）。

- 异常安全：std::weak_ptr的创建和使用是异常安全的。如果由于异常而导致对象没有被正确释放，std::weak_ptr仍然可以正常工作，并且不会引发未定义行为。

总之，std::weak_ptr是一种非拥有性的智能指针，它可以用于安全地访问由std::shared_ptr管理的对象，避免循环引用，并提供异常安全的代码。

#### Connection层

Connection层是奇数层的顶层，每一路客户端连接都对应一个Connection对象，该层一般用于记录连接的各种状态信息。**常见的状态信息有连接状态、数据收发缓冲区信息、数据流量信息、本段和对端的地址和端口号信息等，同时提供对各种网络事件的处理接口，这些接口或自己使用或被Session层使用。Connection层持有一个Channel对象，掌管Channel对象的生命周期。**

#### Channel层

Channel层一般持有一个socket句柄，是实际进行数据收发的地方，因而一个Channel对象会记录当前需要监听的各种网络事件的状态，同时提供对这些事件状态的查询和增删改接口。在一些库设计中，Channel对象管理着socket对象的生命周期，因此Channel对象需要提供创建和关闭socket对象的接口。

一般来说，收发操作会被放在同一个线程中进行，这样可以避免在收发过程中socket状态改变时，对另一个操作产生影响。

#### Socket层

这一层通常只是对常用的socket函数进行封装，屏蔽不同操作系统socket函数的差异性来实现跨平台，方便上层使用。

### 7.8.3 连接信息与EventLoop/Thread的对应关系

综合各层对象，一个socketfd值对应一个channel对象、一个Connection对象和一个Session对象，这组对象构成了一路连接信息。每一路连接信息都只能属于一个loop，也就是说只属于某个线程；反过来，一个loop或者线程可以拥有多个连接信息，这就**保证了我们只会在同一个线程里面处理特定的socket收发事件。**

## 7.9 后端服务的定时器设计

定时器模块是后端服务常用的功能之一，用于周期性的执行某些任务的场景中。设计定时器模块的方法很多，但关键是解决其效率问题。

### 7.9.2 定时器设计的基本思路

定时器对象一般需要一个唯一标识、过期时间、重复次数、定时器到期时的触发动作。

#### 定时器对象结合的数据结构优化

为了提高定时器的效率，一般采用两种常用方法：时间轮（拉链法）和时间堆（小根堆）。

### 7.9.4 对时间的缓存

大多数系统上，获取系统时间的函数都属于系统调用，为了提高效率，在一些对事件要求精度不是特别高的情况下，我们可能会缓存一些时间，在较近的下次如果需要系统时间，则可以使用上次缓存的时间，而不是再次调用获取系统时间的函数。

#### NOTES

定时器的设计还有其他一些需要考虑的问题，例如定时器逻辑如何解决服务器及其实践被人为提前或延后，以及定时器事件的事件精度等问题。

## 7.11 非侵入式结构与侵入式结构

根据通信数据从网络框架中流出流入情况，将服务程序的结构分为非侵入式结构和侵入式结构。

### 7.11.1 非侵入式结构

一个服务中的所有通信或业务数据都在网络通信框架内流动，没有外部数据源注入网络通信模块或者从网络通信模块流出。

### 7.11.2 侵入式结构

如果有外部消息流入网络通信模块或从网络通信模块流出，就相当于有外部消息侵入网络通信结构，将这种服务器结构成为侵入式服务结构。

通用结构：网络解包后需要将任务交给专门的业务处理线程处理，处理完后需要再次通过网络通信组件发出去。

# 第9章 服务器开发中的常用模块设计

## 9.1 断线自动重连的应用场景和逻辑设计

服务器程序之间的重连可以被设计成等待时间间隔的定时重连，对于客户端程序，要结合依次放大重连时间间隔、网络状态变化时立即重连或用户主动发起重连三个因素。

---

对于客户端软件，如果发生网络波动，程序就应该检测网络状态，如果网络状态恢复正常，就应该立即进行一次重连，而不是一成不变的按照设置的时间间隔重连。

## 9.2 保活机制与心跳包

由于TCP连接是状态机，所以对于链路中的问题，无论是客户端还是服务器都无法感知与对方的连接是否正常，这类连接称为“死链”；

---

一个客户端在连接服务器以后，如果长时间没和服务器有数据来往，可能会被防火墙程序关闭，有时候我们不想连接被关闭。

心跳检测一般有两个作用：保活和检测死链。

### 9.2.1 TCP keepalive选项

操作系统的TCP/IP协议栈提供了keepalive选项用于socket的保活。

Linux上，可以通过设置socket选项来实现心跳检测。例如，可以使用SO_KEEPALIVE选项来开启TCP心跳检测功能。当SO_KEEPALIVE选项被设置为1时，操作系统会在TCP连接上启动一个计时器，定期发送一些探测报文段来检测连接是否还存活。如果在一定的时间内没有收到回应，就会认为连接已经断开。可以通过设置SO_KEEPIDLE、SO_KEEPINTVL和SO_KEEPCNT选项来控制心跳检测的时间间隔和次数等参数。

```C++
int optval = 1;
setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
int keepalive_idle = 60;  // 60s
setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, &keepalive_idle, sizeof(keepalive_idle));
int keepalive_intvl = 10; // 10s
setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, &keepalive_intvl, sizeof(keepalive_intvl));
int keepalive_cnt = 3;    // 3次
setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, &keepalive_cnt, sizeof(keepalive_cnt));
```

### 9.2.2 应用层的心跳包机制设计

在使用keepalive选项时，需要对每个连接中的socket进行设置，而这不一定是必须的，可能产生大量无意义的带宽浪费，而且keepalive选项不能与应用层很好的交互，因此在实际的服务开发中还是建议在应用层自己设计心跳包机制。

> 一般是客户端主动向服务端发送心跳包，服务端做心跳检测来决定是否断开连接。如果客户端不能主动何其保持连接，就会主动回收与该客户端的连接。当然，服务端在收到客户端的心跳包时，应该给客户一个心跳应答。

### 9.2.3 有代理的心跳包机制

将更新数据包的时间戳机制改为只能通过后端服务的上行数据来统计，这样超过某段时间后若仍然没有上行数据，则说明客户端已经断开（这段时间内，服务端既没有收到客户端的业务数据包，也没有收到客户端的心跳包。）

### 9.2.4 带业务数据的心跳包

在实际应用中，可以在心跳包数据结构中加上需要的业务字段信息。

### 9.2.6 心跳包与调试

一般在调试模式下通过配置开关变量或者条件编译选项关闭心跳包检测逻辑。

```C++
if(config.heartbeatCheckEnabled)
    EnableHeartbeatCheck();
//或者
#ifndef _DEBUG
    EnableHeartbeatCheck();
#endif
```

## 9.3 日志模块设计

### 9.3.2 日志系统的技术实现

#### 1. 同步写日志

同步写日志指在输入日志的地方将日志即时写入文件中。一般用于低频写日志的软件系统。

**在类UNIX系统上，同一个进程内针对同一个FILE*的操作是线程安全的，而在Windows上不是。**

#### 2. 异步写日志

对于QPS要求很高或者对性能有一定要求的服务器程序，同步写日志会对服务的关键性逻辑的快速执行和及时响应带来一定的性能损失。因此我们可以采用异步写日志。

---

异步写日志在产生日志的地方不会将日志实时写入文件中，而是通过一些线程同步技术将日志暂存下来，然后通过一组专门的日志写入线程将其写入磁盘。本质上是一个生产者消费者模型。

### 9.3.8 一条日志应该包含什么内容

日志记录应该尽量精炼、详细，反映当时出错的现场参数、产生的环境等信息。但是不要出现敏感信息，如IP地址，用户名密码等。

## 9.5 监控端口

在实际项目中，出于定位问题和统计数据要求，往往会给一些正在运行的服务开一些对外的口子。可以在服务运行过程中查询和修改服务内存中的一些信息。

