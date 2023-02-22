//
// Created by ljx on 23-2-22.
//
#include "poll.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "fcntl.h"
#include "string.h"
#include "errno.h"
#include "iostream"
#include "unistd.h"
#include "vector"

#define INVALID_FD -1

int main()
{
    int listenFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(listenFd == INVALID_FD)
    {
        std::cout << "fail to create socket" << std::endl;
        return -1;
    }

    //复用地址和端口号
    int on = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof on);
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEPORT, (char*) &on, sizeof on);

    struct sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port = htons(3000);

    if(bind(listenFd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) == -1)
    {
        close(listenFd);
        return -1;
    }
    if(listen(listenFd, SOMAXCONN) == -1)
    {
        close(listenFd);
        return -1;
    }

    std::vector<pollfd> fds;
    pollfd listenFdInfo;
    listenFdInfo.fd = listenFd;
    listenFdInfo.events = POLLIN;
    listenFdInfo.revents = 0;
    fds.push_back(listenFdInfo);

    bool existInvalidFd;
    int n;

    while (true)
    {
        existInvalidFd = false;
        n = poll(&fds[0], fds.size(), 1000);
        if(n < 0)
        {
            if(errno == EINTR)
                continue;
            break;
        }
        else if (n == 0)
            continue;
        for (size_t i = 0; i < fds.size(); ++i)
        {
            if(fds[i].revents & POLLIN)
            {
                if (fds[i].fd == listenFd)
                {
                    struct sockaddr_in clientAddr;
                    socklen_t clientAddrLen = sizeof clientAddr;
                    int clientFd = accept4(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen, SOCK_NONBLOCK);
                    if(clientFd != -1)
                    {
                        struct pollfd clientFdInfo;
                        clientFdInfo.fd = clientFd;
                        clientFdInfo.events = POLLIN;
                        clientFdInfo.revents = 0;
                        fds.push_back(clientFdInfo);
                        std::cout << "new client accepted fd:" << clientFd << std::endl;
                    }
                }
                else
                {
                    char buf[64] = {0};
                    int m = recv(fds[i].fd, buf, 64, 0);
                    if(m <= 0)
                    {
                        if(errno != EINTR && errno != EWOULDBLOCK)
                        {
                            //出错或对方关闭了连接
                            for(auto iter = fds.begin(); iter != fds.end(); ++iter)
                            {
                                if(iter->fd == fds[i].fd)
                                {
                                    std::cout << "client disconnected, clientfd:" << fds[i].fd << std::endl;
                                    close(fds[i].fd);
                                    iter->fd = INVALID_FD;
                                    existInvalidFd = true;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        std::cout << "recv from client: " << buf << ", clientFd:" << fds[i].fd << std::endl;
                    }
                }
            }
            else if(fds[i].revents & POLLERR)
            {
                //code
            }
        }
        if (existInvalidFd)
        {
            for(auto iter=fds.begin(); iter != fds.end(); ++iter)
            {
                if(iter->fd == INVALID_FD)
                    iter = fds.erase(iter);
                else
                    ++iter;
            }
        }
    }

    for(auto iter=fds.begin(); iter != fds.end(); ++iter)
        close(iter->fd);
    return 0;
}