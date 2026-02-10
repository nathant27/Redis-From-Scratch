// Basic TCP server
#include <sys/socket.h>
#include <arpa/inet.h> //htons and l
#include <string>
#include <iostream>
#include <cassert> // for assert
#include <unistd.h>
#include <cstring>

#define HELPERS_H_IMPLEMENTATION
#include "./helpers.h"
// C styling with a little bit of C++ just for nice to haves.
// Return values for successes or failures
// std::cout for printing
// C style casting

static void read_from_user(int connfd) {
    char rbuf[64];
    ssize_t n = recv(connfd, rbuf, sizeof(rbuf) - 1, 0); //Read only rbuf-1 so it properly null terminates
    if (n > 0)
        rbuf[n] = '\0';
    if (n < 0){
        error("read() error");
        return;
    }

    std::cout << "Client msg: " << rbuf << std::endl;
    send(connfd, rbuf, n, 0); 
}

constexpr size_t k_max_msg = 4096;

static int32_t one_request(int connfd){
    // get size of message
    char rbuf[4 + k_max_msg]; // 4 byte header
    errno = 0;
    int32_t read_rv = read_full(connfd, rbuf, 4);
    errno = 0;
    if (read_rv == -1){
        error(errno == 0 ? "EOF" : "read_full() read error");
        return read_rv;
    }
    uint32_t len = 0;
    memcpy(&len, rbuf, 4);
    if (len > k_max_msg){
        error("message too long");
        return -1;
    }

    // request the body of the message
    read_rv = read_full(connfd, &rbuf[4], len);
    if (read_rv == -1){
        error("read() error");
        return -1;
    }

    // action
    printf("client says %.*s\n", len, &rbuf[4]);

    // reply using same protocal
    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    len = static_cast<uint32_t>(strlen(reply));
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len); // can't I just use wbuf + 4? or is it safer to use &wbuf[4]?
    return write_all(connfd, wbuf, 4 + len); 
}

int main (){
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
    if (bind_rv) { 
        return error("bind() error"); 
    }
        // Each octet in address is 8 bits, aka 1 byte, so max value is 255. Can be represented with 2 hexadecimal digits

    if (listen(fd, SOMAXCONN)) { //SOMAXCONN is size of queue. Doesn't matter cause accept() won't bottleneck
        return error("listen () error");
    }

    //listen just makes the socket listen. Return value is just the success or failure
    while (true){
        struct sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addr_len);
        if (connfd < 0){
            //std::cerr << "accept error" << std::endl; 
            continue;   
        }
         
        //printf("connfd %d\n", connfd);
        one_request(connfd);
        one_request(connfd);
        close(connfd); 
    }
    

}
