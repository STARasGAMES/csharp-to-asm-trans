#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "nodeh.h"
#include <istream>

Token *tk = new  Token("N/A", tkNA, 0);
bool isError = false;
void printParseTree(Node *, int);

void Next() {
	tk = tk->next;
	if (tk == 0)
		throw std::exception("next token error");
}

Node* Parse(Token * rootToken) {
	lineNum = 1; 
	isError = false;
	tk = rootToken;
	Node *root = NULL;

	if (Program(rootToken, root))
	{
		printf("Parse OK! \n");
	}
	else
	{
		printf("Parse NOT OK! \n");
		return 0;
	}

	printf("\n*-----Parse Tree-----*\n");
	printParseTree(root, 0);
	return &(*root);
}


// Hard-code to map with enum NodeType declared in Parse.h 
char *nodeTypeStrArr[] = {
	"Program",
	"Literal",
	"BooleanLiteral",
	"Type",

	// EXPRESSIONS
	"PrimaryExpression",
	"AssignmentExpression",
	"RelationalExpression",
	"LogicalExpression",
	// !EXPRESSIONS

	// STATEMENTS
	"StatementBlock",
	"StatementSeq",
	"Statement",
	"LabeledStatement",
	"ExpressionStatement",
	"SelectionStatement",
	"DeclaretionStatement",
	"DeclaretionStatementList",
	// !STATEMENTS

	"DeclaretionGlobalSeq"
};


void printParseTree(Node *root, int level) {
	if (root == 0) return;
	printf("%*s" "%d <%s> ", level * 4, "", level, nodeTypeStrArr[root->nodeType]);
	// printf("%*s" "%s ", level * 4, "", nodeTypeStrArr[root->nodeType]);
	/*
	if (root->token.type != NAtk && root->token.type != EOFtk) {
	// printf(" Token on line #%d is %s", root->token.lineNum, root->token.str);
	printf(" [Token %s on line #%d]", root->token.str, root->token.lineNum);
	}
	*/
	if (root->tokenPtr->size() > 0)
	{
		printf("{Token(s) found: ");
		for (int i = 0; i < root->tokenPtr->size(); i++)
		{
			int lineN = root->tokenPtr->operator[](i).lineNum;
			std::string str = root->tokenPtr->operator[](i).str;
			std::string typeStr = GetStringByTokenType(root->tokenPtr->operator[](i).type);
			printf("%s (%s, #%d)", str.c_str(), typeStr.c_str(), root->nodeType);
			if (root->tokenPtr->size() - i > 1)
				printf(", and ");
		}
		printf("}");
	}
	printf("\n");

	printParseTree(root->child1, level + 1);
	printParseTree(root->child2, level + 1);
	printParseTree(root->child3, level + 1);
	printParseTree(root->child4, level + 1);
	//printf("%*s" "%d </%s>\n ", level * 4, "", level, nodeTypeStrArr[root->nodeType]);

}


Token *getTokenPtr(Token *_tk) {
	Token *tokenPtr = new Token(_tk->str, _tk->type, _tk->lineNum);//(Token *)malloc(sizeof(Token));
	return tokenPtr;
}



void ErrorTokenExceptation(std::string context, TokenType except, Token *recieved) {
	if (isError)
		return;
	isError = true;
	std::string tokenTypeName = GetStringByTokenType(except);
	printf("ERROR[%s]: expect '%s', but received '%s' at (%d,%d) \n", context.c_str(), tokenTypeName.c_str(), tk->str.c_str(), tk->lineNum, tk->rowNum);
}

void ErrorNodeExpectation(std::string context, NodeType expect)
{
	printf("ERROR[%s]: expect '%s', but received '%s' on line #%d \n", context.c_str(), nodeTypeStrArr[expect]);
}

int curMethodLevel = 0;

void OnEnterParseMethod(std::string context)
{
	curMethodLevel++;
	std::string space;
	for (int i = 0; i < curMethodLevel; i++)
	{
		space.append("   ");
	}
	printf("%s %d <%s>, current token:'%s', type:'%s'\n", space.c_str(), curMethodLevel, context.c_str(), tk->str.c_str(), GetStringByTokenType(tk->type).c_str());
}

void OnExitParseMethod(std::string context)
{
	std::string space;
	for (int i = 0; i < curMethodLevel; i++)
	{
		space.append("   ");
	}
	printf("%s %d </%s>, current token: '%s' (%d,%d), type:'%s'\n", space.c_str(), curMethodLevel, context.c_str(), tk->str.c_str(), tk->lineNum, tk->rowNum, GetStringByTokenType(tk->type).c_str());
	curMethodLevel--;
}

/*---------------/TREE---------------*/

bool Program(Token * _tk, Node *& outNode)
{
	OnEnterParseMethod("Program");
	tk = _tk;
	outNode = new Node(nodeProgram);
	DeclaretionGlobalSeq(_tk, outNode->child1);
	if (tk->type != tkKeywordVOID || !(tk = tk->next)->str._Equal("Main") || (tk = tk->next)->type != tkDelimiterLEFT_PA || (tk = tk->next)->type != tkDelimiterRIGHT_PA) {
		ErrorTokenExceptation("Program", tkKeywordVOID, tk);
		OnExitParseMethod("Program 1"); return false;
	}
	Next();
	bool b1 = StatementBlock(tk, outNode->child2);
	OnExitParseMethod("Program 2"); return b1;
}

bool Literal(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("Literal");
	tk = _tk;
	outNode = new Node(nodeLiteral);
	if (_tk->type == tkNUMBER)
	{
		outNode->AddToken(_tk);
		Next();
		OnExitParseMethod("Literal 1"); 
		return true;
	}
	OnExitParseMethod("Literal 2"); 
	return BooleanLiteral(_tk, outNode->child1);
}

bool BooleanLiteral(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("BooleanLiteral");
	outNode = new Node(nodeBooleanLiteral);
	if (_tk->type == tkKeywordTRUE || _tk->type == tkKeywordFALSE)
	{
		outNode->AddToken(_tk);
		tk = _tk->next;
		OnExitParseMethod("BooleanLiteral 1"); return true;
	}
	OnExitParseMethod("BooleanLiteral 2"); return false;
}

bool Type(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("Type");
	outNode = new Node(nodeType);
	if (_tk->type == tkKeywordBOOL || _tk->type == tkKeywordINT)
	{
		outNode->AddToken(_tk);
		tk = _tk->next;
		OnExitParseMethod("Type 1"); return true;
	}
	OnExitParseMethod("Type 2"); return false;
}

bool PrimaryExpression(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("PrimaryExpression");
	tk = _tk;
	outNode = new Node(nodePrimaryExpression);
	if (tk->type == tkID)
	{
		outNode->AddToken(_tk);
		Next();
		OnExitParseMethod("PrimaryExpression 1"); return true;
	}
	if (Literal(_tk, outNode->child1))
	{
		OnExitParseMethod("PrimaryExpression 2"); return true;
	}
	if (tk->type == tkDelimiterLEFT_PA)
	{
		Next();
		if (AssignmentExpression(tk, outNode->child1))
		{
			if (tk->type == tkDelimiterRIGHT_PA)
			{
				Next();
				OnExitParseMethod("PrimaryExpression 3"); return true;
			}
			else
			{
				ErrorTokenExceptation("PrimaryExpression", tkDelimiterRIGHT_PA, tk);
				OnExitParseMethod("PrimaryExpression 4");
				return false;
			}
		}
		OnExitParseMethod("PrimaryExpression 5");
		return false;
	}
	OnExitParseMethod("PrimaryExpression 6"); return false;
}

bool AssignmentExpression(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("AssignmentExpression");
	tk = _tk;
	outNode = new Node(nodeAssignmentExpression);
	if (tk->type == tkID)
	{
		outNode->AddToken(tk);
		Next();
		if (tk->type == tkOperatorASSIGN)
		{
			Next();
			if (tk->type == tkNUMBER || tk->type == tkID)
			{
				outNode->AddToken(tk);
				Next();
				OnExitParseMethod("AssignmentExpression 2"); return true;
			}
			ErrorTokenExceptation("AssignmentExpression", tkNUMBER, tk);
			OnExitParseMethod("AssignmentExpression 3");
			return false;
		}
		outNode->child2 = 0;
		OnExitParseMethod("AssignmentExpression 4");
		return true;
	}
	OnExitParseMethod("AssignmentExpression 5"); return false;
}

bool RelationalExpression(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("RelationalExpression");
	tk = _tk;
	outNode = new Node(nodeRelationalExpression);
	if (PrimaryExpression(_tk, outNode->child1))
	{
		if (tk->type == tkOperatorLESS || tk->type == tkOperatorGREATER || tk->type == tkOperatorLESS_EQUAL || tk->type == tkOperatorGREATER_EQUAL || tk->type == tkOperatorEQUAL || tk->type == tkOperatorNOT_EQUAL)
		{
			outNode->AddToken(tk);
			Next();
			if (PrimaryExpression(tk, outNode->child2))
			{
				OnExitParseMethod("RelationalExpression 1"); return true;
			}
			else {
				OnExitParseMethod("RelationalExpression 2"); return false;
				printf("ERROR RelationalExpression 1\n");
			}
		}
		OnExitParseMethod("RelationalExpression 3"); return true;
	}
	OnExitParseMethod("RelationalExpression 4"); return false;
}

bool LogicalExpression(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("LogicalExpression");
	tk = _tk;
	outNode = new Node(nodeLogicalExpression);
	if (RelationalExpression(_tk, outNode->child1))
	{
		if (tk->type == tkOperatorLOGICAL_AND || tk->type == tkOperatorLOGICAL_OR)
		{
			outNode->AddToken(tk);
			Next();
			if (LogicalExpression(tk, outNode->child2))
			{
				OnExitParseMethod("LogicalExpression 1"); return true;
			}
			else {
				OnExitParseMethod("LogicalExpression 2"); return false;
				printf("ERROR LogicalExpression 1\n");
			}
		}
		OnExitParseMethod("LogicalExpression 3"); return true;
	}
	OnExitParseMethod("LogicalExpression 4"); return false;
}

bool StatementBlock(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("StatementBlock");
	tk = _tk;
	outNode = new Node(nodeStatementBlock);
	if (tk->type == tkDelimiterLEFT_BRACE)
	{
		Next();
		if (StatementSeq(tk, outNode->child1))
		{
			if (tk->type == tkDelimiterRIGHT_BRACE)
			{
				Next();
				OnExitParseMethod("StatementBlock 1"); return true;
			}
			else
			{
				ErrorTokenExceptation("StatementBlock", tkDelimiterRIGHT_BRACE, tk);
				printf("ERROR StatementBlock 1\n");
				OnExitParseMethod("StatementBlock 2"); return false;
			}
		}
		else
		{
			printf("ERROR StatementBlock, somethign wrong in statement sequence\n");
			return false;
		}
	}
	if (Statement(tk, outNode->child1))
	{
		OnExitParseMethod("StatementBlock 3"); return true;
	}
	printf("ERROR StatementBlock 3\n");
	OnExitParseMethod("StatementBlock 4"); return false;
}

bool StatementSeq(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("StatementSeq");
	tk = _tk;
	outNode = new Node(nodeStatementSeq);
	if (Statement(tk, outNode->child1))
	{
		if (StatementSeq(tk, outNode->child2)) {
			OnExitParseMethod("StatementSeq 1"); return true;
		}
		outNode->child2 = 0;
		printf("here is going ouy from StatementSeq, need to check\n");
		OnExitParseMethod("StatementSeq 2"); return true;
	}
	outNode->child1 = 0;
	outNode = 0;
	OnExitParseMethod("StatementSeq 3"); return true;
}

bool Statement(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("Statement");
	tk = _tk;
	outNode = new Node(nodeStatement);
	if (LabeledStatement(_tk, outNode->child1)) {
		OnExitParseMethod("Statement found LabeledStatement"); return true;
	}
	if (ExpressionStatement(_tk, outNode->child1)) {
		OnExitParseMethod("Statement fount ExpressionStatement"); return true;
	}
	if (SelectionStatement(_tk, outNode->child1)) {
		OnExitParseMethod("Statement found SelectionStatement"); return true;
	}
	if (DeclaretionStatement(_tk, outNode->child1)) {
		OnExitParseMethod("Statement found DeclaretionStatement"); return true;
	}
	outNode->child1 = 0;
	tk = _tk;
	OnExitParseMethod("Statement nothing"); return false;
}

bool LabeledStatement(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("LabeledStatement");
	tk = _tk;
	outNode = new Node(nodeLabeledStatement);
	if (tk->type == tkKeywordCASE)
	{
		outNode->AddToken(tk);
		Next();
		if (tk->type == tkNUMBER)
		{
			outNode->AddToken(tk);
			Next();
			if (tk->type == tkOperatorCOLON)
			{
				Next();
				if (StatementSeq(tk, outNode->child1))
				{
					if (tk->type == tkKeywordBREAK)
					{
						Next();
						if (tk->type != tkDelimiterSEMICOLON)
						{
							ErrorTokenExceptation("LabeledStatement", tkKeywordBREAK, tk);
							OnExitParseMethod("LabeledStatement found break but no ';'");
							return false;
						}
						Next();
						OnExitParseMethod("LabeledStatement found break;"); return true;
					}
					ErrorTokenExceptation("LabeledStatement", tkKeywordBREAK, _tk);
					return false;
				}
				else
				{
					printf("ERROR LabeledStatement 1\n");
					OnExitParseMethod("LabeledStatement 2"); return false;
				}
			}
			else
			{
				ErrorTokenExceptation("LabeledStatement", tkOperatorCOLON, tk);
				printf("ERROR LabeledStatement 2\n");
				OnExitParseMethod("LabeledStatement 3"); return false;
			}
		}
		else
		{
			printf("ERROR LabeledStatement 3\n");
			OnExitParseMethod("LabeledStatement 4"); return false;
		}
	}
	if (tk->type == tkKeywordDEFAULT)
	{
		outNode->AddToken(tk);
		Next();
		if (tk->type == tkOperatorCOLON)
		{
			Next();
			if (StatementSeq(tk, outNode->child1))
			{
				if (tk->type == tkKeywordBREAK)
				{
					Next();
					if (tk->type != tkDelimiterSEMICOLON)
					{
						ErrorTokenExceptation("LabeledStatement", tkKeywordBREAK, tk);
						OnExitParseMethod("LabeledStatement found break but no ';'");
						return false;
					}
					Next();
					OnExitParseMethod("LabeledStatement found break;"); return true;
				}
				ErrorTokenExceptation("LabeledStatement", tkKeywordBREAK, _tk);
				return false;
			}
			else
			{
				printf("ERROR LabeledStatement 4\n");
				OnExitParseMethod("LabeledStatement 6"); return false;
			}
		}
		else
		{
			ErrorTokenExceptation("LabeledStatement", tkOperatorCOLON, tk);
			printf("ERROR LabeledStatement 5\n");
			OnExitParseMethod("LabeledStatement 7"); return false;
		}
	}
	OnExitParseMethod("LabeledStatement 8"); return false;
}

bool ExpressionStatement(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("ExpressionStatement");
	tk = _tk;
	outNode = new Node(nodeExpressionStatement);
	if (AssignmentExpression(tk, outNode->child1))
	{
		if (tk->type == tkDelimiterSEMICOLON)
		{
			Next();
			OnExitParseMethod("ExpressionStatement 1"); return true;
		}
		ErrorTokenExceptation("ExpressionStatement", tkDelimiterSEMICOLON, tk);
		OnExitParseMethod("ExpressionStatement 2"); return false;
	}
	OnExitParseMethod("ExpressionStatement 3"); return false;
}

bool SelectionStatement(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("SelectionStatement");
	tk = _tk;
	outNode = new Node(nodeSelectionStatement);
	if (tk->type == tkKeywordIF)
	{
		outNode->AddToken(tk);
		Next();
		if (tk->type != tkDelimiterLEFT_PA) {
			ErrorTokenExceptation("SelectionStatement", tkDelimiterLEFT_PA, tk);
			printf("ERROR SelectionStatement 1\n");
			OnExitParseMethod("SelectionStatement 1"); return false;
		}
		Next();
		if (!RelationalExpression(tk, outNode->child1))
		{
			printf("ERROR SelectionStatement 2: wait for expression\n");
			OnExitParseMethod("SelectionStatement 2"); return false;
		}
		if (tk->type != tkDelimiterRIGHT_PA) {
			ErrorTokenExceptation("SelectionStatement", tkDelimiterRIGHT_PA, tk);
			printf("ERROR SelectionStatement 3\n");
			OnExitParseMethod("SelectionStatement 3"); return false;
		}
		Next();
		if (!StatementBlock(tk, outNode->child2))
		{
			printf("ERROR SelectionStatement 4\n");
			OnExitParseMethod("SelectionStatement 4"); return false;
		}
		if (tk->type != tkKeywordELSE) {
			OnExitParseMethod("SelectionStatement 5"); return true;
		}
		Next();
		if (!StatementBlock(tk, outNode->child3))
		{
			printf("ERROR SelectionStatement 5\n");
			OnExitParseMethod("SelectionStatement 6"); return false;
		}
		OnExitParseMethod("SelectionStatement 7"); return true;
	}
	if (tk->type == tkKeywordSWITCH)
	{
		outNode->AddToken(tk);
		Next();
		if (tk->type != tkDelimiterLEFT_PA) {
			ErrorTokenExceptation("SelectionStatement", tkDelimiterLEFT_PA, tk);
			printf("ERROR SelectionStatement 6\n");
			OnExitParseMethod("SelectionStatement 8"); return false;
		}
		Next();
		if (tk->type != tkID)
		{
			ErrorTokenExceptation("SelectionStatement", tkID, tk);
			OnExitParseMethod("SelectionStatement 9"); return false;
		}
		outNode->AddToken(tk);
		Next();
		if (tk->type != tkDelimiterRIGHT_PA) {
			ErrorTokenExceptation("SelectionStatement", tkDelimiterRIGHT_PA, tk);
			printf("ERROR SelectionStatement 8\n");
			OnExitParseMethod("SelectionStatement 10"); return false;
		}
		Next();
		if (!StatementBlock(tk, outNode->child2))
		{
			printf("ERROR SelectionStatement 9\n");
			OnExitParseMethod("SelectionStatement 11"); return false;
		}
		OnExitParseMethod("SelectionStatement 12"); return true;
	}
	OnExitParseMethod("SelectionStatement 13"); return false;
}

bool DeclaretionStatement(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("DeclaretionStatement");
	tk = _tk;
	outNode = new Node(nodeDeclaretionStatement);
	if (Type(tk, outNode->child1))
	{
		if (DeclaretionStatementList(tk, outNode->child2))
		{
			if (tk->type == tkDelimiterSEMICOLON) {
				Next();
				OnExitParseMethod("DeclaretionStatement 1"); return true;
			}
			ErrorTokenExceptation("DeclaretionStatement", tkDelimiterSEMICOLON, tk);
			OnExitParseMethod("DeclaretionStatement 1"); return false;
		}
		else
		{
			printf("ERROR DeclaretionStatement 1\n");
			OnExitParseMethod("DeclaretionStatement 2"); return false;
		}
	}
	OnExitParseMethod("DeclaretionStatement 3"); return false;
}

bool DeclaretionStatementList(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("DeclaretionStatementList");
	tk = _tk;
	outNode = new Node(nodeDeclaretionStatementList);
	if (tk->type != tkID)
	{
		ErrorTokenExceptation("DeclaretionStatementList", tkID, tk);
		printf("ERROR DeclaretionStatementList 1\n");
		OnExitParseMethod("DeclaretionStatementList 1"); return false;
	}
	outNode->AddToken(tk);
	Next();
	if (tk->type == tkOperatorASSIGN) {
		Next();
		if (!Literal(tk, outNode->child1))
			return false;
	}
	if (tk->type != tkDelimiterCOMMA)
	{
		OnExitParseMethod("DeclaretionStatementList 2"); return true;
	}
	Next();
	if (!DeclaretionStatementList(tk, outNode->child2)) {
		OnExitParseMethod("DeclaretionStatementList 3"); return false;
	}
	OnExitParseMethod("DeclaretionStatementList 4"); return true;
}

bool DeclaretionGlobalSeq(Token * _tk, Node*& outNode)
{
	OnEnterParseMethod("DeclaretionGlobalSeq");
	tk = _tk;
	outNode = new Node(nodeDeclaretionGlobalSeq);
	if (DeclaretionStatement(tk, outNode->child1))
	{
		/*if (tk->type != tkDelimiterSEMICOLON) {
			ErrorTokenExceptation("DeclaretionGlobalSeq", tkDelimiterSEMICOLON, tk);
			printf("ERROR DeclaretionGlobalSeq 1\n");
			OnExitParseMethod("DeclaretionGlobalSeq 1"); return false;
		}
		Next();*/
		if (DeclaretionGlobalSeq(tk, outNode->child2)) {
			OnExitParseMethod("DeclaretionGlobalSeq 2"); return true;
		}
		outNode->child2 = 0;
		OnExitParseMethod("DeclaretionGlobalSeq 3"); return true;
	}
	OnExitParseMethod("DeclaretionGlobalSeq 4"); return false;
}
