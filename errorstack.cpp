#include "errorstack.h"

using namespace std;
vector<string> ErrorStack::stack = {};


void ErrorStack::add_error(string& err) {
  stack.push_back(err);
};

void ErrorStack::print_errors() {
  cout << "Error Stack: \n";
  for (string s : stack) {
    cout << s << "\n";
  };
};