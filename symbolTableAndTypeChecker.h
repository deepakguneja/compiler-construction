//Deepak Guneja
//2015A7PS0083P
#include "symbolTableAndTypeCheckerDef.h"

symTable createSymTable();

int calculateSymTableIndex(char* lexeme);

int insertSymTableNode(symTable st, field* fld);

field* lookupSymbolTable(symTable st, ASTnode* ast);

field* lookupInEveryAncestor(ASTnode* ast, symTable st, symTableStack* stStack);

void printAllSymTables(symTable st, int nestingLevel, char* parentOfCurrScope);

symTableStackNode* createSymTableStackNode(symTable st, char* functionName);

void pushSymTableStackNode(symTableStack* stStack, symTableStackNode* stackNd);

void popSymTableStackNode(symTableStack* stStack);

functionField* createFuncField(symTable childSymTablePtr, int inpParamListSz, int outParamListSz, int* inpIndices, int* outIndices, varType* inParamType, varType* outParamType);

idField* createIdField(int* sz, int outParamInitOrNot, int offset);

field* createField(token* tkn, char* funcScopeName, varType t, functionField* funcPtr, idField* idPtr);

int initSymTables(ASTnode* ast, symTable st, symTableStack* stStack, int prevOffset);

int* checkMatrixSz(ASTnode* ast);

varType* checkFunCallExprsn(ASTnode* ast, symTable st, symTableStack* stStack);

retType* checkArithExprsn(ASTnode* ast, symTable st, symTableStack* stStack);

void checkSizeExprsn(ASTnode* ast, symTable st, symTableStack* stStack, varType t);

void checkBoolExprsn(ASTnode* ast, symTable st, symTableStack* stStack);

void checkOutputParamAssignedVal(symTable st);

int findSz(varType t);

void insertSymTableErr(ASTnode* ast, int errLine, int semanticErr, char* id, char* funId);

void printSemanticErrs();
