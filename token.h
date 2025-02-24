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
  CARET,
  PERCENT,
  HELP,
  UNKNOWN,
  END,
  SPACE,
};

const std::unordered_map<TokenType, std::string> tokens_map = {
  {TokenType::NUMBER, "NUMBER"},
  {TokenType::LPAREN, "("},
  {TokenType::RPAREN, ")"},
  {TokenType::PLUS, "+"},
  {TokenType::MINUS, "-"},
  {TokenType::SLASH, "/"},
  {TokenType::ASTERISK, "*"},
  {TokenType::CARET, "^"},
  {TokenType::PERCENT, "%"},
  {TokenType::SPACE, " "},
  {TokenType::HELP, "?"},
  {TokenType::UNKNOWN, "UNKNOWN"},
  {TokenType::END, "END"},
};

struct Token {
  enum TokenType type;
  std::string literal;

  public:
    Token(){};
    Token(enum TokenType t, const std::string& l);
    Token(enum TokenType t, const char ch);
    Token constructToken(std::string &input);
    std::string toString();
};

#endif // TOKEN_H