#ifndef _DAEMONIZE_H__
#define _DAEMONIZE_H__

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "net_sock.h"

void daemonize(void) {
    pid_t pid;
    struct sigaction sa;
    int fd;

    printf("begin daemonize\n");
    umask(0);

    pid = fork();
    if (pid < 0)
        err_sys("fork error\n");    
    if (pid > 0)//parent
        exit(0);
    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_sys("sigaction error, cannot ignore SIGHUP");
     
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) close(fd);
    }
}

#endif
