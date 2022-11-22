
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
//在实际开发中有时需要在类中包裹返回当前对象的一个shared_ptr给外部使用，有此要求的类只需要继承自std::enable_shared_from_this<T>模版对象即可
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
//thread为输出参数，可以得到线程id；attr指定线程属性，NULL表示使用默认属性；
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