/*
  file: utils.h
  some functions for other part of the program.
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include "globals.h"
#include "Accidence.h"
#define MAXSTRINGLEN 20

//copyString used in parse.cpp when construct the tree.
char * copyString(char *source);
TreeNode * newStmtNode(StmtKind kind);
TreeNode * newExpNode(ExpKind kind);

void setToStart();

void printTreeDriver(TreeNode *root);
void printTree(TreeNode *root);

//the string of the current token




//define in the file scan.cpp
extern char tokenString[MAXSTRINGLEN+1];
//the line number
extern int lineNo;
extern TokenType token;

//get next token
TokenType getToken();



//format the output
#define STEP 4;//using in printSpace
typedef enum
{
	SETTOSTART,KEEP,INCRESE
} SpacePattern;


void printSpace(int space);

void getTypeString(TokenType type,char *typeString);

//free the memory allocated for the tree;
void freeTree(TreeNode *root);

//print the grammar of the programming language
void printGrammar();


#endif

