#include "globals.h"
#include "utils.h"
#include "parse.h"
#include "Accidence.h"
#include "interpret.h"


TreeNode *root;
char tokenString[MAXSTRINGLEN + 1];
int lineNo;
void showHelp();

int main(int argv, char *argc[])
{
	if (argv != 3) {
		showHelp();
		return 1;
	}

	char *actions[] = { "-a", "-s", "-r", "-i", "-g" };

	char *fName = (char *) malloc(20 * sizeof(char));
	char *action = (char *)malloc(20 * sizeof(char));

	strcpy(fName, argc[1]);
	strcpy(action, argc[2]);

	FILE *input;

	if ((input = fopen(fName, "r")) == NULL)
	{
		printf("Can't open file:%s\n", fName);
		exit(1);
	}

	Entity *head = analyze(input);

	if (stricmp(action, actions[0]) == 0) {
		printResult(head);
	}
	else if (stricmp(action, actions[1]) == 0) {
		if (get_errors() == 0)
		{
			root = parse();
			printTreeDriver(root);
		}
		else
		{
			printf("Can't create the syntax tree.\n"
				"Found some errors in addidence analyzing,please correct the program.\n\n");
		}
	}
	else if (stricmp(action, actions[2]) == 0) {
		freeList(head);
		rewind(input);
		head = analyze(input);
		puts("Done!");
	}
	else if (stricmp(action, actions[3]) == 0) {
		if (get_errors() == 0)
		{
			root = parse();
			puts("-------------------------------------------------------");
			puts("The result of the program is as follows:\n");
			interpret(root, RUN);
			puts("-------------------------------------------------------");
		}
		else
		{
			printf("Can't create the syntax tree.\n"
				"Found some errors in addidence analyzing,please correct the program.\n\n");
		}
	}
	else if (stricmp(action, actions[4]) == 0) {
		printGrammar();
	}
	else {
		showHelp();
	}
   	
	fclose(input);
	freeTree(root);
	freeList(head);

	return 0;
}

void showHelp() {
	printf("Usage: CMM filepath option\n"
		"options:\n"
		"\t-g\tprint the grammar of CMM\n"
		"\t-a\tprint the result of the accidence\n"
		"\t-s\tprint syntax tree\n"
		"\t-r\tre-analyse\n"
		"\t-i\tinterpret\n"
		"\t-h\tdisplay help\n\n");
}