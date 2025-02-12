#include <functional>
#include "parser.h"


using namespace std;

// overloads == operator for Token class
bool operator==(const Token& lhs, const Token& rhs) {
  return (lhs.type == rhs.type) && (lhs.literal == rhs.literal);
}
// overloads == operator for Token class
bool operator!=(const Token& lhs, const Token& rhs) {
  return (lhs.type == rhs.type) && (lhs.literal == rhs.literal);
}


void Parser::parse() {
  while (curr < tokens.size()) {

    switch (currToken->type) {
    case 
    default:
      cout << "Do not know how to handle '" << currToken->toString() << "'\n";
      break;
    }
  }  
}

void Parser::parseInfixExpression() {

};

void Parser::parsePrefixExpression() {

};

bool Parser::currTokenIs(TokenType t) {
  return currToken->type == t;
}

bool Parser::nextTokenIs(TokenType t) {
  
}

void Parser::nextToken() {
  
}
