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

const int BUFSZ = 100;

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

    void iferr(bool judge, const string &err = string()) const {
        if (judge)
            cout << err << endl;
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
        clearServ();
        if (sockfd_ == -1)
            failed_ = true;
    }

    ~ClientNet() {
        close(sockfd_);
        cout << "close the sockfd" << endl;
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
             servaddr_.c_str(), (void*)&connaddr.sin_addr.s_addr) != 0) {
            cout << "inet_pton failed" << endl;
            failed_ = true;
            return false;
        }
        connaddr.sin_port = htons(servport);
        connaddr.sin_family = AF_INET;
        
        if (connect(sockfd_, 
             (struct sockaddr*)&connaddr, sizeof(connaddr)) != 0) {
            cout << "connect failed" << endl;
            failed_ = true;
            return false;
        }
        servport_ = servport;
        servaddr_ = servaddr;
        return true;
    }

    //@return : -1:error  ,other is send left byte
    bool Send(const string &msg) const {

        size_t leftsz = msg.size();
        const char *ptr = &*msg.begin(); //顶层const
        size_t nsended;

        while (leftsz > 0) {
            if ( (nsended = write(sockfd_, ptr, leftsz)) <= 0) {
                if (nsended < 0 && errno == EINTR)
                    nsended = 0;
                else
                    return false;
            }
            leftsz -= nsended;
            ptr += nsended; 
        }
        cout << "have send msg:" << msg << endl;
        return true;
    }

    //TODO
    bool Recv(string &msg) const {
        size_t nread;
        string temp;
        
        while (true) {
            if ((nread = read(sockfd_, buffer_, ::BUFSZ - 1)) < 0){
                //这里需要在末尾添加一个null
                if (errno == EINTR)
                    nread = 0;
                else
                    return false;
            } else if (nread == 0)
                break;
            else { //nread > 0
                buffer_[nread] = '\0';
                temp += buffer_;
                cout << "have recv msg:" << buffer_ << endl;
            }
            if (nread < BUFSZ-1) break; 
            //这里不知道怎么退出，如果不主动break，会阻塞在read那里
        }
        msg += temp;
        return true;
    }

    //send
    ClientNet& operator<<(const string &msg) {
        if (failed_)
            return *this;
        if (! Send(msg))
            failed_ = true;
        return *this;
    }

    ClientNet& operator>>(string &msg) {
        if (failed_)
            return *this;
        if (! Recv(msg))
            failed_ = true;
        return *this;
    }

    explicit operator bool() const {
    //explicit表示编译器不会自动执行这一类型转换
    //不过该规定有一个例外：如果表达式用作条件
    //则编译器会显式的类型转换自动应用于它
    //while (cin >> a); 类似于这样

        return !failed_; 
    }

    const string & getServAddr() const {
        return servaddr_;
    }
    const short getServPort() const {
        return servport_;
    }

private:
    void clearServ() {
        servaddr_ = string();
        servport_ = 0;
        failed_ = false;
        memset(buffer_, 0x00, ::BUFSZ);
    }

    //接收缓冲区
    mutable char buffer_[::BUFSZ];
    mutable bool failed_;
    //it means can or cannot to send/write ,just list iostream

    short servport_;
    string servaddr_;
    int sockfd_;
};

int main() {
    auto &err = Err_exit::getInstatce();
    auto cli = ClientNet();
    
    err.ifexit(!cli.Conn("0.0.0.0", 99), "conn error");

    string in_msg;
    while (cin >> in_msg) {
        if (in_msg == "disconn") {
            cli.Disconnect();
            break;
        }
        //err.iferr(!cli.Send(in_msg), "send error");
        if (!(cli << in_msg)) {
        //如果前面加!需要在表达式加上()
        //如果是直接的判断条件语句，不需要加上()
            cout << "send " << in_msg << " failed" << endl;
            break;
        }

        string msg;
        //err.iferr(!cli.Recv(msg), "recv msg error");
        if (!(cli >> msg)) {
            cout << "recv failed" << endl;
            break;
        }
        cout << msg << endl;
        cout << "---------------" << endl;
    }
    return 0;
}
