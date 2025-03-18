#ifndef ERRORQUEUE_H
#define ERRORQUEUE_H

#include <iostream>
#include <string>
#include <queue>
#include <vector>

class ErrorQueue {
  public:
    std::string user_input;
    static std::queue<std::string> errors;
  
    ErrorQueue(){};
    void set_input(std::string input);
    void add_error(std::string errstr);
    void print_errors();
    void clear();
};

#endif