#ifndef ACCIDENCE_H
#define ACCIDENCE_H

#include "globals.h"
#include "utils.h"

extern int errors;
extern int get_errors();//get the number of the errors in accidence analyze

//define struct entity
typedef struct entity
{
	int lineNo;
	TokenType type;
	char tokenString[20];

	struct entity *next;
} Entity;

//the list containing all the token-entities.
extern Entity *tokenList;

#define MAX 100
#define ENDOFLINE '\n'

//get the next line in the file and save it in currentLine
Entity* analyze(FILE *input);

void printResult(Entity *head);

//free the memory allocated for the list;
void freeList(Entity *head);



#endif