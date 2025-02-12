// //  TESTING SECTION
// #include "parser.h"

// using namespace std;

// // represents a test unit for parser
// struct ParserTest {
//   string input;
//   vector<Token> expectedTokens;
//   float expectedResult;
// };


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