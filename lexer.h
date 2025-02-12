#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <cctype>
#include "token.h"

class Lexer {
  std::string input;
  int curr = 0;
  int next = 0;
  char ch;

  public:
    Lexer(){};
    Lexer(std::string &input);
    std::string readNumber();
    std::vector<Token> tokenize();
    void readChar();  
    char peekChar();
    void reverseChar();
    void insertInput(std::string &input);
    void skipwhitespace();
    bool isLetter(char ch);
    bool isDigit(char ch);
};

#endif // LEXER_H