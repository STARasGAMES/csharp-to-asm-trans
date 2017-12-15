// Kursach.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include "scanner.h"
#include "parser.h"
#include "semanticsanalyzer.h"
#include "translator.h"

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
	Token *curToken = 0;
	Token *prevToken = 0;
	int id = 0;
	do
	{
		curToken = Scan(input);
		if (prevToken != 0)
		{
			prevToken->next = curToken;
			curToken->prev = prevToken;
		}
		curToken->id = id++;
		prevToken = curToken;
		printToken(curToken);
		if (curToken->type == tkNA)
		{
			printf("error: unknown token\n");
		}
	} while (curToken->type != tkEOF);
	printf("\n");
	Node* rootNode = 0;
	std::ostringstream ss; 
	std:string s;
	std::istringstream* iss;
	if ((rootNode = Parse(curToken->GetTokenById(0))) == 0)
		goto endofmain;

	if (!SemanticsAnalysis(rootNode)) {
		printf("error in SemanticsAnalysis, break\n");
		goto endofmain;
	}
	printf("Semantics OK\n");
	ss << "\n";
	Translate(ss, rootNode);
	s = ss.str();
	iss = new std::istringstream(s, 0);
	displayStream(iss);
	endofmain:
	system("Pause");
    return 0;
}

