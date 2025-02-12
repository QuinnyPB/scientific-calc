// #include <stdio.h>
#include <map>
#include "parser.h"
#include "errorstack.h"

using namespace std;

int main()
{
  string input;
  ErrorStack errors = ErrorStack(); 
  cout << "Enter your expression to be calculated: \n";

  while (true) {
    getline(cin, input);
    // input = "1+ 2 - 3 * 4";
    
    Lexer lexer = Lexer(input);
    vector<Token> tokens = lexer.tokenize();

    cout << "TOKEN STACK: \n";
    for (Token t : tokens) {
      cout << t.toString();
    }

    cout << "ERROR STACK: \n";
    for (string err : errors.stack) {
      cout << err << "\n";
    }

    cout << "<< END OF CALC >> \n";
  }

  return 0; 
} 





