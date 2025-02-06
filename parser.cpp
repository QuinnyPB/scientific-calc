#include <iostream>
#include <string>
#include "parser.h"
#include <unordered_map>
#include <vector>
#include <tuple>

using namespace std;

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
};

const unordered_map<TokenType, string> tokens = {
  {TokenType::NUMBER, "NUMBER"},
  {TokenType::LPAREN, "LPAREN"},
  {TokenType::RPAREN, "RPAREN"},
  {TokenType::PLUS, "PLUS"},
  {TokenType::MINUS, "MINUS"},
  {TokenType::SLASH, "SLASH"},
  {TokenType::ASTERISK, "ASTERISK"},
  {TokenType::PERCENT, "PERCENT"},
  // {TokenType::INTEGER, "INTEGER"},
  // {TokenType::INTEGER, "INTEGER"},
  // {TokenType::INTEGER, "INTEGER"},
  {TokenType::END, "END"},
};

struct Token {
  enum TokenType type;
  string literal;

};
// overloads == operator for Token class
bool operator==(const Token& lhs, const Token& rhs) {
  return (lhs.type == rhs.type) && (lhs.literal == rhs.literal);
}
// overloads == operator for Token class
bool operator!=(const Token& lhs, const Token& rhs) {
  return (lhs.type == rhs.type) && (lhs.literal == rhs.literal);
}

class Lexer {
  string input;
  int curr;        // current char being analysed
  int next;       // char after current char
  char ch;        // actual char
  
  public:
    Lexer New(string input) {
      Lexer l = new Lexer{input: input};
      l.readChar()
      return l;
    }  
    void readChar(){
      if (next >= input.length()){
        ch = 0;
      } else {
        ch = input[next];
      }
      curr = next;
      next++;
    }
};

Token constructToken(string input);
void parseExpression(string input);
vector<Token> tokenize(string input);

// return data = (token-vector, token-vector-size) 
vector<Token> tokenize(string input)
{
  int currPtr = 0;    // points to current char
  int prevPtr = -1;   // points to previous char
  vector<Token> tokens;
  
  while (){

  }

  // switch token{

  // default:
  //   break;
  // }
   
  return tokens;
}

// float parseNumber(string )

void parseExpression(string input) {
  Token newToken = constructToken(input);
  // checks input is valid (aka  a number)
  try { stof(input); }
  catch(const std::exception& e)
  {
    std::cerr << "Could not " << e.what() << " value '"<< input << "'" << '\n';
  }
  
  newToken.type = TokenType::NUMBER;
}

Token constructToken(string input){
  Token newToken = {literal: input};
  return newToken;
}

// represents a test unit for parser
struct ParserTest {
  string input;
  vector<Token> expectedTokens;
  float expectedResult;
};



//  TESTING SECTION
string errors;

bool testParser() {
  ParserTest test = {
    "1 + 1", 
    {
      Token{TokenType::NUMBER, "1"},
      Token{TokenType::PLUS, "+"},
      Token{TokenType::NUMBER, "1"},
      Token{TokenType::END, "END"},
    },
    2
  };
  vector<Token> result = tokenize(test);
  bool passes = true;

  if (result.size() != test.expectedTokens.size()) {
    cerr << "results has wrong amount of tokens, got=%d, want=%d\n" << result.size(), test.expectedTokens.size();
    passes=false;
  }

  for (int i=0; i < test.expectedTokens.size(); i++) {
    if (result[i] != test.expectedTokens[i]) {
      cerr << "Tokens don't match, got=%d, want=%d\n" << result[i].literal, test.expectedTokens[i].literal; 
      passes=false;
    }
  }
  
  return passes;
}