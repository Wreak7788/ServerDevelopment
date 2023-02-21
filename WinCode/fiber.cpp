
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