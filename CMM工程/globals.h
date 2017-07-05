/*
file: globals.h
globals variables and types
must come before other files
*/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAXCHILDLEN 3


//the kind of token
typedef enum
{
	//book keeping 
	ENDOFFILE,ERROR,
	//reserved words
	IF,ELSE,WHILE,READ,WRITE,INTTYPE,REALTYPE,
    //multicharacter tokens
	ID,POSI_INT,POSI_REAL,
	//special symbols
	PLUS,MINUS,TIMES,DIV,ASSIGN,
	LESS,EQUAL,UNEQUAL,
	LPAREN,RPAREN,SEMI,//semicolon
	LBRACE,RBRACE,LBRACKET,RBRACKET
} TokenType;

//the kind of the tree node
typedef enum
{
	stmtK,expK
} NodeKind;

//the kind of the statement
typedef enum
{
	declareK,assignK,ifK,whileK,writeK,readK
} StmtKind;

//the kind of the declare statement
typedef enum
{
	arrayDK,simpleDK
} DeclareKind;

//the kind of the assign statement
typedef enum
{
	arrayAK,simpleAK
} AssignKind;

//the kind of the express statement
typedef enum
{
	opK,constK,IDK
} ExpKind;

//the kind of the operate express kind
typedef enum
{
	arithK,relationK
} OpKind;

//just for declare statement.
typedef enum
{
	INT,REAL
} VariableType;



struct treeNode
{
	struct treeNode *child[MAXCHILDLEN];
	struct treeNode *sibling;

    //the kind of the node:stmt or expression?
	NodeKind nodeKind;
	union 
	{
		StmtKind stmt;
		ExpKind exp;
	} kind;

	//for assign and declare statement
	union 
	{
		DeclareKind declare;
		AssignKind assign;
	} simpleOrArray;

	//for operation expression:arithK or relationK?
	OpKind opKind;

	//just for declare statement
   	VariableType variableType;
	union 
	{
		TokenType tokenType;
		int int_value;
		double real_value;
		char *name;
	} attribute;


};

typedef struct treeNode TreeNode;

//the syntax tree
extern TreeNode *root;




#endif
