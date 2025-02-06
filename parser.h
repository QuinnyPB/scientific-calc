#ifndef PARSER_H
#define PARSER_H

#include <string>

Token constructToken(string input);
void parseExpression(string input);
Token tokenize(string input);

#endif