#pragma once

#include <string>
#include <vector>

#define MAX 9 // max length of each word string, not including '\0'

extern int lineNum;

typedef enum {
	// Identifier: begin with a letter, and continue with any number
	// of letters. No ID is longer than MAX
	tkID,

	// Keywords (start finish then if repeat var int float do 
	// read print void return dummy program) 
	tkKeywordTHEN, tkKeywordIF, tkKeywordREPEAT, tkKeywordINT, tkKeywordFLOAT,
	tkKeywordDO, tkKeywordVOID, tkKeywordRETURN, tkKeywordELSE,

	// Number: sequence of decimal digits, no sign, no longer than MAX digits
	tkNUMBER,

	// Relational Operators (==  <  >  =!=    =>  =<)
	tkOperatorEQUAL, tkOperatorGREATER, tkOperatorLESS, tkOperatorGREATER_EQUAL, tkOperatorLESS_EQUAL,

	// Other operators (= :  +  -  *  / %)
	tkOperatorASSIGN, tkOperatorADD, tkOperatorSUBTRACT, tkOperatorMUL, tkOperatorDIV, tkOperatorMOD, tkOperatorNOT_EQUAL,

	// Delimiters (. (  ) , { } ; [ ])
	tkDelimiterDOT, tkDelimiterLEFT_PA, tkDelimiterRIGHT_PA, tkDelimiterCOMMA, tkDelimiterLEFT_BRACE, tkDelimiterRIGHT_BRACE,
	tkDelimiterSEMICOLON, tkDelimiterLEFT_BRACKET, tkDelimiterRIGHT_BRACKET,

	tkNA, // N/A token 
	tkEOF

} TokenType;


struct Token {
	std::string str;
	TokenType type;
	int lineNum;

	std::vector<Token> *next; // linked-list, used for parse tree
};

//std::vector<Token> tokens; // list of all tokens (array of numToken) 