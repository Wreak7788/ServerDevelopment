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
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientFd = accept(listen_fd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if(clientFd != INVALID_FD)
        {
            //只接受连接，不调用recv
            std::cout << "accept a client connection" << std::endl;
        }
    }
    close(listen_fd);
    return 0;
}