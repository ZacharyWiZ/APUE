#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 99

#define err_ret(err, strerr) do{ if(err) { printf("%s\n", strerr); }}while(0);
void err_exit(int err, const char *strerr);

//@return : sockfd which bind
int bindSocket();

int main() {
    int sockfd = bindSocket();
    return 0;
}

void err_exit(int err, const char *strerr) {
    //C: not bool, use == is 1:true, 0:false
    if (err) {
        perror(strerr);
        exit(1);
    }
}

int bindSocket() {
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    memset(&addr, 0x00, addrlen);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    err_exit(sockfd==-1, "socket failed");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); //sin_port 16bit
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err_exit(
        0!=bind(sockfd, (struct sockaddr*)&addr, addrlen),
        "bind failed"
    );
    return sockfd;
}
