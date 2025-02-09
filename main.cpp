// #include <stdio.h>
#include <map>
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

  // while (true) {
  //   cout << "Enter your expression to be calculated: \n";
  //   // cin >> input;
  //   getline(cin, input);
  //   // converts string to char array
  //   int n = input.length();
    
  //   Lexer l = Lexer(input);
  //   vector<Token> tokens = l.tokenize();
  //   cout << "Tokens: \n";
  //   for (Token t : tokens) {
  //     cout << t.toString();
  //   }
  // }

  cout << "Enter your expression to be calculated: \n";
  char ch, prev = '\0';
  bool isIllegal = false;

  while (ch = getchar() != '\n') {    
    
    if (isIllegal) {
      
    }
    
    Lexer l = Lexer(input);
    vector<Token> tokens = l.tokenize();
    cout << "Tokens: \n";
    for (Token t : tokens) {
      cout << t.toString();
    }
  }
  
  return 0; 
}





