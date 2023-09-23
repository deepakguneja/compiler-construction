//Deepak Guneja
//2015A7PS0083P

#include "abstractSyntaxTreeDef.h"

void createAST(parseTree* treeNode);

ASTnode* initASTnode();

void newNodeMain(parseTree* treeNode);	

void newNodeFunction(parseTree* treeNode);	

void newNodeDeclaration(parseTree* treeNode);

void newNodeAssignSingleVar(parseTree* treeNode);	

void newNodeAssignListVar(parseTree* treeNode);

void newNodeSize(parseTree* treeNode);

void newNodeIf(parseTree* treeNode);

void newNodeElse(parseTree* treeNode);

void newNodeIoRead(parseTree* treeNode);

void newNodeIoPrint(parseTree* treeNode);

void newNodeFuncall(parseTree* treeNode);

void newNodeArithEx2(parseTree* treeNode);

void newNodeArithTerm2(parseTree* treeNode);

void newNodeBool1(parseTree* treeNode);

void newNodeBool2(parseTree* treeNode);

void newNodeNot(parseTree* treeNode);

void newNodeVarMatrixElem(parseTree* treeNode);

void newNodeMatrixElem(parseTree* treeNode);

void newNodeMatrix(parseTree* treeNode);

void newNodeRow(parseTree* treeNode);

void siblings1(parseTree* treeNode);	//has 2 children and need to make them as siblings

void siblings2(parseTree* treeNode);	//Only for parameter_list

void siblings3(parseTree* treeNode);	//If one children is terminal and other is NT

void siblings4(parseTree* treeNode);	//Only for ROW

void assign1(parseTree* treeNode);		//assign a single child

void assign2(parseTree* treeNode);		//Single Terminal Child

void assign3(parseTree* treeNode);		//has many children, but only single non terminal child, so only need to assign to that child

void printASTInOrder(ASTnode* ast);

void printASTPostOrder(ASTnode* ast);

void makeParentPtrInAST(ASTnode* ast);

char* enumToString2(ASTnodeSym val);

char* enumToString3(varType t);

varType stringToEnum3(char* str);