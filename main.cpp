// #include <stdio.h>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <variant>
#include "lexer.h"
#include "errorstack.h"

using namespace std;
// used for output queue, so operators and numbers can go onto it e.g. '+' and 1234
using queue_var = variant<int, char>;

ostream& operator<<(ostream& os, const queue_var& qv) {
  std::visit([&os](auto&& arg) { os << arg; }, qv);
  return os;
}

// function initialisationg
void handleOperatorPrecedenceSwap(TokenType, char, map<TokenType, int>);
void popStackToQueue();
TokenType getTokenType(char);
int getAssociativity(char);
int applyOperator(int, int, char);
void printHelp();
void clearInput();
void print_queue(queue<queue_var>);
void print_stack(stack<queue_var>);

// global data structures
stack<char> operators;
queue<queue_var> output;
map<TokenType, int> precedences;  
int result;
bool previousIsNumber=false;

int main()
{
  vector<Token> tokens;
  ErrorStack errors = ErrorStack(); 
  string input;
  // setting precedences for order of operations
  precedences.insert(pair<TokenType, int>(TokenType::CARET, 4)); 
  precedences.insert(pair<TokenType, int>(TokenType::SLASH, 3)); 
  precedences.insert(pair<TokenType, int>(TokenType::ASTERISK, 3)); 
  precedences.insert(pair<TokenType, int>(TokenType::PLUS, 2)); 
  precedences.insert(pair<TokenType, int>(TokenType::MINUS, 2)); 
  
  while (true) {
    cout << "Enter your expression to be calculated ('?' for help): \n";
    getline(cin, input);
    input += "\n";
    // input = "1+ 2 - 3 * 4";
    
    Lexer lexer = Lexer(input);
    tokens = lexer.tokenize();
    
    cout << "starting token analysis loop...\n";
    for (Token tok : tokens) {
      char op = tok.literal[0]; // if operator than convert to char, else ignore this var
      
      cout << "TOKEN: " << op << "\n";
      // push to output if number,
      // else determine precedence and pop and/or push to operators
      switch (tok.type) {        
      case TokenType::NUMBER:
        // output.push(queue_var(stoi(tok.literal))); // type enforces number into int
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
        handleOperatorPrecedenceSwap(TokenType::PLUS, op, precedences);
        break;

      case TokenType::MINUS:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap(TokenType::MINUS, op, precedences);
        // operators.push(op);
        break;

      case TokenType::ASTERISK:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap(TokenType::ASTERISK, op, precedences);
        // operators.push(op);
        break;

      case TokenType::SLASH:
        // check if top has precedence, then pop
        handleOperatorPrecedenceSwap(TokenType::SLASH, op, precedences);
        // operators.push(op);
        break;

      case TokenType::LPAREN:
        handleOperatorPrecedenceSwap(TokenType::LPAREN, op, precedences);
        // operators.push(op);
        break;

      case TokenType::RPAREN:
        handleOperatorPrecedenceSwap(TokenType::RPAREN, op, precedences);
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
        cout << "Popping stack to queue...\t";
        popStackToQueue();
        break;
      
      default:
        cout << "cannot handle " << tok.toString() << "\n"; 
        break;
      } // END SWITCH
    }  // END FOR    
    previousIsNumber=false;
      
    // POP OFF QUEUE AND APPLY OPERATORS
    stack<queue_var> stack;
    // pop queue one by one into new stack and apply operators onto last two elements as they come    
    while (!output.empty()) {    
      cout << "Queue:\n";
      print_queue(output);
      cout << "Stack: \n";
      print_stack(stack); 

      // if output.top is number, push to stack
      if (holds_alternative<int>(output.front())) {
        stack.push(output.front());        
        output.pop();
      } 
      // if output.top is operator, take two off stack, do operation, push back onto stack
      else {
        cout << "Doing calculation...\n";
        try
        {
          // calculate value and push back onto stack
          char op = get<char>(output.front()); output.pop(); 
          int i = get<int>(stack.top()); stack.pop();
          int j = get<int>(stack.top()); stack.pop();
          int k = applyOperator(j, i, op);
          
          stack.push(k);  
          cout << "\t" << j << " " << op << " " << i << " = " << k << "\n"; 
        }
        catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }        
      } 
    } // END WHILE
    result = get<int>(stack.top()); stack.pop();
    cout << "Result: \n" << result << "\n";
    
    // clears cin buffer
    cin.clear();
    fflush(stdin);
  } // END WHILE
  
  return 0; 
} 

// actual application of an operator onto given values
int applyOperator(int left, int right, char op) {
  if (!left) {
    left = 0;
  }
  if (!right) {
    right = 0;
  }
  // if (!right && op == '-') {
  //   return -left;
  // } else if (!left && op == '-') {
  //   return 0 - right;
  // } else if (!left && !right) {
  //   return 0;
  // }
  // else if (!right && op != '-') {
  //   return NULL;
  // }

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
    cout << "Unknown operator '" << op << "'\n";
    return 0;
  }
}

// handles the functionality of swapping and taking operators off the operator stack if 
// they are same precedence
void  handleOperatorPrecedenceSwap(TokenType currType, char op, map<TokenType, int> precedences) {
  cout << "handleOperatorPrecedenceSwap()\n";  
  // pushes top of operator stack onto queue  
  while (!operators.empty()) {
    // represents top tokens on operator stack  
    TokenType topToken = getTokenType(operators.top());
    int topPrecedence = precedences.at(topToken);    
    
    if (topPrecedence > precedences.at(currType) && topToken != TokenType::LPAREN) {
      output.push(queue_var(operators.top()));
      operators.pop();
      continue;
    } else {
      break;
    }
  }
  operators.push(op); // regardless, pushes operator onto operator stack
  previousIsNumber=false;
  
  return;
}

// pop entire stack onto queue
void popStackToQueue() {
  while (!operators.empty()) {
    output.push(queue_var(operators.top()));
    operators.pop();
  }
  cout << "finished popping stack...\n";
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
    case '%':
      return TokenType::PERCENT;
    case '(':
      return TokenType::LPAREN;
    case ')':
      return TokenType::RPAREN;

    default:
      return TokenType::UNKNOWN;    
  } 
}

// returns associativity of the operator
// left == 0 and right == 1
int getAssociativity(char ch) {
  if (ch == '^') {
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