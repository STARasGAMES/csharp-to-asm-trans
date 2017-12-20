#include "semanticsanalyzer.h"
#include <string>
#include <map>
#include "parser.h"


VariableType curDeclaretionType = vartNA;

std::map<std::string, Variable> mapOfVars;

std::map<std::string, Variable>& GetMapOfVars()
{
	return mapOfVars;
}

int lvl = 0;

bool SemanticsAnalysis(Node *& rootNode)
{
	if (rootNode == 0) {
		printf("empty node\n");
		return false;
	}
	printf("%*s %d %s\n", lvl * 4, "", lvl, rootNode->toString().c_str());
	lvl++;
	if (rootNode->nodeType == nodeDeclaretionStatement)
	{
		curDeclaretionType = vartNA;
		std::string str = rootNode->child1->tokenPtr->at(0).str;
		if (str._Equal("int"))
			curDeclaretionType = vartInt;
		else
			if (str._Equal("bool"))
				curDeclaretionType = vartBool;
		bool res = SemanticsAnalysis(rootNode->child2);
		curDeclaretionType = vartNA;
		if (res)
		lvl--; return res;
	}
	if (rootNode->nodeType == nodeDeclaretionStatementList)
	{
		if (rootNode->tokenPtr->size() != 1)
		{
			printf("ERROR semant nodeDeclaretionStatementList 1\n");
			return false;
		}
		std::string str = rootNode->tokenPtr->at(0).str;
		if (mapOfVars.find(str) != mapOfVars.end())
		{
			printf("ERROR '%s' already declared\n", str.c_str());
			return false;
		}
		printf("declare new var '%s'\n", str.c_str());
		Variable* newVar = new Variable();
		newVar->str = str;
		newVar->type = curDeclaretionType;
		newVar->isInitialyzed = false;
		newVar->id = mapOfVars.size();
		// if there is initialization
		if (rootNode->child1 != 0) {
			// Literal
			if (rootNode->child1->nodeType == nodeLiteral)
			{
				// NUMBER
				if (rootNode->child1->child1 == 0) {
					if (curDeclaretionType != vartInt) {
						printf("declaration operands must have same type1. found %d, expect %d\n", vartInt, curDeclaretionType);
						return false;
					}
					printf("init of INT\n");
					newVar->isInitialyzed = true;
					newVar->initValue = rootNode->child1->tokenPtr->at(0).str;
				}
				// BOOLEAN
				else {
					if (curDeclaretionType != vartBool) {
						printf("declaration operands must have same type2. found %d, expect %d\n", vartBool, curDeclaretionType);
						return false;
					}
					printf("init of BOOL\n");
					newVar->isInitialyzed = true;
					newVar->initValue = (rootNode->child1->child1->tokenPtr->at(0).str._Equal("true") ? "1" : "0");
				}
			}
			else {
				printf("WTF nodeDeclaretionStatementList\n");
			}
		}
		mapOfVars[str] = *newVar;
		if (rootNode->child2 != 0) {
			if (SemanticsAnalysis(rootNode->child2)) {
				lvl--; return true;
			}
			return false;
		}
		lvl--; return true;
	}

	if (rootNode->nodeType == nodeSelectionStatement) {
		if (rootNode->tokenPtr->operator[](0).type == tkKeywordIF)
		{
			if (rootNode->child1 == 0) {
				return false;
			}
			// if expression
			bool res = SemanticsAnalysis(rootNode->child1);
			if (rootNode->child1->expRetValType != vartBool) {
				printf("if condition must be type of BOOL. found %d\n", rootNode->child1->expRetValType);
				return false;
			}
			if (!res || rootNode->child2 == 0) {
				return false;
			}
			// then statement block
			res = SemanticsAnalysis(rootNode->child2);
			if (!res) {
				return false;
			}
			if (rootNode->child3 != 0) {
				// else statement block
				res = SemanticsAnalysis(rootNode->child3);
				if (!res) {
					return false;
				}
			}
			lvl--; return true;
		}
		if (rootNode->tokenPtr->at(0).type == tkKeywordSWITCH)
		{
			if (rootNode->child2 == 0) {
				printf("HERE1");
				return false;
			}
			// switch statement-block
			if (SemanticsAnalysis(rootNode->child2)) {
				lvl--; return true;
			}
		}
		printf("WTF nodeSelectionStatement\n");
		return false;
	}

	if (rootNode->nodeType == nodePrimaryExpression) {

		if (rootNode->child1 == 0) {
			// IDETIFIER
			if (rootNode->tokenPtr->size() == 1 && rootNode->tokenPtr->at(0).type == tkID) {
				if (mapOfVars.find(rootNode->tokenPtr->at(0).str) != mapOfVars.end())
				{
					rootNode->expRetValType = mapOfVars[rootNode->tokenPtr->at(0).str].type;
					lvl--; return true;
				}
				printf("variable %s is undeclared\n", rootNode->tokenPtr->at(0).str.c_str());
				return false;
			}
			printf("WTF nodePrimaryExpression\n");
			return false;
		}
		// Literal
		if (rootNode->child1->nodeType == nodeLiteral)
		{
			// NUMBER
			if (rootNode->child1->child1 == 0) {
				rootNode->expRetValType = vartInt;
				lvl--; return true;
			}
			// BOOLEAN
			else {
				rootNode->expRetValType = vartBool;
				lvl--; return true;
			}
			printf("wrong literal\n");
			return false;

		}
		printf("empty nodePrimaryExpression\n");
		return false;
	}

	if (rootNode->nodeType == nodeAssignmentExpression) {
		if (mapOfVars.find(rootNode->tokenPtr->at(0).str) == mapOfVars.end()) {
			printf("variable %s is undeclared\n", rootNode->tokenPtr->at(0).str.c_str());
			return false;
		}
		if (rootNode->tokenPtr->at(0).type == tkID && mapOfVars.find(rootNode->tokenPtr->at(0).str) == mapOfVars.end()) {
			printf("variable %s is undeclared\n", rootNode->tokenPtr->at(0).str.c_str());
			return false;
		}
		lvl--;  return true;
	}

	if (rootNode->nodeType == nodeRelationalExpression) {
		bool res = true;
		res &= SemanticsAnalysis(rootNode->child1);
		if (!res) {
			return false;
		}
		if (rootNode->child2 == 0) {
				rootNode->expRetValType = rootNode->child1->expRetValType;
				lvl--; return true;
		}
		if (!SemanticsAnalysis(rootNode->child2)) {
			return false;
		}
		if (rootNode->child1->expRetValType == rootNode->child2->expRetValType) {
			rootNode->expRetValType = vartBool;
			lvl--; return true;
		}
		printf("relational expression operands must have type of int\n");
		return false;
	}

	if (rootNode->nodeType == nodeLogicalExpression)
	{
		bool res = true;
		res &= SemanticsAnalysis(rootNode->child1);
		if (rootNode->child2 == 0) {
			if (res) {
				rootNode->expRetValType = rootNode->child1->expRetValType;
				lvl--; return true;
			}
			return false;
		}
		res &= SemanticsAnalysis(rootNode->child2);
		if (res == false) {
			return false;
		}
		if (rootNode->child1->expRetValType == rootNode->child2->expRetValType == vartBool) {
			rootNode->expRetValType = vartBool;
			lvl--; return true;
		}
		printf("logical or expression operands must have type of bool\n");
		return false;
	}

	if (rootNode->nodeType == nodeStatement) {
		if (rootNode->child1 != 0) {
			bool res = SemanticsAnalysis(rootNode->child1);
			if (res) {
				lvl--; return true;
			}
			return false;
		}
		/*if ((rootNode->tokenPtr->size() == 1) && rootNode->tokenPtr->at(0).type == tkKeywordBREAK) {
			lvl--; return true;
		}*/
		printf("empty statement\n");
		lvl--; return true;
	}

	if (rootNode->nodeType == nodeStatementSeq) {
		if (rootNode->child1 != 0) {
			bool res = SemanticsAnalysis(rootNode->child1);
			if (!res) {
				return false;
			}
		}
		if (rootNode->child2 != 0) {
			bool res = SemanticsAnalysis(rootNode->child2);
			if (!res) {
				return false;
			}
		}
		lvl--; return true;
	}

	bool resm = true;
	if (rootNode->child1 != 0 && resm)
		resm &= SemanticsAnalysis(rootNode->child1);
	if (rootNode->child2 != 0 && resm)
		resm &= SemanticsAnalysis(rootNode->child2);
	if (rootNode->child3 != 0 && resm)
		resm &= SemanticsAnalysis(rootNode->child3);
	if (rootNode->child4 != 0 && resm)
		resm &= SemanticsAnalysis(rootNode->child4);

	if (rootNode->nodeType == nodeProgram) {
		printf("sem over, cur level: %d\n", lvl);
	}
	if (resm)
	lvl--; return resm;
}
