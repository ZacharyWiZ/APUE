#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>

#define MAXLINE 512

static void sig_pip(int);
static int err_sys(char *fmt, ...);

int main(int argc, char *argv[]) {

    int io_cnt, fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];
    char elf_file_path[15] = "./";

    if (argc <2 || strcmp(argv[1], "-h")==0) {
        printf("usage : copress add_sys/add_std\n");
        exit(1);
    }

    strcat(elf_file_path, argv[1]); 

    if (signal(SIGPIPE, sig_pip) == SIG_ERR) {
        err_sys("signal error");
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        err_sys("pipe error");
    }

    pid = fork();

    if (pid == -1)
        err_sys("fork error");

    if (pid == 0) { //chlld
        close(fd1[1]);
        close(fd2[0]);

        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("dup2 error");
            close(fd1[0]);
        }

        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error");
            close(fd2[1]);
        }
        
        if (execl(elf_file_path, argv[1], (char *)0) < 0)
            err_sys("execl error");
    } else { // parent
        close(fd1[0]);
        close(fd2[1]);

        if (setvbuf(stdin, NULL, _IOLBF, 0) != 0)
            err_sys("setvbuf error");
        if (setvbuf(stdout, NULL, _IOLBF, 0) != 0)
            err_sys("setvbuf error");

        while (fgets(line, MAXLINE, stdin) != NULL) {
            io_cnt = strlen(line);
            if (write(fd1[1], line, io_cnt) != io_cnt)
                err_sys("write error");
            if ((io_cnt = read(fd2[0], line, MAXLINE)) < 0)
                err_sys("read error");

            if (io_cnt == 0) {
                printf("child closed pipe");
                break;
            }
            line[io_cnt] = 0;
            if (fputs(line, stdout) == EOF)
                err_sys("fputs error");
        }
        if (ferror(stdin))
            err_sys("fgets error on stdin");
        exit(0);
    }

    exit(0);
}

static int err_sys(char *fmt, ...) {
    va_list va_args;
    char sys_buf[MAXLINE];

    va_start(va_args, fmt);
    vsprintf(sys_buf, fmt, va_args);
    va_end(va_args);

    printf("%s\n", sys_buf);
    
    exit(1);
}
static void sig_pip(int sig) {
    printf("SIGPIPE caught\n");
    exit(1);
}
