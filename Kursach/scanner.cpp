
#include <iostream>
#include "scanner.h"
#include "token.h"
#include "symdef.h"

int lineNum = 1; // silimilar, extern var from token.h

void InitScanner() {
	InitSymbols();
}

Token *Scan(std::istream *filePtr) {
	//printf("\n*** in scanner.c ***\n");
	Token *token = new Token();
	char ch;
	word.clear();
	numStr.clear();
	while ((*filePtr).get(ch)) {
		if (ch == '\n') {
			lineNum++;
			continue;
		}

		// Ignore comment starting with // to the end of line
		if (ch == '/') {
			if ((*filePtr).get(ch) && ch == '/') {
				while ((*filePtr).get(ch) && ch != '\n') {}
				lineNum++;
			}
			//fseek(filePtr, -1, SEEK_CUR);
		}
		while (ch == ' ' && (*filePtr).get(ch)) {
			//printf("skip space at line #%d, cur char is '%c'\n", lineNum, ch);
		}
		
		if (isalpha(ch)) {
			do
				word.push_back(ch);
			while ((*filePtr).get(ch) && isalnum(ch));
			(*filePtr).putback(ch);
			token->setStr(word);
			if (isKeyword(word) != -1) {
				token->type = GetTokenTypeByString(word);
			}
			else {
				token->type = tkID;
			}
			token->lineNum = lineNum;

			//fseek(filePtr, -1, SEEK_CUR);
			return token;
		}

		if (isdigit(ch)) {
			do {
				if (isdigit(ch))
					numStr.push_back(ch);
				else {
					if (isalpha(ch))
					{
						numStr.push_back(ch);
						token->setStr(numStr);
						token->type = tkNA;
						token->lineNum = lineNum;
						return token;
					}
					break;
				}
			}
			while ((*filePtr).get(ch));
			(*filePtr).putback(ch);
			token->setStr(numStr);
			token->type = tkNUMBER;
			token->lineNum = lineNum;

			//fseek(filePtr, -1, SEEK_CUR);
			return token;

		}

		if (ispunct(ch)) {
			//printf("is punct '%c' \n", ch);
			word.clear();
			word = ch;

			if (isDelimiter(word) != -1) {
				token->setStr(word);
				token->type = GetTokenTypeByString(token->str);
				token->lineNum = lineNum;
				return token;
			}

			if ((*filePtr).get(ch))
			{
				word.push_back(ch);
				if (isOperator(word) != -1)
				{
					//printf("is Relational operator %s \n", word.c_str());
					token->lineNum = lineNum;
					token->setStr(word);
					token->type = GetTokenTypeByString(token->str);
					//printf("Relational operator type %d \n", token.type);
					return token;
				}
			}
			(*filePtr).putback(ch);
			word = word[0];
			// operator
			if (isOperator(word) != -1)
			{
				//printf("isOperator '%c' \n", ch);
				token->lineNum = lineNum;
				token->setStr(word);
				token->type = GetTokenTypeByString(token->str);
				return token;
			}
			token->lineNum = lineNum;
			token->setStr(word);
			token->type = tkNA;
			return token;
		} // end if ispunct

	} // end while

	  // rewind(filePtr);

	  //printf("\n*** in scanner.c ***\n");

	token->type = tkEOF;
	return token;
}

int isKeyword(std::string str) {
	for (int i = 0; i < keywords.size(); i++) {
		if (keywords[i] == str)
			return i;
	}
	return -1;

}

int isDelimiter(std::string str) {
	for (int i = 0; i < delimiters.size(); i++) {
		if (delimiters[i] == str)
			return i;
	}
	return -1;
}

int isOperator(std::string str) {
	for (int i = 0; i < operators.size(); i++) {
		//printf("%s %s", str.c_str(), operators[i].c_str());
		if (operators[i]._Equal(str))
			return i;
	}
	return -1;
}



// Besides English letters, and digits, these are extra acceptable characters
int isExAcceptableChar(char c) {
	if (c == '.' || c == '(' || c == ')' || c == ',' || c == '{' || c == '}' ||
		c == ';' || c == '[' || c == ']' ||
		c == ':' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
		c == '=' || c == '<' || c == '>' || c == '!'
		/* || c == '#' */) {

		return 1;
	}
	else
		return 0;
}

std::string GetStringByTokenType(const TokenType type) {
	std::string result;
	switch (type) {
	case tkID: result = "IDENTIFER"; break;
	case tkNUMBER: result = "NUMBER"; break;

	case tkKeywordIF: 		result = "if [KEYWORD]"; break;
	case tkKeywordELSE: result = "else [KEYWORD]"; break;
	case tkKeywordINT: 	result = "int [KEYWORD]"; break;
	case tkKeywordVOID: 	result = "void [KEYWORD]"; break;
	case tkKeywordBOOL: result = "bool [KEYWORD]"; break;
	case tkKeywordTRUE: result = "true [KEYWORD]"; break;
	case tkKeywordFALSE: result = "false [KEYWORD]"; break;
	case tkKeywordBREAK: result = "break [KEYWORD]"; break;
	case tkKeywordSWITCH: result = "switch [KEYWORD]"; break;
	case tkKeywordCASE: result = "case [KEYWORD]"; break;
	case tkKeywordDEFAULT: result = "default [KEYWORD]"; break;

	case tkDelimiterDOT: 	result = "dot [DELIMITER]"; break;
	case tkDelimiterLEFT_PA: 	result = "left-parenthesis [DELIMITER]"; break;
	case tkDelimiterRIGHT_PA: result = "right-parenthesis [DELIMITER]"; break;
	case tkDelimiterCOMMA: 	result = "comma [DELIMITER]"; break;
	case tkDelimiterLEFT_BRACE: 		result = "left-brace [DELIMITER]"; break;
	case tkDelimiterRIGHT_BRACE: 		result = "right-brace [DELIMITER]"; break;
	case tkDelimiterLEFT_BRACKET:		result = "left-bracket [DELIMITER]"; break;
	case tkDelimiterRIGHT_BRACKET:	result = "right-bracket [DELIMITER]"; break;
	case tkDelimiterSEMICOLON: 		result = "semi-colon [DELIMITER]"; break;

	case tkOperatorASSIGN:		result = "assign [OTHER OPERATOR]"; break;
	case tkOperatorADD:			result = "add [OTHER OPERATOR]"; break;
	case tkOperatorSUBTRACT:	result = "subtractk [OTHER OPERATOR]"; break;
	case tkOperatorMUL: 		result = "multiply [OTHER OPERATOR]"; break;
	case tkOperatorDIV: 		result = "division [OTHER OPERATOR]"; break;
	case tkOperatorREMAINDER:	result = "remainder [OTHER OPERATOR]"; break;
	case tkOperatorCOLON:	result = "colon [OTHER OPERATOR]"; break;
	case tkOperatorINC: result = "increment [OTHER OPERATOR]"; break;
	case tkOperatorDEC: result = "decrement [OTHER OPERATOR]"; break;
	case tkOperatorADD_ASSIGN: result = "add assign [OTHER OPERATOR]"; break;
	case tkOperatorSUB_ASSIGN: result = "substruct assign [OTHER OPERATOR]"; break;
	case tkOperatorAND: result = "& [OTHER OPERATOR]"; break;
	case tkOperatorOR: result = "| [OTHER OPERATOR]"; break;
	case tkOperatorXOR: result = "^ [OTHER OPERATOR]"; break;
	case tkOperatorLOGICAL_AND: result = "&& [OTHER OPERATOR]"; break;
	case tkOperatorLOGICAL_OR: result = "|| [OTHER OPERATOR]"; break;
	case tkOperatorQUESTION_MARK: result = "? [OTHER OPERATOR]"; break;

	case tkOperatorEQUAL: 		result = "equal [RELATIONAL OPERATOR]"; break;
	case tkOperatorNOT_EQUAL: 		result = "not equal [RELATIONAL OPERATOR]"; break;
	case tkOperatorGREATER:		result = "greater [RELATIONAL OPERATOR]"; break;
	case tkOperatorLESS:		result = "less than [RELATIONAL OPERATOR]"; break;
	case tkOperatorGREATER_EQUAL:	result = "greater or equal [RELATIONAL OPERATOR]"; break;
	case tkOperatorLESS_EQUAL: 		result = "less than or equal [RELATIONAL OPERATOR]"; break;

		printf("ERROR: scanner.cpp GetStringByTokenType");
	default: result = "UNKNOWN";
	}
	return result;
}

void printToken(Token *token) {
	if (token->type == tkEOF) {
		printf("***** tkEOF ***** \n\n");
		return;
	}
	if (token == NULL)
		printf("shet3");
	if (token->str.c_str() == 0)
		printf("shet1");
	std::string out1 = "'" + (*token).str + "'";
	//printf("shet2 %d", token->str.c_str());
	printf("%10s \t line #%d \t %s \n", out1.c_str(), token->lineNum, GetStringByTokenType(token->type).c_str());
}

TokenType GetTokenTypeByString(std::string &word) {
	return stringToTokenTypeMap[word];
}
void displayStream(std::istream* filePtr) {
	printf("\n-----Source file starts here-----\n\n");

	char ch;
	while (filePtr->get(ch)) {
		printf("%c", ch);
	}
	filePtr->seekg(0, filePtr->beg);
	printf("\n-----/Source file ends here------\n\n");
}