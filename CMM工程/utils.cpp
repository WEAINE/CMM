#include "globals.h"
#include "utils.h"
#include "Accidence.h"

// 该函数在parse.cpp创建树时用到
char* copyString(char* source)
{
    int stringLen = strlen(source);
	char* target = (char*)malloc(stringLen + 1);

	strcpy(target, source);

	return target;
}

TreeNode* newStmtNode(StmtKind pKind)
{
	// 把所有指针置为NULL
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->sibling = NULL;

	int index;

	for (index = 0; index < MAXCHILDLEN; index++)
	{
		t->child[index] = NULL;
	}

	t->nodeKind = stmtK;
	t->kind.stmt = pKind;	
	t->attribute.name = NULL;

	return t;
}


TreeNode* newExpNode(ExpKind kind)
{
	// 把所有指针置为NULL
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->sibling = NULL;

	int index;

	for (index = 0; index < MAXCHILDLEN; index++)
	{
		t->child[index] = NULL;
	}

	t->nodeKind = expK;
	(t->kind).exp = kind;
	t->attribute.name = NULL;

	return t;
}

void printOperator(TokenType);

void printTreeDriver(TreeNode* root)
{
	puts("--------------------------------------------------\n");
	puts("Syntax tree:");
	printTree(root);
	puts("\n--------------------------------------------------\n");
}

int space = 2;

void printTree(TreeNode* root)
{
	// putchar('\n');
	space += STEP;
	
	while (root != NULL)
	{
		printSpace(space);

		if (root->nodeKind == stmtK)
		{
			
			StmtKind stmtKind = root->kind.stmt;

			switch (stmtKind)
			{
				case declareK: // 打印declare statement
					// printSpace(KEEP);
					printf("Declare statement:\n");
					// printSpace(INCRESE);
					printSpace(space);

					if (root->variableType == INT)
					{
						printf("Type:int\n");
					}
					else
					{
						printf("Type:real\n");
					}

					// printSpace(KEEP);
					printSpace(space);
					printf("ID: %s\n", root->attribute.name);

					if (root->simpleOrArray.declare == arrayAK)
					{
						// printSpace(KEEP);
						printSpace(space);
						printf("Array size:\n");
					}
				
					break;

				case assignK: // 打印assign statement
					// printSpace(KEEP);
					printf("Assign to:\n");
					// printSpace(INCRESE);
					printSpace(space);
					printf("ID:%s\n",root->attribute.name );

					if (root->simpleOrArray.assign == arrayAK)
					{
						// printSpace(KEEP);
						printSpace(space);
						printf("Array Index:\n");
					}

					break;

				case ifK: // 打印if statement
					// printSpace(KEEP);
					// printSpace(space);
					printf("If statement:");

					if (root->child[2] != NULL)
					{
						printf("[else]");
					}

					putchar('\n');

					break;

				case whileK: // 打印repeat statement
					// printSpace(KEEP);
					printf("Repeat statement:\n");

					break;

				case writeK: // 打印write statement
					// printSpace(KEEP);
					printf("Write :\n");

					break;

				case readK: // 打印read statement
					//printSpace(KEEP);
					printf("Read :%s\n",root->attribute.name);

					if (root->child[0] != NULL)
					{
						printSpace(space);
						printf("Array Index:\n");
					}

					break;
			}			
				
		}
		else if (root->nodeKind == expK)
		{
			// printSpace(KEEP);
			ExpKind expKind = root->kind.exp;

			switch (expKind)
			{
				case opK: // 打印op kind exp
					// printSpace(INCRESE);
					printf("op:");
					printOperator(root->attribute.tokenType);
					putchar('\n');

					break;

				case constK: // 打印const node
					// printSpace(KEEP);
					printf("const:");

					if (root->variableType == INT)
					{
						printf("%d", root->attribute.int_value);
					}
					else
					{
						printf("%0.2f\n", root->attribute.real_value);
					}

					putchar('\n');

					break;
				case IDK: // 打印ID node
					// printSpace(KEEP);
					printf("ID:%s\n",root->attribute.name);

					if (root->child[0] != NULL)
					{
						printSpace(space);
						printf("Array Index:\n");
					}

					break;
			}
				
		}
		else
		{
			puts("ERROR.");
		}

		if (root->child[0] != NULL)
		{
			// putchar('\n');
			// printSpace(INCRESE);
			printTree(root->child[0]);
		}

		if (root->child[1] != NULL)
		{
			// putchar('\n');
			// printSpace(INCRESE);
			printTree(root->child[1]);
		}

		if (root->child[2] != NULL)
		{
			// putchar('\n');
			// printSpace(INCRESE);
			printTree(root->child[2]);
		}
		
        /*
		if (root->sibling != NULL)
		{
			printSpace(SETTOSTART);
			printTree(root->sibling);
		}
		*/
		
		root = root->sibling;			
	}

	space -= STEP;	
}

// 加，减，乘，除，小于，等于，不等于
void printOperator(TokenType tokenType)
{
	switch(tokenType)
	{
		case PLUS:
			putchar('+');
			break;
		case MINUS:
			putchar('-');
			break;
		case TIMES:
			putchar('*');
			break;
		case DIV:
			putchar('/');
			break;
		case LESS:
			putchar('<');
			break;
		case EQUAL:
			putchar('=');
			putchar('=');
			break;
		case UNEQUAL:
			putchar('!');
			putchar('=');
			break;
		default:
			printf("ERROR.");
	}
}

extern char tokenString[MAXSTRINGLEN + 1];
extern Entity *tokenList;
// Entity *currentNode = tokenList;
// int currentLineNo;
Entity *currentNode;
bool isInit = false;
TokenType temp;

void setToStart()
{
	isInit = false;
}

TokenType getToken()
{
	if (!isInit)
	{
		currentNode = tokenList;
		isInit = true;
	}
	
	if (currentNode != NULL)
	{
		lineNo = currentNode->lineNo;
		strcpy(tokenString, currentNode->tokenString);
		temp = currentNode->type;
		currentNode = currentNode->next;

		return temp;
	}

	return ENDOFFILE;

    /*
	if (tokenList != NULL)
	{
		lineNo = tokenList->lineNo;
		strcpy(tokenString, tokenList->tokenString);
		temp = tokenList->type;
		tokenList = tokenList->next;
		return temp;
	}

	return ENDOFFILE;
	*/
}

/*
// 格式化输出
int space = STEP;

void printSpace(SpacePattern pattern)
{
	putchar('\n');
	if (pattern == SETTOSTART)
	{
		space = STEP;
	}

	if(pattern == KEEP) {}

	if (pattern == INCRESE)
	{
		space += STEP;
	}

	int index;

	for (index = 0; index < space; index++)
	{
		putchar(' ');
	}
}
*/

void printSpace(int space)
{
	int i;

	for(i= 0; i < space; i++)
	{
		printf(" ");
	}
}

void getTypeString(TokenType type, char *typeString)
{
	char string[20];

	switch(type)
	{
		case ENDOFFILE:
			strcpy(string, "ENDOFFILE");
			break;
		case ERROR:
			strcpy(string, "error");
			break;
		case IF:
			strcpy(string, "IF");
			break;
		case ELSE:
			strcpy(string, "ELSE");
			break;
		case WHILE:
			strcpy(string, "WHILE");
			break;
		case READ:
			strcpy(string, "READ");
			break;
		case WRITE:
			strcpy(string, "WRITE");
			break;
		case INTTYPE:
			strcpy(string, "INTTYPE");
			break;
		case REALTYPE:
			strcpy(string, "REALTYPE");
			break;
		case ID:
			strcpy(string, "ID");
			break;
		case POSI_INT:
			strcpy(string, "POSI_INT");
			break;
		case POSI_REAL:
			strcpy(string, "POSI_REAL");
			break;
		case PLUS:
			strcpy(string, "PLUS");
			break;
		case MINUS:
			strcpy(string, "MINUS");
			break;
		case TIMES:
			strcpy(string, "TIMES");
			break;
		case DIV:
			strcpy(string, "DIV");
			break;
		case ASSIGN:
			strcpy(string, "ASSIGN");
			break;
		case LESS:
			strcpy(string, "LESS");
			break;
		case EQUAL:
			strcpy(string, "EQUAL");
			break;
		case UNEQUAL:
			strcpy(string, "UNEQUAL");
			break;
		case LPAREN:
			strcpy(string, "LPAREN");
			break;
		case RPAREN:
			strcpy(string, "RPAREN");
			break;
		case SEMI:
			strcpy(string, "SEMI");
			break;
		case LBRACE:
			strcpy(string, "LBRACE");
			break;
		case RBRACE:
			strcpy(string, "RBRACE");
			break;
		case LBRACKET:
			strcpy(string, "LBRACKET");
			break;
		case RBRACKET:
			strcpy(string, "RBRACKET");
			break;
		default:
			strcpy(string, "UNKNOWN");
	}

	strcpy(typeString, string);
}

// 释放为树分配的内存
void freeTree(TreeNode* root)
{
	if (root != NULL)
	{
		if(root->child[0] != NULL)
		{
		   freeTree(root->child[0]);
		}

	    if(root ->child[1] != NULL)
		{
		    freeTree(root->child[1]);
		}

	    if(root->child[2] != NULL)
		{
		    freeTree(root->child[2]);
		}

	    free(root);
	}
}

// 打印CMM的语法
void printGrammar()
{
	puts("\n----------------------------------------------------------");
	puts("The grammar of CMM:\n");
	puts("<program> ::= <stmt-sequence>");
	puts("<stmt-sequence> ::= <statement>{<statement>}");
	puts("<statement> ::= <declare-stmt> | <assign-stmt> | <repeat-stmt> | <if-stmt> | ");
	puts("<read-stmt> | <write-stmt>");
	puts("");
	puts("<declare-stmt> ::= <type><identifier>['['<arithmetic-exp>']']");
	puts("<type> ::= int | real;");
	puts("");
	puts("<assign-stmt> ::= <identifier> ['[' <arithmetic-exp>']'] = <arithmetic-exp>;");
	puts("");
	puts("<arithmetic-exp> ::= <item>{<addoperator><item>}");
	puts("<item> ::= <factor>{<multoperator><factor>}");
	puts("<factor>::=<identifier>['['<arithmetic-exp>']']|<constant>| '<arithmetic-exp>')'");
	puts("<constant> ::= [+|-](<positive-real> | <positive-integer>)");
	puts("<addoperator> ::= + | -");
	puts("<multioperator> ::= * | /");
	puts("");
	puts("<repeat-stmt> ::= while'('<relation-exp>')''{'<stmt-sequence>'}'");
	puts("<relation-exp> ::= <arithmetic-exp><relation-operator><arithmetic-exp>");
	puts("<relation-operator> ::= < | <> | ==");
	puts("");
	puts("<if-stmt>::= if '('<relation-exp>')''{'<statement>'}' [else'{'< stmt-sequence >'}']");
	puts("");
	puts("<read-stmt> ::= read'('<identifier>['['<arithmetic-exp>']']')'");
	puts("");
	puts("<write-stmt> ::= write'('<arithmetic-exp>')';");
	puts("\n-----------------------------------------------------------");
}
