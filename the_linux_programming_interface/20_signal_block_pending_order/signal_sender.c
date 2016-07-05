#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

int err_sys(char *fmt, ...) {
    va_list va_args;
    char buf[512] = {0};

    va_start(va_args, fmt);
    vsprintf(buf, fmt, va_args);
    printf("%s\n", buf);
    va_end(va_args);
    exit(1);     
}

// usage: ./a.out process_id send_times send_signal
int main(int argc, char *argv[]) {
    int send_cnt, sig, i;
    pid_t pid;
    
    if (argc < 4 || strcmp(argv[1], "-h")==0) {
        err_sys("usage: ./sender process_id send_times send_signal\n");
    }
    
    pid = atoi(argv[1]);
    send_cnt = atoi(argv[2]);
    sig = atoi(argv[3]);

    printf("%s : send to process:%d signal%d %dtimes\n",
        argv[0], pid, sig, send_cnt);
    for (i = 0; i < send_cnt; ++i)
        if (kill(pid, sig) != 0)
            err_sys("kill error");

    if (argc > 4) 
        if (kill(pid, atoi(argv[4])) != 0)
            err_sys("kill error");
    
    printf("send the signal exiting\n");
    exit(0);
}
