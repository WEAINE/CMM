#include "Accidence.h"
//����ԭ��
static void addToList();
static char* nextLine();
static void analyzeLine();
static void appendToString(char ch);
static char nextChar();//get next char in the current line
static void clearString();//clear the string tokenString

int get_errors();

FILE *input = NULL; //the file to be analyzed
int indexAcc;//for nextChar() 
int errors = 0;//the number of the errors
Entity *tokenList = NULL;//point to the head of the list
Entity *tail = NULL;//point to the tail of the list using in addToList 
char currentLine[MAX + 1];//contain current line to be analyzed
bool isComment = false;//a flag mark if current char is in the comment part
char currentTokenString[20];
int currentLineNo = 0;
TokenType currentTokenType;




//create a entity t and set all the pointer to NULL
Entity * newEntity()
{
	Entity *t = (Entity*)malloc(sizeof(Entity));
	t->next = NULL;
	return t;
}


//put new node into the chain table
void addToList()
{
	Entity *t = newEntity();
	t->lineNo = currentLineNo;
	strcpy(t->tokenString,currentTokenString);
	t->type = currentTokenType;

	if(tokenList == NULL)
	{
		tokenList = t;
		tail = t;
	}
	else
	{
		tail->next = t;
		tail = t;
	}
	
}

//analyze the code 
Entity * analyze(FILE *source)
{
	input = source;
	//reset the variables
	currentLineNo = 0;
	errors = 0;
	tokenList = NULL;
	tail = NULL;
	isComment = false;


	char *flag = nextLine();
	while(flag != NULL)
	{
 		analyzeLine();
		flag = nextLine();
	}
	
	return tokenList;
}

void analyzeLine()
{
	char currentChar;
	currentChar = nextChar();

	while(currentChar != ENDOFLINE)
	{
  		clearString();
		//judge whether currentChar is in the comment part
		if(isComment)
		{
			if(currentChar == '*')
			{
				currentChar =nextChar();
				if(currentChar == '/')
				{
					isComment = false;
				}
			}
			currentChar = nextChar();
			continue;
		}
			
		if (currentChar >= 'a' && currentChar <= 'z')
			{
			    char preChar=0;
				
				appendToString(currentChar);
				currentChar = nextChar();
				while (currentChar >= 'a' && currentChar <= 'z'||currentChar == '_' 
					   ||currentChar >= '0' && currentChar <= '9')
				{
					
					appendToString(currentChar);
					preChar = currentChar;
					currentChar = nextChar();
				}
			
				//check if the id is end by '_'
				if (preChar == '_')
				{
					currentTokenType = ERROR;
					addToList();
					
					errors++;
					//flag = false;
					continue;
				}
				//check if the id is a key word
				if(strcmp(currentTokenString,"while") == 0)
				{
					currentTokenType = WHILE;
				}
				else if(strcmp(currentTokenString,"if") == 0)
				{
					currentTokenType = IF;
				}
				else if(strcmp(currentTokenString,"else") == 0)
				{
					currentTokenType = ELSE;
				}
				else if(strcmp(currentTokenString,"read") == 0)
				{
					currentTokenType = READ;
				}
				else if(strcmp(currentTokenString,"write") == 0)
				{
					currentTokenType = WRITE;
				}
				else if(strcmp(currentTokenString,"int") == 0)
				{
					currentTokenType = INTTYPE;
				}
				else if(strcmp(currentTokenString,"real") == 0)
				{
					currentTokenType = REALTYPE;
				}
				else
				{
					currentTokenType = ID;			
				}

				addToList();
				continue;
			}


		//-------------------------------------------------------------------------------------------------------
		//ʵ���������������ֿ�ͷ
			else if (currentChar >= '0' && currentChar <= '9')
			{
				//calculate  the number of the dots
				int dots = 0;
				appendToString(currentChar);
				currentChar = nextChar();
				while (currentChar >= '0' && currentChar <= '9' || currentChar == '.')
				{
					if (currentChar == '.')
					{
						dots++;
					}
					appendToString(currentChar);
					currentChar = nextChar();
				}
				//example:123abc
				if (currentChar >= 'a' && currentChar <= 'z' || currentChar == '_')
				{
					currentTokenType = ERROR;
					errors++;
					appendToString(currentChar);
					currentChar = nextChar();
					while (currentChar >= 'a' && currentChar <= 'z'||currentChar == '_' 
					   ||currentChar >= '0' && currentChar <= '9')
					{
						appendToString(currentChar);
						currentChar = nextChar();
					}					
				    addToList();
					continue;
				}

				if (dots == 0)
				{
					currentTokenType = POSI_INT;
				}
				//֧��12.��ʽ
				else if (dots == 1 )
				{
					currentTokenType = POSI_REAL;
				}
				else 
				{
					currentTokenType = ERROR;
					errors++;
				}
				//����123abc������ʽ�Ĵ���errorsֻ�ɼ�һ��
				addToList();
				continue;

			}
        //-------------------------------------------------------------------------------------------------------
		else if(currentChar == '/')
		{
			char next = nextChar();
			if(next == '*')
			{
				isComment = true;
				currentChar = nextChar();
				continue;
			}

			currentTokenType = DIV;
			appendToString(currentChar);
			
			addToList();
			currentChar = next;
			continue;
		}
		else if(currentChar == '+')
		{
			currentTokenType = PLUS;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == '-')
		{
			currentTokenType = MINUS;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == '*')
		{
			currentTokenType = TIMES;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == '<')
		{
			appendToString(currentChar);
			char next = nextChar();
			if(next == '>')
			{
				currentTokenType = UNEQUAL;
				appendToString(next);
				currentChar = nextChar();
				
			}
			else
			{
				currentTokenType = LESS;
				currentChar = next;
			}		

			addToList();			
			continue;
		}
		else if(currentChar == '=')
		{
			appendToString(currentChar);
			char next = nextChar();
			if(next == '=')
			{
				currentTokenType = EQUAL;
				appendToString(next);
				currentChar = nextChar();
				
			}
			else
			{
				currentTokenType = ASSIGN;
				currentChar = next;
			}		

			addToList();			
			continue;
		}
		else if(currentChar == '(')
		{
			currentTokenType = LPAREN;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == ')')
		{
			currentTokenType = RPAREN;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == ';')
		{
			currentTokenType = SEMI;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}else if(currentChar == '{')
		{
			currentTokenType = LBRACE;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}else if(currentChar == '}')
		{
			currentTokenType = RBRACE;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == '[')
		{
			currentTokenType = LBRACKET;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}
		else if(currentChar == ']')
		{
			currentTokenType = RBRACKET;
			appendToString(currentChar);

			addToList();
			currentChar = nextChar();
			continue;
		}

		else if (currentChar == ' '||currentChar == '\t')
		{
			currentChar = nextChar();
			continue;
		}
		else
		{
			errors++;
			appendToString(currentChar);
			currentTokenType = ERROR;
			addToList();	
			currentChar = nextChar();
			continue;
		}
		

		//todo analyze the line
	}
}


//here begins the definition of the functions
//get next line
char* nextLine()
{
	currentLineNo++;
	indexAcc = 0;
	//in currentLine there is a char '\n' before '\0'
	char *cl = fgets(currentLine,MAX,input);
	return cl;
}

//get next char 
char nextChar()
{
	char ch;
	ch = currentLine[indexAcc];

	//'\0'�����һ����Ч,��Ϊ���һ�п���û�س�
	//��Сд������
	if(isupper(ch))
	{
		ch = tolower(ch);
	}
	if(ch != '\n'&& ch != '\0')
	{
		indexAcc++;
	}
	else 
	{
		ch = ENDOFLINE;
	}

	
	return ch;
}

void appendToString(char ch)
{
	int index = 0;
	while(currentTokenString[index] != '\0')
	{
		index++;
	}
	currentTokenString[index] = ch;
	currentTokenString[index+1] = '\0';
}

void clearString()
{
	currentTokenString[0] = '\0';
}

char typeString[20];

void printResult(Entity *head)
{
	Entity *currentNode = head;
	puts("-----------------------------------------------------------");
	puts("\nThe result is as follows:\n\n");
	printf(" %7s %15s %17s\n\n","LineNo","Token","TokenType");

	while(currentNode != NULL)
	{
		getTypeString(currentNode->type,typeString);
		printf(" Line%5d:%15s%15s\n",currentNode->lineNo,currentNode->tokenString,typeString);
		currentNode = currentNode->next;
	}

	printf("\nFind %d error(s) in the program!\n\n",errors);
	puts("----------------------------------------------------------");
}



//free the memory allocated for the list
void freeList(Entity *head)
{
	if(head->next != NULL)
	{
		freeList(head->next);
	}

	free(head);
}

//return the number of the errors in the accidence analyzing.
int get_errors()
{
	return errors;
}






