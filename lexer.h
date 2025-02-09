#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <cctype>
#include "token.h"

class Lexer {
  std::string input;
  char curr = '\0';
  char next = '\0';
  char ch='a';

  public:
    Lexer(){};
    Lexer(std::string &input);
    void insertInput(std::string &input);
    void readChar();  
    std::vector<Token> tokenize();
};

#endif // LEXER_H