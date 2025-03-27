#include "errorqueue.h"

using namespace std;

std::queue<std::string> ErrorQueue::errors;

void ErrorQueue::set_input(string input) {
  this->user_input = input;
}

void ErrorQueue::add_error(string errstr) {
  errors.push(errstr);
};

void ErrorQueue::print_errors() {
  cout << "Error Stack: \n";
  while (!errors.empty()) {
    printf("%s\n", errors.front().c_str());
    errors.pop();
  };
};

void ErrorQueue::clear() {
  for (double i=0; i<errors.size(); i++) {
    errors.pop();
  }
}