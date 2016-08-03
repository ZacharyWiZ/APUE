#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Err_exit {//打印 并 退出 单例
public:
    static Err_exit& getInstatce() {
        static Err_exit instance;
        return instance;
    }
    
    void operator()(const string &err) const {
        cout << err << endl;
        exit(1);
    }
    void ifexit(bool judge, const string &err = string()) const {
        if (judge) {
            cout << err << endl;
            exit(1);
        }
    }

private:
    Err_exit() = default;
    ~Err_exit() = default;
    Err_exit(const Err_exit &) = delete;
    Err_exit & operator=(const Err_exit &) = delete;
};

//IPv4 : TCP
class ClientNet {
public:
    ClientNet() {
        sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        Err_exit &err = Err_exit::getInstatce();
        err.ifexit( sockfd_ == -1);
        clearServ();
    }

    ~ClientNet() {
        close(sockfd_);
    }

    void Disconnect() {
        if (sockfd_ != -1)
            close(sockfd_);
        sockfd_ = -1;
        clearServ();
    }

    bool Conn(const string &servaddr, short servport) {
        clearServ();
        struct sockaddr_in connaddr;

        memset(&connaddr, 0x00, sizeof(connaddr));
        
        if (inet_pton(AF_INET, 
             servaddr_.c_str(), (void*)&connaddr.sin_addr.s_addr) != 0)
            return false;

        connaddr.sin_port = htons(servport);
        connaddr.sin_family = AF_INET;
        
        if (connect(sockfd_, 
             (struct sockaddr*)&connaddr, sizeof(connaddr)) != 0)
            return false;

        servport_ = servport;
        servaddr_ = servaddr;
        return true;
    }

    //@return : -1:error  ,other is send left byte
    int Send(const string &msg) const {
        size_t leftsz = msg.size();
        const char *ptr = &*msg.begin();
        size_t nsended;

        while (leftsz > 0) {
            if ( (nsended = write(sockfd_, ptr, leftsz)) <= 0) {
                if (nsended < 0 && errno == EINTR)
                    nsended = 0;
                else
                    return -1;
            }
            leftsz -= nsended;
            ptr += nsended; 
        }
        return leftsz;
    }

    

    explicit operator bool() const {

    //explicit表示编译器不会自动执行这一类型转换
    //不过该规定有一个例外：如果表达式用作条件
    //则编译器会显式的类型转换自动应用于它
    //while (cin >> a); 类似于这样

        return failed_; 
    }

private:
    void clearServ() {
        servaddr_ = string();
        servport_ = 0;
        failed_ = true;
    }

    bool failed_;    //it means can or cannot to send/write ,just list iostream
    short servport_;
    string servaddr_;
    int sockfd_;
};

int main() {
    auto &err = Err_exit::getInstatce();
    auto cli = ClientNet();

    return 0;
}
