#ifndef ERRORSTACK_H
#define ERRORSTACK_H

#include <iostream>
#include <string>
#include <vector>

class ErrorStack {
  public:
    static std::vector<std::string> stack;
  
    ErrorStack(){};
    void add_error(std::string& err);
    void print_errors();
};

#endif