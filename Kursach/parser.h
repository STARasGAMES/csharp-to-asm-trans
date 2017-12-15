#pragma once

#ifndef PARSER_H
#define PARSER_H


#include <istream>
#include "token.h"

struct Node;

Node* Parse(Token*);
void ErrorTokenExceptation(std::string context, TokenType except, Token *recieved);
void OnEnterParseMethod(std::string context);
void OnExitParseMethod(std::string context);
// Represent non-terminal token nodes
typedef enum {
	nodeProgram,
	nodeLiteral,
	nodeBooleanLiteral,
	nodeType,

	// EXPRESSIONS
	nodePrimaryExpression,
	nodeExpression,
	nodeAssignmentExpression,
	nodeMultiplicativeExpression,
	nodeAdditiveExpression,
	nodeShiftExpression,
	nodeRelationalExpression,
	nodeEqualityExpression,
	nodeAndExpression,
	nodeExclusiveOrExpression,
	nodeInclusiveOrExpression,
	nodeLogicalAndExpression,
	nodeLogicalOrExpression,
	nodeConditionalExpression,
	nodeAssignmentOperator,
	// !EXPRESSIONS

	// STATEMENTS
	nodeStatementBlock,
	nodeStatementSeq,
	nodeStatement,
	nodeLabeledStatement,
	nodeExpressionStatement,
	nodeSelectionStatement,
	nodeDeclaretionStatement,
	nodeDeclaretionStatementList,
	// !STATEMENTS

	nodeDeclaretionGlobalSeq


} NodeType;

void ErrorNodeExpectation(std::string context, NodeType expect);
/*------- TREE -------*/
struct Node {

	Node(NodeType _nodeType)
	{
		nodeType = _nodeType;
		tokenPtr = new std::vector<Token>();
		parent = child1 = child2 = child3 = child4 = 0;
	}
	NodeType nodeType;
	std::vector<Token> *tokenPtr;
	//Token *tokenPtr; // linked-list of tokens of this node 
	Node* parent;
	Node* child1; // usually only up to 3 children needed 
	Node* child2;
	Node* child3;
	Node* child4; // but for <if> and <loop>, 4 children needed

	void AddToken(Token*& tk) {
		tokenPtr->push_back(*tk);
	}
};

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
bool ShiftExpression(Token* _tk, Node*& outNode);
bool RelationalExpression(Token* _tk, Node*& outNode);
bool EqualityExpression(Token* _tk, Node*& outNode);
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
