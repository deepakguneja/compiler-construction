//Deepak Guneja
//2015A7PS0083P


//Fin means Final State
//Int means Intermediate State
enum state{start, assignFin, eqFin, neInt1, neFin, comment, funcStart, funcFin, idStart, idFin, numStart, rnumInt1, rnumInt2, rnumFin, strStart, strInt1, strFin, leftSq, rightSq, opPar, clPar, semiCol, comma, plus, minus, mul, divOp, sz, boolStart, andInt1, andInt2, andInt3, andFin, orInt1, orInt2, orFin, notInt1, notInt2, notInt3, notFin, lessThan, lessEqual, greaterThan, greaterEqual, errState};

typedef enum state state;

int lexicalErrExist = 0;	//If Lexical Error then no need to do semanticAnalyzer, i.e. lexicalErr = 1;