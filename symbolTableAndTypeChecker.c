//Deepak Guneja
//2015A7PS0083P

/*-------------------This is a complete module for semantic Analyzer------------------------*/


#include "symbolTableAndTypeChecker.h"

symTable createSymTable()
{
	symTable st = (symTable)malloc(43*sizeof(struct symTableNode));
	for (int i = 0; i < 43; ++i)
	{
		st[i].first = NULL;
		st[i].last = NULL;
	}

	return st;
}

int calculateSymTableIndex(char* lexeme)
{
	int len = strlen(lexeme);
	int i;
	int index = 0;
	for(i=len-1; i>=0; i--) {
		//choosing x = 33;
		index = (index*33 + lexeme[i])%43;
	}
	return index;
}

int insertSymTableNode(symTable st, field* fld)
{
	int index = calculateSymTableIndex(fld->tkn->lexeme);
	if (st[index].first == NULL)
	{
		st[index].first = fld;
		st[index].last = fld;
		return index;
	}
	st[index].last->next = fld;
	st[index].last = fld;
	return index;
}

field* lookupSymbolTable(symTable st, ASTnode* ast)
{
	char* lexeme = ast->tkn->lexeme;
	int index = calculateSymTableIndex(lexeme);
	if (st[index].first == NULL)
	{
		return NULL;
	}
	field* fld = st[index].first;
	while(fld!=NULL)
	{
		if (strcmp(fld->tkn->lexeme, lexeme)==0)
		{
			return fld;
		}
		fld = fld->next;
	}
	return NULL;
}

field* lookupInEveryAncestor(ASTnode* ast, symTable st, symTableStack* stStack)
{
	symTableStackNode* currStackNd = stStack->first;
	while(currStackNd!=NULL)
	{
		field* fld = lookupSymbolTable(currStackNd->st, ast);
		if (fld!=NULL)
		{
			return fld;
		}
		currStackNd = currStackNd->next;
	}
	return NULL;
}

void printAllSymTables(symTable st, int nestingLevel, char* parentOfCurrScope)
{
	for (int i = 0; i < 43; ++i)
	{
		if (st[i].first==NULL)
		{
			continue;
		}
		field* fld = st[i].first;
		while(fld!=NULL)
		{
			if (fld->idPtr!=NULL)
			{
				if (fld->t != matrix)
				{
					printf("%-20s   %-20s   %-10d   %-20s   %-15s   %-10d   %-7d\n", fld->tkn->lexeme, fld->funcScopeName, nestingLevel, parentOfCurrScope, enumToString3(fld->t), fld->idPtr->sz[0], fld->idPtr->offset);
				}
				else
				{
					printf("%-20s   %-20s   %-10d   %-20s   %-15s   %dx%-7d   %-7d\n", fld->tkn->lexeme, fld->funcScopeName, nestingLevel, parentOfCurrScope, enumToString3(fld->t), fld->idPtr->sz[0], fld->idPtr->sz[1], fld->idPtr->offset);
				}
			}
			else
			{
				printAllSymTables(fld->funcPtr->childSymTablePtr, nestingLevel+1, fld->funcScopeName);
			}
			fld = fld->next;
		}
	}
}

symTableStackNode* createSymTableStackNode(symTable st, char* functionName)
{
	symTableStackNode* stackNd = (symTableStackNode*)malloc(sizeof(symTableStackNode));
	stackNd->st = st;
	stackNd->functionName = functionName;
	stackNd->next = NULL;
	// stackNd->prev = NULL;
	return stackNd;
}

void pushSymTableStackNode(symTableStack* stStack, symTableStackNode* stackNd)
{
	stackNd->next = stStack->first;
	// stStack->first->prev = stackNd;
	stStack->first = stackNd;
}

void popSymTableStackNode(symTableStack* stStack)
{
	if (stStack->first == NULL)
	{
		return;
	}
	symTableStackNode* temp = stStack->first;
	stStack->first = temp->next;
	// stStack->first->prev = NULL;
	free(temp);
}

functionField* createFuncField(symTable childSymTablePtr, int inpParamListSz, int outParamListSz, int* inpIndices, int* outIndices, varType* inParamType, varType* outParamType)
{
	functionField* funcField = (functionField*)malloc(sizeof(functionField));
	funcField->childSymTablePtr = childSymTablePtr;
	funcField->inpParamListSz = inpParamListSz;
	funcField->outParamListSz = outParamListSz;

	for (int i = 0; i < inpParamListSz; ++i)
	{
		funcField->inpIndices[i] = inpIndices[i];
	}
	
	for (int i = 0; i < outParamListSz; ++i)
	{
		funcField->outIndices[i] = outIndices[i];
	}
	for (int i = 0; i < 20; ++i)
	{
		funcField->inParamType[i] = inParamType[i];
		funcField->outParamType[i] = outParamType[i];
	}
	return funcField;
}

idField* createIdField(int* sz, int outParamInitOrNot, int offset)
{
	idField* idFld = (idField*)malloc(sizeof(idField));
	idFld->offset = offset;
	idFld->sz[0] = sz[0];
	idFld->sz[1] = sz[1];
	idFld->outParamInitOrNot = outParamInitOrNot;

	return idFld;
}

field* createField(token* tkn, char* funcScopeName, varType t, functionField* funcPtr, idField* idPtr)
{
	field* fld = (field*)malloc(sizeof(field));
	fld->tkn = tkn;
	fld->funcScopeName = funcScopeName;
	fld->t = t;
	fld->funcPtr = funcPtr;
	fld->idPtr = idPtr;
	fld->next = NULL;
	return fld;
}

int initSymTables(ASTnode* ast, symTable st, symTableStack* stStack, int prevOffset)
{	
	//first output parameter declared then input parameter
	// printf("%d\n", prevOffset);
	ASTnode* currAST;

	if (ast->sym == FUNCTION_DEF)
	{
		currAST = ast->children->next;
		currAST->t = function;
		char* funId = currAST->tkn->lexeme;
		currAST->symTablePtr = lookupSymbolTable(st, currAST);

		if (currAST->symTablePtr!=NULL)
		{
			int errLine = currAST->tkn->lineNo;
			insertSymTableErr(currAST, errLine, 1, NULL, NULL);
			// semanticErr = 1;	//FunID already Declared in the same scope
		}
		else
		{
			currAST = ast->children;	//OUT_PARAM
			int inpParamListSz = 0;
			int outParamListSz = 0;
			int inpIndices[20];
			int outIndices[20];
			varType inParamType[20];
			varType outParamType[20];

			for (int i = 0; i < 20; ++i)
			{
				inParamType[i] = noType;
				outParamType[i] = noType;
			}
			symTable currSt = createSymTable();

			int tempOffset = 0;	//because these parameters are of different scope

			ASTnode* childParamLst = currAST->children;	//TYPE_ID
			while(childParamLst!=NULL)
			{
				ASTnode* typeNd = childParamLst->children;	//type
				char* typeLexeme = typeNd->tkn->lexeme;
				varType t = stringToEnum3(typeLexeme);
				ASTnode* idNd = typeNd->next;		//id
				idNd->t = t;
				field* fld = lookupSymbolTable(currSt, idNd);
				int index;
				if (fld!=NULL)
				{
					int errLine = currAST->tkn->lineNo;
					insertSymTableErr(currAST, errLine, 2, NULL, NULL);
					// semanticErr = 2;	//Redeclaration of identifier in the same scope
					idNd->symTablePtr = NULL;
				}
				else
				{
					int sz[2];
					sz[0] = findSz(t);
					sz[1] = 0;
					idField* idFld = createIdField(sz, 0, tempOffset);	//Ouput Parameter so 0
					tempOffset += sz[0];
					field* fld = createField(idNd->tkn, funId, t, NULL, idFld);
					index = insertSymTableNode(currSt, fld);
					idNd->symTablePtr = fld;
				}
				childParamLst = childParamLst->next;
				outIndices[outParamListSz] = index;
				outParamType[outParamListSz] = t;
				outParamListSz++;
			}

			currAST = currAST->next->next;	//IN_PARAM
			childParamLst = currAST->children;	//TYPE_ID
			while(childParamLst!=NULL)
			{
				ASTnode* typeNd = childParamLst->children;	//type
				char* typeLexeme = typeNd->tkn->lexeme;
				varType t = stringToEnum3(typeLexeme);
				ASTnode* idNd = typeNd->next;		//id
				idNd->t = t;
				field* fld = lookupSymbolTable(currSt, idNd);
				int index;
				if (fld!=NULL)
				{
					int errLine = currAST->tkn->lineNo;
					insertSymTableErr(currAST, errLine, 2, NULL, NULL);
					// semanticErr = 2;	//Redeclaration of identifier in the same scope
					idNd->symTablePtr = NULL;
				}
				else
				{
					int sz[2];
					sz[0] = findSz(t);
					sz[1] = 0;
					idField* idFld = createIdField(sz, 1, tempOffset);
					tempOffset += sz[0];
					field* fld = createField(idNd->tkn, funId, t, NULL, idFld);
					index = insertSymTableNode(currSt, fld);
					idNd->symTablePtr = fld;
				}
				childParamLst = childParamLst->next;
				inpIndices[inpParamListSz] = index;
				inParamType[inpParamListSz] = t;
				inpParamListSz++;
			}

			functionField* funcField = createFuncField(currSt, inpParamListSz, outParamListSz, inpIndices, outIndices, inParamType, outParamType);
			field* fld = createField(ast->children->next->tkn, stStack->first->functionName, function, funcField, NULL);
			insertSymTableNode(st, fld);
			ast->children->next->symTablePtr = fld;


			symTableStackNode* currStackNd = createSymTableStackNode(currSt, funId);
			pushSymTableStackNode(stStack, currStackNd);
			ASTnode* child = ast->children->next->next->next;

			while(child!=NULL)
			{
				tempOffset = initSymTables(child, currSt, stStack, tempOffset);
				child = child->next;
			}
			popSymTableStackNode(stStack);
		}
	}

	else if (ast->sym == DECL)
	{
		char* typeLexeme = ast->children->tkn->lexeme;
		varType t = stringToEnum3(typeLexeme);
		ast->t = t;
		currAST = ast->children;
		currAST->t = t;
		currAST = ast->children->next;
		while(currAST!=NULL)
		{
			currAST->t = t;
			if (lookupSymbolTable(st, currAST)!=NULL)
			{
				int errLine = currAST->tkn->lineNo;
				insertSymTableErr(currAST, errLine, 2, NULL, NULL);
				// semanticErr = 2;	//Redeclaration of identifier in the same scope
				currAST->symTablePtr = NULL;
			}
			else
			{
				int sz[2];
				sz[0] = findSz(t);
				sz[1] = 0;
				idField* idFld;
				if (sz[0] == 0)	//i.e. string and matrix, so offset = -1 now
				{
					idFld = createIdField(sz, 1, -1);
				}
				else
				{
					idFld = createIdField(sz, 1, prevOffset);
				}
				prevOffset += sz[0];
				field* fld = createField(currAST->tkn, stStack->first->functionName, t, NULL, idFld);
				insertSymTableNode(st, fld);
				currAST->symTablePtr = fld;
			}
			currAST = currAST->next;
		}
	}

	else if (ast->sym == IOREAD)	
	{
		field* fld = lookupInEveryAncestor(ast->children, st, stStack);
		ast->children->symTablePtr = fld;
		if (fld == NULL)
		{
			int errLine = ast->children->tkn->lineNo;
			insertSymTableErr(ast->children, errLine, 3, NULL, NULL);
			// semanticErr = 3;	//variable accessed without declaration
		}
		else
		{
			ast->children->t = fld->t;
			fld->idPtr->outParamInitOrNot = 1;
		}
		if (!(ast->children->t == integer || ast->children->t == real))
		{
			int errLine = ast->children->tkn->lineNo;
			insertSymTableErr(ast->children, errLine, 4, NULL, NULL);
			// semanticErr = 4;	//invalid identifier in read(), i.e. string or matrix
		}
	}

	else if (ast->sym == IOPRINT)		//If IOPRINT, then every id is fine
	{
		field* fld = lookupInEveryAncestor(ast->children, st, stStack);
		ast->children->symTablePtr = fld;
		if (fld == NULL)
		{
			int errLine = ast->children->tkn->lineNo;
			insertSymTableErr(ast->children, errLine, 3, NULL, NULL);
			// semanticErr = 3;	//variable accessed without declaration
		}
		else
		{
			ast->children->t = fld->t;
		}
	}

	else if (ast->sym == IFSTMT)
	{
		currAST = ast->children;	//boolExprsn
		checkBoolExprsn(currAST, st, stStack);
		currAST = currAST->next;	//currAST is STMTS
		ASTnode* child = currAST->children;
		while(child!=NULL)
		{
			prevOffset = initSymTables(child, st, stStack, prevOffset);	//more stmts
			child = child->next;
		}
		currAST = currAST->next;	//Else part
		if (currAST!=NULL)
		{
			prevOffset = initSymTables(currAST, st, stStack, prevOffset);
		}
		
	}
	else if(ast->sym == ELSESTMT)
	{
		currAST = ast->children;	//stmts
		while(currAST!=NULL)
		{
			prevOffset = initSymTables(currAST, st, stStack, prevOffset);
			currAST = currAST->next;
		}
	}
	else if (ast->sym == ASSIGN_SINGLE_VAR)
	{
		currAST = ast->children;
		field* fld = lookupInEveryAncestor(currAST, st, stStack);
		currAST->symTablePtr = fld;
		if (currAST->symTablePtr == NULL)	//looking for ID
		{
			int errLine = currAST->tkn->lineNo;
			insertSymTableErr(currAST, errLine, 3, NULL, NULL);
			// semanticErr = 3;	//variable accessed without declaration
		}
		else
		{
			field* f = (field*)currAST->symTablePtr;
			currAST->t = f->t;
			f->idPtr->outParamInitOrNot = 1;
		}

		varType leftSideType = currAST->t;	//type of left hand side

		currAST = currAST->next;	//RightHandSideType1
		if (currAST->sym == SIZEOP)	//Size Expression
		{
			ASTnode* child = currAST->children;	//id in size(id)
			if (leftSideType!=integer)	//leftSideType should be integer
			{
				int errLine = child->tkn->lineNo;
				child->t = string;
				insertSymTableErr(child, errLine, 16, NULL, NULL);
				// semanticErr = 16;	//type mismatch error on left hand side
			}
			checkSizeExprsn(child, st, stStack, string);
		}
		else if (currAST->sym == FUNCALL)
		{
			varType* outParamType = checkFunCallExprsn(currAST, st, stStack);
			if (outParamType != NULL)
			{
				//because ASSIGN_SINGLE_VAR, so outParamType[0] == leftSideType then no error only if outParamType[1] == noType, otherwise it means number of output parametrs greater than one in ASSIGN_SINGLE_VAR
				if ((outParamType[0] != leftSideType || outParamType[1] != noType))
				{
					int errLine = currAST->children->tkn->lineNo;
					insertSymTableErr(currAST->children, errLine, 5, NULL, NULL);
					// semanticErr = 5;	//no or type of output parameters do not match
					/* code */
				}
			}
		}
		else
		{

			retType* rightSideType = checkArithExprsn(currAST, st, stStack);

			if (currAST->sym == MATRIXDECL)
			{
				currAST = currAST->children->children;
			}

			if (leftSideType != rightSideType->t)
			{
				int errLine = currAST->tkn->lineNo;
				insertSymTableErr(currAST, errLine, 8, NULL, NULL);
				// semanticErr = 8;	//no or type of IDs on left side don't match
			}
			else if (fld != NULL)
			{
				if (fld->idPtr->offset == -1)	//if left hand side is not already declared(i.e. matrix or string), then offset = -1, so need to compute offset and add into prevOffset
				{
					if (rightSideType->t == matrix)
					{
						int offset = rightSideType->sz[0]*rightSideType->sz[1]*2;
						fld->idPtr->sz[0] = rightSideType->sz[0];
						fld->idPtr->sz[1] = rightSideType->sz[1];
						fld->idPtr->offset = prevOffset;
						prevOffset += offset;
					}
					else if (rightSideType->t == string)
					{
						fld->idPtr->sz[0] = rightSideType->sz[0];
						fld->idPtr->sz[1] = 0;
						fld->idPtr->offset = prevOffset;
						prevOffset += rightSideType->sz[0];
					}
				}
				else	//just check type and size, and if matrix or string and size different then give error
				{
					if (rightSideType->t == matrix)
					{
						if (fld->idPtr->sz[0] != rightSideType->sz[0] || fld->idPtr->sz[1] != rightSideType->sz[1])
						{
							int errLine = currAST->tkn->lineNo;
							insertSymTableErr(currAST, errLine, 24, NULL, NULL);
							// semanticErr = 24;	//cannot reassign matrix or string to different size
						}
					}
					else if (rightSideType->t == string)
					{
						if (fld->idPtr->sz[0] != rightSideType->sz[0])
						{
							int errLine = currAST->tkn->lineNo;
							insertSymTableErr(currAST, errLine, 24, NULL, NULL);
							// semanticErr = 24;	//cannot reassign matrix or string to different size
						}
					}
				}
			}
				//check if matrix or string, and if they are not assigned previously then also assign offset
		}

	}
	else if (ast->sym == ASSIGN_LIST_VAR)
	{
		currAST = ast->children;	//LIST_VAR ->Dummy Node
		ASTnode* child = currAST->children;	//IDs start

		varType leftSideType[20];
		for (int i = 0; i < 20; ++i)
		{
			leftSideType[i] = noType;
		}
		
		int listSz = 0;
		varType currType = noType;
		while(child!=NULL)
		{
			child->symTablePtr = lookupInEveryAncestor(child, st, stStack);
			if (child->symTablePtr == NULL)	//looking for ID
			{
				int errLine = child->tkn->lineNo;
				insertSymTableErr(child, errLine, 3, NULL, NULL);
				// semanticErr = 3;	//variable accessed without declaration
			}
			else
			{
				field* f = (field*)child->symTablePtr;
				child->t = f->t;
				currType = f->t;
				f->idPtr->outParamInitOrNot = 1;
			}
			child = child->next;
			leftSideType[listSz] = currType;
			listSz++;
		}
		
		currAST = currAST->next;	//RightHandSideType2
		if (currAST->sym == SIZEOP)
		{
			if (listSz != 2 || leftSideType[0] != integer || leftSideType[1] != integer)
			{
				int errLine = currAST->children->tkn->lineNo;
				insertSymTableErr(currAST->children, errLine, 8, NULL, NULL);
				// semanticErr = 8;	//no or type of IDs on left side don't match
			}
			ASTnode* child = currAST->children;	//id in size(id)
			checkSizeExprsn(child, st, stStack, matrix);
		}
		else if (currAST->sym == FUNCALL)
		{
			varType* outParamType = checkFunCallExprsn(currAST, st, stStack);
			if (outParamType != NULL)	
			{
				for (int i = 0; i < 20; ++i)
				{
					if (outParamType[i] != leftSideType[i])
					{
						int errLine = currAST->children->tkn->lineNo;
						insertSymTableErr(currAST->children, errLine, 5, NULL, NULL);
						break;
						// semanticErr = 5;	//no or type of output parameters do not match
						/* code */
					}
				}
			}
		}

	}
	else if (ast->sym == FUNCALL)
	{
		int errLine = ast->children->tkn->lineNo;
		insertSymTableErr(ast->children, errLine, 5, NULL, NULL);
		// semanticErr = 5;	//Function Called and right hand side is Nothing
	}
	return prevOffset;
	//whenever assignment stmt check if the assigned id is output, so make it 1
}

int* checkMatrixSz(ASTnode* ast)
{
	//ast is MATRIXDECL
	int* matrixSz = (int*)malloc(2*sizeof(int));
	int rows = 0;
	int cols = 0;
	int flag = 0;
	int tempCols = 0;

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
		else
		{
			tempCols = 0;
			while(child!=NULL)
			{	
				tempCols++;
				child = child->next;
			}
			if (tempCols!=cols)
			{
				int errLine = ast->children->children->tkn->lineNo;	//
				insertSymTableErr(ast->children->children, errLine, 11, NULL, NULL);
				//semanticErr = 11;	//matrix doesn't have right number of columns
				break;
			}
		}

		childRow = childRow->next;
		rows++;
	}
	if (rows > 10 || cols > 10)
	{
		int errLine = ast->children->children->tkn->lineNo;
		insertSymTableErr(ast->children->children, errLine, 12, NULL, NULL);
		//semanticErr = 12;	//no of rows or cols >10
	}
	matrixSz[0] = rows;
	matrixSz[1] = cols;
	return matrixSz;
}

varType* checkFunCallExprsn(ASTnode* ast, symTable st, symTableStack* stStack)
{
	ASTnode* currAST = ast->children;	//FUNID token
	currAST->t = function;

	//looking for function defined earlier
	field* fld = lookupInEveryAncestor(currAST, st, stStack);
	currAST->symTablePtr = fld;
	if (fld == NULL)
	{
		int errLine = currAST->tkn->lineNo;
		insertSymTableErr(currAST, errLine, 3, NULL, NULL);
		// semanticErr = 3;	//variable accessed without declaration
		return NULL;
	}

	//Recursion
	field* tempFld = lookupSymbolTable(st, currAST);
	symTableStackNode* currStackNd = stStack->first;
	while(currStackNd!=NULL && currAST->tkn!=NULL && tempFld==NULL)	//tempFld == NULL means that there does not exist case like shown below
	{
		if (strcmp(currStackNd->functionName, currAST->tkn->lexeme) == 0)	//recursion
		{
			int errLine = currAST->tkn->lineNo;
			insertSymTableErr(currAST, errLine, 10, NULL, NULL);
			//semanticErr = 10;
			break;
		}
		currStackNd = currStackNd->next;
	}
/*
	_fun1[]	//FunctionDef

		_fun1[]	//FunctionDef

		end
		_fun1()	//Function call 	--> This is allowed because this is not recursion
	end
*/
	currAST = currAST->next;
	int inputLstSz = 0;
	varType currType = noType;
	while(currAST!=NULL)
	{
		if (currAST->sym == MATRIXDECL)	//Matrix Being Declared
		{
			int* matrixSz;
			matrixSz = checkMatrixSz(currAST);	//sending MATRIX DECL
			free(matrixSz);
			currType = matrix;
		}
		else if (strcmp(currAST->tkn->tkn, "ID") == 0)
		{
			field* fld = lookupInEveryAncestor(currAST, st, stStack);
			currAST->symTablePtr = fld;
			if (fld==NULL)
			{
				int errLine = currAST->tkn->lineNo;
				insertSymTableErr(currAST, errLine, 3, NULL, NULL);
				// semanticErr = 3;	//variable accessed without declaration
			}
			else
			{
				if (currAST->children!=NULL)	//means matrixElement
				{
					int rows = (currAST->children->tkn->value).intVal;	//NUM1
					int cols = (currAST->children->next->tkn->value).intVal;	//NUM2
					if (rows <= 0 || cols <= 0 || rows > fld->idPtr->sz[0] || cols > fld->idPtr->sz[1])
					{
						int errLine = currAST->tkn->lineNo;
						insertSymTableErr(currAST, errLine, 17, NULL, NULL);
						// semanticErr = 17;	//matrix index out of bound exception
					}	
					currType = integer;
				}
				else 	//can be any type
				{
					currType = fld->t;
				}
				currAST->t = fld->t;
			}
		}
		else if (strcmp(currAST->tkn->tkn, "NUM") == 0)
		{
			currType = integer;
		}
		else if (strcmp(currAST->tkn->tkn, "RNUM") == 0)
		{
			currType = real;
		}
		else if (strcmp(currAST->tkn->tkn, "STR") == 0)
		{
			currType = string;
		}

		if (fld->funcPtr->inParamType[inputLstSz] != currType)
		{
			int errLine = ast->children->tkn->lineNo;
			insertSymTableErr(ast->children, errLine, 9, NULL, NULL);
			break;
			// semanticErr = 9;	//no or type of input parameters are not same
		}
		inputLstSz++;
		currAST = currAST->next;
	}
	if (inputLstSz != fld->funcPtr->inpParamListSz)
	{
		int errLine = ast->children->tkn->lineNo;
		insertSymTableErr(ast->children, errLine, 9, NULL, NULL);
		// semanticErr = 9;	//no of input parameters are not same
	}
	return fld->funcPtr->outParamType;
}

retType* checkArithExprsn(ASTnode* ast, symTable st, symTableStack* stStack)
{
	if (ast->sym == MATRIXDECL)	//Matrix Being Declared
	{
		retType* rt = (retType*)malloc(sizeof(retType));
		rt->sz[0] = 0;
		rt->sz[1] = 0;

		int* matrixSz;
		matrixSz = checkMatrixSz(ast);	//sending MATRIX DECL
		rt->t = matrix;
		rt->sz[0] = matrixSz[0];
		rt->sz[1] = matrixSz[1];
		free(matrixSz);

		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "ID") == 0)
	{
		retType* rt = (retType*)malloc(sizeof(retType));
		rt->sz[0] = 0;
		rt->sz[1] = 0;
		rt->t = noType;

		field* fld = lookupInEveryAncestor(ast, st, stStack);
		ast->symTablePtr = fld;
		if (fld==NULL)
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 3, NULL, NULL);
			// semanticErr = 3;	//variable accessed without declaration
		}
		else
		{
			if (ast->children!=NULL)	//means matrixElement
			{
				int rows = (ast->children->tkn->value).intVal;	//NUM1
				int cols = (ast->children->next->tkn->value).intVal;	//NUM2
				if (rows <= 0 || cols <= 0 || rows > fld->idPtr->sz[0] || cols > fld->idPtr->sz[1])
				{
					int errLine = ast->tkn->lineNo;
					insertSymTableErr(ast, errLine, 17, NULL, NULL);
					// semanticErr = 17;	//matrix index out of bound exception
				}	
				rt->t = integer;
			}
			else 	//can be any type
			{
				rt->sz[0] = fld->idPtr->sz[0];
				rt->sz[1] = fld->idPtr->sz[1];
				rt->t = fld->t;
			}
			ast->t = fld->t;
		}

		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "NUM") == 0)
	{
		retType* rt = (retType*)malloc(sizeof(retType));
		rt->sz[0] = 0;
		rt->sz[1] = 0;
		rt->t = integer;
		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "RNUM") == 0)
	{
		retType* rt = (retType*)malloc(sizeof(retType));
		rt->sz[0] = 0;
		rt->sz[1] = 0;
		rt->t = real;
		return rt;
	}
	else if (strcmp(ast->tkn->tkn, "STR") == 0)
	{
		retType* rt = (retType*)malloc(sizeof(retType));
		rt->sz[0] = 0;
		rt->sz[1] = 0;
		rt->t = string;
		rt->sz[0] = strlen(ast->tkn->lexeme);	//string itself
		return rt;
	}
	
	else if (strcmp(ast->tkn->tkn, "PLUS") == 0)
	{
		retType* rtype1 = checkArithExprsn(ast->children, st, stStack);	//child1
		retType* rtype2 = checkArithExprsn(ast->children->next, st, stStack);	//child2

		varType type1 = rtype1->t;
		varType type2 = rtype2->t;

		if (type1 != type2)
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 18, NULL, NULL);
			//semanticErr = 18;	//type mmismatch error
			// rtype1->t = integer;
			return rtype1;	//can return anything
		}

		//type1 == type2
		if (type1 == integer)
		{
			rtype1->t = integer;
			return rtype1;	
		}
		else if(type1 == real)
		{
			rtype1->t = real;
			return rtype1;	
		}
		else if(type1 == matrix)	//type1 is matrix
		{
			if (rtype1->sz[0] != rtype2->sz[0] || rtype1->sz[1] != rtype2->sz[1])
			{
				int errLine = ast->tkn->lineNo;
				insertSymTableErr(ast, errLine, 22, NULL, NULL);
				//semanticErr = 22;	//Matrix Sizes not equal
			}
			rtype1->t = matrix;
			return rtype1;	//can return anything
		}
		else 	//type1 is string
		{
			if ((rtype1->sz[0] + rtype2->sz[0]) > 20)
			{
				int errLine = ast->tkn->lineNo;
				insertSymTableErr(ast, errLine, 23, NULL, NULL);
				//semanticErr = 23;	//String size greater than 20 after concatenation
				rtype1->sz[0] = 20;
			}
			else
			{
				rtype1->sz[0] = rtype1->sz[0] + rtype2->sz[0];
			}

			rtype1->t = string;
			return rtype1;
		}

	}
	else if (strcmp(ast->tkn->tkn, "MINUS") == 0)
	{
		retType* rtype1 = checkArithExprsn(ast->children, st, stStack);	//child1
		retType* rtype2 = checkArithExprsn(ast->children->next, st, stStack);	//child2

		varType type1 = rtype1->t;
		varType type2 = rtype2->t;

		if (type1 != type2)
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 18, NULL, NULL);
			//semanticErr = 18;	//type mmismatch error
			// rtype1->t = integer;
			return rtype1;	//can return anything
		}

		//type1 == type2
		if (!(type1 == integer || type1 == real || type1 == matrix))	//i.e. neither integer or real or matrix
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 21, NULL, NULL);
			//semanticErr = 21;	//Invalid type in MINUS operation
			rtype1->t = string;
			return rtype1;	//can return anything
		}

		if (type1 == integer)
		{
			rtype1->t = integer;
			return rtype1;	
		}
		else if(type1 == real)
		{
			rtype1->t = real;
			return rtype1;	
		}
		else 	//type1 is matrix
		{
			if (rtype1->sz[0] != rtype2->sz[0] || rtype1->sz[1] != rtype2->sz[1])
			{
				int errLine = ast->tkn->lineNo;
				insertSymTableErr(ast, errLine, 22, NULL, NULL);
				//semanticErr = 22;	//Matrix Sizes not equal
			}
			rtype1->t = matrix;
			return rtype1;	//can return anything
		}
	}
	else if (strcmp(ast->tkn->tkn, "MUL") == 0 )
	{
		retType* rtype1 = checkArithExprsn(ast->children, st, stStack);	//child1
		retType* rtype2 = checkArithExprsn(ast->children->next, st, stStack);	//child2

		varType type1 = rtype1->t;
		varType type2 = rtype2->t;

		if (type1 != type2)
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 18, NULL, NULL);
			//semanticErr = 18;	//type mmismatch error
			// rtype1->t = integer;
			return rtype1;	//can return anything
		}

		//type1 == type2
		if (!(type1 == integer || type1 == real))	//i.e. neither integer nor real
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 20, NULL, NULL);
			//semanticErr = 20;	//Invalid type in MUL operation
			// rtype1->t = integer;
			return rtype1;	//can return anything
		}

		if (type1 == integer)
		{
			rtype1->t = integer;
			return rtype1;	
		}
		else
		{
			rtype1->t = real;
			return rtype1;	
		}
		
	}
	else if (strcmp(ast->tkn->tkn, "DIV") == 0)
	{
		retType* rtype1 = checkArithExprsn(ast->children, st, stStack);	//child1
		retType* rtype2 = checkArithExprsn(ast->children->next, st, stStack);	//child2

		varType type1 = rtype1->t;
		varType type2 = rtype2->t;

		if (type1 != type2)
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 18, NULL, NULL);
			//semanticErr = 18;	//type mmismatch error
			rtype1->t = real;
			return rtype1;
		}

		//type1 == type2
		if (!(type1 == integer || type1 == real))	//i.e. neither integer nor real
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 19, NULL, NULL);
			//semanticErr = 19;	//Invalid type in DIV operation
		}
		rtype1->t = real;
		return rtype1;
	}
}

void checkSizeExprsn(ASTnode* ast, symTable st, symTableStack* stStack, varType t)
{
	if (strcmp(ast->tkn->tkn, "ID") != 0)
	{
		int errLine = ast->tkn->lineNo;
		insertSymTableErr(ast, errLine, 6, NULL, NULL);
		// semanticErr = 6;	//size operator has wrong input, i.e. neither of string or matrix
	}
	ast->symTablePtr = lookupInEveryAncestor(ast, st, stStack);
	if (ast->symTablePtr == NULL)
	{
		int errLine = ast->tkn->lineNo;
		insertSymTableErr(ast, errLine, 3, NULL, NULL);
		// semanticErr = 3;	//variable accessed without declaration
		return;
	}
	else
	{
		field* f = (field*)ast->symTablePtr;
		ast->t = f->t;
	}
	if (ast->t != string && ast->t != matrix)
	{
		int errLine = ast->tkn->lineNo;
		insertSymTableErr(ast, errLine, 6, NULL, NULL);
		// semanticErr = 6;	//size operator has wrong input, i.e. neither of string or matrix
	}
	else
	{
		if (ast->t!=t)
		{
			int errLine = ast->tkn->lineNo;
			insertSymTableErr(ast, errLine, 7, NULL, NULL);
			// semanticErr = 7;	//right hand side is wrong	, i,e not equal to left side, i.e. varType sent parameter is not same
		}
	}
}

void checkBoolExprsn(ASTnode* ast, symTable st, symTableStack* stStack)
{
	if (ast->tkn == NULL)	//means BOOL NOT
	{
		checkBoolExprsn(ast->children, st, stStack);
	}
	else if (strcmp(ast->tkn->tkn, "AND") == 0 || strcmp(ast->tkn->tkn, "OR") == 0)
	{
		checkBoolExprsn(ast->children, st, stStack);
		checkBoolExprsn(ast->children->next, st, stStack);
	}	
	else //relational Op
	{
		ASTnode* currAST = ast->children;
		varType prevType, currType;	//type of both children
		int flag = 0;
		while(currAST!=NULL)
		{
			if (!flag)
			{
				if (strcmp(currAST->tkn->tkn, "ID") == 0)
				{
					field* fld = lookupInEveryAncestor(currAST, st, stStack);
					currAST->symTablePtr = fld;
					if (fld == NULL)
					{
						int errLine = currAST->tkn->lineNo;
						insertSymTableErr(currAST, errLine, 3, NULL, NULL);
						// semanticErr = 3;	//variable accessed without declaration
					}
					else
					{
						currAST->t = fld->t;
						if (!(currAST->t == integer || currAST->t == real))
						{
							int errLine = currAST->tkn->lineNo;
							insertSymTableErr(currAST, errLine, 15, NULL, NULL);
							// semanticErr = 15;	//Invalid type in boolean.
						}
						prevType = currAST->t;
					}
				}
				else if (strcmp(currAST->tkn->tkn, "NUM") == 0)
				{
					prevType = integer;
				}
				else if (strcmp(currAST->tkn->tkn, "RNUM") == 0)
				{
					prevType = real;
				}
				flag = 1;
			}
			else
			{
				if (strcmp(currAST->tkn->tkn, "ID") == 0)
				{
					field* fld = lookupInEveryAncestor(currAST, st, stStack);
					currAST->symTablePtr = fld;
					if (fld == NULL)
					{
						int errLine = currAST->tkn->lineNo;
						insertSymTableErr(currAST, errLine, 3, NULL, NULL);
						// semanticErr = 3;	//variable accessed without declaration
					}
					else
					{
						currAST->t = fld->t;
						if (!(currAST->t == integer || currAST->t == real))
						{
							int errLine = currAST->tkn->lineNo;
							insertSymTableErr(currAST, errLine, 15, NULL, NULL);
							// semanticErr = 15;	//Invalid type in boolean.
						}
						currType = currAST->t;
					}
				}
				else if (strcmp(currAST->tkn->tkn, "NUM") == 0)
				{
					currType = integer;
				}
				else if (strcmp(currAST->tkn->tkn, "RNUM") == 0)
				{
					currType = real;
				}

				if (prevType != currType)	
				{
					int errLine = currAST->tkn->lineNo;
					insertSymTableErr(currAST, errLine, 14, NULL, NULL);
					// semanticErr = 14;	//type mismatch error in boolean
				}
			}
			currAST = currAST->next;
		}
	}
}

void checkOutputParamAssignedVal(symTable st)
{
	for (int i = 0; i < 43; ++i)
	{
		if (st[i].first==NULL)
		{
			continue;
		}
		field* fld = st[i].first;
		while(fld!=NULL)
		{
			if (fld->funcPtr!=NULL)
			{
				symTable childSt = (symTable)fld->funcPtr->childSymTablePtr;
				int* outIndices = fld->funcPtr->outIndices;
				int outParamListSz = fld->funcPtr->outParamListSz;
				for (int j = 0; j < outParamListSz; ++j)
				{
					field* childFld = childSt[outIndices[j]].first;
					while(childFld!=NULL)
					{
						if (childFld->idPtr!=NULL)
						{
							if (childFld->idPtr->outParamInitOrNot!=1)	//output parameter not initialised within that function
							{
								int errLine = childFld->tkn->lineNo;
								insertSymTableErr(NULL, errLine, 13, childFld->tkn->lexeme, childFld->funcScopeName);
								//semanticErr = 13;
							}
						}
						childFld = childFld->next;
					}
				}
				checkOutputParamAssignedVal(childSt);
			}
			fld = fld->next;
		}
	}
}

int findSz(varType t)	//gives size of integer and real, will calculate size of matrix and string later on
{
	if (t==integer)
	{
		return 2;
	}
	else if (t==real)
	{
		return 4;
	}
	else
	{
		return 0;
	}
}

void insertSymTableErr(ASTnode* ast, int errLine, int semanticErr, char* id, char* funId)
{
	//char* id and funId for semanticErr = 13


	if (ast==NULL || ast->tkn == NULL)
	{
		if (semanticErr!=13)	//outParamInitOrNot kind of Error
		{
			return;
		}
	}

	semanticErrExist = 1;
	errors* er = (errors*)malloc(sizeof(errors));
	er->lineNo = errLine;
	char str[2048];
	switch(semanticErr)
	{
		case 1:	strcpy(str, "The function ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " already declared in the same scope.");
				break;
		case 2:	strcpy(str, "The identifier ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " already declared in the same scope.");
				break;
		case 3:	strcpy(str, "The identifier ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " accessed without declaration.");
				break;
		case 4:	strcpy(str, "Invalid identifier ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " in read().");
				break;
		case 5:	strcpy(str, "The function ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " call does not match with number or type of output parameters expected.");
				break;
		case 6:	strcpy(str, "Invalid identifier ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " in SIZEOP.");
				break;
		case 7:	strcpy(str, "Invalid identifier ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, " on right hand side having type as ");
				strcat(str, enumToString3(ast->t));
				strcat(str, ".");
				break;
		case 8:	strcpy(str, "The number or type of identifiers on left side do not match.");
				break;
		case 9:	strcpy(str, "The number or type of input parameters do not match in function call ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, ".");
				break;
		case 10: strcpy(str, "Recursion not allowed. Function which is recursively called is: ");
				strcat(str, ast->tkn->lexeme);
				strcat(str, ".");
				break;
		case 11:	strcpy(str, "Size of cloumns don't match with each othe, which violates the matrix size rule.");
				break;
		case 12:	strcpy(str, "Size of rows or columns greater than 10, which violates the language.");
				break;
		case 13:	strcpy(str, "Output Parameter ");
				strcat(str, id);
				strcat(str, " not initialised in function definition ");
				strcat(str, funId);
				strcat(str, ".");
				break;
		case 14:	strcpy(str, "Type mismatch error in boolean.");
				break;
		case 15:	strcpy(str, "Boolean does not allow ");
				strcat(str, enumToString3(ast->t));
				strcat(str, " type in its statements.");
				break;
		case 16:	strcpy(str, "Type: ");
				strcat(str, enumToString3(ast->t));
				strcat(str, " mismatch error on lefthand side.");
				break;
		case 17:	strcpy(str, "Matrix Index out of bound exception.");
				break;
		case 18:	strcpy(str, "Type mismatch error in arithmetic expression.");
				break;
		case 19:	strcpy(str, "Invalid type in DIV operation.");
				break;
		case 20:	strcpy(str, "Invalid type in MUL operation.");
				break;
		case 21:	strcpy(str, "Invalid type in MINUS operation.");
				break;
		case 22:	strcpy(str, "Matrix Sizes are not equal.");
				break;
		case 23:	strcpy(str, "String size greater than 20 after concatenation.");
				break;
		case 24:	strcpy(str, "Can not reassign matrix or string to different size matrix or string.");
				break;
		default:
			break;
	}

	strcpy(er->errStr, str);
	er->next = NULL;
	if(symTableErr->first==NULL)
	{
		symTableErr->first = er;
		symTableErr->last = er;
		return;
	}
	symTableErr->last->next = er;
	symTableErr->last = er;
}

void printSemanticErrs()
{
	errors* er1 = symTableErr->first;
	printf("\n-------Errors which correspond to <Output parameters not initialised inside function Definition> are shown at last along with their line no.---------\n");
	while(er1!=NULL)
	{
		printf("\n%d: Semantic Error: %s\n", er1->lineNo, er1->errStr);
		er1 = er1->next;
	}

}

//left associativity