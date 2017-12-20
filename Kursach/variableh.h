#ifndef VARIABLEH_H
#define VARIABLEH_H

#include <string>

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

	std::string ToString() {
		return "var_" + str;
	}
};
#endif // !VARIABLEH_H