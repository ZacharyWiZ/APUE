#include "net_sock.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define ERR_BUF 512
#define RECV_BUF_LEN 128
#define RECONNECT_TIME 10

int connect_retry(int domain, int type, int protocol,
    const struct sockaddr *addr, socklen_t alen) {
    
    int numsec , fd;

    for (numsec = 1; numsec <= RECONNECT_TIME; numsec <<= 1) {
        fd = socket(domain, type, protocol);
        if (fd < 0)
            return -1;

        if (connect(fd, addr, alen) == 0)
            return fd;
        close(fd);
        
        if (numsec <= RECONNECT_TIME/2)
            sleep(numsec);
    }
    return -1;
}

int err_sys(const char *fmt, ...) {
    va_list va_args;
    char out_buf[ERR_BUF];    
     
    va_start(va_args, fmt);
    vsprintf(out_buf, fmt, va_args);
    va_end(va_args);

    printf("%s\n", out_buf);
    exit(1);
}

int set_cloexec(int fd) {
    
    int val;
    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        return -1;
    val |= FD_CLOEXEC;
    
    return fcntl(fd, F_SETFD, val);
}

int initserver(int type, const struct sockaddr *addr, 
                socklen_t addrlen, int listen_max) {

    int sockfd;
    int err;
 
    sockfd = socket(addr->sa_family, type, 0);
    if (sockfd < 0){ 
        printf("socket failed");
        return -1;
    } 

    if (bind(sockfd, addr, addrlen) < 0){
        printf("bind failed"); 
        goto errout;
    }

    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
        if (listen(sockfd, listen_max) < 0) {
            printf("type : %s, error : %d (%s)",
            type == SOCK_STREAM ? "SOCK_STREAM": "SOCK_SEQPACKET",
            errno, strerror(errno));
            goto errout;
        }
   
    return sockfd;

errout:
    err = errno;
    close(sockfd);
    errno = err;
    return -1;
}

int send_msg(int sockfd, char *buf, int nbytes) {
    int sended_len = 0;
    int left_len = nbytes;
    char *send_buf = buf;
    
    while ((sended_len = send(sockfd, send_buf, left_len, 0)) != -1) {
        if ((left_len -= sended_len) == 0)
            return 0;
        send_buf += sended_len;
    }
    printf("send_msg send error\n");
    return -1;
}
