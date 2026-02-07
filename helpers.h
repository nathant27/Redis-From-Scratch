#ifndef HELPERS_H 
#define HELPERS_H 

#include <iostream>
#include <string>

int error(std::string);

#ifdef HELPERS_H_IMPLEMENTATION
int error(std::string err_msg)
    { std::cout << err_msg << std::endl; return -1; }

#endif

#endif
