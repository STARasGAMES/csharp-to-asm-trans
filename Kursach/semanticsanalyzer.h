
#ifndef SEMANTIC_ANALIZER_H
#define SEMANTIC_ANALIZER_H

#include <string>
#include <map>
#include "parser.h"
#include "variableh.h"

std::map<std::string, Variable>& GetMapOfVars();


bool SemanticsAnalysis(Node*&);

#endif // !SEMANTIC_ANALIZER_H
