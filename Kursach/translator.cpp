#include "translator.h"
#include "parser.h"
#include "token.h"
#include "semanticsanalyzer.h"

void Translate(std::ostringstream& s, Node*& rootNode)
{
	s << ".386\n";
	s << ".model flat, stdcall\n";

	s << "option casemap : none\n";
	s << "include \\masm32\\include\\kernel32.inc\n";
	s << "include \\masm32\\include\\user32.inc\n";

	s << "includelib \\masm32\\lib\\kernel32.lib\n";
	s << "includelib \\masm32\\lib\\user32.lib\n";

	s << ".data\n";
	for (const auto& pair : GetMapOfVars())
	{
		s << "variable" << pair.second.id << ' ';
		switch (pair.second.type)
		{
		case vartBool:
			s << "db ";
			break;
		case vartInt:
			s << "dd ";
			break;
		default:
			break;
		}

		if (!pair.second.isInitialyzed)
			s << "?\n";
		else
			s << pair.second.initValue.c_str() << "\n";
	}


	s << ".code\n";
	s << "main :\n";


	s << "invoke ExitProcess, 0\n";
	s << "end main\n";
}

void trans(std::ostringstream& s, Node*& rootNode)
{
	if (rootNode->child1 != 0)
		trans(s, rootNode->child1);
	if (rootNode->child2 != 0)
		trans(s, rootNode->child2);
	if (rootNode->child3 != 0)
		trans(s, rootNode->child3);
	if (rootNode->child4 != 0)
		trans(s, rootNode->child4);
}
