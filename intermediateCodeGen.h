//Deepak Guneja
//2015A7PS0083P

#include "intermediateCodeGenDef.h"

void intCodeGen(ASTnode* ast, symTable st);

void checkBoolExprsnIntCode(ASTnode* ast, symTable st);

ASTnode* createTempLabelASTnode();

ASTnode* createASTNodeForValue(int val);	//this node is NUM node with value size

ASTnode* createTempASTNode(int* sz, varType t, symTable st);	//This is to create a temporary ASTnode to assign something, this node has size = sz, and type = t

int* checkMatrixSzIntCode(ASTnode* ast);

char* newTemp();

char* newLabel();

intCode* createQuadruple(ASTnode* result, operatorSym op, ASTnode* arg1, ASTnode* arg2);

int calculateMatrixElemOffset(int offset, int dim1, int dim2, int i, int j);

retTypeIntCode* checkArithExprsnForIntCode(ASTnode* ast, symTable st);

void printQuadruples(ASTnode* ast);

char* enumToString4(operatorSym op);
