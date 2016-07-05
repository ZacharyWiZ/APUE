#include <stdio.h>
#include <signal.h>

static void wrk_process(int sig) {
    int i;
    printf("wrk_process. I get signal:%d threadid:%d\n", 
        sig, pthread_self());
    
    for (i=0; i<5; ++i) {
        printf("wrk_process: %d\n", i);
        sleep(1);
    }
}

int main(void) {
    struct sigaction act, oact;
    act.sa_handler = wrk_process;
    act.sa_flags = SA_NODEFER ;
    sigaction(SIGINT, &act, &oact);
    
    printf("main threadid : %d\n", pthread_self());
    
    while(1) sleep(5);
    
    return 0;
}
