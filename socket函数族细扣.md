# 错误码及其含义

- EACCES：权限不足，通常是因为试图绑定低端口号（小于1024）时未获得足够的权限。  
- EADDRINUSE：地址已在使用中。通常是因为试图使用已在使用中的IP地址或端口号。  
- EAFNOSUPPORT：地址族不支持。通常是因为试图使用未被支持的地址族（如试图使用IPv6地址族，但系统并不支持IPv6）。  
- EINVAL：无效的参数。通常是由于提供给socket函数的某个参数无效。  
- EMFILE：达到了进程的文件描述符限制。通常是因为进程使用了太多的文件描述符。  
- ENFILE：达到了系统的文件描述符限制。通常是因为系统中使用的文件描述符太多。  
- ENOBUFS或ENOMEM：内存不足。通常是由于系统内存不足，无法为socket分配内存。  
- EPROTONOSUPPORT：协议不支持。通常是由于试图使用未被支持的协议。  
- EINTR：操作被中断。通常是由于在socket函数执行期间收到了一个信号。  
- EAGAIN或EWOULDBLOCK：非阻塞socket的操作正在进行，但当前没有数据可用或数据已经被读取完毕。  
- ECONNREFUSED：连接被拒绝。通常是因为目标主机在给定的端口上没有正在运行的服务。  
- ETIMEDOUT：连接超时。通常是由于连接远程主机时超时。  
- EHOSTUNREACH：目标主机不可达。通常是因为没有到达目标主机的网络路径。  
- EADDRNOTAVAIL：地址不可用。通常是因为提供给socket函数的IP地址不是本地IP地址。  

# socket函数

## 描述符

```C++
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

## 参数

### domain：网络协议族

- AF_UNIX, AF_LOCAL   Local communication              unix(7)  
- AF_INET             IPv4 Internet protocols          ip(7)  
- AF_INET6            IPv6 Internet protocols          ipv6(7)  
- AF_IPX              IPX - Novell protocols  
- AF_NETLINK          Kernel user interface device     netlink(7)  
- AF_X25              ITU-T X.25 / ISO-8208 protocol   x25(7)  
- AF_AX25             Amateur radio AX.25 protocol  
- AF_ATMPVC           Access to raw ATM PVCs  
- AF_APPLETALK        AppleTalk                        ddp(7)  
- AF_PACKET           Low level packet interface       packet(7)  
- AF_ALG              Interface to kernel crypto API  

### type：通信类型

- SOCK_STREAM     流式套接字，即 TCP 协议
- SOCK_DGRAM      数据报套接字，即 UDP 协议
- SOCK_SEQPACKET  提供面向连接的、可靠的、基于消息的服务。与SOCK_STREAM不同的是，SOCK_SEQPACKET保证发送的数据按照发送顺序接收，而且接收端每次接收一个完整的消息。这意味着SOCK_SEQPACKET可以避免TCP流传输中可能出现的粘包和分包问题。
- SOCK_RAW        原始套接字
- SOCK_RDM        通常用于提供可靠的数据传输服务。SOCK_RDM套接字可以发送和接收具有固定大小的数据包，但是这些数据包并不保证按照发送的顺序被接收端接收到，也不保证它们被正确接收。这种类型的套接字通常用于网络管理和控制应用程序，因为它们提供了比UDP更可靠的传输，但比TCP更灵活的选择性能和实时性。
- SOCK_PACKET     被弃用的套接字类型

**Linux2.6.27后新加：可以与上面的类型同时指定，使用|连接**

- SOCK_NONBLOCK   用于创建一个非阻塞的socket，即在进行网络I/O操作时，不会被阻塞住。这意味着在进行读写操作时，如果当前没有数据可读或写，则函数会立即返回，而不是一直等待数据的到来或数据被写入。

- SOCK_CLOEXEC    用于在调用 socket() 函数时指定创建的 socket 文件描述符会自动在执行 exec 时关闭（也就是在进程映像替换时，这个文件描述符不会传递给新程序）。这个选项通常和 O_CLOEXEC 文件描述符标记一起使用，以确保在程序中不会出现文件描述符泄漏的问题。这种技术可以用于安全地启动一个新进程而不会对父进程中的文件描述符产生影响。

### protocol：传输协议

当protocol参数为0时，系统会根据socket类型自动选择一个合适的协议。例如，如果type参数为SOCK_STREAM，那么默认使用IPPROTO_TCP协议；如果type参数为SOCK_DGRAM，那么默认使用IPPROTO_UDP协议。

- IPPROTO_TCP：TCP协议，提供面向连接的可靠传输服务；
- IPPROTO_UDP：UDP协议，提供无连接、不可靠的传输服务；
- IPPROTO_SCTP：SCTP协议，提供基于消息的面向连接的可靠传输服务，适用于多点通信；
- IPPROTO_ICMP：ICMP协议，提供网络控制和错误报文传输；
- IPPROTO_RAW：原始协议，允许应用程序直接访问底层协议头。

# bind函数

## 描述符

```C++
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

## 参数

- sockfd：表示需要绑定的套接字的文件描述符。
- addr：是一个指向sockaddr结构体的指针，表示需要绑定的本地地址。
- addrlen：是sockaddr结构体的长度。
- bind函数的作用是将sockfd所表示的套接字绑定到一个本地地址addr上。addr可以是以下几种类型：
    - struct sockaddr_in：IPv4地址结构体。
    - struct sockaddr_in6：IPv6地址结构体。
    - struct sockaddr_un：Unix域地址结构体。

在绑定之前，我们需要将addr结构体中的地址和端口号填写好。对于sockaddr_in和sockaddr_in6结构体，需要分别使用inet_pton函数将IP地址转换为网络字节序，并将端口号转换为网络字节序。而对于sockaddr_un结构体，需要将Unix域套接字的路径名填写好。

# listen函数

## 描述符

```C++
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```

## 参数

- sockfd 表示要监听的套接字描述符
- backlog 表示系统允许在该套接字上排队等待的连接数。在 Linux 中，这个参数的取值范围是 1 到 SOMAXCONN，其中 SOMAXCONN 是系统定义的一个宏，表示系统内核允许的最大连接数。

## NOTES

在使用listen()函数时，需要注意以下几点：

1. 在调用listen()函数前，必须先调用bind()函数将socket绑定到一个IP地址和端口号上。

2. 调用listen()函数后，该socket仅仅是一个被动socket，只能接受来自客户端的连接请求，而不能主动向其它socket发起连接请求。

3. 参数backlog指定了内核为该socket排队的最大连接请求数量，如果连接请求的数量超过了该值，那么连接将被拒绝。如果该值设置为0，那么内核不会为该socket排队连接请求，而是直接拒绝。

4. listen()函数成功返回0，失败返回-1，并设置errno变量。如果失败，需要根据errno的值判断出错的原因，并进行相应的处理。

5. 如果在listen()函数返回之后还想修改backlog的值，可以通过调用setsockopt()函数来实现。

# accept函数

## 描述符

```C++
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/socket.h>

int accept4(int sockfd, struct sockaddr *addr,
            socklen_t *addrlen, int flags);
```

## 参数

- sockfd：已创建并绑定到某个地址的socket文件描述符，通常是服务端socket的文件描述符。
- addr：指向sockaddr类型的指针，用于存储连接成功的对端的地址信息。如果不关心对端的地址信息，可以将该参数设为NULL。
- addrlen：指向socklen_t类型的指针，用于存储addr指向的地址信息结构体的大小。
- flags：
    - SOCK_NONBLOCK：表示创建的套接字是非阻塞的；
    - SOCK_CLOEXEC：表示创建的套接字在执行execve时会被关闭；
    - SOCK_CLOFORK：表示创建的套接字在执行fork时会被关闭。Linux 5.6

## NOTES

- accept函数仅适用于流式套接字（SOCK_STREAM）。
- 如果多个进程同时调用accept函数，那么同一时刻只会有一个进程获得连接，其他进程会被阻塞等待。
- accept函数返回的socket文件描述符继承了父进程的非阻塞和关闭-on-exec属性。
- 一旦成功建立连接，就可以使用新返回的socket文件描述符进行读写操作，不需要再使用listen函数进行监听。

> 在某些情况下，当多线程同时调用accept()函数时，会出现“惊群现象”，即多个线程都被唤醒去接受同一个连接请求，但只有一个线程能够成功接受。为了避免这种情况，可以使用accept4()函数，并将其标志参数设置为SOCK_CLOEXEC和SOCK_NONBLOCK，这样新创建的套接字将自动设置为非阻塞模式，并且在调用exec()函数时自动关闭，从而避免了惊群现象的发生。

# connect函数

## 描述符

```C++
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

## 参数

- sockfd：调用socket函数返回的套接字描述符。
- addr：指向目的地址信息的指针。
- addrlen：目的地址长度。

## NOTES

1. 在调用connect函数之前，必须先调用socket函数创建一个套接字，并使用bind函数将套接字绑定到一个本地地址（如果需要的话）。

2. 调用connect函数时，如果连接成功，套接字就可以用于读写数据；如果连接失败，返回-1并设置errno变量，此时不能使用该套接字进行数据传输。

3. 如果套接字是非阻塞的（使用fcntl函数或者sockfd选项设置O_NONBLOCK），调用connect函数可能会立即返回-1，并设置errno为EINPROGRESS。此时需要使用select或poll函数来等待连接完成或者出错。如果连接完成，则可以使用套接字进行数据传输；如果连接出错，则需要关闭套接字。

4. 在使用IPv4地址时，addr参数需要强制转换为(struct sockaddr *)类型。

# send函数族

send函数族是用于在TCP/IP协议中发送数据的函数。它们用于将数据从应用程序发送到与其通信的远程主机。在发送数据之前，必须先建立与目标主机的连接。  
其中，send函数是最基本的函数，用于在已经建立连接的套接字上发送数据。sendto函数则是用于在未建立连接的套接字上发送数据，并且可以指定数据的目的地址。sendmsg函数可以在已建立连接的套接字上发送数据，也可以在未建立连接的套接字上发送数据。

## 描述符

```C++
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen);

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);

/*
msg_name：指向目标套接字地址的指针。对于TCP和UDP套接字来说，通常为指向struct sockaddr结构体的指针，表示接收或发送数据的对端的地址。对于UNIX域套接字来说，通常为指向结构体sockaddr_un的指针，表示套接字的本地地址或目标地址。

msg_namelen：msg_name指针指向的地址的大小。

msg_iov：指向一个或多个iovec结构体的指针，表示要发送或接收的数据缓冲区。

msg_iovlen：msg_iov数组中iovec结构体的数量。

msg_control：指向一个缓冲区，用于传递辅助数据，例如控制消息、错误信息等。

msg_controllen：msg_control指针指向的缓冲区的大小。

msg_flags：用于控制消息的接收和发送方式，如是否使用OOB数据、是否忽略SIGPIPE信号等。
*/
struct msghdr {
               void         *msg_name;       /* optional address */
               socklen_t     msg_namelen;    /* size of address */
               struct iovec *msg_iov;        /* scatter/gather array */
               size_t        msg_iovlen;     /* # elements in msg_iov */
               void         *msg_control;    /* ancillary data, see below */
               size_t        msg_controllen; /* ancillary data buffer len */
               int           msg_flags;      /* flags (unused) */
           };
```

## 参数

- sockfd：表示要发送数据的套接字的文件描述符。
- buf：表示待发送的数据缓冲区的起始地址。
- len：表示待发送的数据的长度。
- flags：表示发送数据的选项。这些标志位可以按位或使用
   - MSG_OOB：用于发送带外数据。带外数据是指高优先级的数据，会在传输时优先处理。

   - MSG_DONTROUTE：不查找路由表，直接发送数据。一般用于本地网络测试。

   - MSG_NOSIGNAL：防止SIGPIPE信号被发送给进程。如果发送方向一个已关闭的socket发送数据，系统会向发送进程发送SIGPIPE信号，如果进程没有对该信号进行处理，就会使进程退出。使用MSG_NOSIGNAL可以避免这种情况发生。

   - MSG_MORE：提示内核发送数据时还有更多数据要发送，可以提高性能。

   - MSG_WAITALL：阻塞等待直到所有请求的数据都发送完毕。

   - MSG_CONFIRM：在UDP中，用于要求对方确认收到数据。对方如果没有回应，数据包会被重传。
- dest_addr：表示目的地址，只用于sendto函数。
- addrlen：表示目的地址的长度，只用于sendto函数。
- msg：表示待发送的消息结构体指针，只用于sendmsg函数。

# recv函数族

## 描述符

```C++
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```

## 参数

### flags

- MSG_DONTWAIT：指定接收操作不应该被阻塞，即使没有数据也是如此。
- MSG_ERRQUEUE：接收辅助错误消息。
- MSG_OOB：接收带外数据。
- MSG_PEEK：接收数据但不将其从接收队列中移除。
- MSG_TRUNC：截断接收数据，以适应指定的缓冲区大小。
- MSG_WAITALL：阻塞操作，直到指定的缓冲区被完全填充。

# select函数族

select函数是一个经典的IO多路复用函数，可以用于实现异步IO、并发服务器等功能。但是它也有一些缺点，比如每次调用都需要遍历所有需要监听的文件描述符，效率比较低；同时，它最大能够监听的文件描述符数量也有限制。因此，随着网络应用的不断发展，新的IO多路复用函数，如epoll，逐渐成为主流。

## 描述符

```C++
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, struct timeval *timeout);

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);

#include <sys/select.h>

int pselect(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask);
struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* microseconds */
};

and

struct timespec {
    long    tv_sec;         /* seconds */
    long    tv_nsec;        /* nanoseconds */
};

/*
(i)    select() uses a timeout that is a struct timeval (with seconds and microseconds), while pselect() uses a struct timespec (with sec‐onds and nanoseconds).
(ii)   select() may update the timeout argument to indicate how much time was left.  pselect() does not change this argument.
(iii)  select() has no sigmask argument, and behaves as pselect() called with NULL sigmask.
*/
```

## 参数

- nfds：需要监听的最大文件描述符值加1；
- readfds：需要监听可读事件的文件描述符集合；
- writefds：需要监听可写事件的文件描述符集合；
- exceptfds：需要监听异常事件的文件描述符集合；
- timeout：超时时间，如果为NULL，则表示一直阻塞等待事件发生；如果为0，则表示立即返回。
- sigmask：信号掩码，用于阻塞指定的信号。

## 返回值

如果文件描述符集合中至少有一个描述符准备好，select函数将返回可读/可写/错误的文件描述符数量。如果超时，select将返回0。如果出现错误，则返回-1，并设置errno错误号。

## 相关函数

1. FD_CLR(int fd, fd_set *set)：将fd从set中删除；
2. FD_ISSET(int fd, fd_set *set)：测试fd是否在set中，如果在则返回非零值，否则返回0；
3. FD_SET(int fd, fd_set *set)：将fd添加到set中；
4. FD_ZERO(fd_set *set)：将set清零；
5. FD_COPY(fd_set \*srcset, fd_set \*destset)：将srcset复制到destset中。

## NOTES

有些代码会调用select()，将三个集合都设置为空，nfds设置为零，并使用非空的timeout参数，作为一种比较可移植的以毫秒为单位的精度的睡眠方式。

---

pselect() 的必要性在于需要进行原子测试以避免竞态条件。例如，假设我们想要等待信号或文件描述符准备就绪，那么就需要原子测试来防止竞态条件的发生。

假设信号处理程序设置了一个全局标志并返回。然后，在进行此全局标志测试并在调用select()之前，如果信号到达时间介于测试之后但在调用之前，则此全局标志测试后的select()调用可能无限期地挂起。与之相反，pselect() 允许我们首先阻止信号，处理已经到达的信号，然后使用所需的 sigmask 调用 pselect()，从而避免了竞争条件的发生。

---

> 在 Linux 上，select() 函数会修改传入的 timeout 参数，使其反映未休眠的时间量；而其他实现则不会这样做。（POSIX.1 允许任一种行为。）这会导致两个问题：一是在将读取 timeout 参数的 Linux 代码移植到其他操作系统时会出现问题；二是在重复使用同一个 timeval 结构体进行多次 select() 的循环时，如果没有重新初始化 timeout，那么代码移植到 Linux 上也会出现问题。因此，需要将 timeout 视为在 select() 返回后为未定义状态。

---

若一个文件描述符在另一个线程中被关闭，则在不同的操作系统中 select() 的行为可能会有所不同。在某些 UNIX 系统上，select() 会解除阻塞并返回一个指示文件描述符已准备就绪的结果（后续的 I/O 操作可能会因错误而失败，除非在 select() 返回和 I/O 操作之间另一个文件描述符被重新打开）。而在 Linux（和其他一些系统）中，在另一个线程中关闭文件描述符对 select() 没有影响。因此，任何依赖这种场景下特定行为的应用程序都应被视为有缺陷的。

# poll函数

poll() 函数族是一组用于等待多个文件描述符上的事件发生的函数。它们与 select() 函数族类似，但是提供了一些改进和优化。

## 描述符

```C++
struct pollfd {
               int   fd;         /* file descriptor */
               short events;     /* requested events（由常量 POLLIN、POLLOUT、POLLERR、POLLHUP、POLLPRI 组合而成） */
               short revents;    /* returned events */
           };
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <signal.h>
#include <poll.h>

int ppoll(struct pollfd *fds, nfds_t nfds,
        const struct timespec *tmo_p, const sigset_t *sigmask);
```

## 参数

- fds：一个指向 struct pollfd 数组的指针，每个结构体描述了一个待检查的文件描述符，包括文件描述符号码和等待的事件类型。详见下文。
- nfds：指定 fds 数组中元素的个数。
- timeout：超时时间，单位是毫秒。当 timeout 为负数时，poll 将一直阻塞，直到有文件描述符就绪；当 timeout 为零时，poll 检查所有描述符后立即返回，不等待；当 timeout 大于 0 时，poll 最多等待 timeout 毫秒，如果在超时前有文件描述符就绪，就立即返回。注意，poll 的超时时间并不是精确的，只是一个近似值。

## NOTES

超时间隔将会四舍五入到系统时钟精度，并且内核调度延迟意味着阻塞间隔可能会超过一小段时间。在timeout中指定负值表示无限超时。指定超时时间为0会导致poll()立即返回，即使没有文件描述符准备好。

# epoll函数

epoll是Linux系统提供的一种I/O事件通知机制，可以用于替代select和poll函数。它的主要优点是在处理大量文件描述符时性能更好，因为它使用了基于事件驱动的方式而不是轮询方式。

---

使用epoll需要进行以下步骤：

1. 使用epoll_create函数创建一个epoll对象，该对象可以管理多个文件描述符的I/O事件。
2. 使用epoll_ctl函数将需要监视的文件描述符添加到epoll对象中。
3. 使用epoll_wait函数等待文件描述符的I/O事件。

---

epoll有三个触发模式，分别是：

- 水平触发（LT）：默认模式，只要有数据可读或可写，就会通知一次，直到数据被读取完毕或者写入缓冲区已满。
- 边缘触发（ET）：只有在数据状态发生改变时才会通知一次，例如读取完毕或写入缓冲区已满。使用边缘触发需要在添加文件描述符到epoll对象时设置EPOLLONESHOT标志。
- 边缘触发+EPOLLONESHOT：与边缘触发类似，但是使用后需要重新将文件描述符添加到epoll对象中才能再次接收事件通知。适用于需要在多个线程中处理同一文件描述符的情况。

## LT和ET

考虑以下场景：

1. 将表示管道读取端的文件描述符（rfd）在 epoll 实例上注册。
2. 管道写入者在管道写入端写入了 2KB 的数据。
3. 调用 epoll_wait(2) 函数，该函数将返回 rfd 作为准备就绪的文件描述符。
4. 管道读取者从 rfd 中读取了 1KB 的数据。
5. 再次调用 epoll_wait(2) 函数。

> 如果使用EPOLLET (边缘触发)标志将rfd文件描述符添加到epoll接口中，并在步骤5中调用epoll_wait(2)，尽管文件输入缓冲区中仍有可用数据，但该调用可能会挂起。同时，远程对等方可能会基于其已发送的数据期望响应。原因是边缘触发模式仅在监视文件描述符发生更改时才会传递事件。因此，在步骤5中，调用者可能会等待已经存在于输入缓冲区中的一些数据。在上面的示例中，由于2中进行了写入，因此将在rfd上生成事件，并且在3中使用该事件。由于4中执行的读取操作未消耗整个缓冲区数据，因此在步骤5中执行的epoll_wait(2)调用可能会无限期地阻塞。

---

在使用 EPOLLET 标志的应用程序中，应该使用非阻塞文件描述符，以避免阻塞读取或写入占用处理多个文件描述符的任务。建议使用 epoll 作为边沿触发（EPOLLET）接口的方式如下：

1. 使用非阻塞文件描述符；
2. 仅在 read(2) 或 write(2) 返回 EAGAIN 后等待事件。

相比之下，当作为级别触发接口使用（未指定 EPOLLET 时的默认值），epoll 只是更快的 poll(2)，并且可以在后者使用的地方使用，因为它具有相同的语义。

由于即使在边沿触发 epoll 下，多个数据块的接收可能会生成多个事件，因此调用者可以选择指定 EPOLLONESHOT 标志，告诉 epoll 在使用 epoll_wait(2) 接收到一个事件后禁用关联的文件描述符。当指定 EPOLLONESHOT 标志时，重新启用文件描述符的责任归调用者，使用 epoll_ctl(2) 和 EPOLL_CTL_MOD 重新启用。

## 相关函数

### epoll_create()函数

epoll_create() 函数返回一个指向新 epoll 实例的文件描述符。这个文件描述符用于所有后续的 epoll 接口调用。当不再需要它时，应使用 close(2) 函数关闭 epoll_create() 返回的文件描述符。当所有指向 epoll 实例的文件描述符都已关闭时，内核将销毁该实例并释放相关资源以供重用。

---

```C++
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_create1(int flags);
```

size参数指定epoll实例可以处理的文件描述符的最大数量，但是这个值只是一个提示，内核并不保证准确地返回能够处理这么多文件描述符的实例。Since Linux 2.6.8, the size argument is ignored, but must be greater than zero;

If  flags  is 0, then, other than the fact that the obsolete size argument is dropped, epoll_create1() is the same as epoll_create().  The following value can be included in flags to obtain different behavior:  
**EPOLL_CLOEXEC**

#### NOTES

在初始的 epoll_create() 实现中，size 参数向内核指定调用者期望添加到 epoll 实例中的文件描述符的数量。内核将此信息用作在描述事件的内部数据结构中最初分配的空间量的提示。(如果需要，如果调用者的使用超过了 size 中给定的提示，内核将分配更多的空间。) 现在，这个提示不再是必需的(内核动态调整所需的数据结构的大小而不需要提示)，但 size 仍然必须大于零，以确保在旧内核上运行新的 epoll 应用程序时具有向后兼容性。

## eopll_ctl()函数

在 epoll 实例中注册、修改或删除一个文件描述符的事件。

---

```C++
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};

#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```

- 合法op：
    - EPOLL_CTL_ADD
    - EPOLL_CTL_MOD
    - EPOLL_CTL_DEL
- 合法event.events：
    - EPOLLIN：可读事件（数据已经准备好可以从文件描述符中读取）；
    - EPOLLOUT：可写事件（数据可以写入文件描述符）；
    - EPOLLRDHUP：socket 对端关闭连接或关闭写操作（从而导致读操作返回 0）；
    - EPOLLPRI：高优先级可读事件（如带外数据）；
    - EPOLLERR：错误事件（包括监听的文件描述符被关闭、socket 发生错误等）；
    - EPOLLHUP：挂起事件（对应文件描述符的连接被挂起）；
    - EPOLLET：边沿触发模式（只在状态变化时通知，需要使用非阻塞文件描述符）；
    - EPOLLONESHOT：单次触发模式（事件被触发后自动从 epoll 实例中删除）。
    - EPOLLWAKEUP：Linux3.5

## epoll_wait函数族

epoll_wait是Linux系统中用于I/O多路复用的函数族之一，用于等待一个或多个文件描述符上的事件

---

```C++
typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;    /* Epoll events */
    epoll_data_t data;      /* User data variable */
};

#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event *events,
                int maxevents, int timeout);
int epoll_pwait(int epfd, struct epoll_event *events,
                int maxevents, int timeout,
                const sigset_t *sigmask);
```

### 参数

- epfd：epoll实例的文件描述符。
- events：用于存放就绪的事件的结构体数组。
- maxevents：events数组的大小，即最多可以存放多少个就绪事件。
- timeout：等待超时时间，以毫秒为单位。若为负数，则表示等待无限长时间；若为0，则表示不等待，立即返回；若为正数，则表示等待指定时间。
- sigmask：指向信号集的指针。在等待期间阻止这些信号。

### 返回值

成功时，返回就绪事件的个数，0 表示超时。
失败时，返回 -1，并设置 errno。

### NOTES

当一个线程在调用epoll_pwait()时被阻塞，另一个线程有可能会向等待的epoll实例中添加一个文件描述符。如果新的文件描述符变为可用状态，它将导致epoll_wait()调用解除阻塞。
