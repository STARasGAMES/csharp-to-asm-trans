#pragma once

#ifndef PARSER_H
#define PARSER_H


#include <istream>
#include "token.h"

void Parse(std::istream *);
void ErrorTokenExceptation(std::string context, TokenType except, Token *recieved);
void OnEnterParseMethod(std::string context);
void OnExitParseMethod(std::string context);
// Represent non-terminal token nodes
typedef enum {
	programNode, blockNode, varNode, mvarsNode, exprNode, xNode,
	tNode, yNode, fNode, rNode, statsNode, mStatNode, statNode,
	inNode, outNode, ifNode, loopNode, assignNode, roNode
} NodeType;

/*------- TREE -------*/
struct Node {

	Node()
	{
		tokenPtr = new std::vector<Token>();
	}
	NodeType nodeType;
	std::vector<Token> *tokenPtr;
	//Token *tokenPtr; // linked-list of tokens of this node 
	struct Node *child1; // usually only up to 3 children needed 
	struct Node *child2;
	struct Node *child3;
	struct Node *child4; // but for <if> and <loop>, 4 children needed
};
#endif // !PARSER_H
