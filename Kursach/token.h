#pragma once

#ifndef TOKEN_H
#define TOKEN_H



#include <string>
#include <vector>
#include "strformat.h"

#define MAX 9 // max length of each word string, not including '\0'

extern int lineNum;

typedef enum {
	// Identifier: begin with a letter, and continue with any number
	// of letters. No ID is longer than MAX
	//tkID,

	//// Keywords (start finish then if repeat var int float do 
	//// read print void return dummy program) 
	//tkKeywordTHEN, tkKeywordIF, tkKeywordWHILE, tkKeywordINT, tkKeywordFLOAT,
	//tkKeywordDO, tkKeywordVOID, tkKeywordRETURN, tkKeywordELSE, 

	//// Number: sequence of decimal digits, no sign, no longer than MAX digits
	//tkNUMBER,

	//// Relational Operators (==  <  >  =!=    =>  =<)
	//tkOperatorEQUAL, tkOperatorGREATER, tkOperatorLESS, tkOperatorGREATER_EQUAL, tkOperatorLESS_EQUAL,

	//// Other operators (= :  +  -  *  / %)
	//tkOperatorASSIGN, tkOperatorADD, tkOperatorSUBTRACT, tkOperatorMUL, tkOperatorDIV, tkOperatorMOD, tkOperatorNOT_EQUAL,

	//// Delimiters (. (  ) , { } ; [ ])
	//tkDelimiterDOT, tkDelimiterLEFT_PA, tkDelimiterRIGHT_PA, tkDelimiterCOMMA, tkDelimiterLEFT_BRACE, tkDelimiterRIGHT_BRACE,
	//tkDelimiterSEMICOLON, tkDelimiterLEFT_BRACKET, tkDelimiterRIGHT_BRACKET,

	//tkNA, // N/A token 
	//tkEOF


	tkID,

	// Keywords (start finish then if repeat var int float do 
	// read print void return dummy program) 
	tkKeywordIF, tkKeywordELSE, tkKeywordSWITCH, tkKeywordCASE, tkKeywordDEFAULT,  tkKeywordINT, tkKeywordBOOL,
	tkKeywordVOID, tkKeywordBREAK, tkKeywordTRUE, tkKeywordFALSE,

	// Number: sequence of decimal digits
	tkNUMBER,

	// Relational Operators (==  !=  >  <  >=  <=)
	tkOperatorEQUAL, tkOperatorNOT_EQUAL, tkOperatorGREATER, tkOperatorLESS, tkOperatorGREATER_EQUAL, tkOperatorLESS_EQUAL,

	// Other operators (= :  +  -  *  / %  ++  --  +=  -=  &  |  ^  &&  ||  ?)
	tkOperatorASSIGN, tkOperatorCOLON, tkOperatorADD, 
	tkOperatorSUBTRACT, tkOperatorMUL, tkOperatorDIV, 
	tkOperatorREMAINDER, tkOperatorINC, tkOperatorDEC, 
	tkOperatorADD_ASSIGN, tkOperatorSUB_ASSIGN, 
	tkOperatorAND, tkOperatorOR, tkOperatorXOR,
	tkOperatorLOGICAL_AND, tkOperatorLOGICAL_OR,
	tkOperatorQUESTION_MARK,

	// Delimiters (. (  ) , { } ; [ ])
	tkDelimiterDOT, tkDelimiterLEFT_PA, tkDelimiterRIGHT_PA, tkDelimiterCOMMA, tkDelimiterLEFT_BRACE, tkDelimiterRIGHT_BRACE,
	tkDelimiterSEMICOLON, tkDelimiterLEFT_BRACKET, tkDelimiterRIGHT_BRACKET,

	tkNA, // N/A token 
	tkEOF

} TokenType;


struct Token {

	Token(std::string str, TokenType type, int lineNum)
	{
		std::string buf = str;
		this->str = buf;
		this->type = type;
		this->lineNum = lineNum;
	}

	Token(TokenType type)
	{
		std::string buf = "non initialized token";
		this->str = buf;
		this->type = type;
		this->lineNum = -1;
		this->rowNum = -1;
	}

	Token()
	{
		std::string buf = "non initialized token";
		this->str = buf;
		this->type = tkNA;
		this->lineNum = -1;
		this->rowNum = -1;
	}

	std::string str;
	TokenType type;
	int lineNum;
	int rowNum;

	int id = -1;
	Token *next = 0;
	Token *prev = 0;

	void setStr(std::string s)
	{
		std::string buf = s;
		this->str = buf;
	}

	Token *GetTokenById(int findId)
	{
		if (findId == id)
			return this; 
		if (findId > id)
		{
			if (next == 0)
			{
				printf("ERROR: token.h:GetTokenById 1");
				return 0;
			}
			return next->GetTokenById(findId);
		}
		else
		{
			if (prev == 0)
			{
				printf("ERROR: token.h:GetTokenById 2");
				return 0;
			}
			return prev->GetTokenById(findId);
		}
	}

	
	/*std::vector<Token> *next;*/ // linked-list, used for parse tree
};

//std::vector<Token> tokens; // list of all tokens (array of numToken) 

#endif // !TOKEN_H