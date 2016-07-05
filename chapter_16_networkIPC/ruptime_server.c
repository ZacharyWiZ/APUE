#define _GNU_SOURCE

#include "net_sock.h"
#include "daemonize.h"
#include <netdb.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

#define LISTEN_MAX 2 
#define NETBUFSIZE 128

static void uptime_server(int sockfd);

int main(int argc, char *argv[]) {
    int sockfd = 0, getaddrinfo_errno = 0;    
    char hostname[HOST_NAME_MAX] = {0};
    struct addrinfo hint = {0}, *list, *addr_iter;

    if (argc != 2 || strcmp(argv[1], "-help") == 0)
        err_sys("usage: server server-port");

    if (gethostname(hostname, HOST_NAME_MAX) == -1)
        err_sys("gethostname error");

    printf("gethostname : %s\n", hostname);

    hint.ai_flags     = AI_PASSIVE | AI_CANONNAME;
    hint.ai_family    = AF_INET;
    hint.ai_socktype  = SOCK_STREAM;
    hint.ai_protocol  = 0;
    hint.ai_canonname = NULL;
    hint.ai_addr      = NULL;
    hint.ai_next      = NULL;
    if ((getaddrinfo_errno = getaddrinfo(hostname, 
            argv[1], &hint, &list)) != 0) 
        err_sys("getaddrinfo error: %d(%s)", getaddrinfo_errno, 
            gai_strerror(getaddrinfo_errno));

    for (addr_iter = list; addr_iter != NULL; 
            addr_iter = addr_iter->ai_next) {
        sockfd = initserver(addr_iter->ai_socktype, 
        addr_iter->ai_addr, addr_iter->ai_addrlen, LISTEN_MAX);
        if (sockfd == -1)
            continue;
        else
            break;
    }
    if (addr_iter == NULL)
        err_sys("initserver error");
    else 
        printf("initserver ok, sockfd %d\n", sockfd);
    
    uptime_server(sockfd);
    exit(0);
}

void uptime_server(int sockfd) {
    //struct sockaddr c_addr;
    //socklen_t c_addrlen;
    int readtime_fd = 0;
    char buf[NETBUFSIZE] = {0};
    int clientfd = 0;
    FILE *fp; 
    daemonize();
    
    //if ((sockfd = set_cloexec(sockfd)) == -1)
    //    err_sys("set_cloexec error");

    for ( ; ; ) {
        if ((clientfd = accept(sockfd, NULL, NULL)) < 0) 
            err_sys("accept ret : %d, errno:%d (%s)", 
               clientfd, errno, strerror(errno)); 

        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
            sprintf(buf, "error: %s\n", strerror(errno));
        else
            fgets(buf, NETBUFSIZE, fp);
        if (send_msg(clientfd, buf, strlen(buf)) == -1)
            printf("send error\n");
        pclose(fp);
        close(clientfd);
    }
}
