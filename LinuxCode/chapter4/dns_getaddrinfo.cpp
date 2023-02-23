//
// Created by ljx on 23-2-23.
//
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "iostream"
#include "string.h"
#include "stdlib.h"

bool dns_server(const char* server, short port)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    /*
    AI_PASSIVE：用于服务器应用程序，表示返回绑定到任何可用网络地址的套接字地址结构。
    AI_CANONNAME：在返回的addrinfo结构体中，ai_canonname成员指向规范化的主机名。
    AI_NUMERICHOST：表示host参数为一个数字字符串，不要进行主机名或服务名的解析。
    AI_NUMERICSERV：表示service参数为一个数字字符串，不要进行主机名或服务名的解析。
    AI_ADDRCONFIG：表示只返回适用于主机所在网络协议族的地址。
    AI_V4MAPPED：表示如果没有找到IPv6地址，则返回一个IPv4映射的IPv6地址。
     */
    hints.ai_flags = AI_CANONNAME;

    int n = getaddrinfo(server, "http", &hints, &res);
    if(n != 0)
    {
        std::cout << "n:" << n << std::endl;
        return false;
    }
    if(res == NULL)
    {
        std::cout << "res is nullptr" << std::endl;
        return false;
    }
    std::cout << res->ai_canonname << std::endl;
    std::cout << res->ai_addr << std::endl;
    return true;
}

int main()
{
    char* host = "baidu.com";
    dns_server(host, 80);
    return 0;
}