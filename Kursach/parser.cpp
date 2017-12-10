#include "token.h"
#include "scanner.h"
#include "parser.h"
#include <istream>

Token *tk = new  Token( "N/A", tkNA, 0 );
std::istream *fP = NULL;

Node * getNode(NodeType);
void printParseTree(Node *, int);

Node * program();
void Parse(std::istream *filePtr) {
	lineNum = 1; // reset line number
	fP = filePtr;
	//rewind(fP); TODO

	tk = Scan(fP);
	Node *root = NULL;

	root = program();

	if (tk->type == tkEOF)
		printf("Parse OK! \n");
	else {
		printf("Parse NOT OK! \n");
		return;
		//exit(1);
	}

	printf("\n*-----Parse Tree-----*\n");
	printParseTree(root, 0);
	return;
}

void insertToken(Node *, Token *);
Node * var();
Node * block();
Node * program() {
	OnEnterParseMethod("program");
	Node *node = getNode(programNode);
	node->child1 = var();
	if (tk->type == tkKeywordDO || tk->type == tkKeywordVAR) {
		//insertToken(node, tk);
		tk = Scan(fP);
	}
	else {
		ErrorTokenExceptation("program", tkKeywordVAR, tk);
		return 0;//exit(1);
	}

	node->child2 = block();
	if (tk->type == tkKeywordRETURN) {
		//insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("program");
		return node;
	}
	else {
		ErrorTokenExceptation("program", tkKeywordRETURN, tk);
		return 0;//exit(1);
	}
}

Node * mvars();
Node * var() {
	OnEnterParseMethod("var");
	Node *node = getNode(varNode);
	if (tk->type == tkKeywordVAR) {
		//insertToken(node, tk);
		tk = Scan(fP);
		if (tk->type == tkID) {

			insertToken(node, tk);
			//node->token = tk;
			tk = Scan(fP);
		}
		else {
			ErrorTokenExceptation("var", tkID, tk);
			return 0;//exit(1);
		}
		node->child1 = mvars();
		if (tk->type == tkDelimiterDOT) {
			//insertToken(node, tk);
			tk = Scan(fP);
			OnExitParseMethod("var 1");
			return node;
		}
		else {
			ErrorTokenExceptation("var", tkDelimiterDOT, tk);
			return 0;//exit(1);
		}
	}
	else {
		printf("WARNING: no VAR\n");
		OnExitParseMethod("var 2");
		return node; // predict <var> --> empty
	}
}

Node * mvars() {
	OnEnterParseMethod("mvars");
	Node * node = getNode(mvarsNode);
	if (tk->type == tkOperatorCOLON) {
		//insertToken(node, tk);
		tk = Scan(fP);
		if (tk->type == tkID) {
			insertToken(node, tk);
			//node->token = tk;
			tk = Scan(fP);
		}
		else {
			ErrorTokenExceptation("mvars", tkID, tk);
			return 0;//exit(1);
		}
		node->child1 = mvars();
		OnExitParseMethod("mvars 1");
		return node;
	}
	else {
		OnExitParseMethod("mvars 2");
		return node; // predict <mvars> --> empty
	}
}

Node * stats();
Node * block() {
	OnEnterParseMethod("block");
	Node *node = getNode(blockNode);
	if (tk->type == tkSTART) {
		//insertToken(node, tk);
		tk = Scan(fP);
		node->child1 = var();
		node->child2 = stats();
		if (tk->type == tkFINISH) {
			//insertToken(node, tk);
			tk = Scan(fP);
			OnExitParseMethod("block");
			return node;
		}
		else {
			ErrorTokenExceptation("block", tkFINISH, tk);
			return 0;//exit(1);
		}
	}
	else {
		ErrorTokenExceptation("block", tkSTART, tk);
		return 0;//exit(1);
	}
}

Node * stat();
Node * mStat();
Node * stats() {
	OnEnterParseMethod("stats");
	Node *node = getNode(statsNode);
	node->child1 = stat();
	node->child2 = mStat();
	OnExitParseMethod("stats");
	return node;
}

Node * in();
Node * out();
Node * loop();
Node * ifTk();
Node * assign();
Node * stat() {
	OnEnterParseMethod("stat");
	Node *node = getNode(statNode);
	if (tk->type == tkKeywordREAD) {
		node->child1 = in();
		OnExitParseMethod("stat 1");
		return node;
	}
	else if (tk->type == tkKeywordPRINT) {
		node->child1 = out();
		OnExitParseMethod("stat 2");
		return node;
	}
	else if (tk->type == tkSTART) {
		node->child1 = block();
		OnExitParseMethod("stat 3");
		return node;
	}
	else if (tk->type == tkKeywordIF) {
		node->child1 = ifTk();
		OnExitParseMethod("stat 4");
		return node;
	}
	else if (tk->type == tkKeywordREPEAT) {
		node->child1 = loop();
		OnExitParseMethod("stat 5");
		return node;
	}
	else if (tk->type == tkID) {
		node->child1 = assign();
		OnExitParseMethod("stat 6");
		return node;
	}
	else {
		printf("ERROR: expect either tkKeywordREAD, tkKeywordPRINT, tkSTART, Iftk, tkKeywordREPEAT, or tkID. ");
		printf("But received %s on line #%d \n", tk->str.c_str(), tk->lineNum);
		return 0;//exit(1);
	}
}

Node * mStat() {
	OnEnterParseMethod("mStat");
	Node *node = getNode(mStatNode);
	if (tk->type == tkKeywordREAD || tk->type == tkKeywordPRINT || tk->type == tkSTART
		|| tk->type == tkKeywordIF || tk->type == tkKeywordREPEAT || tk->type == tkID) {
		node->child1 = stat();
		node->child2 = mStat();
		OnExitParseMethod("mStat 1");
		return node;
	}
	else {
		OnExitParseMethod("mStat 2");
		return node; // predict <mStat> --> empty
	}
}

Node * in() {
	OnEnterParseMethod("in");
	Node *node = getNode(inNode);
	if (tk->type == tkKeywordREAD) {
		//insertToken(node, tk);
		tk = Scan(fP);
		if (tk->type == tkID) {
			insertToken(node, tk);
			//node->token = tk;
			tk = Scan(fP);
			if (tk->type == tkDelimiterDOT) {
				//insertToken(node, tk);
				tk = Scan(fP);
				OnExitParseMethod("in");
				return node;
			}
			else {
				ErrorTokenExceptation("in", tkDelimiterDOT, tk);
				return 0;//exit(1);
			}
		}
		else {
			ErrorTokenExceptation("in", tkID, tk);
			return 0;//exit(1);
		}
	}
	else {
		ErrorTokenExceptation("in", tkKeywordREAD, tk);
		return 0;//exit(1);
	}
}

Node * expr();
Node * out() {
	OnEnterParseMethod("out");
	Node *node = getNode(outNode);
	if (tk->type == tkKeywordPRINT) {
		//insertToken(node, tk);
		tk = Scan(fP);
		node->child1 = expr();
		if (tk->type == tkDelimiterDOT) {
			//insertToken(node, tk);
			tk = Scan(fP);
			OnExitParseMethod("out");
			return node;
		}
		else {
			ErrorTokenExceptation("out", tkDelimiterDOT, tk);
			return 0;//exit(1);
		}
	}
	else {
		ErrorTokenExceptation("out", tkKeywordPRINT, tk);
		return 0;//exit(1);
	}
}

Node * t();
Node * expr() {
	OnEnterParseMethod("expr");
	Node *node = getNode(exprNode);
	node->child1 = t();
	if (tk->type == tkOperatorMUL) {
		tk = Scan(fP);
		node->child2 = expr();
		OnExitParseMethod("expr 1");
		return node;
	}
	else if (tk->type == tkOperatorDIV) {
		tk = Scan(fP);
		node->child2 = expr();
		OnExitParseMethod("expr 2");
		return node;
	}
	else {
		OnExitParseMethod("expr 3");
		return node; // predict empty after <T>
	}
}

Node * f();
Node * t() {
	OnEnterParseMethod("t");
	Node *node = getNode(tNode);
	node->child1 = f();
	if (tk->type == tkOperatorADD) {
		tk = Scan(fP);
		node->child2 = t();
		OnExitParseMethod("t 1");
		return node;
	}
	else if (tk->type == tkOperatorSUBTRACT) {
		tk = Scan(fP);
		node->child2 = t();
		OnExitParseMethod("t 2");
		return node;
	}
	else {
		OnExitParseMethod("t 3");
		return node; // predict empty after <F>
	}
}

Node * r();
Node * f() {
	OnEnterParseMethod("f");
	Node *node = getNode(fNode);
	if (tk->type == tkOperatorSUBTRACT) {
		insertToken(node, tk);
		tk = Scan(fP);
		node->child1 = f();
		OnExitParseMethod("f 1");
		return node;
	}
	else {
		node->child1 = r();
		OnExitParseMethod("f 2");
		return node;
	}
}

Node * r() {
	OnEnterParseMethod("r");
	Node *node = getNode(rNode);
	if (tk->type == tkDelimiterLEFT_PA) {
		insertToken(node, tk);
		tk = Scan(fP);
		node->child1 = expr();
		if (tk->type == tkDelimiterRIGHT_PA) {
			insertToken(node, tk);
			tk = Scan(fP);
			OnExitParseMethod("r 1");
			return node;
		}
		else {
			ErrorTokenExceptation("r", tkDelimiterRIGHT_PA, tk);
			return 0;//exit(1);
		}
	}
	else if (tk->type == tkID) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("r 2");
		return node;
	}
	else if (tk->type == tkNUMBER) {
		insertToken(node, tk);
		//node->token = tk;
		tk = Scan(fP);
		OnExitParseMethod("r 3");
		return node;
	}
	else {
		printf("ERROR: expect either tkDelimiterLEFT_PA, or tkID, or tkNUMBER. ");
		printf("But received %s on line #%d \n", tk->str.c_str(), tk->lineNum);
		return 0;//exit(1);
	}
}

/*------ for re-written grammar ------*/
void y() {
	OnEnterParseMethod("y");
	if (tk->type == tkOperatorADD) {
		tk = Scan(fP);
		t();
		OnExitParseMethod("y 1");
		return;
	}
	else if (tk->type == tkOperatorSUBTRACT) {
		tk = Scan(fP);
		t();
		OnExitParseMethod("y 2");
		return;
	}
	else {
		OnExitParseMethod("y 3");
		return; // predict <Y> --> empty	
	}
}

void x() {
	OnEnterParseMethod("x");
	if (tk->type == tkOperatorMUL) {
		tk = Scan(fP);
		expr();
		OnExitParseMethod("x 1");
		return;
	}
	else if (tk->type == tkOperatorDIV) {
		tk = Scan(fP);
		expr();
		OnExitParseMethod("x 2");
		return;
	}
	else {
		OnExitParseMethod("x 3");
		return; // predict <X> --> empty
	}
}
/*------ /for re-written grammar ------*/

Node * ro();
Node * ifTk() {
	OnEnterParseMethod("ifTk");
	Node *node = getNode(ifNode);
	if (tk->type == tkKeywordIF) {
		//insertToken(node, tk);
		tk = Scan(fP);
		if (tk->type == tkDelimiterLEFT_BRACKET) {
			tk = Scan(fP);
			node->child1 = expr();
			node->child2 = ro();
			node->child3 = expr();
			if (tk->type == tkDelimiterRIGHT_BRACKET) {
				tk = Scan(fP);
				node->child4 = block();
				OnExitParseMethod("ifTK 1");
				return node;
			}
			else {
				ErrorTokenExceptation("ifTk", tkDelimiterRIGHT_BRACKET, tk);
				return 0;//exit(1);
			}
		}
		else {
			ErrorTokenExceptation("ifTk", tkDelimiterLEFT_BRACKET, tk);
			return 0;//exit(1);
		}
	}
	else {
		ErrorTokenExceptation("ifTk", tkKeywordIF, tk);
		return 0;//exit(1);
	}
}

Node * ro() {
	OnEnterParseMethod("ro");
	Node *node = getNode(roNode);
	if (tk->type == tkOperatorLESS_EQUAL) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("ro 1");
		return node;
	}
	else if (tk->type == tkOperatorGREATER_EQUAL) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("ro 2");
		return node;
	}
	else if (tk->type == tkOperatorEQUAL) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("ro 3");
		return node;
	}
	else if (tk->type == tkOperatorGREATER) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("ro 4");
		return node;
	}
	else if (tk->type == tkOperatorLESS) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("ro 5");
		return node;
	}
	else if (tk->type == tkOperatorDIFF) {
		insertToken(node, tk);
		tk = Scan(fP);
		OnExitParseMethod("ro 6");
		return node;
	}
	else {
		printf("ERROR: expect relational operator, but received %s on line #%d \n",
			tk->str.c_str(), tk->lineNum);
		return 0;//exit(1);
	}
}

Node * assign() {
	OnEnterParseMethod("assign");
	Node *node = getNode(assignNode);

	if (tk->type == tkID) {
		insertToken(node, tk);
		//node->token = tk;
		tk = Scan(fP);
		if (tk->type == tkOperatorASSIGN) {
			//insertToken(node, tk);
			tk = Scan(fP);
			expr();
			if (tk->type == tkDelimiterDOT) {
				insertToken(node, tk);
				tk = Scan(fP);
				OnExitParseMethod("assign ");
				return node;
			}
			else {
				ErrorTokenExceptation("assign", tkDelimiterDOT, tk);
				return 0;//exit(1);
			}
		}
		else {
			ErrorTokenExceptation("assign", tkOperatorASSIGN, tk);
			return 0;//exit(1);
		}
	}
	else {
		ErrorTokenExceptation("assign", tkID, tk);
		return 0;//exit(1);
	}
}


Node * loop() {
	OnEnterParseMethod("loop");
	Node *node = getNode(loopNode);
	if (tk->type == tkKeywordREPEAT) {
		//insertToken(node, tk);
		tk = Scan(fP);
		if (tk->type == tkDelimiterLEFT_BRACKET) {
			tk = Scan(fP);
			node->child1 = expr();
			node->child2 = ro();
			node->child3 = expr();
			if (tk->type == tkDelimiterRIGHT_BRACKET) {
				tk = Scan(fP);
				node->child4 = block();
				OnExitParseMethod("loop");
				return node;
			}
			else {
				ErrorTokenExceptation("loop", tkDelimiterRIGHT_BRACKET, tk);
				return 0;//exit(1);
			}
		}
		else {
			ErrorTokenExceptation("loop", tkDelimiterLEFT_BRACKET, tk);
			return 0;//exit(1);
		}
	}
	else {
		ErrorTokenExceptation("loop", tkKeywordREPEAT, tk);
		return 0;//exit(1);
	}
}


/*---------------TREE---------------*/

// Hard-code to map with enum NodeType declared in Parse.h 
char *nodeTypeStrArr[] = {
	"<program>", "<block>", "<var>", "<mvars>", "<expr>", "<x>", "<t>", "<y>", "<f>", "<r>",
	"<stats>", "<mStat>", "<stat>", "<in>", "<out>", "<if>", "<loop>", "<assign>", "<ro>"
};


void printParseTree(Node *root, int level) {
	if (root == NULL) return;
	printf("%*s" "%d %s ", level * 4, "", level, nodeTypeStrArr[root->nodeType]);
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
			printf("%s (%s, #%d)", str.c_str(), typeStr.c_str(), lineN);
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
}

// Mark the new node by its type
Node* getNode(NodeType nodeType) {
	Node *node = new Node();
	//Node *node;
	//node = &nNode;//(Node *)malloc(sizeof(Node));

	node->nodeType = nodeType;
	Token someDefToken ("SHIET", tkNA, -2);
	/*
	node->tokenPtr->push_back(someDefToken);
	node->tokenPtr->push_back(someDefToken);
	node->tokenPtr->push_back(someDefToken);
	node->tokenPtr->push_back(someDefToken);*/
	node->child1 = node->child2 = node->child3 = node->child4 = NULL;

	return node;
}

Token *getTokenPtr(Token *tk) {
	Token *tokenPtr = new Token(tk->str, tk->type, tk->lineNum);//(Token *)malloc(sizeof(Token));
	return tokenPtr;
}

// Insert new token at the end of the linked-list of tokens 
void insertToken(Node *node, Token *tk) {
	Token *newToken = new Token(tk->str, tk->type, tk->lineNum);// getTokenPtr(tk);
	/*printf("\n\n");
	printToken(newToken);
	printf("\n\n");*/
	node->tokenPtr->push_back(*newToken);
	/*if (node->tokenPtr == NULL) {
		node->tokenPtr = newToken;
	}
	else {
		printf("here %s", node->tokenPtr->str.c_str());
		Token *tmp = node->tokenPtr;
		int c = 0;
		printf("gere %d\n", c++);
		while (tmp->next != NULL) {
			printf("gere %d\n", c++);
			tmp = tmp->next;
		}
		tmp->next = newToken;
	}*/
}


void ErrorTokenExceptation(std::string context, TokenType except, Token *recieved) {
	std::string tokenTypeName = GetStringByTokenType(except);
	printf("ERROR[%s]: expect '%s', but received '%s' on line #%d \n", context.c_str(), tokenTypeName.c_str(), tk->str.c_str(), tk->lineNum);
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
	printf("%s %d </%s>, current token: '%s', type:'%s'\n", space.c_str(), curMethodLevel, context.c_str(), tk->str.c_str(), GetStringByTokenType(tk->type).c_str());
	curMethodLevel--;
}

/*---------------/TREE---------------*/