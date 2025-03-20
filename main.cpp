// #include <stdio.h>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <variant>
#include <format>
#include "lexer.h"
#include "errorqueue.h"

using namespace std;
// used for output queue, so operators and numbers can go onto it e.g. '+' and 1234
using queue_var = variant<int, char>;

ostream& operator<<(ostream& os, const queue_var& qv) {
  std::visit([&os](auto&& arg) { os << arg; }, qv);
  return os;
}

// function initialisationg
void handleOperatorPrecedenceSwap(TokenType, char);
void handleFunctionCall();
void popStackToQueue();
TokenType getTokenType(char);
bool isUnary(char, int, vector<Token>*);
bool isOperator(string&);
void handleOperator(char, stack<queue_var> *stack);
int applyOperator(int, int, char);
int getAssociativity(char);
void printHelp();
void clearInput();
void print_queue(queue<queue_var>);
void print_stack(stack<queue_var>);
string queue_to_string(queue<queue_var>);
string stack_to_string(stack<queue_var>);
void tests();

// global data structures
ErrorQueue errors_list = ErrorQueue(); 
stack<char> operators;
queue<queue_var> output;
map<TokenType, int> precedences;  
int result;
bool previousIsNumber=false;
bool isCustomInput=false;

int calculator(string);

int main() {
  // setting precedences for order of operations
  precedences.insert(pair<TokenType, int>(TokenType::CARET, 4)); 
  precedences.insert(pair<TokenType, int>(TokenType::NEGATIVE, 4)); // the negative unary
  precedences.insert(pair<TokenType, int>(TokenType::SLASH, 3)); 
  precedences.insert(pair<TokenType, int>(TokenType::ASTERISK, 3)); 
  precedences.insert(pair<TokenType, int>(TokenType::PLUS, 2)); 
  precedences.insert(pair<TokenType, int>(TokenType::MINUS, 2)); 
  // this is needed to prevent map.at() error in handleOperatorPrecedenceSwap()
  precedences.insert(pair<TokenType, int>(TokenType::LPAREN, 2)); 

  // for using test set
  tests();

  // for custom use
  while (true) { 
    try
    {
      int res = calculator(""); 
      printf("%d\n", res);
    }
    catch(const std::exception& e)
    {
      printf("Could not handle that expression!!\n");
      std::cerr << e.what() << '\n';
      errors_list.print_errors();
    }    
  }
}

int calculator(string input="")
{
  vector<Token> tokens;
  
    while (input == "") {
      cout << "Enter your expression to be calculated ('?' for help): \n";
      getline(cin, input);
      isCustomInput = true;
    }
    input += "\n";

    errors_list.add_error(input.c_str());
    
    // record initial input for error list
    errors_list.set_input(input);
    
    Lexer lexer = Lexer(input);
    tokens = lexer.tokenize();
    
    for (int i=0; i < int(tokens.size()); i++) {
      Token tok = tokens[i];
      char op = tok.literal[0]; // if operator than convert to char, else ignore this var

      // information for error queue if it appears
      string errstr = tok.toString().c_str();
      errors_list.add_error(errstr);
      
      // push to output if number,
      // else determine precedence and pop and/or push to operators
      switch (tok.type) {        
      case TokenType::NUMBER:
        // correct for incorrect user spacing between numbers
        if (previousIsNumber) {
          int x = get<int>(output.back());
          string str = to_string(x);
          str += tok.literal;
          output.back() = queue_var(stoi(str));
        } else {
          output.push(queue_var(stoi(tok.literal))); // type enforces number into int
        }
        previousIsNumber=true;
        break;

      case TokenType::PLUS:
        handleOperatorPrecedenceSwap(TokenType::PLUS, op);
        break;

      case TokenType::MINUS:
        // check if unary, make negative by pushing 'n' 
        if (isUnary(op, i, &tokens)) {
          handleOperatorPrecedenceSwap(TokenType::NEGATIVE, 'n');
        } else {
          // check if top has precedence, then pop
          handleOperatorPrecedenceSwap(TokenType::MINUS, op);
        }
          // handleOperatorPrecedenceSwap(TokenType::MINUS, op);
        break;

      case TokenType::ASTERISK:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap(TokenType::ASTERISK, op);
        break;

      case TokenType::SLASH:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap(TokenType::SLASH, op);
        break;

      case TokenType::LPAREN:
        operators.push('(');
        break;

      case TokenType::RPAREN:
        { 
          bool found=false;

          try {
            while (operators.top() != '(') {
              // check for mismatched parentheses 
              if (operators.empty() && found == false) {
                string errstr = "calculator.switch-RPAREN: Error: Mismatched parentheses\n";
                errors_list.add_error(errstr);
                return 0;
              }
              
              output.push(queue_var(operators.top()));
              operators.pop();
  
              // NOTE: add functionality for handling functions
              if (operators.top() == '(') {
                found=true;
                operators.pop();
                break;
              }
            }
          }
          catch(const std::exception& e)
          {
            std::cerr << e.what() << '\n';
            errors_list.add_error("Error while looping operators for '('\n");
          }
          
        }
        break;

      // clears screen and prints useful instructions/tips/commands
      case TokenType::QUESTIONMARK:
        system("cls");
        printHelp();
        break;
      
      // clears input from last calculations 
      case TokenType::CLEAR:
        clearInput();
        break;

      case TokenType::END:
        // pop entire operator stack to output   
        errors_list.add_error("Popping stack to queue...");
        popStackToQueue();
        break;
      
      default:
        string errstr = "calculator.switch-default: cannot handle " + tok.toString();
        errors_list.add_error(errstr);
        break;
      } // END SWITCH
    }  // END FOR    
    previousIsNumber=false;
      
    // POP OFF QUEUE AND APPLY OPERATORS
    stack<queue_var> stack;
    // pop queue one by one into new stack and apply operators onto last two elements as they come    
    while (!output.empty()) {    
      errors_list.add_error("Queue: " + queue_to_string(output));
      errors_list.add_error("Stack: " + stack_to_string(stack));
      
      // if output.top is number, push to stack
      if (holds_alternative<int>(output.front())) {
        stack.push(output.front());        
        output.pop();
      } 
      // if output.top is operator, handle that operator type
      // take two off stack, do operation, push back onto stack
      else {
        // calculate value and push back onto stack
        char op = get<char>(output.front()); output.pop();
        handleOperator(op, &stack);           
      } 
    } // END INNER WHILE
    result = get<int>(stack.top()); stack.pop();   

    // clears cin buffer
    cin.clear();
    fflush(stdin);

    // print runtime errors if custom input
    if (isCustomInput) {
      errors_list.print_errors();
      errors_list.clear();
    }
  
  return result; 
} 

// The '-' token is at the very start of the input
// The '-' token is after a ( token
// The '-' token is after a binary operator token such as +
// The '-' token is after after another - token    
bool isUnary(char ch, int currIndex, vector<Token> *tokens) {
  if (currIndex == 0 && ch == '-') {
    return true;
  } 
  if (*&tokens->at(currIndex - 1).literal == string("(") ) {
    return true;
  }
  string c = tokens->at(currIndex - 1).literal;
  if (isOperator(c)) {
    return true;
  }

  return false;
}

bool isOperator(string& ch) {
  if (ch == "+" || ch == "-" || ch == "*" || ch == "/" || ch == "%" ) {
    return true;
  }
  return false;
}

// handles operator functionality
void handleOperator(char op, stack<queue_var> *stack) {
  // string errstr("handling operator '%c' (ASCII: %d)\n", op, op);
  // errors_list.add_error(errstr);

  try {
    if (op == 'n') {
      int& topNum = get<int>(stack->top());
      topNum = -topNum;
    } else {
      int right = get<int>(stack->top()); stack->pop();
      int left = get<int>(stack->top()); stack->pop();  
      queue_var res = applyOperator(left, right, op);  
      stack->push(res);
    }
  }
  catch(const std::exception& e) {
    // printf("Cannot handle operator '%c' (ASCII: %d)\n", op, op);
    string errstr("handleOperator(): Cannot handle operator '%c' (ASCII: %d)", op, op);
    errors_list.add_error(errstr);
    std::cerr << e.what() << '\n';
  } 

}

// actual application of an operator onto given values
int applyOperator(int left, int right, char op) {
  printf("%d %c %d\n", left, op, right);
  switch (op) {
  case '+':
    return left + right;
  case '-':
    return left - right;
  case '*':
    return left * right;
  case '/':
    return left / right;
  case '%':
    return left % right;

  default:
    // printf("Unknown operator '%c' (ASCII: %d)\n", op, op);
    string errstr("applyOperator(): Unknown operator '%c' (ASCII: %d)", op, op);
    errors_list.add_error(errstr);
    
    return 0;
  }
}

// handles the functionality of swapping and taking operators off the operator stack if 
// they are same precedence
void  handleOperatorPrecedenceSwap(TokenType currType, char op) {
  // pushes top of operator stack onto queue  
  int counter=0, operators_size = int(operators.size());
  while (
    !operators.empty() || 
    counter > operators_size  
  ){    
    // represents top tokens on operator stack  
    TokenType topToken = getTokenType(operators.top());
    int topPrecedence = precedences.at(topToken);    
    counter++;

    if (topToken != TokenType::LPAREN && 
      (topPrecedence > precedences.at(currType) || 
      (topPrecedence == precedences.at(currType) && getAssociativity(op) == 0)
      )
    ) {
      output.push(queue_var(operators.top()));
      operators.pop();
      continue;
    } 
  
    break;
  }

  // error check
  if (counter > operators_size) {
    string errstr("handleOperatorPrecedenceSwap(): counter exceeded hard limit operators.size() (%d)", operators_size);
    errors_list.add_error(errstr);
  }
  
  operators.push(op); // regardless, pushes operator onto operator stack
  previousIsNumber=false; // necessary for handling incorrect user spacing
  
  return;
}

void handleFunctionCall() {}

// pop entire stack onto queue
void popStackToQueue() {
  int counter = 0, operators_size = int(operators.size());
  while (!operators.empty() || counter > operators_size) {
    output.push(queue_var(operators.top()));
    operators.pop();
    counter++;
  }
  if (counter > operators_size) {
    string errstr("popStackToQueue(): counter (%d) exceeded hard limit of operators.size() (%d)", counter, operators_size);
    errors_list.add_error(errstr);
  }
}

TokenType getTokenType(char ch) {
  switch (ch) {
    case '+':
      return TokenType::PLUS;
    case '-':
      return TokenType::MINUS;
    case '*':
      return TokenType::ASTERISK;
    case '/':
      return TokenType::SLASH;
    case '^':
      return TokenType::CARET;
    case 'n':
      return TokenType::NEGATIVE;
    case '%':
      return TokenType::PERCENT;
    case '(':
      return TokenType::LPAREN;
    case ')':
      return TokenType::RPAREN;

    default:
      string errstr("getTokenType(): unknown char %c", ch);
      errors_list.add_error(errstr);
      return TokenType::UNKNOWN;    
  } 
}

// returns associativity of the operator
// left == 0 and right == 1
int getAssociativity(char ch) {
  if (ch == '^' || ch == 'n') {
    printf("'%c' is right assc\n", ch);
    return 1;
  } else {
    return 0;
  }
}

void printHelp() {
  cout << "This scientific calculator can do the following: \n";
  cout << "'?': Prints this help screen.\n";
  return;
}

void clearInput() {
  cout << "clearing...\n";
  #undef result
}

// Print the queue
void print_queue(queue<queue_var> q)
{
    queue<queue_var> temp = q;
    while (!temp.empty()) {
        // print for chars
      cout << temp.front() <<" ";
      temp.pop();
    }
    cout << '\n';
}

// Print the stack
void print_stack(stack<queue_var> q)
{
  stack<queue_var> temp = q;
  while (!temp.empty()) {
    // print for chars
    cout << temp.top() <<" ";
    temp.pop();
  }
  cout << '\n';
}

string queue_to_string(queue<queue_var> q) {
  queue<queue_var> temp = q;
  string str;
  while (!temp.empty()) {
    // print for chars

    // either an int or char
    if (holds_alternative<int>(temp.front())) {
      int curr = get<int>(temp.front()); 
      str += to_string(curr);
    } else {
      char curr = get<char>(temp.front());
      str += curr;
    }
    str += " ";    
    
    temp.pop();
  }
  return str;
}

string stack_to_string(stack<queue_var> q) {
  stack<queue_var> temp = q;
  string str;
  while (!temp.empty()) {
    // either an int or char
    if (holds_alternative<int>(temp.top())) {
      int curr = get<int>(temp.top()); 
      str += to_string(curr);
    } else {
      char curr = get<char>(temp.top());
      str += curr;
    }
    str += " ";    
    
    temp.pop();
  }
  return str;
}

struct TestCase {
  string input;
  int expected;
};

void tests() {
  vector<TestCase> tests = {
    TestCase{"1", 1},
    TestCase{"1 + 1", 2},
    TestCase{"1 - 1", 0},
    TestCase{"1 * 1", 1},
    TestCase{"1 / 1", 1},
    TestCase{"1 + 2 + 3", 6},
    TestCase{"3 - 2 - 1", 0},
    TestCase{"1 * 2 * 3", 6},
    TestCase{"4 / 2 / 1", 2},
    TestCase{"1 - 1", 0},
    TestCase{"1 - 3", -2},
    TestCase{"1 * 2", 2},
    TestCase{"1 - 1 * 3", -2},
    TestCase{"1 + 1 * 4 - 2", 3},
    TestCase{"1  1", 11},
    TestCase{"11 0 2  3", 11023},
    TestCase{"-1", -1},
    TestCase{"1 + -1", 0},
    TestCase{"1 * -1", -1},
    TestCase{"1 + 4 + -1", 4},
    TestCase{"10 + 2 * -1", 8},
    TestCase{"-1 + -1", -2},
    TestCase{"(1 + 1)", 2},
    TestCase{"(1 + 1) * 4", 8},
    TestCase{"(1 + 1) * (3 + 1)", 8},
    TestCase{"((3 + 3) * 2) / (3 + 1)", 3},
    TestCase{"((1 0 + 2 10) * 2) / (10 +      3     0)", 11},
  };

  for ( int i = 0; i < int(tests.size()); i++ ) {
    try
    {
      printf("TEST: %s\n", tests[i].input.c_str());
      int res = calculator(tests[i].input.c_str());
          
      if (res != tests[i].expected) {
        printf("Test %d FAILED:\n\tinput: %s\n\texpected: %d\n\tgot=%d\n", i, tests[i].input.c_str(), tests[i].expected, res);
  
        printf("List of errors:\n");
        errors_list.print_errors();
      } else {
        // printf("Test %d SUCCESS:\n\tinput: %s\n\tgot: %d\n", i, tests[i].input.c_str(), result);
        printf("Test %d SUCCESS: Result == Expected : %d == %d\n", i, res, tests[i].expected);
      }
      // clears error list for new test set
      printf("\n");
    }
    catch(const std::exception& e)
    {
      errors_list.add_error("Error while looping tests...\n");
      std::cerr << e.what() << '\n';
      errors_list.print_errors();
    }
    
    errors_list.clear();
  }

  return;
}

