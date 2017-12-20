#include "translator.h"
#include "parser.h"
#include "token.h"
#include "nodeh.h"
#include "semanticsanalyzer.h"

void trans(std::ostringstream& s, Node*& root);

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
		s << "var_" << pair.second.str.c_str() << ' ';
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
		printf("var declaration %s, %d\n", pair.second.str.c_str(), pair.second.isInitialyzed);
		if (!pair.second.isInitialyzed)
			s << "?\n";
		else
			s << pair.second.initValue.c_str() << "\n";
	}


	s << ".code\n";
	s << "main :\n";

	trans(s, rootNode->child2);


	s << "invoke ExitProcess, 0\n";
	s << "end main\n";
}

int ifCount = 0;
int switchCount = 0;
int caseCount = 0;

void trans(std::ostringstream& s, Node*& root)
{

	if (root->nodeType == nodeAssignmentExpression) {
		// ID = ID
		if (root->tokenPtr->at(1).type == tkID) {
			s << "MOV eax, " << GetMapOfVars()[root->tokenPtr->at(1).str].ToString().c_str() << '\n';
			s << "MOV " << GetMapOfVars()[root->tokenPtr->at(0).str].ToString().c_str() << ", eax" << '\n';
		}
		// ID = NUMBER
		else {
			s << "MOV " << GetMapOfVars()[root->tokenPtr->at(0).str].ToString().c_str() << ", " << root->tokenPtr->at(1).str.c_str() << '\n';
		}
		return;
	}

	if (root->nodeType == nodeSelectionStatement) {
		if (root->tokenPtr->at(0).type == tkKeywordIF) {
			Node& relexpr = *(root->child1);
			if (relexpr.child1->child1 == 0)
				s << "MOV eax, " << GetMapOfVars()[relexpr.child1->tokenPtr->at(0).str].ToString().c_str() << '\n';
			else
				s << "MOV eax, " << relexpr.child1->child1->tokenPtr->at(0).str.c_str() << '\n';

			if (relexpr.child2->child1 == 0)
				s << "MOV ebx, " << GetMapOfVars()[relexpr.child2->tokenPtr->at(0).str].ToString().c_str() << '\n';
			else
				s << "MOV ebx, " << relexpr.child2->child1->tokenPtr->at(0).str.c_str() << '\n';
			s << "cmp eax, ebx" << '\n';
			switch (relexpr.tokenPtr->at(0).type) {
			case tkOperatorLESS:
				s << "JGE ";
				break;
			case  tkOperatorGREATER:
				s << "JLE ";
				break;
			case tkOperatorLESS_EQUAL:
				s << "JG ";
				break;
			case tkOperatorGREATER_EQUAL:
				s << "JL ";
				break;
			case tkOperatorEQUAL:
				s << "JNE ";
				break;
			case tkOperatorNOT_EQUAL:
				s << "JE ";
				break;
			}
			int ifc = ifCount++;
			printf("HERE I WAS");
			s << "label_IF_ELSE_" << ifc << "\n";
			//block THEN
			trans(s, root->child2);
			s << "JMP label_IF_END_" << ifc << "\n";
			//block ELSE
			if (root->child3 != 0) {
				s << "label_IF_ELSE_" << ifc << ":\n";
				trans(s, root->child3);
			}
			s << "label_IF_END_" << ifc << ":\n";
			return;
		}
		if (root->tokenPtr->at(0).type == tkKeywordSWITCH) {
			int switchc = switchCount++;
			int casec = 0;
			printf("here SWITCH\n");
			s << "MOV eax, " << GetMapOfVars()[root->tokenPtr->at(1).str].ToString().c_str() << '\n';
			Node* curN = &(*(root->child2->child1));
			do {
				if (curN->child1 != 0 && curN->child1->child1->nodeType == nodeLabeledStatement) {
					if (curN->child1->child1->tokenPtr->at(0).type == tkKeywordCASE)
					{
						casec++;
						s << "CMP eax, " << curN->child1->child1->tokenPtr->at(1).str.c_str() << "\n";
						s << "JE " << "label_SWITCH_" << switchc << "_CASE_" << curN->child1->child1->tokenPtr->at(1).str.c_str() << "\n";
					}
					if (curN->child1->child1->tokenPtr->at(0).type == tkKeywordDEFAULT)
					{
						s << "JMP label_SWITCH_" << switchc << "_DEFAULT\n";
						break;
					}
				}
				curN = &(*(curN->child2));
			} while (curN != 0);

			casec = 0;
			curN = &(*(root->child2->child1));
			do {
				if (curN->child1 != 0 && curN->child1->child1->nodeType == nodeLabeledStatement) {
					if (curN->child1->child1->tokenPtr->at(0).type == tkKeywordCASE)
					{
						casec++;
						s << "label_SWITCH_" << switchc << "_CASE_" << curN->child1->child1->tokenPtr->at(1).str.c_str() << ":\n";
						if (curN->child1->child1->child1 != 0)
							trans(s, curN->child1->child1->child1);
						s << "JMP label_SWITCH_" << switchc << "_END\n";
					}
					if (curN->child1->child1->tokenPtr->at(0).type == tkKeywordDEFAULT)
					{
						s << "label_SWITCH_" << switchc << "_DEFAULT:\n";
						if (curN->child1->child1->child1 != 0)
							trans(s, curN->child1->child1->child1);
						s << "JMP label_SWITCH_" << switchc << "_END\n";
						break;
					}
				}
				curN = &(*(curN->child2));
			} while (curN != 0);
			s << "label_SWITCH_" << switchc << "_END\n";
			return;
		}
	}
	if (root->nodeType == nodeStatementBlock) {
		trans(s, root->child1);
		return;
	}
	if (root->nodeType == nodeStatementSeq) {
		trans(s, root->child1);
		if (root->child2 != 0)
			trans(s, root->child2);
		return;
	}
	if (root->nodeType == nodeStatement) {
		if (root->child1 != 0)
			trans(s, root->child1);
		return;
	}

	//Ignore
	if (root->nodeType == nodeLabeledStatement || root->nodeType == nodeDeclaretionStatement)
		return;

	if (root->child1 != 0)
		trans(s, root->child1);
	if (root->child2 != 0)
		trans(s, root->child2);
	if (root->child3 != 0)
		trans(s, root->child3);
	if (root->child4 != 0)
		trans(s, root->child4);
}
