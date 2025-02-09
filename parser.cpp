// #include "parser.h"

// using namespace std;

// // overloads == operator for Token class
// bool operator==(const Token& lhs, const Token& rhs) {
//   return (lhs.type == rhs.type) && (lhs.literal == rhs.literal);
// }
// // overloads == operator for Token class
// bool operator!=(const Token& lhs, const Token& rhs) {
//   return (lhs.type == rhs.type) && (lhs.literal == rhs.literal);
// }


// void parseExpression(string input) {
//   Token newToken = constructToken(input);
//   // checks input is valid (aka  a number)
//   try { stof(input); }
//   catch(const std::exception& e)
//   {
//     std::cerr << "Could not " << e.what() << " value '"<< input << "'" << '\n';
//   }
  
//   newToken.type = TokenType::NUMBER;
// } 

// // represents a test unit for parser
// struct ParserTest {
//   string input;
//   vector<Token> expectedTokens;
//   float expectedResult;
// };



// //  TESTING SECTION
// string errors;

// bool testParser() {
//   ParserTest test = {
//     "1 + 1", 
//     {
//       Token{TokenType::NUMBER, "1"},
//       Token{TokenType::PLUS, "+"},
//       Token{TokenType::NUMBER, "1"},
//       Token{TokenType::END, "END"},
//     },
//     2
//   };
//   vector<Token> result = tokenize(test);
//   bool passes = true;

//   if (result.size() != test.expectedTokens.size()) {
//     cerr << "results has wrong amount of tokens, got=%d, want=%d\n" << result.size(), test.expectedTokens.size();
//     passes=false;
//   }

//   for (int i=0; i < test.expectedTokens.size(); i++) {
//     if (result[i] != test.expectedTokens[i]) {
//       cerr << "Tokens don't match, got=%d, want=%d\n" << result[i].literal, test.expectedTokens[i].literal; 
//       passes=false;
//     }
//   }
  
//   return passes;
// }