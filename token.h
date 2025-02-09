#ifndef TOKEN_H
#define TOKEN_H 

#include <iostream>
#include <string>
#include <unordered_map>

enum class TokenType {
  NUMBER,
  LPAREN,
  RPAREN,
  PLUS,
  MINUS,
  SLASH,
  ASTERISK,
  PERCENT,
  END,
  SPACE,
};

const std::unordered_map<TokenType, std::string> tokens_map = {
  {TokenType::NUMBER, "NUMBER"},
  {TokenType::LPAREN, "LPAREN"},
  {TokenType::RPAREN, "RPAREN"},
  {TokenType::PLUS, "PLUS"},
  {TokenType::MINUS, "MINUS"},
  {TokenType::SLASH, "SLASH"},
  {TokenType::ASTERISK, "ASTERISK"},
  {TokenType::PERCENT, "PERCENT"},
  {TokenType::SPACE, "SPACE"},
  // {TokenType::INTEGER, "INTEGER"},
  // {TokenType::INTEGER, "INTEGER"},
  {TokenType::END, "END"},
};

struct Token {
  enum TokenType type;
  std::string literal;

  public:
    Token(){};
    Token(enum TokenType t, const std::string& l);
    Token constructToken(std::string &input);
    std::string toString();
};

#endif // TOKEN_H