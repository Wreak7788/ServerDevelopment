//
// Created by ljx on 23-2-23.
//
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "iostream"

bool connectToServer(const char* server, short port)
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1)
        return false;
    struct sockaddr_in addr = {0};
    struct hostent* pHostEnt = NULL;

    if(addr.sin_addr.s_addr = inet_addr(server) == INADDR_NONE)
    {
        pHostEnt = gethostbyname(server);
        if(pHostEnt == NULL)
            return false;
        addr.sin_addr.s_addr = *((unsigned long*)pHostEnt->h_addr_list[0]);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int ret = connect(socketFd, (struct sockaddr*)&addr, sizeof addr);
    if(ret == -1)
        return false;
    return true;
}

int main()
{
    std::string hostName;
    short port;
    while(1)
    {
        std::cin >> hostName;
        std::cin >> port;
        if (connectToServer(hostName.c_str(), port))
            std::cout << "connect to " << hostName << ":" << port << std::endl;
        else
            std::cout << "error" << std::endl;
    }
}