//Deepak Guneja
//2015A7PS0083P

#include "intermediateCodeGen.h"

void intCodeGen(ASTnode* ast, symTable st)
{	
	ASTnode* currAST;

	if (ast->sym == IOREAD)	
	{
		field* fld = (field*)ast->children->symTablePtr;
		if (fld != NULL && fld->t == integer)	//Only Integer can be read
		{
			intCode* code = createQuadruple(ast->children, READOP, NULL, NULL);
			ast->code = code;
			ast->addr = ast->children;
		}
	}

	else if (ast->sym == IOPRINT)		//If IOPRINT, then every id is fine
	{
		field* fld = (field*)ast->children->symTablePtr;
		if (fld != NULL)
		{
			if (fld->t == string || fld->t == integer)
			{
				intCode* code = createQuadruple(ast->children, PRINTOP, NULL, NULL);	//string and integer can be directly printed
				ast->code = code;
				ast->addr = ast->children;
			}
			else if (fld->t == matrix)
			{
				int rows = fld->idPtr->sz[0];
				int cols = fld->idPtr->sz[1];

				int matrixOffset = fld->idPtr->offset;
				intCode* prevCode;
				intCode* headCode;
				for (int i = 0; i < rows; ++i)
				{
					for (int j = 0; j < cols; ++j)
					{
						/* code */
						int elemOffset = calculateMatrixElemOffset(matrixOffset, rows, cols, i+1, j+1);
						ASTnode* offsetAST = createASTNodeForValue(elemOffset);	//Value AST
						// printf("%d\n", elemOffset);
						// printf("%d\n", offsetAST->tkn->value.intVal);
						int sz[2];
						sz[0] = 2;
						sz[1] = 0;
						ASTnode* temp1 = createTempASTNode(sz, integer, st);
						offsetForIntCode += 2;
						intCode* temp1Code = createQuadruple(temp1,  ADDRESSOFFSET, offsetAST, NULL);	//sending first ROW, Dummy Node
						sz[0] = 2;
						ASTnode* temp2 = createTempASTNode(sz, integer, st);
						offsetForIntCode += 2;
						intCode* temp2Code = createQuadruple(temp2, VALUEAT, temp1, NULL);
						intCode* temp3Code = createQuadruple(temp2, PRINTOP, NULL, NULL);

						temp1Code->next = temp2Code;
						temp2Code->next = temp3Code;
						if (i == 0 && j == 0)	//i.e. first element is headnode
						{
							headCode = temp1Code;
							prevCode = temp3Code;
						}
						else
						{	
							prevCode->next = temp1Code;
							prevCode = temp3Code;
						}
					}
				}
				ast->code = headCode;
				ast->addr = ast->children;
			}
		}
	}

	else if (ast->sym == IFSTMT)
	{
		// ast->nextLabel = createTempLabelASTnode();	//S.next
		ASTnode* SNext = ast->nextLabel;
		// printf("%s\n", SNext->tkn->lexeme);
		currAST = ast->children;	//boolExprsn
		ASTnode* boolAST = currAST;
		currAST->trueLabel = createTempLabelASTnode();	//B.true

		if (ast->children->next->next != NULL)	//If ELSE exists
		{
			currAST->falseLabel = createTempLabelASTnode();	//B.false
		}
		else
		{
			boolAST->falseLabel = SNext;
		}

		ASTnode* trueLabel = currAST->trueLabel;
		ASTnode* falseLabel = currAST->falseLabel;

		checkBoolExprsnIntCode(currAST, st);	
		intCode* boolCode = currAST->code;	//B.code

		currAST = currAST->next;	//currAST is STMTS
		ASTnode* child = currAST->children;	//stmts starts

		intCode* prevCode;
		intCode* headCode;
		int flagIntCode = 0;

/*---------------------------------------------S1 starts--------------------------*/		
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
					child->nextLabel = SNext;
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
		//Right now S1.code = headCode and S1.next is prevCode->next
		intCode* S1Code = headCode;	//S1.code

/*---------------------------------------------S1 ends--------------------------*/		
		prevCode = NULL;
		headCode = NULL;
		flagIntCode = 0;

		currAST = currAST->next;	//ELSESTMT if NOT NULL
		if (currAST!=NULL)
		{
			child = currAST->children;	//stmts starts

/*---------------------------------------------S2 starts--------------------------*/		

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
						child->nextLabel = SNext;
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
			//Right now S2.code = headCode and S2.next is prevCode->next
			intCode* S2Code = headCode;	//S2.code

/*---------------------------------------------S2 ends--------------------------*/	
			ast->code = boolCode;	//S.code = B.code
			intCode* tempCode = boolCode;
			while(tempCode->next != NULL)
			{
				tempCode = tempCode->next;
			}
			intCode* code1 = createQuadruple(trueLabel, LABELOP, NULL, NULL);
			tempCode->next = code1;
			code1->next = S1Code;

			tempCode = S1Code;
			while(tempCode->next != NULL)
			{
				tempCode = tempCode->next;
			}

			intCode* code2 = createQuadruple(SNext, GOTO, NULL, NULL);
			tempCode->next = code2;

			intCode* code3 = createQuadruple(falseLabel, LABELOP, NULL, NULL);
			code2->next = code3;
			code3->next = S2Code;

			intCode* code4 = createQuadruple(SNext, LABELOP, NULL, NULL);
			// printf("%s\n", SNext->tkn->lexeme);

			tempCode = S2Code;
			while(tempCode->next != NULL)
			{
				tempCode = tempCode->next;
			}
			tempCode->next = code4;
		}
		else
		{
			ast->code = boolCode;	//S.code = B.code
			intCode* tempCode = boolCode;
			while(tempCode->next != NULL)
			{
				tempCode = tempCode->next;
			}
			intCode* code1 = createQuadruple(trueLabel, LABELOP, NULL, NULL);
			tempCode->next = code1;
			code1->next = S1Code;

			intCode* code2 = createQuadruple(SNext, LABELOP, NULL, NULL);
			// printf("%s\n", SNext->tkn->lexeme);

			tempCode = S1Code;
			while(tempCode->next != NULL)
			{
				tempCode = tempCode->next;
			}
			tempCode->next = code2;
		}
		
	}
	else if (ast->sym == ASSIGN_SINGLE_VAR)
	{
		currAST = ast->children;	//leftHandSide ID

		currAST = currAST->next;	//RightHandSideType1

		if (currAST->sym == SIZEOP)	//Size Expression
		{
			ASTnode* child = currAST->children;	//id in size(id)

			field* fld = (field*)child->symTablePtr;
			ASTnode* sizeNode = createASTNodeForValue(fld->idPtr->sz[0]);	//creating an ASTNode which is a NUM with size value
			// printf("%d\n", sizeNode->tkn->value.intVal);
			intCode* code = createQuadruple(ast->children, ASSIGN, sizeNode, NULL);
			ast->code = code;
			ast->addr = ast->children;	//ASSIGN_SINGLE_VAR.addr = id
		}
		else if (currAST->sym == FUNCALL)
		{
			//Nothing
		}
		else
		{
			retTypeIntCode* rightSide = checkArithExprsnForIntCode(currAST, st);

			intCode* code = createQuadruple(ast->children, ASSIGN, rightSide->addr, NULL);
			intCode* tempCode = rightSide->code;
			while(tempCode->next != NULL)
			{
				tempCode = tempCode->next;
			} 
			tempCode->next = code;
			ast->code = rightSide->code;
		}
	}
	else if (ast->sym == ASSIGN_LIST_VAR)
	{
		currAST = ast->children;	//LIST_VAR ->Dummy Node
		ASTnode* child = currAST->children;	//IDs start
	
		currAST = currAST->next;	//RightHandSideType2
		field* fld;
		if (currAST->sym == SIZEOP)
		{
			ASTnode* childId = currAST->children;	//id in size(id)
			fld = (field*)childId->symTablePtr;
			int val1 = fld->idPtr->sz[0];
			int val2 = fld->idPtr->sz[1];

			ASTnode* sizeNode1 = createASTNodeForValue(val1);	//creating an ASTNode which is a NUM with size value
			ASTnode* sizeNode2 = createASTNodeForValue(val2);	//creating an ASTNode which is a NUM with size value
			// printf("%d\n", sizeNode1->tkn->value.intVal);
			// printf("%d\n", sizeNode2->tkn->value.intVal);

			intCode* code1 = createQuadruple(child, ASSIGN, sizeNode1, NULL);
			intCode* code2 = createQuadruple(child->next, ASSIGN, sizeNode2, NULL);
			code1->next = code2;

			ast->code = code1;
			ast->addr = child;	//ASSIGN_LIST_VAR.addr = first id in list
		}
	}
}

void checkBoolExprsnIntCode(ASTnode* ast, symTable st)
{
	if (ast->tkn == NULL)	//means BOOL NOT
	{
		ast->children->trueLabel = ast->falseLabel;
		ast->children->falseLabel = ast->trueLabel;

		checkBoolExprsnIntCode(ast->children, st);	//B = BOOL NOT and !B1, so B1 is child of BOOL NOT
		ast->code = ast->children->code;
	}
	else if (strcmp(ast->tkn->tkn, "AND") == 0)
	{
		ast->children->trueLabel = createTempLabelASTnode();
		ast->children->falseLabel = ast->falseLabel;
		ast->children->next->trueLabel = ast->trueLabel;
		ast->children->next->falseLabel = ast->falseLabel;

		checkBoolExprsnIntCode(ast->children, st);
		checkBoolExprsnIntCode(ast->children->next, st);
		
		ast->code = ast->children->code;

		intCode* tempCode = ast->children->code;
		while(tempCode->next != NULL)
		{
			tempCode = tempCode->next;
		}

		intCode* code1 = createQuadruple(ast->children->trueLabel, LABELOP, NULL, NULL);
		tempCode->next = code1;
		code1->next = ast->children->next->code;

	}
	else if (strcmp(ast->tkn->tkn, "OR") == 0)
	{
		ast->children->trueLabel = ast->trueLabel;
		ast->children->falseLabel = createTempLabelASTnode();
		ast->children->next->trueLabel = ast->trueLabel;
		ast->children->next->falseLabel = ast->falseLabel;

		checkBoolExprsnIntCode(ast->children, st);
		checkBoolExprsnIntCode(ast->children->next, st);
		
		ast->code = ast->children->code;

		intCode* tempCode = ast->children->code;
		while(tempCode->next != NULL)
		{
			tempCode = tempCode->next;
		}

		intCode* code1 = createQuadruple(ast->children->falseLabel, LABELOP, NULL, NULL);
		tempCode->next = code1;
		code1->next = ast->children->next->code;
	}
	else //relational Op
	{
		operatorSym op;	//LT LE EQ GT GE NE
		if (strcmp(ast->tkn->tkn, "LT") == 0)
		{
			op = LESSTHAN;
		}
		else if (strcmp(ast->tkn->tkn, "LE") == 0)
		{
			op = LESSTHANEQ;
		}
		else if (strcmp(ast->tkn->tkn, "EQ") == 0)
		{
			op = ISEQUAL;
		}
		else if (strcmp(ast->tkn->tkn, "GT") == 0)
		{
			op = GREATERTHAN;
		}
		else if (strcmp(ast->tkn->tkn, "GE") == 0)
		{
			op = GREATERTHANEQ;
		}
		else if (strcmp(ast->tkn->tkn, "NE") == 0)
		{
			op = NOTEQUAL;
		}

		ASTnode* child1 = ast->children;
		ASTnode* child2 = child1->next;
		int flag = 0;

		intCode* E1Code = NULL;
		intCode* E2Code = NULL;
		ASTnode* E1Addr = NULL;
		ASTnode* E2Addr = NULL;

		if (strcmp(child1->tkn->tkn, "ID") == 0)
		{
			E1Code = NULL;
			E1Addr = child1;
		}
		else if (strcmp(child1->tkn->tkn, "NUM") == 0)
		{
			int sz[2];
			sz[0] = 2;
			sz[1] = 0;

			ASTnode* result = createTempASTNode(sz, integer, st);
			intCode* code = createQuadruple(result, ASSIGN, child1, NULL);
			offsetForIntCode += 2;
			E1Code = code;
			E1Addr = result;
		}

		if (strcmp(child2->tkn->tkn, "ID") == 0)
		{
			E2Code = NULL;
			E2Addr = child2;
		}
		else if (strcmp(child2->tkn->tkn, "NUM") == 0)
		{
			int sz[2];
			sz[0] = 2;
			sz[1] = 0;

			ASTnode* result = createTempASTNode(sz, integer, st);
			intCode* code = createQuadruple(result, ASSIGN, child2, NULL);
			offsetForIntCode += 2;
			E2Code = code;
			E2Addr = result;
		}

		intCode* code1 = createQuadruple(ast->trueLabel, op, E1Addr, E2Addr);	//see in Book
		intCode* code2 = createQuadruple(ast->falseLabel, GOTO, NULL, NULL);
		code1->next = code2;

		if (E1Code == NULL)
		{
			if (E2Code == NULL)
			{
				ast->code = code1;
			}
			else
			{
				ast->code = E2Code;
				intCode* tempCode = E2Code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code1;				
			}
		}
		else
		{
			if (E2Code == NULL)
			{
				ast->code = E1Code;
				intCode* tempCode = E1Code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code1;
			}
			else
			{
				ast->code = E1Code;
				intCode* tempCode = E1Code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = E2Code;
				tempCode = E2Code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code1;		
			}
		}
	}
}

ASTnode* createTempLabelASTnode()
{
	char* lexeme = newLabel();

	token* tkn = (token*)malloc(sizeof(token));
	tkn->lexeme = lexeme;

	ASTnode* ast = (ASTnode*)malloc(sizeof(ASTnode));
	ast->tkn = tkn;
	ast->symTablePtr = NULL;
	ast->code = NULL;

	return ast;
}

ASTnode* createASTNodeForValue(int val)	//this node is NUM node with value size
{
	token* tkn = (token*)malloc(sizeof(token));

	char* lexeme = (char*)malloc(20*sizeof(char));
	strcpy(lexeme, "NUM");

	tkn->lexeme = lexeme;
	strcpy(tkn->tkn, lexeme);
	tkn->value.intVal = val;

	ASTnode* ast = (ASTnode*)malloc(sizeof(ASTnode));
	ast->tkn = tkn;
	ast->symTablePtr = NULL;
	ast->code = NULL;
	ast->t = integer;

	return ast;
}

ASTnode* createTempASTNode(int* sz, varType t, symTable st)	//This is to create a temporary ASTnode to assign something, this node has size = sz, and type = t
{
	char* lexeme = newTemp();

	token* tkn = (token*)malloc(sizeof(token));
	tkn->lexeme = lexeme;
	char* str1 = (char*)malloc(20*sizeof(char));
	strcpy(str1, "ID");
	strcpy(tkn->tkn, str1);

	idField* idPtr = createIdField(sz, 1, offsetForIntCode);
	field* fld = createField(tkn, NULL, t, NULL, idPtr);	//functionName = NULL, funcPtr = NULL
	insertSymTableNode(st, fld);

	ASTnode* ast = (ASTnode*)malloc(sizeof(ASTnode));
	ast->tkn = tkn;
	ast->symTablePtr = fld;
	ast->code = NULL;
	ast->t = t;

	return ast;
}

int* checkMatrixSzIntCode(ASTnode* ast)
{
	//ast is MATRIXDECL
	int* matrixSz = (int*)malloc(2*sizeof(int));
	int rows = 0;
	int cols = 0;
	int flag = 0;

	ASTnode* childRow = ast->children;	//ROW -> Dummy Node
	while(childRow!=NULL)
	{
		ASTnode* child = childRow->children;	//NUM in ROW starts

		if (!flag)
		{
			while(child!=NULL)
			{	
				cols++;
				child = child->next;
			}
			flag = 1;
		}
		childRow = childRow->next;
		rows++;
	}
	matrixSz[0] = rows;
	matrixSz[1] = cols;
	return matrixSz;
}

char* newTemp()
{
	char* temp = (char*)malloc(20*sizeof(char));
	strcpy(temp, "t");
	char noTemp[20];
	snprintf(noTemp, sizeof(noTemp), "%d", tempVarNo);
	strcat(temp, noTemp);
	tempVarNo++;
	
	return temp;
}

char* newLabel()
{
	char* temp = (char*)malloc(20*sizeof(char));
	strcpy(temp, "L");
	char noTemp[20];
	snprintf(noTemp, sizeof(noTemp), "%d", labelNo);
	strcat(temp, noTemp);
	labelNo++;
	
	return temp;
}

intCode* createQuadruple(ASTnode* result, operatorSym op, ASTnode* arg1, ASTnode* arg2)
{
	intCode* code = (intCode*)malloc(sizeof(intCode));
	code->result = result;
	code->op = op;
	code->arg1 = arg1;
	code->arg2 = arg2;
	code->next = NULL;
}

int calculateMatrixElemOffset(int offset, int dim1, int dim2, int i, int j)
{
	int retOffset = offset + ((i-1)*dim2 + (j-1))*2;
	return retOffset;
}

retTypeIntCode* checkArithExprsnForIntCode(ASTnode* ast, symTable st)
{
	if (ast->sym == MATRIXDECL)	//Matrix Being declared
	{
		retTypeIntCode* rt = (retTypeIntCode*)malloc(sizeof(retTypeIntCode));
		rt->t = matrix;

		int* matrixSz = checkMatrixSzIntCode(ast);	//sending MATRIX DECL
		rt->sz[0] = matrixSz[0];
		rt->sz[1] = matrixSz[1];

		ASTnode* result = createTempASTNode(matrixSz, matrix, st);

		intCode* code = createQuadruple(result, ASSIGN, ast, NULL);	//sending MATRIXDECL

		rt->code = code;
		rt->addr = result;
		offsetForIntCode += matrixSz[0]*matrixSz[1]*2;
		free(matrixSz);

		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "ID") == 0)
	{
		retTypeIntCode* rt = (retTypeIntCode*)malloc(sizeof(retTypeIntCode));
		rt->sz[0] = 0;
		rt->sz[1] = 0;
		rt->t = noType;

		field* fld = ast->symTablePtr;

		if (fld!=NULL)
		{
			if (ast->children!=NULL)	//means matrixElement
			{
				int i = (ast->children->tkn->value).intVal;	//NUM1
				int j = (ast->children->next->tkn->value).intVal;	//NUM2

				int dim1 = fld->idPtr->sz[0];
				int dim2 = fld->idPtr->sz[1];

				int matrixOffset = fld->idPtr->offset;
				int elemOffset = calculateMatrixElemOffset(matrixOffset, dim1, dim2, i, j);
				ASTnode* offsetAST = createASTNodeForValue(elemOffset);	//Value AST

				rt->sz[0] = 2;
				ASTnode* temp1 = createTempASTNode(rt->sz, integer, st);
				offsetForIntCode += 2;
				intCode* temp1Code = createQuadruple(temp1,  ADDRESSOFFSET, offsetAST, NULL);	//sending first ROW, Dummy Node
				rt->sz[0] = 2;
				ASTnode* temp2 = createTempASTNode(rt->sz, integer, st);
				offsetForIntCode += 2;
				intCode* temp2Code = createQuadruple(temp2, VALUEAT, temp1, NULL);

				temp1Code->next = temp2Code;

				rt->t = integer;
				rt->code = temp1Code;
				rt->addr = temp2;	//because the variable generated which would be used is temp2

				return rt;
			}
			else 	//can be any type
			{
				rt->sz[0] = fld->idPtr->sz[0];
				rt->sz[1] = fld->idPtr->sz[1];
				rt->t = fld->t;
				// offsetForIntCode = offsetForIntCode;
				rt->addr = ast;
				rt->code = NULL;
			}
		}

		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "NUM") == 0)
	{
		retTypeIntCode* rt = (retTypeIntCode*)malloc(sizeof(retTypeIntCode));
		rt->sz[0] = 2;
		rt->sz[1] = 0;
		rt->t = integer;

		ASTnode* result = createTempASTNode(rt->sz, integer, st);
		intCode* code = createQuadruple(result, ASSIGN, ast, NULL);
		offsetForIntCode += 2;
		rt->code = code;
		rt->addr = result;
		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "STR") == 0)
	{
		retTypeIntCode* rt = (retTypeIntCode*)malloc(sizeof(retTypeIntCode));
		rt->sz[0] = strlen(ast->tkn->lexeme);	//string itself
		rt->sz[1] = 0;
		rt->t = string;

		ASTnode* result = createTempASTNode(rt->sz, string, st);
		intCode* code = createQuadruple(result, ASSIGN, ast, NULL);
		offsetForIntCode += rt->sz[0];
		rt->code = code;
		rt->addr = result;
		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "PLUS") == 0)
	{
		retTypeIntCode* rtype1 = checkArithExprsnForIntCode(ast->children, st);	//child1
		retTypeIntCode* rtype2 = checkArithExprsnForIntCode(ast->children->next, st);	//child2

		ASTnode* result = createTempASTNode(rtype1->sz, rtype1->t, st);
		intCode* code = createQuadruple(result, PLUSOP, rtype1->addr, rtype2->addr);
		
		rtype1->addr = result;

		if (rtype1->code == NULL)
		{
			if (rtype2->code == NULL)
			{
				rtype1->code = code;
			}
			else
			{
				rtype1->code = rtype2->code;
				intCode* tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;				
			}
		}
		else
		{
			if (rtype2->code == NULL)
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;
			}
			else
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = rtype2->code;
				
				tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;			
			}
		}
		
		varType type1 = rtype1->t;
		if (type1 == integer)
		{
			offsetForIntCode += 2; 
		}
		else if (type1 == string)
		{
			if (rtype1->sz[0] + rtype2->sz[0] < 20)
			{
				offsetForIntCode = offsetForIntCode + rtype1->sz[0] + rtype2->sz[0];
				rtype1->sz[0] = rtype1->sz[0] + rtype2->sz[0];
			}
			else
			{
				offsetForIntCode += 20;
				rtype1->sz[0] = 20;
			}
		}
		else if (type1 == matrix)
		{
			offsetForIntCode += rtype1->sz[0]*rtype1->sz[1]*2;
		}

		return rtype1;
	}
	else if (strcmp(ast->tkn->tkn, "MINUS") == 0)
	{
		retTypeIntCode* rtype1 = checkArithExprsnForIntCode(ast->children, st);	//child1
		retTypeIntCode* rtype2 = checkArithExprsnForIntCode(ast->children->next, st);	//child2

		ASTnode* result = createTempASTNode(rtype1->sz, rtype1->t, st);
		intCode* code = createQuadruple(result, MINUSOP, rtype1->addr, rtype2->addr);
		
		rtype1->addr = result;

		if (rtype1->code == NULL)
		{
			if (rtype2->code == NULL)
			{
				rtype1->code = code;
			}
			else
			{
				rtype1->code = rtype2->code;
				intCode* tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;				
			}
		}
		else
		{
			if (rtype2->code == NULL)
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;
			}
			else
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = rtype2->code;
				
				tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;			
			}
		}

		varType type1 = rtype1->t;

		if (type1 == integer)
		{
			offsetForIntCode += 2; 
		}
		else if (type1 == matrix)
		{
			offsetForIntCode += rtype1->sz[0]*rtype1->sz[1]*2;
		}

		return rtype1;
	}
	else if (strcmp(ast->tkn->tkn, "MUL") == 0 )
	{
		retTypeIntCode* rtype1 = checkArithExprsnForIntCode(ast->children, st);	//child1
		retTypeIntCode* rtype2 = checkArithExprsnForIntCode(ast->children->next, st);	//child2

		ASTnode* result = createTempASTNode(rtype1->sz, rtype1->t, st);
		intCode* code = createQuadruple(result, MULOP, rtype1->addr, rtype2->addr);
		
		rtype1->addr = result;

		if (rtype1->code == NULL)
		{
			if (rtype2->code == NULL)
			{
				rtype1->code = code;
			}
			else
			{
				rtype1->code = rtype2->code;
				intCode* tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;				
			}
		}
		else
		{
			if (rtype2->code == NULL)
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;
			}
			else
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = rtype2->code;
				
				tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;			
			}
		}

		offsetForIntCode += 2; 
		return rtype1;
	}
	else if (strcmp(ast->tkn->tkn, "DIV") == 0)
	{
		retTypeIntCode* rtype1 = checkArithExprsnForIntCode(ast->children, st);	//child1
		retTypeIntCode* rtype2 = checkArithExprsnForIntCode(ast->children->next, st);	//child2

		ASTnode* result = createTempASTNode(rtype1->sz, rtype1->t, st);
		intCode* code = createQuadruple(result, DIVOP, rtype1->addr, rtype2->addr);
		
		rtype1->addr = result;

		if (rtype1->code == NULL)
		{
			if (rtype2->code == NULL)
			{
				rtype1->code = code;
			}
			else
			{
				rtype1->code = rtype2->code;
				intCode* tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;				
			}
		}
		else
		{
			if (rtype2->code == NULL)
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;
			}
			else
			{
				intCode* tempCode = rtype1->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = rtype2->code;
				
				tempCode = rtype2->code;
				while(tempCode->next != NULL)
				{
					tempCode = tempCode->next;
				}
				tempCode->next = code;			
			}
		}

		offsetForIntCode += 2; 
		return rtype1;
	}
}

void printQuadruples(ASTnode* ast)
{
	intCode* currCode = (intCode*)ast->code;
	while(currCode != NULL)
	{
		char* arg1str = NULL;
		char* arg2str = NULL;
		if (currCode->arg1 != NULL)
		{
			if (currCode->arg1->tkn != NULL)
			{
				arg1str = currCode->arg1->tkn->lexeme;
			}
			else
			{
				arg1str = enumToString2(currCode->arg1->sym);
			}
		}
		if (currCode->arg2 != NULL)
		{
			if (currCode->arg2->tkn != NULL)
			{
				arg2str = currCode->arg2->tkn->lexeme;
			}
			else
			{
				arg2str = enumToString2(currCode->arg2->sym);
			}
		}
		printf("%s %s %s %s\n", currCode->result->tkn->lexeme, enumToString4(currCode->op), arg1str, arg2str) ;
		currCode = currCode->next;
	}
}

char* enumToString4(operatorSym op)
{
    return convert4[op].str;
}
//ASSIGN means a = 2
//PLUS means a = b + c; so OP is PLUSOP
//Will print Row Major
//initialise variables in ASTnode
//boolExprsn dummy Node is present but STMTS or ELSESTMT are not present