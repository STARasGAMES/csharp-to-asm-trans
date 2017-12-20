#ifndef NODEH_H
#define NODEH_H

#include <vector>
#include <string>
#include "token.h"
#include "variableh.h"

// Represent non-terminal token nodes
typedef enum {
	nodeProgram,
	nodeLiteral,
	nodeBooleanLiteral,
	nodeType,

	// EXPRESSIONS
	nodePrimaryExpression,
	nodeAssignmentExpression,
	nodeRelationalExpression,
	nodeLogicalExpression,
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

struct Node {

	Node(NodeType _nodeType)
	{
		nodeType = _nodeType;
		tokenPtr = new std::vector<Token>();
		parent = child1 = child2 = child3 = child4 = 0;
		expRetValType = vartNA;
	}
	NodeType nodeType;
	std::vector<Token> *tokenPtr;
	//Token *tokenPtr; // linked-list of tokens of this node 
	Node* parent;
	Node* child1; // usually only up to 3 children needed 
	Node* child2;
	Node* child3;
	Node* child4; // but for <if> and <loop>, 4 children needed

	VariableType expRetValType;

	void AddToken(Token*& tk) {
		tokenPtr->push_back(*tk);
	}

	std::string toString() {
		return nodeTypeStrArr[nodeType];
	}

private:
	std::vector<std::string> nodeTypeStrArr = {
		"Program",
		"Literal",
		"BooleanLiteral",
		"Type",

		// EXPRESSIONS
		"PrimaryExpression",
		"AssignmentExpression",
		"RelationalExpression",
		"LogicalExpression",
		// !EXPRESSIONS

		// STATEMENTS
		"StatementBlock",
		"StatementSeq",
		"Statement",
		"LabeledStatement",
		"ExpressionStatement",
		"SelectionStatement",
		"DeclaretionStatement",
		"DeclaretionStatementList",
		// !STATEMENTS

		"DeclaretionGlobalSeq"
	};
};

#endif
