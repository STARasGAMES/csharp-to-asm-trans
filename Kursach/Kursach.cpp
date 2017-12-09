// Kursach.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include "scanner.h"

using namespace std;

string sampleCode = "float int123 \n 123\n13void 1341234.234]n23 some shet ;\n + = - / { }\n [ ( ] >= <= !=)";
istream* input;
int main()
{
	/*FILE* filePtr;
	if ((filePtr = fopen("in.txt", "r")) == 0)
	{
		printf("cant find file");
	}
	else
	{
		printf("found file");
	}*/
	input = new istringstream(sampleCode);
	InitScanner();
	Token token;
	while ((token = Scan(input)).type != tkEOF) {

		printToken(token);
		if (token.type == tkNA)
		{
			printf("error: unknown token\n");
			break;
		}
	}
	system("Pause");
    return 0;
}

