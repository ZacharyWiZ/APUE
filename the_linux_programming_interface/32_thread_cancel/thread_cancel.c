#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

static int err_sys(char *fmt, ...) {
    va_list va_args;
    char stdout_buf[512];

    va_start(va_args, fmt);
    vsprintf(stdout_buf, fmt, va_args);
    va_end(va_args);

    printf("%s\n", stdout_buf);
    exit(1);
}

static void *thread_func(void *arg) {
    int j;
    printf("New thread started\n");
    for (j=0; ; j++) {
        printf("Loop %d\n", j);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thr;
    int is_pthread_ok = 0;
    void *res;
    
    is_pthread_ok = pthread_create(&thr, NULL, thread_func, NULL);
    if (is_pthread_ok != 0) {
        err_sys("pthread_create error");
    }

    sleep(3);

    is_pthread_ok = pthread_cancel(thr);
    if (is_pthread_ok) {
        err_sys("pthread_cancel error");
    }

    is_pthread_ok = pthread_join(thr, &res);
    if (is_pthread_ok) {
        err_sys("pthread_join error");
    }

    if (res == PTHREAD_CANCELED) 
        printf("Thread was cancel\n");
    else 
        printf("Thread was not cancel\n");
    
    exit(0);
}
