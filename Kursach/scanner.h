#pragma once

#ifndef SCANNER_H
#define SCANNER_H


#include "token.h"
#include <istream>

int isExAcceptableChar(char);

int isDelimiter(const std::string);
int isOperator(const std::string);
int isKeyword(const std::string);


void InitScanner();

Token *Scan(std::istream *);

void printToken(Token*&);

TokenType GetTokenTypeByString(std::string &);
std::string GetStringByTokenType(const TokenType);

void displayStream(std::istream *);

#endif // !SCANNER_H
