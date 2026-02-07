// Basic TCP server
#include <sys/socket.h>
#include <arpa/inet.h> //htons and l
#include <string>
#include <iostream>

#define HELPERS_H_IMPLEMENTATION
#include "./helpers.h"
// C styling with a little bit of C++ just for nice to haves.
// Return values for successes or failures
// std::cout for printing
// C style casting

static void read_from_user(int connfd) {
    char rbuf[64];
    ssize_t n = recv(connfd, rbuf, sizeof(rbuf) - 1, 0); //Read only rbuf-1 so it properly null terminates
    if (n < 0){
        error("read() error");
        return;
    }

    std::cout << "Client msg: " << rbuf << std::endl;
    send(connfd, rbuf, sizeof(rbuf), 0); 
}


int main (int argc, char *argv[]){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET is for IPv4, SOCK_STREAM FOR TCP cause byte stream. Third arg is additional settings, but unnecessary

    int sso_val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sso_val, sizeof(sso_val));
    // 2nd and 3rd args specify which option to set. SO_REUSEADDR is in general, wanted behavior. Allows reuse of port after restart
    
    // initialize in address. Make sure to use htons to convert host to network byte order(big endian). last is l(uint32 long), s(uint16 short). 
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234); // port
    addr.sin_addr.s_addr = htonl(0); // wildcard IP 0.0.0.0
    int bind_rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr)); // sizeof(addr) needed to determine between IPv4 and IPv6
    if (bind_rv) { error("bind() error"); }
        // Each octet in address is 8 bits, aka 1 byte, so max value is 255. Can be represented with 2 hexadecimal digits

    int listen_fd = listen(fd, SOMAXCONN); //SOMAXCONN is size of queue. Doesn't matter cause accept() won't bottleneck
                                           
    while (true){
        struct sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addr_len);
        if (connfd < 0) continue;
        
        
    }
}
