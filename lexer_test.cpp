#include "lexer.h"

using namespace std;

void test_lexer(){
  const vector<string, vector<Token>> input_results = {
    {
      "1", 
      {TokenType: TokenType::NUMBER, Literal: "1"},
    },
    {
      "-1", 
      {TokenType: TokenType::MINUS, Literal: "-"},
      {TokenType: TokenType::NUMBER, Literal: "1"},
    },
    {
      "1 - 2", 
      {TokenType: TokenType::NUMBER, Literal: "1"},
      {TokenType: TokenType::MINUS, Literal: "-"},
      {TokenType: TokenType::NUMBER, Literal: "2"},
    },
    {
      "1 - 2 + 3 * 4 / 5", 
      {TokenType: TokenType::NUMBER, Literal: "1"},
      {TokenType: TokenType::MINUS, Literal: "-"},
      {TokenType: TokenType::NUMBER, Literal: "2"},
      {TokenType: TokenType::PLUS, Literal: "+"},
      {TokenType: TokenType::NUMBER, Literal: "3"},
      {TokenType: TokenType::ASTERISK, Literal: "*"},
      {TokenType: TokenType::NUMBER, Literal: "4"},
      {TokenType: TokenType::SLASH, Literal: "/"},
      {TokenType: TokenType::NUMBER, Literal: "5"},
    },
  }; 


  // for (string s : input_results) {
  //   Lexer l = Lexer(s);
  //   vector<Token> results = l.tokenize();
    
        
  // }
}