//Deepak Guneja
//2015A7PS0083P

#include "lexer.c"
#include "parserDef.h"

symbol stringToEnum(char* str);

Grammar readGrammar(char* GrammarFile);

void insertSymInRule(Grammar G, symbol lhsSym, symbol symToInsert);

void printGrammar(Grammar G);

char* enumToString(symbol val);

void printRules(Grammar G);

FirstAndFollowSets computeFirstAndFollow(Grammar G);

unsigned int* first(FirstAndFollowSets firstFollow, int i, Grammar G);

int equal(unsigned int* set1, unsigned int* set2, int sz);

unsigned int* insertInSet(unsigned int* l, int j);

int findInSet(unsigned int* l, int j);

unsigned int* UnionExceptEpsilon(unsigned int* l1, unsigned int* l2, int sz);

unsigned int* Intersection(unsigned int* l1, unsigned int* l2, int sz);

void printFirstFollow(FirstAndFollowSets firstFollow, Grammar G);

unsigned int* follow(FirstAndFollowSets firstFollow, int i, Grammar G);

void findMidwayFirstSet(FirstAndFollowSets firstFollow, node* ptr, unsigned int* temp);

parseTable createParseTable(FirstAndFollowSets firstFollow, Grammar G);

void printParseTable(parseTable T);

stack* createAndInitialiseStack(parseTree* ptrToRootNode);

stack* push(stack* S, node* ptr, parseTree* ptrToTreeNode);

stack* pop(stack* S);

stackNode* top(stack* S);

void insertChildren(rhs* rule, parseTree* parentNode, stack* S);

parseTree* createParseTree(tokenHead* allTkns, parseTable T, Grammar G);

void printParseTree(parseTree* tree, FILE* fp);

void printParseTreeOnStdOut(parseTree* treeNode);

void insertParserErr(token* currTkn, symbol val);

void printSyntacticErrors();