
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