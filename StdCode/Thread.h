#ifdef __WIN32
#include<windows.h>
typedef HANDLE THREAD_HANDLE;
#else
#include<pthread.h>
typedef pthread_t THREAD_HANDLE;
#endif

class CThread
{
public:
    CThread();
    virtual ~CThread();

    //创建线程
    virtual bool Create();

    //获得本线程对象存储的线程句柄
    THREAD_HANDLE GetHandle();

    //线程睡眠
    void OSSleep(int nSecond);
    void SleepMs(int nMilliseconds);
    bool Join();
    bool IsCurrentThread();
    void ExitThread();

private:
#ifdef __WIN32
    static DWORD WINAPI _ThreadEntry(LPVOID pParam);
#else
    static void* _Thread_Entry(void* pParam);
#endif

    virtual bool InitInstance();

    virtual void ExitInstance();

    //纯虚函数，子类必须重写，线程函数的实际业务逻辑
    virtual void Run() = 0;     

private:
    //线程句柄
    THREAD_HANDLE m_hThread;
    DWORD m_IDThread;
};