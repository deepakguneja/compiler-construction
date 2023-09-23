//Deepak Guneja
//2015A7PS0083P

#include "intermediateCodeGen.c"

void initializeCodeGen(symTable st, intCode* code, FILE* fp);

void createCode(symTable st, intCode* code, FILE* fp);

void codeForValueAt(symTable st, intCode* code, FILE* fp);

void codeForAddrOffset(symTable st, intCode* code, FILE* fp);

void codeForPrint(symTable st, intCode* code, FILE* fp);

void codeForRead(symTable st, intCode* code, FILE* fp);

void codeRelOp(symTable st, intCode* code, FILE* fp);

void codeAssign(symTable st, intCode* code, FILE* fp);

void copyStringFrmID(symTable st, intCode* code, FILE* fp, int offsetToStr);

void copyMatrixFrmID(symTable st, intCode* code, FILE* fp);

void codeArithOp(symTable st, intCode* code, FILE* fp);