#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "APUE.h"

void cleanup(void *arg) {
  printf("cleanup : %s.\n", (char *)arg);
}

void *thr_fn1(void *arg) {
  printf("thread 1 start.\n");
  pthread_cleanup_push(cleanup, "thread 1 first handler");
  pthread_cleanup_push(cleanup, "thread 1 seconde handler");
  printf("thread 1 push complete.\n");
  if (arg)
    return ((void*)1);
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
  return ((void*)3);
}

void *thr_fn2(void *arg) {
  printf("thread 2 start.\n");
  pthread_cleanup_push(cleanup, "thread 2 first handler");
  pthread_cleanup_push(cleanup, "thread 2 seconde handler");
  printf("thread 2 push complete.\n");
  if (arg)
    pthread_exit((void*)2);
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
  pthread_exit((void*)4);
}

int main(void) {
  pthread_t tid1, tid2;
  void *tret = NULL;

  if (pthread_create(&tid1, NULL, thr_fn1, (void*)1) != 0)
    err_sys("create thread 1 failed.\n");
  if (pthread_create(&tid2, NULL, thr_fn2, (void*)1) != 0)
    err_sys("create thread 2 failed.\n");

  if (pthread_join(tid1, &tret) != 0)
    err_sys("cannot join the thread 1.\n");
  printf("thread 1 's exit code :%ld.\n", (long)tret);

  if (pthread_join(tid2, &tret) != 0)
    err_sys("cannot join the thread 2.\n");
  printf("thread 2 's exit code :%ld.\n", (long)tret);
  exit(0);
}
