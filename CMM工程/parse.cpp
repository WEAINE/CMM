#include "parse.h"

TokenType token;


void match(TokenType expectedToken)
{
	if(token == expectedToken)
	{
		token = getToken();
	}
	else
	{
		error();
	}
}
void error()
{
	if(token == ENDOFFILE)
	{
		strcpy(tokenString,"ENDOFFILE");
	}
	printf("Error in line: %d \nUnexpected token: \"%s\"\n",lineNo,tokenString);
	printf("The syntax is not true!\n");
	
	printf("Press [Enter] key to exit...");
	getchar();
	exit(0);
}
//the main function to parse the program
TreeNode *parse()
{
	//get the first token.
	TreeNode *t;
	setToStart();
	token = getToken();
	if(token != ENDOFFILE)
	{
		t = stmtSquence();
	}
	else
	{
		error();
	}
	return t;

}


//<program> ::= {<statement>}
TreeNode * stmtSquence()
{
	TreeNode *t;

	t = statement();

	TreeNode *temp;
	temp = t;
    //若为} 则可认为是内部stmtSquence的结束
	while(token != ENDOFFILE && token != RBRACE)
	{
		temp->sibling = statement();
		temp = temp->sibling;
	}

	return t;
	
}

TreeNode * statement()
{
	TreeNode *t;
	switch(token)
	{
	case INTTYPE:		
	case REALTYPE:
		t = declareStmt();
		break;
	case ID:
		t = assignStmt();
		break;
	case WHILE:
		t = repeatStmt();
		break;
	case IF:
		t = ifStmt();
		break;
	case READ:
		t = readStmt();
		break;
	case WRITE:
		t = writeStmt();
		break;
	default:
		error();
	}
	return t;
}

//<declare-stmt> ::= <type><identifier>['['<arithmetic-exp>']']
TreeNode * declareStmt()
{
	TreeNode *t = newStmtNode(declareK);
	if (token == INTTYPE)
	{
		t->variableType = INT;
	}
	else if(token == REALTYPE)
	{
		t->variableType = REAL;
	}
	else 
	{
		error();
	}
	match(token);
	
	t->attribute.name = copyString(tokenString);
	match(ID);
	//array declare statement
	if(token == LBRACKET)
	{
		//array declare kind
		t->simpleOrArray.declare = arrayDK;
		match(LBRACKET);
		t->child[0] = arithExp();
		match(RBRACKET);
	}
	else
	{
		t->simpleOrArray.declare = simpleDK;
	}
	match(SEMI);

	return t;

}

//<assign-stmt> ::= <identifier> ['[' <arithmetic-exp>']'] = <arithmetic-exp>;
TreeNode * assignStmt()
{
	TreeNode *t = newStmtNode(assignK);
	t->attribute.name = copyString(tokenString);
	match(ID);

	if (token == LBRACKET)
	{
		t->simpleOrArray.assign = arrayAK;
		match(LBRACKET);
		t->child[0] = arithExp();
		match(RBRACKET);
	}
	else
	{
		t->simpleOrArray.assign = simpleAK;
	}
	match(ASSIGN);
	t->child[1] = arithExp();
	match(SEMI);
	
	return t;
}

//<if-stmt>::= if '('<relation-exp>')''{'<statement>'}' [else'{'< stmt-sequence >'}']
TreeNode * ifStmt()
{
	TreeNode * t = newStmtNode(ifK);
	match(IF);
	match(LPAREN);
	t->child[0] = relationExp();
	match(RPAREN);
	match(LBRACE);
	t->child[1] = stmtSquence();
	match(RBRACE);
	if(token == ELSE)
	{
		match(ELSE);
		match(LBRACE);
		t->child[2] = stmtSquence();
		match(RBRACE);
	}
	return t;
}

//<repeat-stmt> ::= while'('<relation-exp>')''{'<stmt-sequence>'}'
TreeNode * repeatStmt()
{
	TreeNode *t = newStmtNode(whileK);
	match(WHILE);
	match(LPAREN);
	t->child[0] = relationExp();
	match(RPAREN);
	match(LBRACE);
	t->child[1] = stmtSquence();
	match(RBRACE);

	return t;
}
//<read-stmt> ::= read'('<identifier>')'
TreeNode * readStmt()
{
	TreeNode *t = newStmtNode(readK);
	match(READ);
	match(LPAREN);
	t->attribute.name = copyString(tokenString);
	match(ID);
	//---------------------------------------------------------------------
	//TODO!
	if(token == LBRACKET)
	{
		match(token);
		t->child[0] = arithExp();
		match(RBRACKET);
	}
	//--------------------------------------------------------------------
	match(RPAREN);
	match(SEMI);

	return t;
}
//<write-stmt> ::= write'('<arithmetic-exp>')';
TreeNode * writeStmt()
{
	TreeNode *t = newStmtNode(writeK);
	match(WRITE);
	match(LPAREN);
	t->child[0] = arithExp();
	match(RPAREN);
	match(SEMI);

	return t;
}

/*
TreeNode * simpleDeclareStmt()
{}
TreeNode * arrayDeclareStmt()
{}
TreeNode * simpleAssignStmt()
{}
TreeNode * arrayAssignStmt()
{}
*/
//arithmetic expression
/*
   <arithmetic-exp> ::= <item>{<addoperator><item>}
   <item> ::= <factor>{<multoperator><factor>}
   <factor> ::= <identifier> | <constant> | '('<arithmetic-exp>')'
   <constant> ::= [+|-](<positive-real> | <positive-integer>)
   <addoperator> ::= + | -
   <multioperator> ::= * | /

*/
TreeNode * arithExp()
{
	TreeNode *t;
	t = item();
	while(token == PLUS || token == MINUS)
	{
		TreeNode *t1 = newExpNode(opK);
		t1->opKind = arithK;
		t1->attribute.tokenType = token;
		match(token);
		t1->child[0] = t;
		t1->child[1] = item();

		t = t1;
	}

	return t;
}
/*
   <item> ::= <factor>{<multoperator><factor>}
   <factor> ::= <identifier> | <constant> | '('<arithmetic-exp>')'
   <constant> ::= [+|-](<positive-real> | <positive-integer>)
   <addoperator> ::= + | -
   <multioperator> ::= * | /
*/
TreeNode * item()
{
	TreeNode *t;
	t = factor();
	while(token == TIMES || token == DIV)
	{
		TreeNode *t1 = newExpNode(opK);
		t1->attribute.tokenType = token;
		match(token);
		t1->child[0] =t;
		t1->child[1] = factor();

		t = t1;
	}
	return t;
}
/*
  <factor> ::= <identifier> | <constant> | '('<arithmetic-exp>')'
   <constant> ::= [+|-](<positive-real> | <positive-integer>)
  
*/
TreeNode * factor()
{
	TreeNode *t;
	if (token == ID)
	{
		t = newExpNode(IDK);
		t->attribute.name = copyString(tokenString);
		match(ID);
		//------------------------------------------
        if(token == LBRACKET)
		{
			match(token);
			t->child[0] = arithExp();
			match(RBRACKET);
		}
		//------------------------------------------
	}
	else if(token == LPAREN )
	{
		match(token);
		t = arithExp();
		match(RPAREN);
	}
	else if(token == PLUS || token == MINUS)
	{
		bool isPLUS = false;
		if( token == PLUS)
		{
			isPLUS = true;
		}
		match(token);
		t = newExpNode(constK);
		if (token == POSI_INT)
		{
			t->variableType = INT;
			if(isPLUS)
			{
				(t->attribute).int_value = atoi(tokenString);
			}
			else
			{
				t->attribute.int_value = 0 - atoi(tokenString);
			}
		
			match(token);
		}
		else if(token == POSI_REAL)
		{
			t->variableType = REAL;
			if(isPLUS)
			{
				t->attribute.real_value = atof(tokenString);
			}
			else
			{
				t->attribute.real_value = 0 - atof(tokenString);
			}
			
			match(token);
		}
		else 
		{
			error();
		}
	}
	else if(token == POSI_INT || token == POSI_REAL)
	{
		t = newExpNode(constK);
		if(token == POSI_INT)
		{
			t->variableType = INT;
			(t->attribute).int_value = atoi(tokenString);
			match(POSI_INT);
		}
		else
		{
			t->variableType = REAL;
			t->attribute.real_value = atof(tokenString);
			match(POSI_REAL);
		}
	}
	else
	{
		error();
	}

	return t;
}



/*
    <relation-exp> ::= <arithmetic-exp><relation-operator><arithmetic-exp>
    <relation-operator> ::= < | <> | = =
	LESS,EQUAL,UNEQUAL
*/
TreeNode * relationExp()
{
	TreeNode *t = newExpNode(opK);
	t->opKind = relationK;
	t->child[0] = arithExp();
	if(token == LESS || token == EQUAL || token == UNEQUAL)
	{
		t->attribute.tokenType = token;
		match(token);
	}
	else
	{
		error();
	}
	t->child[1] = arithExp();

	return t;
}



