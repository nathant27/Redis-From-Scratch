#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h> //htons and l
#define HELPERS_H_IMPLEMENTATION
#include "helpers.h"
#include <sys/stat.h>
//#include <fcntl.h>
#include <unistd.h>

constexpr uint32_t k_max_msg = 4096;

static uint32_t query(int fd, const char *text){
    std::cout << "querying server with msg: " << text << std::endl;
    uint32_t len = static_cast<uint32_t>(strlen(text));
    if (len > k_max_msg) {
        return error("msg len too long");
    }

    // send request
    char wbuf[4 + k_max_msg];
    memcpy(wbuf, &len, 4); // assume little endian
    memcpy(&wbuf[4], text, len);
    if(int32_t err = write_all(fd, wbuf, 4 + len)) {
        return err;
    }
    std::cout << "done sending request" << std::endl;
    // 4 byte header
    char rbuf[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(fd, rbuf, 4);
    std::cout << "after read_full()" << std::endl;
    if (err){
        error(errno == 0 ? "EOF" : "read() error");
        return err;
    }
    memcpy(&len, rbuf, 4);
    if (len > k_max_msg){
        error("msg too long");
        return -1;
    }
    
    printf("before read");
    // read reply body
    err = read_full(fd, &rbuf[4], len);
    if (err){
        error("read() error");
        return err;
    }
    printf("after read");
    printf("server says: %.*s\n", len, &rbuf[4]);
    return 0;
}

int main (){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) 
        error("socket() error");

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
    
    int connect_ret = connect(socket_fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (connect_ret < 0)
        return error("connect() error");
    
    /*
    //char msg[20] = "hello";
    send(socket_fd, msg, strlen(msg), 0);

    char rbuf[64] = {};
    ssize_t n = recv(socket_fd, rbuf, sizeof(rbuf) - 1, 0);
    if (n < 0) 
        error("read() error");

    printf("server says %s\n", rbuf);
    close(socket_fd); // Why not make this happen with destructor? As a safety thing?
}*/
    int32_t err = query(socket_fd, "hello1");
    if (err) {
        goto L_DONE;
    }

    err = query(socket_fd, "hello2");
    if (err) {
        goto L_DONE;
    }
    
L_DONE:
    close(socket_fd);
    return 0;
}
