#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ev.h>

#define PORT 99
#define LISTEN_QUESZ 5
#define BUFSZ 100

void iferr_exit(int err, const char *strerr);

//@return : sockfd which bind
static int getServSocket();

static void servAcceptCb(EV_P_ ev_io *ac, int revents);

static void servEchoCb(EV_P_ ev_io *ac, int revents);

int main() {
    struct ev_loop *loop = EV_DEFAULT;
    int sockfd = getServSocket();
    iferr_exit(0==sockfd, "getServSocket failed");
    ev_io servAcceptWatcher;

    iferr_exit(
        0 != listen(sockfd, LISTEN_QUESZ),
        "listen failed"
    );

    ev_io_init(&servAcceptWatcher, servAcceptCb, sockfd, EV_READ);
    ev_io_start(loop, &servAcceptWatcher);
    
    ev_run(loop, 0);
    return 0;
}

void iferr_exit(int err, const char *strerr) {
    //C: not bool, use == is 1:true, 0:false
    if (err) {
        perror(strerr);
        exit(1);
    }
}

int getServSocket() {
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    memset(&addr, 0x00, addrlen);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    iferr_exit(sockfd==-1, "socket failed");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); //sin_port 16bit
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if( 1 !=
            inet_pton(AF_INET, "192.168.112.129", &addr.sin_addr)) {
        printf("inet_pton failed\n");
        return 0;
    }
        
    iferr_exit(
        0!=bind(sockfd, (struct sockaddr*)&addr, addrlen),
        "bind failed"
    );
    return sockfd;
}

void servAcceptCb(EV_P_ ev_io *ac, int revents) {
    int clifd;
    struct sockaddr_in cliaddr = {0};
    socklen_t cliaddr_len = sizeof(struct sockaddr_in);
    char cliipStr[INET_ADDRSTRLEN] = {0};
    ev_io *clientevWatcher = NULL;

acc_again:
    clifd = accept(ac->fd, 
        (struct sockaddr*)&cliaddr, &cliaddr_len);
    if (clifd < 0 && errno == EINTR) 
        goto acc_again;
    else if (clifd < 0) {
        printf("accept failed\n");
        return ;
    }

    if (!inet_ntop(AF_INET, 
            &(cliaddr.sin_addr), cliipStr, sizeof(cliipStr))) {
        printf("inet_ntop failed\n");
        return ;
    }

    printf("accept client:%s\n", cliipStr);
    clientevWatcher = (ev_io*) malloc(sizeof(ev_io));
    ev_io_init(clientevWatcher, servEchoCb, clifd, EV_READ);
    ev_io_start(EV_A, clientevWatcher);
    return ;
}

void servEchoCb(EV_P_ ev_io *ac, int revents) {
    int clifd = ac->fd;
    char buf[BUFSZ] = {0};
    char *sendptr = 0;
    int recvCnt = 0;
    int sendLeft = 0;
    int sendCnt = 0;

    while (1) {
        recvCnt = read(clifd, buf, BUFSZ);
        if (recvCnt < 0 && errno == EINTR)
            continue;
        else if (recvCnt < 0) {
            printf("servEchoCb read failed\n");
            break;
        }
        break;
    }

    if (recvCnt == 0) {
        printf("servEchoCb recv FIN\n");
        close(clifd);
        return ;
    }
    sendLeft = recvCnt;
    sendptr = buf;
    while(sendLeft > 0) {
        sendCnt = write(clifd, buf, sendLeft);
        sendptr += sendCnt;
        sendLeft -= sendCnt;        
    }
    return ;
}
