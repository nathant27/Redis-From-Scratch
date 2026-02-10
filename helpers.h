#ifndef HELPERS_H 
#define HELPERS_H 

#include <iostream>
#include <string>
#include <unistd.h>
#include <cassert>

int error(std::string);

#ifdef HELPERS_H_IMPLEMENTATION
int error(std::string err_msg)
    { std::cout << err_msg << std::endl; return -1; }

// properly reading from a TCP socket. Must be through a loop.
static int32_t read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return -1; // read_rv: 0 EOF, -1 ERROR
        }
        assert(static_cast<size_t>(rv) <= n);
        n -= static_cast<size_t>(rv);
        buf += rv;
    }
    return 0;
}

//properly writing n bytes to TCP socket. Also must be in a loop
static int32_t write_all(int fd, char *buf, size_t n){
    while (n > 0){
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0){
            return -1; // ERROR or early EOF
        }

        assert(static_cast<size_t>(rv) <= n);
        n -= static_cast<size_t>(rv);
        buf += rv;
    }
    return 0;
}
#endif

#endif
