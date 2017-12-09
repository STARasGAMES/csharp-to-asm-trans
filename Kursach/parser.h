#pragma once

#include <istream>
#include "token.h"

void parser(std::istream *);

// Represent non-terminal token nodes
typedef enum {
	programNode, blockNode, varNode, mvarsNode, exprNode, xNode,
	tNode, yNode, fNode, rNode, statsNode, mStatNode, statNode,
	inNode, outNode, ifNode, loopNode, assignNode, roNode
} NodeType;

/*------- TREE -------*/
struct Node {
	NodeType nodeType;
	std::vector<Token> tokenPtr; // linked-list of tokens of this node 
	struct Node *child1; // usually only up to 3 children needed 
	struct Node *child2;
	struct Node *child3;
	struct Node *child4; // but for <if> and <loop>, 4 children needed
};