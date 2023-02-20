
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