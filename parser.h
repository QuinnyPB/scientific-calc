#ifndef PARSER_H
#define PARSER_H

#include <tuple>
#include <map>
#include "lexer.h"

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

class Parser {
  vector<Token> tokens;
  int curr;
  int next;
  Token* currToken;
  
  public:
    Parser(){};
    Parser(std::vector<Token>& tokens) : tokens(tokens) {}
    void parse();
    void parseInfixExpression();
    void parsePrefixExpression();
    bool currTokenIs(TokenType t);
    bool nextTokenIs(TokenType t); 
    void nextToken();
};

#endif 