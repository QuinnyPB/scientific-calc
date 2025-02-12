#include "lexer.h"
#include "errorstack.h"

using namespace std;

Lexer::Lexer(string &input) : input(input), curr(0), next(0) {      
  readChar();
}  

vector<Token> Lexer::tokenize() {  
  vector<Token> tokens;

  while (curr < input.length()) {   
    Token t;
    skipwhitespace();  

    cout << "curr ch: " << input[curr] << "\t next ch: " << input[next] <<"\n";
        
    switch (ch) {
    case '+':
      t = Token(TokenType::PLUS, ch);
      break;
    case '-':
      t = Token(TokenType::MINUS, ch);
      break;
    case '*':
      t = Token(TokenType::ASTERISK, ch);
      break;
    case '/':
      t = Token(TokenType::SLASH, ch);
      break;
    case '(':
      t = Token(TokenType::LPAREN, ch);
      break;
    case ')':
      t = Token(TokenType::RPAREN, ch);
      break;
    case '%':
      t = Token(TokenType::PERCENT, ch);
      break;
    default:
    // check if is illegal letter
    // NOTE: REWRITE THIS FOR FUTURE SCIENCE FUNCTIONS
      if (isLetter(ch)) {
        // cout << "is letter\n";   
        t = Token(TokenType::UNKNOWN, ch);

      } 
      else if (isDigit(ch)) {
        t.type = TokenType::NUMBER;          
        t.literal = readNumber();        
        tokens.push_back(t);
        continue; // required for avoiding double readChar() call from readNumber()
      } 
      else {
        t = Token(TokenType::UNKNOWN, ch);
      }
      break;
    }     

    
    tokens.push_back(t);
    if (peekChar() != 0) {
      readChar();
    } else {
      return tokens;
    }
  }

  return tokens;
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

char Lexer::peekChar() {
  if (next >= input.length()) {
    return 0;
  } else {
    return input[next];
  }
}

void Lexer::reverseChar() {
  if (curr == 0) {
    return;
  } else {
    next = curr;
    curr--;    
    ch = input[curr];
  }
}

void Lexer::insertInput(string& input) {
  Lexer::input = input;
  readChar();
}

// RUNS OVER INPUT LENGTH, has to do with needing space at the end
string Lexer::readNumber() {
  int pos = curr;
  while (curr < input.length() && isDigit(input.at(curr))) {
    readChar();
  }

  return input.substr(pos, curr - pos);
}

void Lexer::skipwhitespace() {
  while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
    readChar();
  };
}

bool Lexer::isLetter(char ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_'; 
}

bool Lexer::isDigit(char ch) {
  return '0' <= ch && ch <= '9';
}