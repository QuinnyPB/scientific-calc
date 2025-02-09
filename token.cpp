#include "token.h"

using namespace std;

Token::Token(TokenType t, const string &l) : type(t), literal(l) {};
Token Token::constructToken(string &input){
  Token newToken;
  newToken.literal = input;
  return newToken;
};
string Token::toString() {
  return "{TokenType: " + tokens_map.at(type) + ", Literal: " + literal + "}\n";
}   

