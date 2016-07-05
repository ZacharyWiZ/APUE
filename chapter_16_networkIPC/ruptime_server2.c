#define _GNU_SOURCE

#include "net_sock.h"
#include "daemonize.h"
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define LISTEN_MAX 3
#define UPTIME_PATH "/usr/bin/uptime"
#define UPTIME "uptime"

static void uptime_server(int serverfd);

int main(int argc, char *argv[]) {
    char hostname[HOST_NAME_MAX] = {0};    
    struct addrinfo hint, *addr_list = NULL, *addr_iter = NULL;
    int serverfd = 0;

    if (argc != 2 || strcmp(argv[1], "-help") == 0) {
        err_sys("usage: ./server server-name");
    }

    if (gethostname(hostname, HOST_NAME_MAX) == -1)
        err_sys("gethostname error");

    printf("hostname : %s\n", hostname);
    
    daemonize();    

    memset(&hint, 0x00, sizeof(hint));
    hint.ai_flags     = AI_PASSIVE | AI_CANONNAME;
    hint.ai_family    = AF_INET;
    hint.ai_socktype  = SOCK_STREAM;
    hint.ai_addr      = NULL;
    hint.ai_canonname = NULL;
    hint.ai_next      = NULL;
    if (getaddrinfo(hostname, argv[1], &hint, &addr_list) != 0)
        err_sys("getaddrinfo error");

    for (addr_iter = addr_list; addr_iter != NULL; 
         addr_iter = addr_iter->ai_next) {
        
        serverfd = initserver(addr_iter->ai_socktype, 
            addr_iter->ai_addr, addr_iter->ai_addrlen, LISTEN_MAX);
        if (serverfd == -1)
            continue;
        else
            break;
    }
    if (addr_iter == NULL)
        err_sys("initserver cannot get sockfd\n");
    printf("serverfd : %d\n", serverfd);

    uptime_server(serverfd);

    exit(0);
}

static void exec_uptime(int clientfd) {
    pid_t pid = -1;
    int child_status = 0;

    pid = vfork();
    if (pid == -1) err_sys("fork error");
    if (pid > 0) {
        close(clientfd);
        if (waitpid(pid, &child_status, 0) == -1)
            err_sys("waitpid error");
        if (WIFEXITED(child_status))
            printf("exec return %d\n", WEXITSTATUS(child_status));
        return ;
    } else {
        if (dup2(clientfd, STDOUT_FILENO) == -1)
            err_sys("dup2 error");
        if (dup2(clientfd, STDERR_FILENO) == -1)
            err_sys("dup2 error");
        close(clientfd); 
        execl(UPTIME_PATH, UPTIME, (char *)0);        
    }
}

void uptime_server(int serverfd) {
    int clientfd = 0;    

    for (;;) {
        clientfd = accept(serverfd, NULL, NULL);
        if (clientfd == -1)
            err_sys("accept error :%d (%s)", errno, strerror(errno));
        
        exec_uptime(clientfd);            
    }
}

