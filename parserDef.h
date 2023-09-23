//Deepak Guneja
//2015A7PS0083P

enum symbol {mainFunction, stmtsAndFunctionDefs, stmtsAndFunctionDefs2, stmtOrFunctionDef, stmt, functionDef, parameter_list, type, remainingList, declarationStmt, var_list, more_ids, assignmentStmt_type1, assignmentStmt_type2, leftHandSide_singleVar, leftHandSide_listVar, rightHandSide_type1, rightHandSide_type2, sizeExpression, ifStmt, elsePart, otherStmts, ioStmt, funCallStmt, inputParameterList, listVar, arithmeticExpression, arithmeticExpression2, arithmeticTerm, arithmeticTerm2, factor, operator_lowPrecedence, operator_highPrecedence, booleanExpression, constrainedVars, var, matrixElement, matrixDeclaration, rows, rows2, row, row2, logicalOp, relationalOp, ASSIGNOP, FUNID, ID, NUM, RNUM, STR, END, INT, REAL, STRING, MATRIX, MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA, IF, ELSE, ENDIF, READ, PRINT, FUNCTION, PLUS, MINUS, MUL, DIV, SIZE, AND, OR, NOT, LT, LE, EQ, GT, GE, NE, DOLLAR, EPSILON};

typedef enum symbol symbol;

int syntacticErrExist = 0;	//If Syntactical Error then no need to proceed further, i.e. syntacticErr = 1;

struct
{
	symbol sym;
	char* str;
	int TOrNT;	//0 = Terminal, 1 = Non Terminal
} convert[] = {
	{mainFunction, "mainFunction", 1}, {stmtsAndFunctionDefs, "stmtsAndFunctionDefs", 1}, {stmtsAndFunctionDefs2, "stmtsAndFunctionDefs2", 1}, {stmtOrFunctionDef, "stmtOrFunctionDef", 1}, {stmt, "stmt", 1}, {functionDef, "functionDef", 1}, {parameter_list, "parameter_list", 1}, {type, "type", 1}, {remainingList, "remainingList", 1}, {declarationStmt, "declarationStmt", 1}, {var_list, "var_list", 1}, {more_ids, "more_ids", 1}, {assignmentStmt_type1, "assignmentStmt_type1", 1}, {assignmentStmt_type2, "assignmentStmt_type2", 1}, {leftHandSide_singleVar, "leftHandSide_singleVar", 1}, {leftHandSide_listVar, "leftHandSide_listVar", 1}, {rightHandSide_type1, "rightHandSide_type1", 1}, {rightHandSide_type2, "rightHandSide_type2", 1}, {sizeExpression, "sizeExpression", 1}, {ifStmt, "ifStmt", 1}, {elsePart, "elsePart", 1}, {otherStmts, "otherStmts", 1}, {ioStmt, "ioStmt", 1}, {funCallStmt, "funCallStmt", 1}, {inputParameterList, "inputParameterList", 1}, {listVar, "listVar", 1}, {arithmeticExpression, "arithmeticExpression", 1}, {arithmeticExpression2, "arithmeticExpression2", 1}, {arithmeticTerm, "arithmeticTerm", 1}, {arithmeticTerm2, "arithmeticTerm2", 1}, {factor, "factor", 1}, {operator_lowPrecedence, "operator_lowPrecedence", 1}, {operator_highPrecedence, "operator_highPrecedence", 1}, {booleanExpression, "booleanExpression", 1}, {constrainedVars, "constrainedVars", 1}, {var, "var", 1}, {matrixElement, "matrixElement", 1}, {matrixDeclaration, "matrixDeclaration", 1}, {rows, "rows", 1}, {rows2, "rows2", 1}, {row, "row", 1}, {row2, "row2", 1}, {logicalOp, "logicalOp", 1}, {relationalOp, "relationalOp", 1}, {ASSIGNOP, "ASSIGNOP", 0}, {FUNID, "FUNID", 0}, {ID, "ID", 0}, {NUM, "NUM", 0}, {RNUM, "RNUM", 0}, {STR, "STR", 0}, {END, "END", 0}, {INT, "INT", 0}, {REAL, "REAL", 0}, {STRING, "STRING", 0}, {MATRIX, "MATRIX", 0}, {MAIN, "MAIN", 0}, {SQO, "SQO", 0}, {SQC, "SQC", 0}, {OP, "OP", 0}, {CL, "CL", 0}, {SEMICOLON, "SEMICOLON", 0}, {COMMA, "COMMA", 0}, {IF, "IF", 0}, {ELSE, "ELSE", 0}, {ENDIF, "ENDIF", 0}, {READ, "READ", 0}, {PRINT, "PRINT", 0}, {FUNCTION, "FUNCTION", 0}, {PLUS, "PLUS", 0}, {MINUS, "MINUS", 0}, {MUL, "MUL", 0}, {DIV, "DIV", 0}, {SIZE, "SIZE", 0}, {AND, "AND", 0}, {OR, "OR", 0}, {NOT, "NOT", 0}, {LT, "LT", 0}, {LE, "LE", 0}, {EQ, "EQ", 0}, {GT, "GT", 0}, {GE, "GE", 0}, {NE, "NE", 0}, {DOLLAR, "$", 0}, {EPSILON, "EPSILON", 0}
};

struct node
{
	symbol val;
	int termOrNonTerm;	//term = 0; NonTerminal = 1
	struct node* nextSym;
};

typedef struct node node;

struct rhs
{
	node* firstSym;
	node* lastSym;
	int ruleNo;	//For 1st rule it is 1, then for each '|', it increases by 1 and also when new line comes'
	struct rhs* nextRule;
};

typedef struct rhs rhs;

struct nonTermInRule {
	node* ptr;
	symbol sym;	//Non Terminal in whose rule this Non terminal is occurring
	struct nonTermInRule* next;
};

typedef struct nonTermInRule nonTermInRule;

struct lhs {
	symbol val;
	rhs* firstRule;
	rhs* lastRule;
	nonTermInRule* firstPtr;
	nonTermInRule* lastPtr;	//pointer to the places at which this non terminal is occurring in rules, i.e. if E -> AB; A -> DE, so 'E' is occurring in A's rule at second place
};

typedef struct lhs* Grammar;

//Structure is like: (lhs)->(rhs->sym->sym->sym)->(rhs->sym->sym->sym)->(rhs->sym->sym->sym)

struct FirstAndFollow {
	unsigned int* firstSet;
	unsigned int* followSet;
};

typedef struct FirstAndFollow* FirstAndFollowSets;

int noOfRules;

struct parseTableElem
{
	rhs* rule;
	int sync;
};

typedef struct parseTableElem** parseTable;

struct parseTree
{
	symbol sym;
	token* tkn;
	int termOrNonTerm;
	struct parseTree* children;
	struct parseTree* next;
	struct parseTree* parent;
	void* addr;
	void* inh_addr;
	rhs* rule;
};

typedef struct parseTree parseTree;

struct stackNode
{
	symbol val;
	int termOrNonTerm;	//term = 0; NonTerminal = 1
	struct stackNode* next;
	parseTree* ptrToTreeNode;
};

typedef struct stackNode stackNode;

struct stack
{
	stackNode* head;
};

typedef struct stack stack;

