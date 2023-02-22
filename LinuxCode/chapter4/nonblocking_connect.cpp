//
// Created by ljx on 23-2-22.
//
#include <sys/types.h>
#include "arpa/inet.h"
#include "sys/socket.h"
#include "unistd.h"
#include "string.h"
#include "iostream"
#include "fcntl.h"
#include "errno.h"


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
    //设置非阻塞，不能在创建时设置，否则会影响connect行为
    int oldSockFl = fcntl(clientFd, F_GETFL, 0);
    int newSockFl = oldSockFl | O_NONBLOCK;
    if(fcntl(clientFd, F_SETFL, newSockFl)==-1)
    {
        std::cout << "set socket to nonblock error" << std::endl;
        close(clientFd);
        return -1;
    }

    for(;;)
    {
        int ret = connect(clientFd, (struct sockaddr*)&serverAddr, serverAddrLen);
        if(ret == 0)
        {
            std::cout << "[connect] connect to server successfully." << std::endl;
        }
        else if(ret == -1)
        {
            if(errno == EINTR)
            {
                std::cout << "connect interrupted by signal." << std::endl;
                continue;
            } else if (errno == EINPROGRESS)
            {
                //正在连接
                std::cout << "connecting..." << std::endl;
                break;
            }
            else{
                std::cout << "unexpected error" << std::endl;
                close(clientFd);
                return -1;
            }
        }
    }

    fd_set writerSet;
    FD_ZERO(&writerSet);
    FD_SET(clientFd, &writerSet);
    struct timeval outTime = {3, 0};
    //调用select判断socket是否可写
    int fd = select(clientFd+1, NULL, &writerSet, NULL, &outTime);
//    std::cout << "clientFd:" << clientFd << " fd:" << fd << std::endl;
    if(fd != 1)
    {
        std::cout << "[select] connect to server error." << std::endl;
        close(clientFd);
        return -1;
    }
    int err;
    socklen_t len = static_cast<socklen_t>(sizeof err);
    if(::getsockopt(clientFd, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
    {
        close(clientFd);
        return -1;
    }
    if(err == 0)
        std::cout << "connect to server successfully." << std::endl;
    else
        std::cout << "connect to server failed." << std::endl;
    close(clientFd);
    return 0;
}