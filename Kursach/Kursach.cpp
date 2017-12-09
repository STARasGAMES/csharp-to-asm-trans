// Kursach.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include "scanner.h"
#include "parser.h"

using namespace std;

string sampleCode = "var x : y";
istream* input;
int main()
{
	FILE* filePtr;
	if ((filePtr = fopen("in.txt", "r")) == 0)
	{
		printf("cant find file");
	}
	else
	{
		printf("found file");
	}
	input = new std::ifstream(filePtr);
	//input = new istringstream(sampleCode);
	displayStream(input);
	input->clear();
	input->seekg(0, std::ios::beg);
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
	printf("\n");

	input->clear();
	input->seekg(0, std::ios::beg);
	Parse(input);
	system("Pause");
    return 0;
}

