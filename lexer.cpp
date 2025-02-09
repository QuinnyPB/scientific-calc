#include "lexer.h"

using namespace std;

Lexer::Lexer(string &input) : input(input), curr(0), next(0), ch('a') {      
  readChar();
}  

void Lexer::readChar() {
  if (next >= input.length()){
    ch = 0;
  } else {
    ch = input[next];
  }
  curr = next;
  next++;
}

vector<Token> Lexer::tokenize() {
  vector<Token> tokens_list;
  
  for (int i=0; i<input.length(); i++) {
    char c = input[i];
    Token t = Token();
    
    cout << i  << ": " << c << "\n";
    
    switch (input[i]) {
    case ' ':
      t.type = TokenType::SPACE;
      t.literal = " ";
      break;
    case '+':
      t.type = TokenType::PLUS; 
      t.literal = "+";
      tokens_list.push_back(t);
      break;
    case '-':
      t.type = TokenType::MINUS;
      t.literal = "-";
      break;
    case '*':
      t.type = TokenType::ASTERISK;
      t.literal = "*";
      break;
    case '\\':
      t.type = TokenType::SLASH;
      t.literal = "\\";
      break;
    case '(':
      t.type = TokenType::LPAREN;
      t.literal = "(";
      break;
    case ')':
      t.type = TokenType::RPAREN;
      t.literal = ")";
      break;
    case '%':
      t.type = TokenType::PERCENT;
      t.literal = "%";
      break;
    default:
    // check if is illegal letter
    // NOTE: REWRITE THIS FOR FUTURE SCIENCE FUNCTIONS
      if (isalpha(static_cast<int>(c))) {
        cout << "is letter\n"; 
        
        break;
      } else {
        t.type = TokenType::NUMBER;          
        int start = i;

        // go over each char while it is a number, to get whole number
        while (isdigit(input[i]) && i<input.length()) {          
          i++;
        }

        string s =  input.substr(start, i);
        t.literal = s;
        int num = stoi(s);
        cout << "is numeral\n";
      }
      break;
    }     
  
    readChar();
  }

  return tokens_list;
}

