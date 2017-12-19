#pragma once

#ifndef PARSER_H
#define PARSER_H


#include <istream>
#include "token.h"
#include "variableh.h"
#include "nodeh.h"

Node* Parse(Token*);
void ErrorTokenExceptation(std::string context, TokenType except, Token *recieved);
void OnEnterParseMethod(std::string context);
void OnExitParseMethod(std::string context);

void ErrorNodeExpectation(std::string context, NodeType expect);
/*------- TREE -------*/

bool Program(Token* _tk, Node *& outNode);
bool Literal(Token* _tk, Node*& outNode);
bool BooleanLiteral(Token* _tk, Node*& outNode);
bool Type(Token* _tk, Node*& outNode);

// EXPRESSIONS
bool PrimaryExpression(Token* _tk, Node*& outNode);
bool Expression(Token* _tk, Node*& outNode);
bool AssignmentExpression(Token* _tk, Node*& outNode);
bool MultiplicativeExpression(Token* _tk, Node*& outNode);
bool AdditiveExpression(Token* _tk, Node*& outNode);
bool RelationalExpression(Token* _tk, Node*& outNode);
bool AndExpression(Token* _tk, Node*& outNode);
bool ExclusiveOrExpression(Token* _tk, Node*& outNode);
bool InclusiveOrExpression(Token* _tk, Node*& outNode);
bool LogicalAndExpression(Token* _tk, Node*& outNode);
bool LogicalOrExpression(Token* _tk, Node*& outNode);
bool ConditionalExpression(Token* _tk, Node*& outNode);
bool AssignmentOperator(Token* _tk, Node*& outNode);
// !EXPRESSIONS

// STATEMENTS
bool StatementBlock(Token* _tk, Node*& outNode);
bool StatementSeq(Token* _tk, Node*& outNode);
bool Statement(Token* _tk, Node*& outNode);
bool LabeledStatement(Token* _tk, Node*& outNode);
bool ExpressionStatement(Token* _tk, Node*& outNode);
bool SelectionStatement(Token* _tk, Node*& outNode);
bool DeclaretionStatement(Token* _tk, Node*& outNode);
bool DeclaretionStatementList(Token* _tk, Node*& outNode);
// !STATEMENTS

bool DeclaretionGlobalSeq(Token* _tk, Node*& outNode);


#endif // !PARSER_H
