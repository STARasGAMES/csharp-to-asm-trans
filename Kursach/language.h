#pragma once

#ifndef SYMDEF_H
#define SYMDEF_H



#include <vector>
#include <string>
#include <map>
#include "token.h"
//class Language_Cpp : public Language {};
//class Language_Pascal : public Language {};
//class Language_Kurs : public Language {};


class Language {
protected:
	std::vector<TokenType> &keywordsTokens();
	std::vector<TokenType> &operatorsTokens();
	std::vector<TokenType> &delimitersTokens();
	std::vector<std::string> &keywords();
	std::vector<std::string> &operators();
	std::vector<std::string> &delimiters();
	std::map<TokenType, std::string> &tokenToStringMap();
	std::map<std::string, TokenType> &stringToTokenTypeMap();

	std::string word;
	std::string numStr;

public:
	Language();
	void InitSymbols();

	int isDelimiter(const std::string);
	int isOperator(const std::string);
	int isKeyword(const std::string);

	TokenType GetTokenTypeByString(std::string &);
	std::string GetStringByTokenType(const TokenType);
};



std::vector<TokenType> keywordsTokens = {
	tkKeywordTHEN, tkKeywordIF, tkKeywordREPEAT, tkKeywordINT, tkKeywordFLOAT,
	tkKeywordDO, tkKeywordVOID, tkKeywordRETURN, tkKeywordELSE, tkSTART, tkFINISH, tkKeywordVAR, tkKeywordPRINT
};

std::vector<TokenType> operatorsTokens = {
	// Relational Operators (==  <  >  =!=    =>  =<)
	tkOperatorEQUAL, tkOperatorGREATER, tkOperatorLESS, tkOperatorGREATER_EQUAL, tkOperatorLESS_EQUAL, tkOperatorNOT_EQUAL,

	// Other operators (= :  +  -  *  / %)
	tkOperatorASSIGN, tkOperatorCOLON, tkOperatorADD, tkOperatorSUBTRACT, tkOperatorMUL, tkOperatorDIV, tkOperatorREMAINDER, tkOperatorNOT_EQUAL
};

std::vector<TokenType> delimitersTokens = {
	// Delimiters (. (  ) , { } ; [ ])
	tkDelimiterDOT, tkDelimiterLEFT_PA, tkDelimiterRIGHT_PA, tkDelimiterCOMMA, tkDelimiterLEFT_BRACE, tkDelimiterRIGHT_BRACE,
	tkDelimiterSEMICOLON, tkDelimiterLEFT_BRACKET, tkDelimiterRIGHT_BRACKET
};

std::map<TokenType, std::string> tokenToStringMap =
{
	{tkEOF, "\n"},
	{tkNA, "N/A token"},
	// KEYWORDS
	{ tkKeywordDO, "do"},
	{ tkKeywordFLOAT, "float" },
	{ tkKeywordIF, "if" },
	{ tkKeywordINT, "int" },
	{ tkKeywordREPEAT, "repeat" },
	{ tkKeywordRETURN, "return" },
	{ tkKeywordTHEN, "then" },
	{ tkKeywordVOID, "void" },
	{ tkKeywordELSE, "else" },
	{ tkKeywordVAR, "var" },
	{ tkSTART, "start"},
	{ tkFINISH, "finish"},
	{ tkKeywordPRINT, "print"},


	// OPERATORS
	{ tkOperatorADD, "+" },
	{ tkOperatorASSIGN, "=" },
	{ tkOperatorCOLON, ":"},
	{ tkOperatorDIV, "/" },
	{ tkOperatorEQUAL, "==" },
	{ tkOperatorGREATER, ">" },
	{ tkOperatorGREATER_EQUAL, ">=" },
	{ tkOperatorLESS, "<" },
	{ tkOperatorLESS_EQUAL, "<=" },
	{ tkOperatorREMAINDER, "%" },
	{ tkOperatorMUL, "*" },
	{ tkOperatorNOT_EQUAL, "!=" },
	{ tkOperatorSUBTRACT, "-" },

	// DELIMITERS
	{ tkDelimiterCOMMA, ","},
	{ tkDelimiterDOT, "." },
	{ tkDelimiterLEFT_BRACKET, "[" },
	{ tkDelimiterLEFT_PA, "(" },
	{ tkDelimiterLEFT_BRACE, "{" },
	{ tkDelimiterRIGHT_BRACE, "}" },
	{ tkDelimiterRIGHT_BRACKET, "]" },
	{ tkDelimiterRIGHT_PA, ")" },
	{ tkDelimiterSEMICOLON, ";" }
};

std::map<std::string, TokenType> stringToTokenTypeMap;



#endif // !SYMDEF_H