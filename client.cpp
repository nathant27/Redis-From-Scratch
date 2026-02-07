#include <sys/socket.h>
#include <iostream>

int main (int argc, char* argv[]){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) 
