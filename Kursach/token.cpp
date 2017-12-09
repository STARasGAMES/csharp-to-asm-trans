#include "token.h"
//#include "scanner.h"

std::string Token::ToString()
{
	std::string s = "some";
	return string_format("str:'%s', type:'%s'(%d), lineNum: #%d", str, GetStringByTokenType(type), type, lineNum);
}