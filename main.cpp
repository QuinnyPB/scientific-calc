// #include <stdio.h>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <variant>
#include <format>
#include "lexer.h"
#include "errorqueue.h"
#include "Fraction.h"

// Explanation of class T
// I have designed this program with the idea that it will use either int or Fraction(int, int) so far

using namespace std;
// used for output queue, so operators and numbers can go onto it e.g. '+' and 1234
template <class T>
using queue_var = variant<typename std::decay_t<T>::type, char>; // typename... is necessary for avoiding an Internal Compiler Error due to std::variant usage

template <class T>
ostream& operator<<(ostream& os, const queue_var<T>& qv) {
  std::visit([&os](auto&& arg) { os << arg; }, qv);
  return os;
}

// advanced operators
enum class AdvOp { 
  MODULO,
};

const std::unordered_map<AdvOp, string> adv_operations;

// function initialisationg
template <class T>
T calculator(string input = "", T type = T());
template <class T>
void handleOperatorPrecedenceSwap(TokenType, char);
void handleFunctionCall();
template <class T>
void popStackToQueue();
TokenType getTokenType(char);
bool isUnary(char, int, vector<Token>*);
bool isOperator(string&);
template <class T>
void handleOperator(char, stack<queue_var<T>> *stack);
template <class T>
T applyOperator(T, T, char);
int getAssociativity(char);
void printHelp();
void clearInput();
template <class T>
void print_queue(queue<queue_var<T>>);
template <class T>
void print_stack(stack<queue_var<T>>);
template <class T>
string queue_to_string(queue<queue_var<T>>);
template <class T>
string stack_to_string(stack<queue_var<T>>);
void tests();

// global data structures
ErrorQueue errors_list = ErrorQueue(); 
stack<char> operators;
template <class T>
queue<queue_var<T>> output;
map<TokenType, int> precedences;  
double result;
bool previousIsNumber=false;
bool isCustomInput=false;



int main() {
  // setting precedences for order of operations
  precedences.insert(pair<TokenType, int>(TokenType::CARET, 4)); 
  precedences.insert(pair<TokenType, int>(TokenType::NEGATIVE, 4)); // the negative unary
  precedences.insert(pair<TokenType, int>(TokenType::SLASH, 3)); 
  precedences.insert(pair<TokenType, int>(TokenType::ASTERISK, 3)); 
  precedences.insert(pair<TokenType, int>(TokenType::PLUS, 2)); 
  precedences.insert(pair<TokenType, int>(TokenType::MINUS, 2)); 
  
  precedences.insert(pair<TokenType, int>(TokenType::LPAREN, 2));  // this is needed to prevent map.at() error in handleOperatorPrecedenceSwap()

  // for using test set
  tests();

  // for custom use
  while (true) { 
    try
    {
      // change this value for different types
      // Fraction f(1, 1);
      // Fraction res = calculator("", i); 
      // printf("%s\n", res); // Fraction()
      
      int i = 1;
      int res = calculator("", i); 
      printf("%d\n", res); // int
    }
    catch(const std::exception& e)
    {
      printf("Could not handle that expression!!\n");
      std::cerr << e.what() << '\n';
      errors_list.print_errors();
    }    
  }
}

template <class T>
T calculator(string input, T type)
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
          T x = get<T>(output<T>.back());
          string str = to_string(x);
          str += tok.literal;
          output<T>.back() = queue_var(stod(str));
        } else {
          output<T>.push(queue_var(stod(tok.literal))); // type enforces number into double
        }
        previousIsNumber=true;
        break;

      case TokenType::PLUS:
        handleOperatorPrecedenceSwap<T>(TokenType::PLUS, op);
        break;

      case TokenType::MINUS:
        // check if unary, make negative by pushing 'n' 
        if (isUnary(op, i, &tokens)) {
          handleOperatorPrecedenceSwap<T>(TokenType::NEGATIVE, 'n');
        } else {
          // check if top has precedence, then pop
          handleOperatorPrecedenceSwap<T>(TokenType::MINUS, op);
        }
          // handleOperatorPrecedenceSwap<T>(TokenType::MINUS, op);
        break;

      case TokenType::ASTERISK:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap<T>(TokenType::ASTERISK, op);
        break;

      case TokenType::SLASH:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap<T>(TokenType::SLASH, op);
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
              
              output<T>.push(queue_var(operators.top()));
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
        popStackToQueue<T>();
        break;
      
      default:
        string errstr = "calculator.switch-default: cannot handle " + tok.toString();
        errors_list.add_error(errstr);
        break;
      } // END SWITCH
    }  // END FOR    
    previousIsNumber=false;
      
    // POP OFF QUEUE AND APPLY OPERATORS
    stack<queue_var<T>> stack;
    // pop queue one by one into new stack and apply operators onto last two elements as they come    
    while (!output<T>.empty()) {    
      errors_list.add_error("Queue: " + queue_to_string<T>(output<T>));
      errors_list.add_error("Stack: " + stack_to_string<T>(stack));
      
      // if output.top is number, push to stack
      if (holds_alternative<double>(output<T>.front())) {
        stack.push(output<T>.front());        
        output<T>.pop();
      } 
      // if output<T>.top is operator, handle that operator type
      // take two off stack, do operation, push back onto stack
      else {
        // calculate value and push back onto stack
        char op = get<char>(output<T>.front()); output<T>.pop();
        handleOperator<T>(op, &stack);           
      } 
    } // END INNER WHILE
    result = get<T>(stack.top()); stack.pop();   

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
template <class T>
void handleOperator(char op, stack<queue_var<T>> *stack) {
  // string errstr("handling operator '%c' (ASCII: %f)\n", op, op);
  // errors_list.add_error(errstr);

  try {
    if (op == 'n') {
      T& topNum = get<T>(stack->top());
      topNum = -topNum;
    } else {
      T right = get<T>(stack->top()); stack->pop();
      T left = get<T>(stack->top()); stack->pop();  
      queue_var res = applyOperator(left, right, op);  
      stack->push(res);
    }
  }
  catch(const std::exception& e) {
    // printf("Cannot handle operator '%c' (ASCII: %f)\n", op, op);
    string errstr("handleOperator(): Cannot handle operator '%c' (ASCII: %f)", op, op);
    errors_list.add_error(errstr);
    std::cerr << e.what() << '\n';
  } 

}

// actual application of an operator onto given values
template <class T>
int applyOperator(T left, T right, char op) {
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
  // case '%':
  //   return left % right;

  default:
    // printf("Unknown operator '%c' (ASCII: %f)\n", op, op);
    string errstr("applyOperator(): Unknown operator '%c' (ASCII: %f)", op, op);
    errors_list.add_error(errstr);
    
    return 0;
  }
}

// handles the functionality of swapping and taking operators off the operator stack if 
// they are same precedence
template <class T>
void handleOperatorPrecedenceSwap(TokenType currType, char op) {
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
      output<T>.push(queue_var(operators.top()));
      operators.pop();
      continue;
    } 
  
    break;
  }

  // error check
  if (counter > operators_size) {
    string errstr("handleOperatorPrecedenceSwap(): counter exceeded hard limit operators.size() (%f)", operators_size);
    errors_list.add_error(errstr);
  }
  
  operators.push(op); // regardless, pushes operator onto operator stack
  previousIsNumber=false; // necessary for handling incorrect user spacing
  
  return;
}

void handleFunctionCall() {}

// pop entire stack onto queue
template <class T>
void popStackToQueue() {
  int counter = 0, operators_size = int(operators.size());
  while (!operators.empty() || counter > operators_size) {
    output<T>.push(queue_var(operators.top()));
    operators.pop();
    counter++;
  }
  if (counter > operators_size) {
    string errstr("popStackToQueue(): counter (%f) exceeded hard limit of operators.size() (%f)", counter, operators_size);
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
template <class T>
void print_queue(queue<queue_var<T>> q)
{
    queue<queue_var<T>> temp = q;
    while (!temp.empty()) {
        // print for chars
      cout << temp.front() <<" ";
      temp.pop();
    }
    cout << '\n';
}

// Print the stack
template <class T>
void print_stack(stack<queue_var<T>> q)
{
  stack<queue_var<T>> temp = q;
  while (!temp.empty()) {
    // print for chars
    cout << temp.top() <<" ";
    temp.pop();
  }
  cout << '\n';
}

template <class T>
string queue_to_string(queue<queue_var<T>> q) {
  queue<queue_var<T>> temp = q;
  string str;
  while (!temp.empty()) {
    // print for chars

    // either an double or char
    if (holds_alternative<T>(temp.front())) {
      T curr = get<T>(temp.front()); 
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

template <class T>
string stack_to_string(stack<queue_var<T>> q) {
  stack<queue_var<T>> temp = q;
  string str;
  while (!temp.empty()) {
    // either an double or char
    if (holds_alternative<T>(temp.top())) {
      T curr = get<T>(temp.top()); 
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

template <class T>
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
      int res = calculator(tests[i].input.c_str(), i);
          
      if (res != tests[i].expected) {
        printf("Test %f FAILED:\n\tinput: %s\n\texpected: %f\n\tgot=%f\n", i, tests[i].input.c_str(), tests[i].expected, res);
  
        printf("List of errors:\n");
        errors_list.print_errors();
      } else {
        // printf("Test %f SUCCESS:\n\tinput: %s\n\tgot: %f\n", i, tests[i].input.c_str(), result);
        printf("Test %f SUCCESS: expected=%f, got=%f\n", i, res, tests[i].expected);
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

