#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "APUE.h"

pthread_t ntid;

void print_ids(const char *s) {
  
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();
  printf("%s pid: %lu tid %lu (0x%lx)\n", 
    s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg) {

  print_ids("new thread: ");
  return ((void*)0);
}

int main(void) {

  int err;

  err = pthread_create(&ntid, NULL, thr_fn, NULL);
  if (err != 0) 
    err_sys("cannot create thread %d.\n", err);
  print_ids("main thread: ");
  sleep(1);
  exit(0);
}
