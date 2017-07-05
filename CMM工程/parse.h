/*
 file: parse.h
 this program parse the input file and construct a syntax tree;
*/

#ifndef _PARSE_H_
#define _PARSE_H_

#include "globals.h"
#include "utils.h"

void match(TokenType expectedToken);
void error();
//the main function to parse the program
TreeNode *parse();


//<program> ::= {<statement>}
TreeNode * stmtSquence();

TreeNode * statement();

TreeNode * declareStmt();
TreeNode * assignStmt();
TreeNode * repeatStmt();
TreeNode * ifStmt();
TreeNode * readStmt();
TreeNode * writeStmt();

/*
TreeNode * simpleDeclareStmt();
TreeNode * arrayDeclareStmt();
TreeNode * simpleAssignStmt();
TreeNode * arrayAssignStmt();
*/
//arithmetic expression
TreeNode * arithExp();
TreeNode * item();
TreeNode * factor();




TreeNode * relationExp();


#endif