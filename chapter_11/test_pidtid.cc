#include <sys/syscall.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

void printTask() {
    pid_t this_pid = getpid();
    pid_t this_ppid = getppid();
    pthread_t this_pthreadid = pthread_self();
    pid_t this_tid = syscall(SYS_gettid);
    
    cout << "pid:" << this_pid
         << " ppid:" << this_ppid
         << " pthreadid:" << this_pthreadid
         << " tid:" << this_tid << endl;
}

void *pthreadFun(void *arg) {
    char *str = (char*)arg;
 
    sleep(2);
    cout << str << endl;
    printTask();
    return ((void*)0);
}

int main() {
    int err;
    pthread_t tid;
    char showNewPthread[100] = "new thread :";
    cout << "main pthread:" << endl;
    printTask();

    err = ::pthread_create(&tid, NULL, pthreadFun, 
          static_cast<void*>(showNewPthread));
    if (err != 0) {
        cout << "pthread_create error" << endl;
        exit(1);
    }
    if (::pthread_join(tid, NULL) != 0) {
        cout << "pthread join error" << endl;
        exit(1);
    }
    cout << "main end" << endl;
    return 0; 
}
