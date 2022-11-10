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