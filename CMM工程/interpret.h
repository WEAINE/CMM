/*
 file: interpret.h
*/
#ifndef INTERPRET_H
#define INTERPRET_H

#include "globals.h"
#include "utils.h"

typedef enum 
{
	RUN,CHECK
} RunType;

//interpret the syntax tree
void interpret(TreeNode *root,RunType runType);

#endif