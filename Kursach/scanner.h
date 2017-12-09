#pragma once

#include "token.h"
#include <istream>

int isExAcceptableChar(char);

int isDelimiter(const std::string);
int isOperator(const std::string);
int isKeyword(const std::string);


void InitScanner();

Token Scan(std::istream *);

void printToken(Token);

TokenType getTokenTypeByString(const std::string);
std::string GetStringByTokenType(const TokenType);

void displayStream(std::istream *);