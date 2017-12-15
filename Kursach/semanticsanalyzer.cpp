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
bool SemanticsAnalysis(Node *& rootNode)
{
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
		return res;
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
		mapOfVars[str] = *newVar;
		if (rootNode->child2 != 0)
			return SemanticsAnalysis(rootNode->child2);
		return true;
	}
	bool res = true;
	if (rootNode->child1 != 0)
		res &= SemanticsAnalysis(rootNode->child1);
	if (rootNode->child2 != 0)
		res &= SemanticsAnalysis(rootNode->child2);
	if (rootNode->child3 != 0)
		res &= SemanticsAnalysis(rootNode->child3);
	if (rootNode->child4 != 0)
		res &= SemanticsAnalysis(rootNode->child4);
	return res;
}
