//Deepak Guneja
//2015A7PS0083P

#include "parser.c"
enum ASTnodeSym {_MAIN, FUNCTION_DEF, DECL, ASSIGN_SINGLE_VAR, ASSIGN_LIST_VAR, SIZEOP, IFSTMT, ELSESTMT, IOREAD, IOPRINT, FUNCALL, BOOL_NOT, MATRIXDECL, STMTS, IN_PARAM, OUT_PARAM, TYPE_ID, LIST_VAR, ROW
};

typedef enum ASTnodeSym ASTnodeSym;

struct
{
	ASTnodeSym sym;
	char* str;
} convert2[] = {
	{_MAIN, "_MAIN"}, {FUNCTION_DEF, "FUNCTION_DEF"}, {DECL, "DECL"}, {ASSIGN_SINGLE_VAR, "ASSIGN_SINGLE_VAR"}, {ASSIGN_LIST_VAR, "ASSIGN_LIST_VAR"}, {SIZEOP, "SIZEOP"}, {IFSTMT, "IFSTMT"}, {ELSESTMT, "ELSESTMT"}, {IOREAD, "IOREAD"}, {IOPRINT, "IOPRINT"}, {FUNCALL, "FUNCALL"}, {BOOL_NOT, "BOOL_NOT"}, {MATRIXDECL, "MATRIXDECL"}, {STMTS, "STMTS"}, {IN_PARAM, "IN_PARAM"}, {OUT_PARAM, "OUT_PARAM"}, {TYPE_ID, "TYPE_ID"}, {LIST_VAR, "LIST_VAR"}, {ROW, "ROW"}
};

enum varType{integer, real, string, matrix, function, boolean, noType};
//noType is used in the function checkFunCallExprsn
typedef enum varType varType;

struct
{
	varType t;
	char* str;
} convert3[] = {
	{integer, "int"}, {real, "real"}, {string, "string"}, {matrix, "matrix"}, {function, "function"}, {boolean, "boolean"}, {noType, "noType"}
};

struct ASTnode
{
	struct ASTnode* parent;
	struct ASTnode* next;
	struct ASTnode* children;
	int symValid;	//if the node has valid symbol or not: symValid = 1
	ASTnodeSym sym;
	token* tkn;	//if symbol is not valid, means it is a terminal so pointer to its
	void* symTablePtr;	//pointer to entry in symbol symTablePtr,i.e. pointer to field
	void* code;	//pointer to intermediate code generation
	struct ASTnode* addr;	//addr for intermediate code generation
	struct ASTnode* trueLabel;	//these all are for if-else intermediate code generation
	struct ASTnode* falseLabel;
	struct ASTnode* nextLabel;
	varType t;
};

typedef struct ASTnode ASTnode;