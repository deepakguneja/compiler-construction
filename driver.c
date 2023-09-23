//Deepak Guneja
//2015A7PS0083P

#include "codeGenerator.c"

int main(int args, char* argv[]) {

	int option;

	printf("\n\nLevel 4: SymbolTable/TypeChecking/SemanticAnalyzer/IntermediateCodeGeneration/CodeGeneration modules work.\n\n");
	printf("\nPress:\n");
	printf("1 - For printing the token list\n");
	printf("2 - For parsing\n");
	printf("3 - For printing Abstract Syntax Tree\n");
	printf("4 - For Displaying amount of allocated memory\n");
	printf("5 - For printing the Symbol Table\n");
	printf("6 - For printing All Errors\n");
	printf("7 - For producing Assembly Code\n");
	printf("0 - For Exit!\n");

	parseTree* tree = NULL;
	tokenHead* allTkns = NULL;
	hashNode* hashTable = NULL;
	Grammar G = NULL;
	ASTnode* ast = NULL;
	symTable st = NULL;

	lErr = (lexerErrHead*)malloc(sizeof(lexerErrHead));
	lErr->first = NULL;
	lErr->last = NULL;

	pErr = (parserErrHead*)malloc(sizeof(parserErrHead));
	pErr->first = NULL;
	pErr->last = NULL;

	symTableErr = (semanticErrHead*)malloc(sizeof(semanticErrHead));
	symTableErr->first = NULL;
	symTableErr->last = NULL;

	int prevOffset = 0;

	scanf("%d",&option);

	while(option != 0)
	{
		if (option == 1)
		{
			if(allTkns==NULL)
			{
				char keyWrd[20] = "allKeywords.txt";    //first line will tell how many keywords
				//each keyword in single line in text file
				hashTable = createHashTable(keyWrd);
				allTkns = tokenList(argv[1], hashTable);
				//printHashTable(hashTable);
				free(hashTable);
				// freeTkn(tkns);
			}
			printf("line_number   name_of_the_lexeme   Token_name\n");
			printTokenList(allTkns);
			// printLexerErrs();
		}
		else if (option == 2)
		{
			if(allTkns==NULL)
			{
				char keyWrd[20] = "allKeywords.txt";    //first line will tell how many keywords
				//each keyword in single line in text file
				hashTable = createHashTable(keyWrd);
				allTkns = tokenList(argv[1], hashTable);
				//printHashTable(hashTable);
				// printTokenList(allTkns);
				free(hashTable);
				// freeTkn(tkns);
			}

			if(G==NULL)
			{
				char GrammarFile[20] = "Grammar.txt";
				Grammar G = readGrammar(GrammarFile);
				FirstAndFollowSets firstFollow = computeFirstAndFollow(G);
				// printGrammar(G);
				// printRules(G);
				// printFirstFollow(firstFollow, G);

				parseTable T = createParseTable(firstFollow, G);
				noOfParseTreeNodes = 0;
				tree = createParseTree(allTkns, T, G);
			// printParseTable(T);
			}
			printParseTreeOnStdOut(tree);
			printf("\nLexical and Syntactic Errors: \n");
			printSyntacticErrors();  //defined in parser.c

		}
		else if (option == 3)
		{
			if(lexicalErrExist != 1 && syntacticErrExist != 1)
			{
				if (ast == NULL)
				{
					if (tree == NULL)
					{
						printf("\nFirst create Parse Tree\n");
					}
					else
					{
						createAST(tree);
						ast = tree->addr;
						printf("\nAbstract Syntax Tree has been created successfully.\n");
						// freeParseTree(tree);
						// free(tree);
						// tree = NULL;
					}
				}
					
				if (ast != NULL)
				{
					makeParentPtrInAST(ast);
					printf("\nPrinting by In-Order Traversal -> Order of Printing: Child1 then Parent and then Rest Children\n\n");
               	 	printf("(Dummy Node): %s   Parent: NULL\n", enumToString2(ast->sym));
					printASTInOrder(ast);
				}
			}
			else
			{
				printf("\nAs syntactically not correct, so can not create Abstract Syntax Tree!\n");
			}
		}
		else if (option == 4)
		{
			if(lexicalErrExist != 1 && syntacticErrExist != 1)
			{
				if(ast != NULL)
				{
					int memParseTree = noOfParseTreeNodes*sizeof(parseTree);
					int memAST = noOfASTnodes*sizeof(ASTnode);
					double percentCompression = (double)(memParseTree - memAST)/(double)memParseTree;
					percentCompression *= 100;

					printf("\nParse Tree Number of nodes = %d   Allocated Memory = %d Bytes\n", noOfParseTreeNodes, memParseTree);
					printf("AST Number of nodes = %d   Allocated Memory = %d Bytes\n", noOfASTnodes, memAST);
					printf("Compression Percentage = %lf\n", percentCompression);
				}
				else
				{
					printf("\nCreate Abstract Syntax Tree first!\n");
				}
			}
			else
			{
				printf("\nAs syntactically not correct, so Abstract Syntax Tree can not be created!\n");
			}

		}
		else if (option == 5)
		{
			if(lexicalErrExist != 1 && syntacticErrExist != 1)
			{
				if (ast != NULL)
				{
					st = createSymTable();
					symTableStack* stStack = (symTableStack*)malloc(sizeof(symTableStack));
					stStack->first = NULL;
					char* functionName = (char*)malloc(20*sizeof(char));
					strcpy(functionName, "_main");
					symTableStackNode* currStackNd = createSymTableStackNode(st, functionName);
					pushSymTableStackNode(stStack, currStackNd);
					ASTnode* child = ast->children;
					//prevOffset defined above
					prevOffset = 0;
					while(child!=NULL)
					{
						prevOffset = initSymTables(child, st, stStack, prevOffset);
						child = child->next;
					}
					printf("\nIf matrix or string not initialised then offset written is -1.\n");
					printf("\nId                    scope               nestingLevel  nameOfStaticParent       type            width        offset\n");
					printAllSymTables(st, 1, NULL);
					checkOutputParamAssignedVal(st);	//to check all output parameters are assigned initialised
					// printSemanticErrs();
				}
				else
				{
					printf("\nCreate Abstract Syntax Tree first!\n");					
				}
			}
			else
			{
				printf("\nAs syntactically not correct, so Abstract Syntax Tree and Symbol Table can not be created!\n");
			}
		}
		else if (option == 6)
		{
			if(lexicalErrExist == 1 || syntacticErrExist == 1)
			{
				if (tree != NULL)
				{
					printf("\nAll Syntactic or Lexical Erros follow: \n");
					printSyntacticErrors();
				}
				else
				{
					printf("\nFirst create Parse Tree\n");
				}
			}
			else if(semanticErrExist == 1)
			{
				if (ast != NULL && st != NULL)
				{
					printf("\nAll Semantic Errors follow:\n");
					printSemanticErrs();
				}
				else
				{
					printf("\nFirst create AST and Symbol Table!\n");
				}
			}
			else
			{
				if(ast != NULL && st != NULL)
				{
					printf("\nCode Compiles Successfully!\n");
				}
				else
				{		
					printf("\nFirst create AST and Symbol Table!\n");
				}
			}
		}
		else if(option == 7)
		{
			if(lexicalErrExist != 1 && syntacticErrExist != 1 && semanticErrExist != 1)	//no errors exist only then
			{
				tempVarNo = 1;
				labelNo = 1;
				offsetForIntCode = prevOffset;
				if (ast != NULL && st != NULL)
				{
					ASTnode* child = ast->children;
					intCode* prevCode;
					intCode* headCode;
					int flagIntCode = 0;
					
					while(child != NULL)
					{
						if (!flagIntCode)
						{
							if (child->sym == IOREAD || child->sym == IOPRINT || child->sym == IFSTMT || child->sym == ASSIGN_SINGLE_VAR || child->sym == ASSIGN_LIST_VAR)
							{
								if (child->sym == IFSTMT)
								{
									child->nextLabel = createTempLabelASTnode();
								}
								intCodeGen(child, st);
								headCode = child->code;
								intCode* tempCode = child->code;
								while(tempCode->next != NULL)
								{
									tempCode = tempCode->next;
								}
								prevCode = tempCode;
								flagIntCode = 1;
							}
						}
						else
						{
							if (child->sym == IOREAD || child->sym == IOPRINT || child->sym == IFSTMT || child->sym == ASSIGN_SINGLE_VAR || child->sym == ASSIGN_LIST_VAR)
							{
								if (child->sym == IFSTMT)
								{
									child->nextLabel = createTempLabelASTnode();
								}
								intCodeGen(child, st);
								prevCode->next = child->code;
								intCode* tempCode = child->code;
								while(tempCode->next != NULL)
								{
									tempCode = tempCode->next;
								}
								prevCode = tempCode;
							}
						}
						child = child->next;
					}
					prevCode->next = NULL;
					ast->code = headCode;
					printf("\nNote that Real Number can not be run on NASM, so Do Not give such test cases!\n\n");
					printf("Matrix would be printed in Row-Major Format.\n\n");
					printQuadruples(ast);
					// printAllSymTables(st, 1, NULL);
					FILE* fp;
					fp = fopen(argv[2], "w");
					if(fp==NULL)
					{
						printf("Give the correct path for .asm file!\n");
						return 0;
					}
					initializeCodeGen(st, ast->code, fp);
				}
				else
				{
					printf("\nFirst create AST and Symbol Table!\n");
				}

			}
			else
			{
				printf("\nAssembly Code can not be generated because of errors in the program!\n");
			}
			return 0;
		}

		printf("\nPress:\n");
		printf("1 - For printing the token list\n");
		printf("2 - For parsing\n");
		printf("3 - For printing Abstract Syntax Tree\n");
		printf("4 - For Displaying amount of allocated memory\n");
		printf("5 - For printing the Symbol Table\n");
		printf("6 - For printing All Errors\n");
		printf("7 - For producing Assembly Code\n");
		printf("0 - For Exit!\n");
		scanf("%d", &option);
	}

return 0;
}
