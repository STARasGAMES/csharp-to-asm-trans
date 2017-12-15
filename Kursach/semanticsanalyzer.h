
#ifndef SEMANTIC_ANALIZER_H
#define SEMANTIC_ANALIZER_H

#include <string>
#include <map>
#include "parser.h"

enum VariableType {
	vartInt,
	vartBool,
	vartNA
};

struct Variable {
	VariableType type = vartNA;
	bool isInitialyzed;
	std::string initValue;
	std::string str;
	int id = -1;
};

std::map<std::string, Variable>& GetMapOfVars();


bool SemanticsAnalysis(Node*&);

#endif // !SEMANTIC_ANALIZER_H
