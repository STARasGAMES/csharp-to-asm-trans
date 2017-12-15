#include "language.h"
#include <vector>

Language::Language() {
	InitSymbols();
};

void Language::InitSymbols() {
	for (const auto& pair : tokenToStringMap())
	{
		stringToTokenTypeMap()[pair.second] = pair.first;
	}
	// SETUP KEYWORDS
	for (int i = 0; i < keywordsTokens().size(); i++)
	{
		keywords().push_back(tokenToStringMap()[keywordsTokens()[i]]);
	}
	// SETUP OPERATORS
	for (int i = 0; i < operatorsTokens().size(); i++)
	{
		operators().push_back(tokenToStringMap()[operatorsTokens()[i]]);
	}
	// SETUP DELIMITERS
	for (int i = 0; i < delimitersTokens().size(); i++)
	{
		delimiters().push_back(tokenToStringMap()[delimitersTokens()[i]]);
	}
}