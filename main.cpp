// #include <stdio.h>
#include <iostream>
#include <string.h>
#include <map>
#include <sstream>
#include "parser.h"

using namespace std;

enum OPERATIONS {
  ADDITION,
  SUBTRACT,
  MULTIPLICATION,
  DIVISION,
};

map<char, OPERATIONS> operationTypes = {
  {'+', OPERATIONS::ADDITION},
  {'-', OPERATIONS::SUBTRACT},
  {'*', OPERATIONS::MULTIPLICATION},
  {'/', OPERATIONS::DIVISION},
};

int main()
{
  string input;

  cout << "Enter your expression to be calculated: \n";
  cin >> input;

  // converts string to char array
  int n = input.length();

  stringstream ss(input);
  string next;
  while (ss >> next) {
    parseExpression(next);
  };

  return 0; 
}





