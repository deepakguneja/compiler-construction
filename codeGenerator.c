//Deepak Guneja
//2015A7PS0083P

#include "codeGenerator.h"

void initializeCodeGen(symTable st, intCode* code, FILE* fp)
{
	fprintf(fp, "%cinclude 'readPrint.asm'\n",'%');
	fprintf(fp, "section .bss\n");
	fprintf(fp, "newLine RESB 1\n");
	fprintf(fp, "tempIp\tRESB\t20\n");

	int reserveOffset = 2*offsetForIntCode;
	
	fprintf(fp, "stackBase\tRESB\t%d", reserveOffset);
	fprintf(fp, "\n");
	fprintf(fp, "section .text\n");
	fprintf(fp, "global _start\n");
	fprintf(fp, "_start:\n\n");
	fprintf(fp, "MOV EBP, stackBase\n");
	fprintf(fp, "MOV AL, 0AH\n");
	fprintf(fp, "MOV EBX, newLine\n");
	fprintf(fp, "MOV [EBX], AL\n");

	intCode* currCode = code;
	while(currCode != NULL)
	{
		createCode(st, currCode, fp);
		currCode = currCode->next;
	}

	fprintf(fp, "MOV EBX, 0\n");
	fprintf(fp, "MOV EAX, 1\n");
	fprintf(fp, "INT 80H\n");
}

void createCode(symTable st, intCode* code, FILE* fp)
{
	operatorSym op = code->op;
	switch(op)
	{
		case ASSIGN:	codeAssign(st, code, fp);
			fprintf(fp, "\n");
			break;

		case LESSTHAN:	codeRelOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case LESSTHANEQ:	codeRelOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case GREATERTHAN:	codeRelOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case GREATERTHANEQ:	codeRelOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case NOTEQUAL:	codeRelOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case ISEQUAL:	codeRelOp(st, code, fp);
			fprintf(fp, "\n");
			break;

		case PLUSOP:	codeArithOp(st, code, fp);
			fprintf(fp, "\n");
			break;

		case MINUSOP:	codeArithOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case MULOP:	codeArithOp(st, code, fp);
			fprintf(fp, "\n");
			break;


		case DIVOP:	printf("NASM does not support REAL operation. So, correct Code will not be generated. Change the input file!\n\n");
			fprintf(fp, "\n");
			break;


		case READOP:	codeForRead(st, code, fp);
			fprintf(fp, "\n");
			break;


		case PRINTOP:	codeForPrint(st, code, fp);
			fprintf(fp, "\n");
			break;

		
		case VALUEAT:	codeForValueAt(st, code, fp);
			fprintf(fp, "\n");
			break;

		case ADDRESSOFFSET:	codeForAddrOffset(st, code, fp);
			fprintf(fp, "\n");
			break;


		case LABELOP:	fprintf(fp, "%s:\n", code->result->tkn->lexeme);
			fprintf(fp, "\n");
			break;

		
		case GOTO:	fprintf(fp, "JMP %s\n", code->result->tkn->lexeme);
			fprintf(fp, "\n");
			break;

		default:
			break;
	}
}

void codeForValueAt(symTable st, intCode* code, FILE* fp)
{
	field* fld = (field*)code->arg1->symTablePtr;
	fprintf(fp, "MOV EBX, [EBP + %d]\n", fld->idPtr->offset);
	fprintf(fp, "ADD EBX, EBP\n");
	fprintf(fp, "MOV AX, [EBX]\n");
	fld = (field*)code->result->symTablePtr;
	fprintf(fp, "MOV [EBP + %d], AX\n", fld->idPtr->offset);
}

void codeForAddrOffset(symTable st, intCode* code, FILE* fp)
{
	fprintf(fp, "MOV AX, %d\n", (int)code->arg1->tkn->value.intVal);
	field* fld = (field*)code->result->symTablePtr;
	fprintf(fp, "MOV [EBP + %d], AX\n", fld->idPtr->offset);
}

void codeForPrint(symTable st, intCode* code, FILE* fp)
{
	field* fld = (field*)code->result->symTablePtr;

	if (fld->t == string)
	{
		fprintf(fp, "MOV EDX, %d\n", fld->idPtr->sz[0]);
		fprintf(fp, "MOV ECX, EBP\n");
		fprintf(fp, "ADD ECX, %d\n", fld->idPtr->offset);
		fprintf(fp, "MOV EBX, 1\n");
		fprintf(fp, "MOV EAX, 4\n");
		fprintf(fp, "INT 80H\n");

		fprintf(fp, "MOV EDX, 1\n");
		fprintf(fp, "MOV ECX, newLine\n");
		fprintf(fp, "MOV EBX, 1\n");
		fprintf(fp, "MOV EAX, 4\n");
		fprintf(fp, "INT 80H\n");
	}
	else if (fld->t == integer)
	{
		fprintf(fp, "XOR EAX, EAX\n");
		fprintf(fp, "MOV AX,[EBP + %d]\n", fld->idPtr->offset);
		fprintf(fp, "CALL iprintLF\n");
	}
}

void codeForRead(symTable st, intCode* code, FILE* fp)
{
	field* fld = (field*)code->result->symTablePtr;
	if (fld->t == integer)
	{
		fprintf(fp, "MOV EDX, 20\n");
		fprintf(fp, "MOV ECX, tempIp\n");
		fprintf(fp, "MOV EBX, 0\n");
		fprintf(fp, "MOV EAX, 3\n");
		fprintf(fp, "INT 80H\n");

		fprintf(fp, "MOV EAX, tempIp\n");
		fprintf(fp, "CALL atoi\n");
		fprintf(fp, "MOV [EBP + %d], AX\n",fld->idPtr->offset);
	}
}

void codeRelOp(symTable st, intCode* code, FILE* fp)
{
	operatorSym op = code->op;
	char jumpSign[20];
	switch(op)
	{
		case LESSTHAN:	strcpy(jumpSign, "JL");
			break;


		case LESSTHANEQ:	strcpy(jumpSign, "JLE");
			break;


		case GREATERTHAN:	strcpy(jumpSign, "JG");
			break;


		case GREATERTHANEQ:	strcpy(jumpSign, "JGE");
			break;


		case NOTEQUAL:	strcpy(jumpSign, "JNE");
			break;


		case ISEQUAL:	strcpy(jumpSign, "JE");
			break;

		default: 
			break;
	}

	field* fld = (field*)code->arg1->symTablePtr;
	if(strcmp(code->arg1->tkn->tkn, "ID") == 0)		//child1 is ID because I have created a temp for each NUM
	{
		fprintf(fp, "MOV AX,[EBP + %d]\n",fld->idPtr->offset);
	}

	fld = (field*)code->arg2->symTablePtr;
	if(strcmp(code->arg2->tkn->tkn, "ID") == 0)		//child2 is ID
	{
		fprintf(fp, "MOV BX,[EBP + %d]\n", fld->idPtr->offset);
	}

	fprintf(fp, "CMP AX, BX\n");
	fprintf(fp, "%s %s\n", jumpSign, code->result->tkn->lexeme);

}

void codeAssign(symTable st, intCode* code, FILE* fp)
{
	field* fld = (field*)code->result->symTablePtr;
	int offset = fld->idPtr->offset;

	if (code->arg1->sym == MATRIXDECL)	//matrix directly assigned
	{
		ASTnode* childRow = code->arg1->children;	//ROW -> Dummy Node
		while(childRow!=NULL)
		{
			ASTnode* child = childRow->children;	//NUM in ROW starts
			while(child!=NULL)
			{	
				fprintf(fp, "MOV AX, %d\n",(int)child->tkn->value.intVal);
				fprintf(fp, "MOV [EBP + %d], AX\n", offset);
				offset += 2;
				child = child->next;
			}

			childRow = childRow->next;
		}
	}
	else if(strcmp(code->arg1->tkn->tkn, "NUM") == 0)	//integer directly assigned
	{
		// printf("NUM\n");
		fprintf(fp, "MOV AX, %d\n", (int)code->arg1->tkn->value.intVal);
		fprintf(fp, "MOV [EBP + %d], AX\n",offset);
	}
	else if (strcmp(code->arg1->tkn->tkn, "STR") == 0)	//string drectly assigned
	{
		char* lexeme = code->arg1->tkn->lexeme;
		int len = strlen(lexeme);
		for(int i = 0; i < len; ++i)
		{
			fprintf(fp, "MOV AL, '%c'\n", lexeme[i]);
			fprintf(fp, "MOV [EBP + %d], AL\n", offset);
			offset++;
		}
	}
	else if (strcmp(code->arg1->tkn->tkn, "ID") == 0)
	{
		fld = (field*)code->arg1->symTablePtr;

		if (fld->t == integer)
		{
			fprintf(fp, "MOV AX, [EBP + %d]\n", fld->idPtr->offset);
			fprintf(fp, "MOV [EBP + %d], AX\n", offset);		
		}
		else if (fld->t == string)
		{
			copyStringFrmID(st, code, fp, offset);
		}
		else if (fld->t = matrix)
		{
			copyMatrixFrmID(st, code, fp);
		}
	}
}

void copyStringFrmID(symTable st, intCode* code, FILE* fp, int offsetToStr)
{
	field* fld1 = (field*)code->arg1->symTablePtr;
	int offsetFrmStr = fld1->idPtr->offset;

	field* fld2 = (field*)code->result->symTablePtr;
	// int offsetToStr = fld2->idPtr->offset;

	for(int i = 0; i < fld1->idPtr->sz[0]; ++i)
	{
		fprintf(fp, "MOV AL, [EBP + %d]\n", offsetFrmStr);
		fprintf(fp, "MOV [EBP + %d], AL\n", offsetToStr);
		offsetFrmStr++;
		offsetToStr++;
	}
}

void copyMatrixFrmID(symTable st, intCode* code, FILE* fp)
{
	field* fld1 = (field*)code->arg1->symTablePtr;
	int offsetFrmMatrix = fld1->idPtr->offset;

	field* fld2 = (field*)code->result->symTablePtr;
	int offsetToMatrix = fld2->idPtr->offset;

	for(int i = 0; i < fld1->idPtr->sz[0]*fld1->idPtr->sz[0]; ++i)
	{
		fprintf(fp, "MOV AX, [EBP + %d]\n", offsetFrmMatrix);
		fprintf(fp, "MOV [EBP + %d], AX\n", offsetToMatrix);
		offsetFrmMatrix += 2;
		offsetToMatrix += 2;
	}
}

void codeArithOp(symTable st, intCode* code, FILE* fp)
{
	operatorSym op = code->op;
	field* fld = (field*)code->result->symTablePtr;
	field* fld1 = (field*)code->arg1->symTablePtr;
	field* fld2 = (field*)code->arg2->symTablePtr;

	int offsetResult = fld->idPtr->offset;
	int offset1 = fld1->idPtr->offset;
	int offset2 = fld2->idPtr->offset;

	if (op == PLUSOP)
	{
		if (fld->t == integer)
		{
			fprintf(fp, "MOV AX, [EBP+%d]\n", offset1);
			fprintf(fp, "MOV BX, [EBP+%d]\n", offset2);
			fprintf(fp, "ADD AX, BX\n");
			fprintf(fp, "MOV [EBP+%d], AX\n", offsetResult);
		}
		else if (fld->t == matrix)
		{
			for(int i = 0; i < fld1->idPtr->sz[0]*fld1->idPtr->sz[1]; ++i)
			{
				fprintf(fp, "MOV AX, [EBP + %d]\n", offset1);
				fprintf(fp, "MOV BX, [EBP + %d]\n", offset2);
				fprintf(fp, "ADD AX, BX\n");
				fprintf(fp, "MOV [EBP + %d], AX\n", offsetResult);

				offset1 += 2;
				offset2 += 2;
				offsetResult += 2;
			}
		}
		else if (fld->t == string)
		{
			int lenOfStr1 = fld1->idPtr->sz[0];
			copyStringFrmID(st, code, fp, offsetResult);
			copyStringFrmID(st, code, fp, offsetResult + lenOfStr1);
		}
	}
	else if (op == MINUSOP)
	{
		if (fld->t == integer)
		{
			fprintf(fp, "MOV AX, [EBP+%d]\n", offset1);
			fprintf(fp, "MOV BX, [EBP+%d]\n", offset2);
			fprintf(fp, "SUB AX, BX\n");
			fprintf(fp, "MOV [EBP+%d], AX\n", offsetResult);
		}
		else if (fld->t == matrix)
		{
			for(int i = 0; i < fld1->idPtr->sz[0]*fld1->idPtr->sz[1]; ++i)
			{
				fprintf(fp, "MOV AX, [EBP + %d]\n", offset1);
				fprintf(fp, "MOV BX, [EBP + %d]\n", offset2);
				fprintf(fp, "SUB AX, BX\n");
				fprintf(fp, "MOV [EBP + %d], AX\n", offsetResult);

				offset1 += 2;
				offset2 += 2;
				offsetResult += 2;
			}
		}
	}
	else if (op == MULOP)
	{
		if (fld->t == integer)
		{
			fprintf(fp, "MOV AX, [EBP+%d]\n", offset1);
			fprintf(fp, "MOV BX, [EBP+%d]\n", offset2);
			fprintf(fp, "MUL BX\n");
			fprintf(fp, "MOV [EBP+%d], AX\n", offsetResult);
		}
	}
}