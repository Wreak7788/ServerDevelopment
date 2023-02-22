//
// Created by ljx on 23-2-22.
//
#include "sys/epoll.h"
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

    int epollFd = epoll_create(1);
    if(epollFd == INVALID_FD)
        return -1;
    struct epoll_event event, events[64];
    event.data.fd = listenFd;
    event.events = EPOLLIN;

    if(epoll_ctl(epollFd, EPOLL_CTL_ADD, listenFd, &event) == -1)
        return -1;

    while (1)
    {
        int nfd = epoll_wait(epollFd, events, 64, 1000);
        if(nfd == 0)
            continue;
        else if(nfd == -1)
        {
            if(errno == EINTR)
                continue;
            break;
        }
        for(size_t i = 0; i < nfd; ++i)
        {
            if(events[i].events & EPOLLIN)
            {
                //读事件
                if(events[i].data.fd == listenFd)
                {
                    struct sockaddr_in clientAddr;
                    socklen_t clientAddrLen = sizeof clientAddr;
                    int clientFd = accept4(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen, SOCK_NONBLOCK);
                    if(clientFd == -1)
                    {
                        std::cout << "accept error." << std::endl;
                        continue;
                    }
                    epoll_event clientEvent;
                    clientEvent.data.fd = clientFd;
                    clientEvent.events = EPOLLIN;
                    epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &clientEvent);
                }
                else
                {
                    //client读事件
                    char buf[64] = {0};
                    int nrecv = recv(events[i].data.fd, buf, 64, 0);
                    if(nrecv == 0)
                    {
                        std::cout << "disconnect to client:" << events[i].data.fd << std::endl;
                        //对方关闭了连接
                        events[i].data.fd = INVALID_FD;
                        epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        close(events[i].data.fd);
                    }
                    else if(nrecv < 0)
                    {
                        if(errno == EINTR || errno == EWOULDBLOCK)
                            continue;
                        events[i].data.fd = INVALID_FD;
                        epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        close(events[i].data.fd);
                    }
                    else
                        std::cout << "recv data: " << buf << " from clientFd:" << events[i].data.fd << std::endl;
                }
            }
        }
    }
    return 0;
}