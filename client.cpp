#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h> //htons and l
#define HELPERS_H_IMPLEMENTATION
#include "helpers.h"
#include <sys/stat.h>
//#include <fcntl.h>
#include <unistd.h>

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
        error("connect() error");

    char msg[20] = "hello";
    send(socket_fd, msg, strlen(msg), 0);

    char rbuf[64] = {};
    ssize_t n = recv(socket_fd, rbuf, sizeof(rbuf) - 1, 0);
    if (n < 0) 
        error("read() error");

    printf("server says %s\n", rbuf);
    close(socket_fd); // Why not make this happen with destructor? As a safety thing?
}
