#ifndef __NET_SOCK_H__
#define __NET_SOCK_H__

#include <netdb.h>
#include <sys/socket.h>

int connect_retry(int domain, int type, int protocol,
    const struct sockaddr *addr, socklen_t alen);

int err_sys(const char *fmt, ...); 

int set_cloexec(int fd); 

int initserver(int type, const struct sockaddr *addr, 
                socklen_t alen, int listen_max);

int send_msg(int sockfd, char *buf, int nbytes);

#endif
