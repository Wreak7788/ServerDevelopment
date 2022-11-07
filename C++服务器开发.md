
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

使用C++11新引入的对象std::initializer_list\<T\>在自定义类中支持花括号语法：
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

std::initializer_list\<T\>还提供了三个成员函数：
- size_type size() const;   //返回列表中的元素个数
- const T* begin() const;   //返回第一个元素指针
- const T* end() const; //返回最后一个元素的下一个元素位置，代表结束

### 1.1.2 C++17注解标签(attributes)

 使用注解标签的语法：
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

- override：子类重写父类的方法

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

- =default语法：让编译器自动生成构造/析构/拷贝构造函数和operator=这几类函数

```C++
class A{
public:
    A() = default;
    ~A() = default;
}
```
- =delete语法：禁止编译器生成构造/析构/拷贝构造函数和operator=这几类函数
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
对象至少实现如下两个方法：
- Iterator begin();
- Iterator end();

以上的Iterator是自定义数据类型的迭代子类型，必须支持如下三种操作：
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

- auto_ptr：C++98，在C++11被弃用，当复制一个auto_ptr对象时，原auto_ptr对象所持有的堆空间会被转移给新的auto_ptr对象。**应该避免在容器中使用。**

- unique_ptr:禁止了复制语义，对持有的堆内存具有唯一拥有权。
  - 自定义资源释放函数：std::unique_ptr\<T, DeletorFuncPtr>，DeletorFuncPtr是一个函数指针。

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
