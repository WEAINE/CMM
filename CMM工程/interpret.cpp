#include "interpret.h"
#include<stdlib.h>
#include<string.h>

//the data structure
//INT REAL
typedef VariableType IDType;

#undef SIZE
#define SIZE 23

#undef MAXSTRINGLEN
#define MAXSTRINGLEN 25

//elements of the hash table, containing the information of a symbol

typedef struct sHashTableNode
{
	struct sHashTableNode *next;


	char name[MAXSTRINGLEN];
	IDType type;
	
	union 
	{
		int intValue;
		double realValue;
	} value;
    //for array identifer
	int arraySize;
	
	int *intArray;
	double *realArray;
} HashTableNode;

//the list of the tables

typedef struct sHashTableList
{
	struct sHashTableList *next;
	HashTableNode *hashTable[SIZE];//???优先级?
} HashTableList;

//design whether run of just check
/*
typedef enum
{
	RUN,CHECK
} RunType;
*/





//function prototype
//the public function
void interpret(TreeNode *root,RunType runType);

//private functions

//interpret statments
static void interDecStmt(TreeNode *node);
static void interAssStmt(TreeNode *node,RunType runType);
static void interIfStmt(TreeNode *node, RunType runType);
static void interRepStmt(TreeNode *node, RunType runType);
static void interReadStmt(TreeNode *node, RunType runType);
static void interWriteStmt(TreeNode *node, RunType runType);

//interpert expressions 

static IDType getExpType(TreeNode *node);
static int getExpValueInt(TreeNode *node);
static double getExpValueReal(TreeNode *node);

//for relational expression

static bool checkCondition(TreeNode *node);

// the operations on the symbol table
static HashTableList* newHashTable(HashTableList *currentHead);
static void insertIDInfo(HashTableList *currentHead, HashTableNode *idInfo);
static HashTableNode * lookup(HashTableList *currentHead, char *name);
//lookup the indentifer in the most inner hash table
static HashTableNode * curLookup(HashTableList *currentHead, char *name);
static void deleteHashTable(HashTableList *currentHead); 


//other support functions

//creat a node of HashTableNode
static HashTableNode * newHTNode();
static bool equalType(IDType type1, IDType type2);
static void typeError(char *message);
//-------------------------------------------------
//arguments:  id tree node IDType in which IDType is a return type
//get id value from the symbol table.
static double getIDValue(char *name, int index);
//if the type is INT, use (int)value
static void setIDValue(char *name,int index,double value);
static IDType getIDType(char *name);
//-------------------------------------------------



//free function
static void freeIDInfoList(HashTableNode *head);
static void freeIDInfoNode(HashTableNode *node);



//hash function

static int hash(char *key);

//the implements are as follows


//hash function

static int hash(char *key)
{
	int hashVal = 0;
	int index = 0;

	for(index = 0; key[index] != '\0'; index++)
	{
		hashVal = (hashVal*37 + key[index])%SIZE;
	}

	return hashVal;
}




//variables
static HashTableList *listHead = NULL; 

//the main function of this file:interpret
void interpret(TreeNode *root,RunType runType)
{
	listHead = newHashTable(listHead);
	while(root != NULL)
	{		
		StmtKind stmtKind = root->kind.stmt;

		switch(stmtKind)
		{
		case declareK:
			interDecStmt(root);
			break;
		case assignK:
			interAssStmt(root,runType);
			break;
		case ifK:
			interIfStmt(root,runType);
			break;
		case whileK:
			interRepStmt(root,runType);
			break;
		case readK:
			interReadStmt(root,runType);
			break;
		case writeK:
			interWriteStmt(root,runType);
			break;
		default:
			break;
		}

		root = root->sibling;		
	}
	//test
	/*
	puts("END\n freeing.....");
	deleteHashTable(listHead);
	puts("Done!");
	*/
	
}


static void interDecStmt(TreeNode *node)
{
	if(curLookup(listHead,node->attribute.name) == NULL)
	{       
		 HashTableNode *temp = newHTNode();
	     strcpy(temp->name,node->attribute.name);
	     temp->type = node->variableType;
         //Array
	    if(node->child[0] != NULL)
		{
		     if(getExpType(node->child[0]) == INT && getExpValueInt(node->child[0]) > 0)
			 {
			     int arraySize = getExpValueInt(node->child[0]);
			     temp->arraySize = arraySize;
			     if(temp->type == INT)
				 {
				     temp->intArray = (int *)malloc(sizeof(int) * arraySize);
				 }
			     else
				 {
				     temp->realArray = (double *)malloc(sizeof(double) * arraySize);
				 }
			 }
		     else
			 {	 char message[50]; 	
			     strcpy(message,node->attribute.name);
			    // char *message = ": array size should opositive integer";
			     strcat(message,": array size should opositive integer");
			     typeError(message);
			 }
		}
	     insertIDInfo(listHead,temp);

	}
	else
	{
		//typeError("Redefined");
		char message[50]; 	
		strcpy(message,node->attribute.name);			    
		strcat(message,": Redefinition");
		typeError(message);
	}
	

}
//modify............................................
static void interAssStmt(TreeNode *node,RunType runType)
{
	if(runType == RUN)
	{
		if(equalType(getIDType(node->attribute.name),getExpType(node->child[1]))
			|| getIDType(node->attribute.name) == REAL)
		{
		    if(node->child[0] != NULL)
			{
			    int index = getExpValueInt(node->child[0]);
			    setIDValue(node->attribute.name,index,getExpValueReal(node->child[1]));
			}
		    else
			{
			    setIDValue(node->attribute.name,-1,getExpValueReal(node->child[1]));
			}
		}
	    else
		{
		    //typeError("assigment:not the same type");
			char message[50]; 	
		    strcpy(message,node->attribute.name);			    
		    strcat(message,": Assignment: Not the same type");
		    typeError(message);
		}

	}
	else
	{
      //just check
		getIDType(node->attribute.name);
		
		if(node->child[0] != NULL)
		{
			//array
			
			//getExpValueInt(node->child[0]);
			getIDValue(node->attribute.name,getExpValueInt(node->child[0]));
		}
		getExpValueReal(node->child[1]);
	}
	
	
}
static void interIfStmt(TreeNode *node, RunType runType)
{
	bool condition = checkCondition(node->child[0]);
	if(condition)
	{
		interpret(node->child[1],RUN);
		if(node->child[2] != NULL)
		{
		    interpret(node->child[2],CHECK);
		}
	}
	else
	{
		interpret(node->child[1],CHECK);
		if(node->child[2] != NULL)
		{
			interpret(node->child[2],RUN);
		}
	}
	
}
static void interRepStmt(TreeNode *node, RunType runType)
{
	bool condition = checkCondition(node->child[0]);
	if(!condition)
	{
		interpret(node->child[1],CHECK);
	}
	else
	{
		while(condition)
		{
		    interpret(node->child[1],RUN);
		    condition = checkCondition(node->child[0]);
		}
	}
	
}
static void interReadStmt(TreeNode *node, RunType runType)
{
	IDType type = getIDType(node->attribute.name);
	switch(runType)
	{
	case RUN:
		{
			if(type == INT)
			{
				int input;
				puts("Please enter an integer:");
				while(scanf("%d",&input) != 1)
				{
					puts("Please enter an integer:");
					//clear the line
					while(getchar() != '\n')
					{
				      continue;
					}
				}
				//an array?
				if(node->child[0] == NULL)
				{
					//not an array
					setIDValue(node->attribute.name,-1,input);
				}
				else
				{
					int index = getExpValueInt(node->child[0]);
					setIDValue(node->attribute.name,index,input);
				}
			}
			else
			{
				double input;
				puts("Please enter a float:");
				while(scanf("%lf",&input) != 1)
				{
					puts("Please enter a float:");
					//clear the line
					while(getchar() != '\n')
					{
				      continue;
					}
				}
				//an array?
				if(node->child[0] == NULL)
				{
					//not an array
					setIDValue(node->attribute.name,-1,input);
				}
				else
				{
					int index = getExpValueInt(node->child[0]);
					setIDValue(node->attribute.name,index,input);
				}
			}
			//clear the line
			while(getchar() != '\n')
			{
				continue;
			}
		}
		break;
	case CHECK:
		{
			if(node->child[0] != NULL)
			{
				//just check
				getIDValue(node->attribute.name,getExpValueInt(node->child[0]));
			}
			else
			{
				getIDValue(node->attribute.name,-1);
			}
		}
		break;
	default:
		break;
	}
}
static void interWriteStmt(TreeNode *node, RunType runType)
{
	//just a test
	switch(runType)
	{
	case RUN:
		{
			if(getExpType(node->child[0]) == INT)
			{
				printf("OUTPUT:%d\n",getExpValueInt(node->child[0]));
			}
			else
			{
				printf("OUTPUT:%0.3f\n",getExpValueReal(node->child[0]));
			}		    
		}		
		break;
	case CHECK:
		//just check if ids have been defined
		getExpType(node->child[0]);
		break;
	}
}



//interpert expressions 

static IDType getExpType(TreeNode *node)
{
	if(node != NULL)
	{
		//IdType temp = INT;
		ExpKind expKind;
		expKind = node->kind.exp;

		switch(expKind)
		{
		case constK:
			{
				if(node->variableType == REAL)
				{
					return REAL;
				}
				else
				{
					return INT;
				}
			}
			break;
		case IDK:
			{
				return getIDType(node->attribute.name);				
			}
			break;
		case opK:
			{
				IDType leftType = getExpType(node->child[0]);
				IDType rightType = getExpType(node->child[1]);
				if(leftType == REAL || rightType == REAL)
				{
					return REAL;
				}
				else
				{
					return INT;
				}
			}
			break;
		default:
			puts("I am sorry,my program have some bugs!");
			exit(1);

		}	
		
	}
	else
	{
		puts("I am sorry,my program have some bugs!");
		exit(2);
	}	
}
static int getExpValueInt(TreeNode *node)
{
	return (int)getExpValueReal(node);
}
static double getExpValueReal(TreeNode *node)
{
	double expValue = 0.0;
	if(node != NULL )
	{
		ExpKind expKind;
		expKind = node->kind.exp;
		switch(expKind)
		{
		case constK:
			if(node->variableType == INT)
			{
				expValue = node->attribute .int_value;
			}
			else
			{
				expValue = node->attribute.real_value;
			}
			break;
		case opK:
			{
			//PLUS,MINUS,TIMES,DIV
			    TokenType opType = node->attribute.tokenType;
			    switch(opType)
				{
			    case PLUS:
				    expValue = getExpValueReal(node->child[0]) + getExpValueReal(node->child[1]);
				    break;
			    case MINUS:
				    expValue = getExpValueReal(node->child[0]) - getExpValueReal(node->child[1]);
				    break;
			    case TIMES:
				    expValue = getExpValueReal(node->child[0]) * getExpValueReal(node->child[1]);
				    break;
			    case DIV:
				    expValue = getExpValueReal(node->child[0]) / getExpValueReal(node->child[1]);
				    break;
			    default:
				    puts("I am sorry,my program have some bugs!");
					exit(3);
				    break;
			}
			break;
			}
		case IDK:
			{
				int index = -1;
				if(node->child[0] == NULL)
				{
					expValue = getIDValue(node->attribute.name,index);
				}
				else
				{
					IDType indexType = getExpType(node->child[0]);
					if(indexType != INT)
					{
						//char *message = *(node->attribute.name) + " :array index is not a interger";
						char message[50]; 	
		                strcpy(message,node->attribute.name);			    
		                strcat(message,":array index is not a interger");
		                typeError(message);
						
					}
					else
					{
						index = getExpValueInt(node->child[0]);
						expValue = getIDValue(node->attribute.name,index);
					}
				}
				
			}
		
			break;
		default:
			puts("I am sorry,my program have some bugs.");
			exit(4);
			break;

			
		}

	}
	else
	{
		puts("I am sorry,my program have some bugs.");
		exit(5);
	}
	return expValue;	
	
}

//for relational expression

static bool checkCondition(TreeNode *node)
{
	//LESS,EQUAL,UNEQUAL
	TokenType relOpType = node->attribute.tokenType;
	IDType leftExpType = getExpType(node->child[0]);
	IDType rightExpType = getExpType(node->child[1]);

	switch(relOpType)
	{
	case LESS:
		return getExpValueReal(node->child[0]) < getExpValueReal(node->child[1]);
		break;
	case EQUAL:
		{
			if(leftExpType == rightExpType)
			{
				if(leftExpType == INT)
				{
					return getExpValueInt(node->child[0]) == getExpValueInt(node->child[1]);
				}
				else
				{
					return getExpValueReal(node->child[0]) == getExpValueReal(node->child[1]);
				}
			}
			else
			{
				return false;
			}
			
		}
		break;
	case UNEQUAL:
		{
			if(leftExpType == rightExpType)
			{
				if(leftExpType == INT)
				{
					return getExpValueInt(node->child[0]) != getExpValueInt(node->child[1]);
				}
				else
				{
					return getExpValueReal(node->child[0]) != getExpValueReal(node->child[1]);
				}
			}
			else
			{
				return true;
			}
			
		}
		break;
	default:
		puts("I am sorry,my program have some bugs.");
		exit(5);
		break;

	}
	
}

// the operations on the symbol table

static HashTableList* newHashTable(HashTableList *currentHead)
{
	HashTableList *temp = (HashTableList *)malloc(sizeof(HashTableList));	
	
	if(temp != NULL)
	{
		//initial the hash table
	    int index;
	    for(index = 0; index < SIZE; index++)
		{
		    temp->hashTable[index] = NULL;
		}
	    temp->next = NULL;
        //Add the hash table to the list
		if(currentHead == NULL)
		{
			//set the new hash table as the head of the list
			currentHead = temp;
		}
		else
		{
			//add the hashTable to the head of the list.
			temp->next = currentHead;
			currentHead = temp;
		}

		return currentHead;
	}
	else
	{
		puts("Failed to allocate the memory!");
		exit(1);
	}

}
//to be test
static void insertIDInfo(HashTableList *currentHead, HashTableNode *idInfo)
{
	int hashVal = hash(idInfo->name);
	HashTableNode *temp = currentHead->hashTable[hashVal];
	if(temp == NULL)
	{
		currentHead->hashTable[hashVal] = idInfo;
	}
	else
	{
		idInfo->next = temp;
		currentHead->hashTable[hashVal] = idInfo;
	}
}
//to be test
static HashTableNode * lookup(HashTableList *currentHead, char *name)
{
	int hashVal = hash(name);
	HashTableNode *temp = NULL;
	while(currentHead != NULL)
	{
		temp = currentHead->hashTable[hashVal];
	    while(temp != NULL)
		{
		    if(strcmp(temp->name,name) == 0)
			{
			    return temp;
			}
		    temp = temp->next;
		}

		currentHead = currentHead->next;

	}
	return temp;
	
}
static void deleteHashTable(HashTableList *currentHead)
{
	if(currentHead != NULL)
	{
		HashTableList *oldNode = currentHead;
		//.............................................
		//currentHead = currentHead->next;
		//应该修改全局变量指针listHead
		listHead = currentHead->next;
		//free the memory
		int index;
		for(index = 0; index < SIZE; index++)
		{
			freeIDInfoList(oldNode->hashTable[index]);			
		}
		free(oldNode);
	}
}

//free function
//to be test
static void freeIDInfoList(HashTableNode *head)
{
	if(head !=NULL)
	{
		if(head->next != NULL)
		{
		    freeIDInfoList(head->next);
		}
	    freeIDInfoNode(head);
	}
	
}
static void freeIDInfoNode(HashTableNode *node)
{
	//free array and then free the node
	if(node->arraySize == 0)
	{
		free(node);
	}
	else
	{
		//free the array
		if(node->type == INT)
		{
			free(node->intArray);			
		}
		else
		{
			free(node->realArray);			
		}
		//free the node
		free(node);
	}
}



//other support functions

//creat a node of HashTableNode
static HashTableNode * newHTNode()
{
	//allocate memory and initial the node
	HashTableNode *temp = (HashTableNode *)malloc(sizeof(HashTableNode));
	if(temp != NULL)
	{
		//initial the node
		temp->next = NULL;
	    temp->intArray = NULL;
	    temp->realArray = NULL;
		temp->arraySize = 0;
		return temp;

	}
	else
	{
		puts("Failed to allocate the memory!");
		exit(1);
	}
}

static bool equalType(IDType type1, IDType type2)
{
	return type1 == type2;
}
static void typeError(char *message)
{
	printf("Type error: %s\n",message);
	getchar();
	exit(0);
}


//----------------------------------------------------------
//note:be careful about the tree structure
static double getIDValue(char *name,int index)
{
	HashTableNode *temp = lookup(listHead,name);
	double value;
	//check if the identifer has been defined
	if(temp != NULL)
	{
		if(index == -1 && temp->arraySize != 0)
		{
			//char *message = *name + " is an array, lack of array index.";
			//typeError(message);
			char message[50];
		    strcpy(message,name);
		    strcat(message,": is an array, lack of array index.");
		    typeError(message);
		}
		else if(index != -1 && temp->arraySize == 0)
		{
			//char *message = *name + " is not an array, should not use index on the indentifer";
			char message[50];
		    strcpy(message,name);
		    strcat(message,": not an array, should not use index on the indentifer");
		    typeError(message);
		}
		else if(index == -1 && temp->arraySize == 0)
		{
			//is not an array
			if(temp ->type == INT)
			{
				value = temp->value.intValue;
				return value;
			}
			else
			{
				value = temp->value.realValue;
				return value;
			}
		}
		else
		{
			//is an array. 
			// no checking on index 
			if(temp ->type == INT)
			{
				value = temp->intArray[index];
				return value;
			}
			else
			{
				value = temp->realArray[index];
				return value;
			}
		}
		
	}
	else
	{
		//char *message = "Undefined indentifer: " + *(name);
		char message[50];
		strcpy(message,name);
		strcat(message,":Undefined indentifer");
		typeError(message);
	}
}

//if the type is INT , use (int)value
static void setIDValue(char *name,int index,double value)
{
	HashTableNode *temp = lookup(listHead,name);

	//check if the identifer has been defined
	if(temp != NULL)
	{
		if(index == -1 && temp->arraySize != 0)
		{
			char *onestring = " is an array, lack of array index.";
			char *message = strcat(name,onestring);
			typeError(message);
		}
		else if(index != -1 && temp->arraySize == 0)
		{
			char message[50];
		    strcpy(message,name);
		    strcat(message,": not an array, should not use index on the indentifer");
		    typeError(message);
			//char *message = *name + " is not an array, should not use index on the indentifer";
		}
		else if(index == -1 && temp->arraySize == 0)
		{
			//is not an array
			if(temp ->type == INT)
			{
				temp->value.intValue = (int)value;				
			}
			else
			{
				temp ->value.realValue = value;
			}
		}
		else
		{
			//is an array. 
			// no checking on index 
			if(temp ->type == INT)
			{
				 temp->intArray[index] = (int)value;				
			}
			else
			{
			    temp->realArray[index] = value;				
			}
		}
		
	}
	else
	{
		//char *message = "Undefined indentifer: " + *(name);
		//typeError(message);
		char message[50];
		strcpy(message,name);
		strcat(message,":Undefined indentifer");
		typeError(message);
	}
	
}


static IDType getIDType(char *name)
{
	//listHead 为全局变量,好像不是很好
	HashTableNode *temp = lookup(listHead,name);

	//check whether the indentifer has been declared.
	if(temp != NULL)
	{
		IDType tempType = temp->type;
		if(tempType == INT)
		{
			return INT;
		}
		else
		{
			return REAL;
		}

	}
	else
	{
			//char *message = "Undefined indentifer: ";
			//typeError(message);
		char message[50];
		strcpy(message,name);
		strcat(message,":Undefined indentifer");
		typeError(message);
	}

}

//for check redefinition
static HashTableNode * curLookup(HashTableList *currentHead, char *name)
{
	int hashVal = hash(name);
	HashTableNode *temp = NULL;
	temp = currentHead->hashTable[hashVal];
	while(temp != NULL)
	{
	   if(strcmp(temp->name,name) == 0)
		{
			  return temp;
		}
		temp = temp->next;
	}
	
	return temp;
}



