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