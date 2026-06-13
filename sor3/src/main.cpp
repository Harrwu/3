#include <iostream>
#include <span>
#include <bit>
#include <cstdint>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include <netinet/in.h>
#include <sys/select.h>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <memory>

class Server{
private:
    int portnum, serverfd;
    struct MonitorNode;
    struct sockaddr_in address{};
    std::vector<std::unique_ptr<MonitorNode>> nodes;
    fd_set read_fds;
    
public:
    Server(int portno): portnum(portno), serverfd(-1){}
    inline void printBarthingi(int count);
    void coutPort();

    bool init();
    void process();
};

bool Server::init(){
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){
        std::cerr << "Failed to create socket\n";
        return false;
    }
    std::cout << "Server FD assigned\n";

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portno); // Big endian
    
    if (bind(server_fd, (struct sockaddr*)& address, sizeof(address)) < 0) {
        std::cerr << "Bind Failed\n";
        return false;
    }

    std::cout << "Binding Success\n";

    if (listen(server_fd, 10) < 0){
        std::cerr << "Listen failed\n";
        return false;
    }

    std::cout << "Init success Listening on port " << portno << "\n";
    return true;

}

void Server::init(){
    FD_ZERO(&read_fds); // Removes every FD in read_fds
    FD_SET(server_fd, &read_fds); // Adds Server FD;
    
    int max_fd = server_fd; 

}

struct Server::MonitorNode{
    int fd; // Client File descriptor
    std::vector<uint8_t> rawBuffer; // 
    Server::MonitorNode(int passfd): fd(passfd);
}


inline void Server::printBarthingi(int count){
    while (count --) {std::cout << '=';}
    std::cout << "\n";
}

void Server::coutPort(){
    std::cout << portnum << "\n";
}

int main(){
    for (int i = 0; i < 5; i ++){
        auto pass = std::make_unique<Server>(i);
        pass->printBarthingi(8);
        pass->coutPort();
        pass->printBarthingi(8);
        std::cout << "\033[2J\033[1;1H";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}
