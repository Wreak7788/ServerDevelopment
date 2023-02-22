//
// Created by ljx on 23-2-22.
//
#include <sys/types.h>
#include "arpa/inet.h"
#include "sys/socket.h"
#include "unistd.h"
#include "string.h"
#include "iostream"


#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA "helloworld"

int main()
{
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientFd == -1)
    {
        std::cout << "create socket error" << std::endl;
        return -1;
    }
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_HOST);
    serverAddr.sin_port = htons(SERVER_PORT);
    if(connect(clientFd, (struct sockaddr*)&serverAddr, serverAddrLen) == -1)
    {
        std::cout << "connect error" << std::endl;
        close(clientFd);
        return -1;
    }

    //不断向服务器发送数据
    int count = 0;
    while(true)
    {
        int ret = send(clientFd, SEND_DATA, strlen(SEND_DATA), 0);
        if(ret != strlen(SEND_DATA))
        {
            std::cout << "send data error." << std::endl;
            break;
        }
        else
        {
            count++;
            std::cout << "send data successfully, count= " << count << std::endl;
        }
    }
    close(clientFd);
    return 0;
}