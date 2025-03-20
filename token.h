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
  NEGATIVE,
  PERCENT,
  QUESTIONMARK,
  CLEAR,
  UNKNOWN,
  END,
  SPACE,
};

const std::unordered_map<TokenType, std::string> tokens_strings = {
  {TokenType::NUMBER,       "TokenType::NUMBER"},
  {TokenType::LPAREN,       "TokenType::LPAREN"},
  {TokenType::RPAREN,       "TokenType::RPAREN"},
  {TokenType::PLUS,         "TokenType::PLUS"},
  {TokenType::MINUS,        "TokenType::MINUS"},  
  {TokenType::SLASH,        "TokenType::SLASH"},
  {TokenType::ASTERISK,     "TokenType::ASTERISK"},
  {TokenType::CARET,        "TokenType::CARET"},
  {TokenType::NEGATIVE,     "TokenType::NEGATIVE"},
  {TokenType::PERCENT,      "TokenType::PERCENT"},
  {TokenType::SPACE,        "TokenType::SPACE"},
  {TokenType::QUESTIONMARK, "TokenType::QUESTIONMARK"},
  {TokenType::CLEAR,        "TokenType::CLEAR"},
  {TokenType::UNKNOWN,      "TokenType::UNKNOWN"},
  {TokenType::END,          "TokenType::END"},
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
  {TokenType::NEGATIVE, "n"},
  {TokenType::PERCENT, "%"},
  {TokenType::SPACE, " "},
  {TokenType::QUESTIONMARK, "?"},
  {TokenType::CLEAR, "c"},
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