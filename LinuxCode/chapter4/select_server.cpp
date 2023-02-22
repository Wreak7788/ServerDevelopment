//
// Created by ljx on 23-2-22.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <errno.h>
#include <string.h>

#define INVALID_FD -1

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == INVALID_FD)
    {
        std::cout << "create listen_fd error." << std::endl;
        return -1;
    }

    struct sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port = htons(3000);
    if(bind(listen_fd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listen_fd);
        return -1;
    }

    if(listen(listen_fd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listen_fd);
        return -1;
    }

    std::vector<int> client_fds;
    int maxfd;

    while(true)
    {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(listen_fd, &readSet);
        maxfd = listen_fd;

        int clientFdsLength = client_fds.size();
        for(int i = 0; i < clientFdsLength; ++i)
        {
            if(client_fds[i] != INVALID_FD)
            {
                FD_SET(client_fds[i], &readSet);
                if(maxfd < client_fds[i])
                    maxfd = client_fds[i];
            }
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;
        int ret = select(maxfd+1, &readSet, NULL, NULL, &tm);
        if(ret == -1)
        {
            if(errno != EINTR)
                break;
        }
        else if(ret == 0)
        {
            //select 函数超时
            continue;
        }
        else
        {
            if(FD_ISSET(listen_fd, &readSet))
            {
                //如果是监听socket的可读事件，说明有新连接
                struct sockaddr_in clientAddr;
                socklen_t  clientAddrLen = sizeof(clientAddr);
                int clientFd = accept(listen_fd, (struct  sockaddr*)&clientAddr, &clientAddrLen);
                if(clientFd == INVALID_FD)
                {
                    break;
                }
                std::cout << "accept a client connection, fd:" << clientFd << std::endl;
                client_fds.push_back(clientFd);
            }
            else
            {
                char recvBuf[64];
                int clientFdsLength = client_fds.size();
                for(int i = 0;i < clientFdsLength; ++i)
                {
                    if(client_fds[i] != INVALID_FD && FD_ISSET(client_fds[i], &readSet))
                    {
                        //如果其他socket的读事件则是客户端发来的消息
                        memset(recvBuf, 0, sizeof(recvBuf));
                        int length = recv(client_fds[i], recvBuf, 64, 0);
                        //recv函数返回0说明对端关闭了连接
                        if(length <= 0)
                        {
                            std::cout << "recv data error, clientfd:" << client_fds[i] << std::endl;
                            close(client_fds[i]);
                            client_fds[i] = INVALID_FD;
                            continue;
                        }
                        std::cout << "clientfd: " << client_fds[i] << ", recv data:" << recvBuf << std::endl;
                    }
                }
            }
        }
    }
    int clientFdsLength = client_fds.size();
    for(int i = 0; i < clientFdsLength; ++i)
    {
        if(client_fds[i] == INVALID_FD)
        {
            close(client_fds[i]);
        }
    }
    close(listen_fd);
    return 0;
}