#include "net_sock.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>

#define SERVER "ruptime"
#define IOBUFSIZE 512
static void print_uptime(int fd);

int main(int argc, char *argv[]) {

    struct addrinfo hints, *addr_list, *addr_iter;
    int ctlfd = 0, err = 0;

    if (argc < 2 || strcmp(argv[1], "-h")==0) 
        err_sys("usage: ./ruptime_client SERVER-NAME");

    memset(&hints, 0x00, sizeof(hints));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = IPPROTO_TCP;
    hints.ai_addr     = NULL;
    hints.ai_next     = NULL;
    hints.ai_canonname = NULL;

    if ((err = getaddrinfo("localhost.localdomain", argv[1],
            &hints, &addr_list)) != 0)
        err_sys("getaddrinfo error, errno:%d (%s)", err,
            gai_strerror(err));
    
    for (addr_iter = addr_list; addr_iter != NULL;
        addr_iter = addr_iter->ai_next) {
        
        ctlfd = connect_retry(addr_iter->ai_family, 
            SOCK_STREAM, 0, addr_iter->ai_addr,
            addr_iter->ai_addrlen);
        if (ctlfd == -1)
            continue;
        else {
            print_uptime(ctlfd);
            break;
        }
    }
    if (addr_iter != NULL)
        close(ctlfd);
    exit(0);
}

static void print_uptime(int fd) {
    char io_buf[IOBUFSIZE];
    int recv_cnt;
    
    while ((recv_cnt = recv(fd, io_buf, IOBUFSIZE, 0)) > 0) {
        write(STDOUT_FILENO, io_buf, recv_cnt);
    }
    if (recv_cnt < 0)
        err_sys("recv error");
}
