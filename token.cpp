#include "token.h"
#include <string.h>
#include <stdio.h>

using namespace std;

Token::Token(enum TokenType t, const string &l) : type(t), literal(l) {};
Token::Token(enum TokenType t, const char ch) : type(t) {
  string s{ch};
  literal = s;
}
Token Token::constructToken(string &input){
  Token newToken;
  newToken.literal = input;
  return newToken;
};
string Token::toString() {
  char c[literal.length() + 1];
  strcpy(c, literal.c_str());
  
  return "{TokenType: " + tokens_strings.at(type) + ", Literal: " + c + "}\n";
}   
 