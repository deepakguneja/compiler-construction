//Deepak Guneja
//2015A7PS0083P

#include "parser.h"

Grammar readGrammar(char* GrammarFile) {
	//This function reads the Grammar and creates its representation
	FILE* fp = fopen(GrammarFile, "r");
	if(fp==NULL)
	{
		printf("Give the GrammarFile\n");
		exit(1);
	}

	char* rule = NULL;

	char *firstLineComment = NULL;
	size_t len = 1000;

	getline(&firstLineComment, &len, fp);
	if(ferror(fp)) {
		printf("Comment in %s exceeds limit of 1000 characters!\n", GrammarFile);
	}
	len = 512;
	getline(&rule, &len, fp);
	noOfRules = atoi(rule);

	Grammar G = (Grammar)malloc(noOfRules*sizeof(struct lhs));
	int i;
	for(i=0; i<noOfRules; i++) {
		G[i].val = -1;
		G[i].firstRule = NULL;
		G[i].lastRule = NULL;
		G[i].firstPtr = NULL;
		G[i].lastPtr = NULL;
	}

	i = 0;
	int ruleNo = 1;
	while(!feof(fp)) {

		getline(&rule, &len, fp);
		if(ferror(fp)) {
			printf("Length of rule exceeds 512 characters!\n");
			exit(0);
		}

		char* token = strtok(rule, " =>\n");
		symbol lhsSym = stringToEnum(token);
		G[lhsSym].val = lhsSym;
		G[lhsSym].firstRule = (rhs*)malloc(sizeof(rhs));

		rhs* currRule = G[lhsSym].firstRule;
		G[lhsSym].lastRule = currRule;
		currRule->nextRule = NULL;
		currRule->firstSym = NULL;
		currRule->lastSym = NULL;
		currRule->ruleNo = ruleNo;

		token = strtok(NULL, " =>\n");

		symbol symToInsert;

		while(token!=NULL) {

			if(strcmp(token, "|")==0) {
				currRule->nextRule = (rhs*)malloc(sizeof(rhs));
				currRule = currRule->nextRule;
				currRule->nextRule = NULL;
				currRule->firstSym = NULL;
				currRule->lastSym = NULL;
				G[lhsSym].lastRule = currRule;
				token = strtok(NULL, " =>\n");
				ruleNo++;
				currRule->ruleNo = ruleNo;
				continue;
			}
			symToInsert = stringToEnum(token);
			insertSymInRule(G, lhsSym, symToInsert);
			token = strtok(NULL, " =>\n");
		}
		ruleNo++;
	}

	free(rule);
	free(firstLineComment);
	fclose(fp);

	return G;
}

symbol stringToEnum(char* str) {
	//This function converts string to enum of convert array defined in parserDef.h
	int i;
	for(i=0; i<sizeof(convert)/sizeof(convert[0]); i++) {
		if(strcmp(str, convert[i].str)==0) {
			return convert[i].sym;
		}
	}

	printf("Error in Rules Name: %s! Change it in parserDef.h\n", str);
}

void insertSymInRule(Grammar G, symbol lhsSym, symbol symToInsert) {
	//This function inserts the symbol read in the rule given.
	rhs* currRule = G[lhsSym].lastRule;

	if(currRule->lastSym==NULL) {
		currRule->firstSym = (node*)malloc(sizeof(node));
		currRule->lastSym = currRule->firstSym;
	}

	else {

		currRule->lastSym->nextSym = (node*)malloc(sizeof(node));
		currRule->lastSym = currRule->lastSym->nextSym;
	}

	currRule->lastSym->val = symToInsert;
	currRule->lastSym->termOrNonTerm = convert[symToInsert].TOrNT;
	currRule->lastSym->nextSym = NULL;

	if(convert[symToInsert].TOrNT==1) {

		if(G[symToInsert].lastPtr==NULL) {
			G[symToInsert].firstPtr = (nonTermInRule*)malloc(sizeof(nonTermInRule));
			G[symToInsert].lastPtr = G[symToInsert].firstPtr;
			G[symToInsert].firstPtr->sym = lhsSym;
			G[symToInsert].lastPtr->sym = lhsSym;
		}

		else {
			G[symToInsert].lastPtr->next = (nonTermInRule*)malloc(sizeof(nonTermInRule));
			G[symToInsert].lastPtr = G[symToInsert].lastPtr->next;
			G[symToInsert].lastPtr->sym = lhsSym;

		}

		G[symToInsert].lastPtr->ptr = currRule->lastSym;
		G[symToInsert].lastPtr->next = NULL;
		//inserting the rule in which this symbol occurring
	}

}

char* enumToString(symbol val) {
	//This function converts the enum to string from convert array in lexerDef.h
	return convert[val].str;
}

void printGrammar(Grammar G) {
	int i;
	for(i=0; i<noOfRules; i++) {
		rhs* currRule = G[i].firstRule;
		printf("%s => ", enumToString(i));
		while(currRule!=NULL) {
			node* currSym = currRule->firstSym;
			printf("RuleNo: %d ", currRule->ruleNo);
			while(currSym!=NULL) {
				printf("%s ", enumToString(currSym->val));
				currSym = currSym->nextSym;
			}
			printf(" | ");
			currRule = currRule->nextRule;
		}
		printf("\n\n");
	}
}

void printRules(Grammar G) {	//print rules where that Non Terminal is coming
	int i;
	for(i=0; i<noOfRules; i++) {
		nonTermInRule* currptr = G[i].firstPtr;
		printf("%s => ", enumToString(i));
		while(currptr!=NULL) {
			node* currSym = currptr->ptr;
			printf("%s: ", enumToString(currptr->sym));
			while(currSym!=NULL) {
				printf("%s ", enumToString(currSym->val));
				currSym = currSym->nextSym;
			}
			printf(" | ");
			currptr = currptr->next;
		}
		printf("\n");
	}
}

FirstAndFollowSets computeFirstAndFollow(Grammar G) {
	//This function computesthe first and follow sets automatically
	FirstAndFollowSets firstFollow = (FirstAndFollowSets)malloc(noOfRules*sizeof(struct FirstAndFollow));
	int i;
	for(i=0; i<noOfRules; i++) {
		int j;
		firstFollow[i].firstSet = (unsigned int*)malloc(3*sizeof(unsigned int));
		firstFollow[i].followSet = (unsigned int*)malloc(3*sizeof(unsigned int));
		for(j=0; j<3; j++) {
			firstFollow[i].firstSet[j] = 0;
			firstFollow[i].followSet[j] = 0;
		}
	}

	int flag = 1;
	unsigned int* prevSet = (unsigned int*)malloc(3*sizeof(unsigned int));
	while(flag) {
		flag = 0;
		// printf("%d\n",flag );
		for(i=0; i<noOfRules; i++) {
			int k;
			for(k=0; k<3; k++) {
				prevSet[k] = firstFollow[i].firstSet[k];
			}
			firstFollow[i].firstSet = first(firstFollow, i, G);
			if(!equal(prevSet, firstFollow[i].firstSet, 3)) {
				flag = 1;
			}
		}
	}

	flag = 1;
	firstFollow[0].followSet = insertInSet(firstFollow[0].followSet, DOLLAR);	//Follow of Start Symbol is $

	while(flag) {
		flag = 0;
		// printf("%d\n",flag );
		for(i=1; i<noOfRules; i++) {
			int k;
			for(k=0; k<3; k++) {
				prevSet[k] = firstFollow[i].followSet[k];
			}
			firstFollow[i].followSet = follow(firstFollow, i, G);
			if(!equal(prevSet, firstFollow[i].followSet, 3)) {
				flag = 1;
			}
		}
	}

	free(prevSet);
	return firstFollow;
}

void printFirstFollow(FirstAndFollowSets firstFollow, Grammar G) {
	int i;
	printf("\nFirst Sets:\n");
	for(i=0; i<noOfRules; i++) {
		printf("%s: ", enumToString(G[i].val));
		symbol j = ASSIGNOP;
		for(; j<=EPSILON; j++) {
			if(findInSet(firstFollow[i].firstSet, j)){
				printf("%s, ", enumToString(j));
			}
		}
		printf("\n");
	}

	printf("\nFollow Sets:\n");
	for(i=0; i<noOfRules; i++) {
		printf("%s: ", enumToString(G[i].val));
		symbol j = ASSIGNOP;
		for(; j<=EPSILON; j++) {
			if(findInSet(firstFollow[i].followSet, j)){
				printf("%s, ", enumToString(j));
			}
		}
		printf("\n");
	}
}

unsigned int* first(FirstAndFollowSets firstFollow, int i, Grammar G) {
	//This function computes the first set.
	rhs* currRule = G[i].firstRule;
	while(currRule!=NULL) {
		node* currSym = currRule->firstSym;

		int flag = 0;
		while(currSym!=NULL&&currSym->termOrNonTerm) {	//symbol is Non Terminal
			firstFollow[i].firstSet = UnionExceptEpsilon(firstFollow[i].firstSet, firstFollow[currSym->val].firstSet, 3);

			symbol sym = EPSILON;
			if(!findInSet(firstFollow[currSym->val].firstSet, sym)) {
				flag = 1;
				currSym = currSym->nextSym;
				break;
			}
			else {
				currSym = currSym->nextSym;
			}
		}

		if(currSym!=NULL&&!currSym->termOrNonTerm&&!flag) {
			firstFollow[i].firstSet = insertInSet(firstFollow[i].firstSet, currSym->val);
		}
		else if(!flag) {
			firstFollow[i].firstSet = insertInSet(firstFollow[i].firstSet, EPSILON);
		}
		currRule = currRule->nextRule;
	}

	return firstFollow[i].firstSet;
}

void findMidwayFirstSet(FirstAndFollowSets firstFollow, node* ptr, unsigned int* temp) {
	//This function computes the first set of the rule starting from the given symbol
	node* currSym = ptr;

	int flag = 0;
	while(currSym!=NULL&&currSym->termOrNonTerm) {	//symbol is Non Terminal
		temp = UnionExceptEpsilon(temp, firstFollow[currSym->val].firstSet, 3);

		symbol sym = EPSILON;
		if(!findInSet(firstFollow[currSym->val].firstSet, sym)) {
			flag = 1;
			currSym = currSym->nextSym;
			break;
		}
		else {
			currSym = currSym->nextSym;
		}
	}

	if(currSym!=NULL&&!currSym->termOrNonTerm&&!flag) {
		temp = insertInSet(temp, currSym->val);
	}
	else if(!flag) {
		temp = insertInSet(temp, EPSILON);
	}

	return;
}


unsigned int* follow(FirstAndFollowSets firstFollow, int i, Grammar G) {
	//This function computes the follow set
	nonTermInRule* currptr = G[i].firstPtr;

	unsigned int* temp = (unsigned int*)malloc(3*sizeof(unsigned int));
	while(currptr!=NULL) {
		node* ptr = currptr->ptr;

		if(ptr->nextSym==NULL) {
			firstFollow[i].followSet = UnionExceptEpsilon(firstFollow[i].followSet, firstFollow[currptr->sym].followSet, 3);
			currptr = currptr->next;
			continue;
		}

		temp[0] = 0; temp[1] = 0; temp[2] = 0;

		findMidwayFirstSet(firstFollow, ptr->nextSym, temp);
		firstFollow[i].followSet = UnionExceptEpsilon(firstFollow[i].followSet, temp, 3);

		if(findInSet(temp, EPSILON)) {
			firstFollow[i].followSet = UnionExceptEpsilon(firstFollow[i].followSet, firstFollow[currptr->sym].followSet, 3);
		}

		currptr = currptr->next;
	}

	free(temp);
	return firstFollow[i].followSet;
}

int equal(unsigned int* set1, unsigned int* set2, int sz) {
	//This function checks if 2 sets are equal
	int i;
	for(i=0; i<sz; i++) {
		// printf("%u %u\n", set1[i], set2[i]);
		if(set1[i]!=set2[i]) {
			return 0;
		}
	}
	return 1;
}

unsigned int* insertInSet(unsigned int* l, int j) {
	//This function inserts a symbol in a Set
	int temp1 = j/32;
	int temp2 = j%32;
	unsigned int b = ((unsigned int)1)<<temp2;
	l[temp1] = l[temp1]|b;
return l;
}

int findInSet(unsigned int* l, int j) {
	//This function searches for a symbol in a set.
	int temp1 = j/32;
	int temp2 = j%32;
	unsigned int b = ((unsigned int)1)<<temp2;
	if((l[temp1]&b) == b) {
		return 1;
	}

	return 0;
}

unsigned int* UnionExceptEpsilon(unsigned int* l1, unsigned int* l2, int sz) {
	//This function takes union of 2 sets, but do not give Epsilon in the final result
	int i;
	for(i=0; i<sz; i++) {
		l1[i] = l1[i]|l2[i];
	}
	symbol sym = EPSILON;
	int leftShiftEps = EPSILON%32;
	unsigned int b = ((unsigned int)1) << leftShiftEps;
	l1[sz-1] = l1[sz-1] - (l1[sz-1]&b);			//EPSILON is the second last Symbol in enum
return l1;
}

unsigned int* Intersection(unsigned int* l1, unsigned int* l2, int sz) {
	int i;
	for(i=0; i<sz; i++) {
		l1[i] = l1[i]&l2[i];
	}
return l1;
}

parseTable createParseTable(FirstAndFollowSets firstFollow, Grammar G) {
	//This function creates the Parse Table of the given grammar
	parseTable T = (parseTable)malloc(noOfRules*sizeof(struct parseTableElem*));	//noOfRules = No of Non Terminals

	int cols = sizeof(convert)/sizeof(convert[0]) - noOfRules - 1;	//Total Symbols - Non Terminals - EPSILON (DOLLAR INCLUDED)

	int i, j;
	for(i=0; i<noOfRules; i++) {
		T[i] = (struct parseTableElem*)malloc(cols*sizeof(struct parseTableElem));
	}

	for(i=0; i<noOfRules; i++) {
		for(j=0; j<cols; j++) {
			T[i][j].rule = NULL;
			T[i][j].sync = 0;
		}
	}


	for(i=0; i<noOfRules; i++) {
		for(j=0; j<cols; j++) {

			if(findInSet(firstFollow[i].followSet, j+noOfRules))
			{
				T[i][j].sync = 1;
			}
		}
	}

	unsigned int* temp = (unsigned int*)malloc(3*sizeof(unsigned int));

	for(i=0; i<noOfRules; i++) {
		for(j=0; j<cols; j++) {
			symbol sym = noOfRules + j;

			if(findInSet(firstFollow[i].firstSet, sym))
			{
				rhs* currRule = G[i].firstRule;

				while(currRule!=NULL)
				{

					node* ptr = currRule->firstSym;
					temp[0] = 0; temp[1] = 0; temp[2] = 0;
					findMidwayFirstSet(firstFollow, ptr, temp);
					if(findInSet(temp, sym))
					{
						T[i][j].rule = currRule;
						break;
					}
					currRule = currRule->nextRule;
				}
			}
		}
	}

	for (i = 0; i < noOfRules; i++)
	{
		rhs* lastRule = G[i].lastRule;
		node* ptr = lastRule->firstSym;
		if(ptr->val==EPSILON) {
			for(j=0; j<cols; j++)
			{
				symbol sym = noOfRules + j;

				if(findInSet(firstFollow[i].followSet, sym))
				{
					T[i][j].rule = lastRule;
				}
			}
		}
	}

	free(temp);
	return T;
}

void printParseTable(parseTable T)
{
	int i, j;
	int cols = sizeof(convert)/sizeof(convert[0]) - noOfRules - 1;	//Total Symbols - Non Terminals - EPSILON (DOLLAR INCLUDED)

	for(i=0; i<noOfRules; i++)
	{
		for(j=0; j<cols; j++)
		{
			rhs* currRule = T[i][j].rule;
			printf("(%s, %s) => ", enumToString(i), enumToString(noOfRules + j));
			if (currRule==NULL)
			{
				printf("\n");
				continue;
			}
			node* currSym = currRule->firstSym;
			while(currSym!=NULL) {
				printf("%s ", enumToString(currSym->val));
				currSym = currSym->nextSym;
			}
			printf("\n");
		}
	}
}

parseTree* createParseTree(tokenHead* allTkns, parseTable T, Grammar G)
{	//This function creates the parse tree from the using tokens from Lexer and parse table
	parseTree* tree = (parseTree*)malloc(sizeof(parseTree));
	noOfParseTreeNodes++;
	parseTree* currTreeNode = tree;

	tree->sym = mainFunction;	//initialising the root node
	tree->parent = NULL;
	tree->next = NULL;
	tree->termOrNonTerm = 1;
	tree->inh_addr = NULL;
	tree->rule = G[0].firstRule;	//mainFunction rule is first rule of grammar
	tree->children = NULL;
	tree->tkn = NULL;

	token* prevTkn = NULL;

	token* currTkn = allTkns->first;
	stack* S =  createAndInitialiseStack(tree);

  	while(currTkn!=NULL) {
    	// printToken(currTkn);
  		if(top(S)->termOrNonTerm)	//Non Terminal
  		{
  			symbol sym = stringToEnum(currTkn->tkn);
  			sym -= noOfRules;	//making it in range of 0 to cols
  			if(T[top(S)->val][sym].rule != NULL)
  			{
  				currTreeNode = top(S)->ptrToTreeNode;
  				currTreeNode->tkn = NULL;
  				currTreeNode->rule = T[top(S)->val][sym].rule;
  				insertChildren(T[top(S)->val][sym].rule, currTreeNode, S);
  			}
  			else
  			{
  				//Report Error
  				insertParserErr(currTkn, top(S)->val);
  				if(T[top(S)->val][sym].sync)	//Symbol is in Follow Set
  				{
  					pop(S);
  				}
  				else
  				{
  					currTkn = getNextToken(currTkn);
  				}
  			}
  		}

  		else if (top(S)->val==DOLLAR)
  		{
	  		//Report Error
  			insertParserErr(currTkn, top(S)->val);
  			break;
  		}

  		else if(!top(S)->termOrNonTerm)	//Terminal
  		{
  			symbol sym = stringToEnum(currTkn->tkn);
  			if(top(S)->val==sym)
  			{
  				top(S)->ptrToTreeNode->tkn = currTkn;
  				pop(S);
		    	currTkn = getNextToken(currTkn);
  			}
  			else
  			{
  				//Report Error
  				if (currTkn!=prevTkn)
  				{
  					insertParserErr(currTkn, top(S)->val);
  				}
  				pop(S);
  				prevTkn = currTkn;
				// currTkn = getNextToken(currTkn);

  			}
  		}
  	}

  	if(currTkn==NULL&&top(S)->val!=DOLLAR)
  	{
  		//Report Error!
		insertParserErr(currTkn, top(S)->val);

  	}
  	else if (currTkn==NULL&&top(S)->val==DOLLAR&&pErr->first==NULL)
  	{
  		printf("Input source code is syntactically correct!\n\n");
  	}
	return tree;
}

void freeParseTree(parseTree* treeNode)
{
	if (treeNode == NULL)
	{
		return;
	}

	parseTree* child = treeNode->children;
	parseTree* temp = child;
	while(child!=NULL)
	{
		freeParseTree(child);
		temp = child;
		child = child->next;
		free(temp);
	}
}

void printParseTree(parseTree* treeNode, FILE* fp)
{
	if(treeNode==NULL)
	{
		return;
	}
	parseTree* currTreeNode = treeNode->children;
	printParseTree(currTreeNode, fp);

	currTreeNode = treeNode;

	if(currTreeNode->termOrNonTerm)
	{
		fprintf(fp, "- - - - ");
		// printf("- - - - ");
		if(currTreeNode->parent==NULL)
		{
			fprintf(fp, "ROOT ");
			// printf("ROOT ");
		}
		else
		{
			fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			// printf("%s ", enumToString(currTreeNode->parent->sym));
		}
		fprintf(fp, "No ");
		// printf("No ");
		fprintf(fp, "%s\n", enumToString(currTreeNode->sym));
		// printf("%s\n", enumToString(currTreeNode->sym));
	}
	else
	{
		if(currTreeNode->sym==EPSILON)
		{
			fprintf(fp, "- - - - ");
			// printf("- - - - ");
			fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			// printf("%s ", enumToString(currTreeNode->parent->sym));
			fprintf(fp, "Yes EPSILON\n");
			// printf("Yes EPSILON\n");
		}

		else if (currTreeNode->tkn==NULL)
		{
			fprintf(fp, "- - - - ");
			// printf("- - - - ");
			fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			// printf("%s ", enumToString(currTreeNode->parent->sym));
			fprintf(fp, "Yes -\n");
			// printf("Yes -\n");
		}

		else
		{
			fprintf(fp, "%s %d %s ", currTreeNode->tkn->lexeme, currTreeNode->tkn->lineNo, currTreeNode->tkn->tkn);
			// printf("%s %d %s ", currTreeNode->tkn->lexeme, currTreeNode->tkn->lineNo, currTreeNode->tkn->tkn);
			if(strcmp(currTreeNode->tkn->tkn, "NUM")==0)
			{
				fprintf(fp, "%d ", (currTreeNode->tkn->value).intVal);
				// printf("%d ", (currTreeNode->tkn->value).intVal);
			}
			else if (strcmp(currTreeNode->tkn->tkn, "RNUM")==0)
			{
				fprintf(fp, "%lf ", (currTreeNode->tkn->value).doubleVal);
				// printf("%lf ", (currTreeNode->tkn->value).doubleVal);
			}
			else
			{
				fprintf(fp, "- ");
				// printf("- ");
			}

			fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			// printf("%s ", enumToString(currTreeNode->parent->sym));
			fprintf(fp, "Yes ");
			// printf("Yes ");
			fprintf(fp, "-\n");
			// printf("-\n");
		}
	}

	if(treeNode->children==NULL)
	{
		return;
	}

	currTreeNode = treeNode->children->next;

	while(currTreeNode!=NULL)
	{
		printParseTree(currTreeNode, fp);
		currTreeNode = currTreeNode->next;
	}

}

void printParseTreeOnStdOut(parseTree* treeNode)
{
	if(treeNode==NULL)
	{
		return;
	}
	parseTree* currTreeNode = treeNode->children;
	printParseTreeOnStdOut(currTreeNode);

	currTreeNode = treeNode;

	if(currTreeNode->termOrNonTerm)
	{
		// fprintf(fp, "- - - - ");
		printf("- - - - ");
		if(currTreeNode->parent==NULL)
		{
			// fprintf(fp, "ROOT ");
			printf("ROOT ");
		}
		else
		{
			// fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			printf("%s ", enumToString(currTreeNode->parent->sym));
		}
		// fprintf(fp, "No ");
		printf("No ");
		// fprintf(fp, "%s\n", enumToString(currTreeNode->sym));
		printf("%s\n", enumToString(currTreeNode->sym));
	}
	else
	{
		if(currTreeNode->sym==EPSILON)
		{
			// fprintf(fp, "- - - - ");
			printf("- - - - ");
			// fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			printf("%s ", enumToString(currTreeNode->parent->sym));
			// fprintf(fp, "Yes EPSILON\n");
			printf("Yes EPSILON\n");
		}

		else if (currTreeNode->tkn==NULL)
		{
			// fprintf(fp, "- - - - ");
			printf("- - - - ");
			// fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			printf("%s ", enumToString(currTreeNode->parent->sym));
			// fprintf(fp, "Yes -\n");
			printf("Yes -\n");
		}

		else
		{
			// fprintf(fp, "%s %d %s ", currTreeNode->tkn->lexeme, currTreeNode->tkn->lineNo, currTreeNode->tkn->tkn);
			printf("%s %d %s ", currTreeNode->tkn->lexeme, currTreeNode->tkn->lineNo, currTreeNode->tkn->tkn);
			if(strcmp(currTreeNode->tkn->tkn, "NUM")==0)
			{
				// fprintf(fp, "%d ", (currTreeNode->tkn->value).intVal);
				printf("%d ", (currTreeNode->tkn->value).intVal);
			}
			else if (strcmp(currTreeNode->tkn->tkn, "RNUM")==0)
			{
				// fprintf(fp, "%lf ", (currTreeNode->tkn->value).doubleVal);
				printf("%lf ", (currTreeNode->tkn->value).doubleVal);
			}
			else
			{
				// fprintf(fp, "- ");
				printf("- ");
			}

			// fprintf(fp, "%s ", enumToString(currTreeNode->parent->sym));
			printf("%s ", enumToString(currTreeNode->parent->sym));
			// fprintf(fp, "Yes ");
			printf("Yes ");
			// fprintf(fp, "-\n");
			printf("-\n");
		}
	}

	if(treeNode->children==NULL)
	{
		return;
	}

	currTreeNode = treeNode->children->next;

	while(currTreeNode!=NULL)
	{
		printParseTreeOnStdOut(currTreeNode);
		currTreeNode = currTreeNode->next;
	}

}

void insertChildren(rhs* rule, parseTree* parentNode, stack* S)
{	//This function inserts chidlren in the given Non Terminal
	node* currSym = rule->firstSym;
	parseTree* currTreeNode;

	if(currSym!=NULL)
	{
		parseTree* treeNode = (parseTree*)malloc(sizeof(parseTree));
		noOfParseTreeNodes++;
		treeNode->sym = currSym->val;
		treeNode->tkn = NULL;
		treeNode->termOrNonTerm = currSym->termOrNonTerm;
		// treeNode->rule = rule;
		treeNode->inh_addr = NULL;
		treeNode->parent = parentNode;
		treeNode->children = NULL;
		treeNode->next = NULL;
		parentNode->children = treeNode;

		currTreeNode = treeNode;
		currSym = currSym->nextSym;
	}

	while(currSym!=NULL)
	{
		parseTree* treeNode = (parseTree*)malloc(sizeof(parseTree));
		noOfParseTreeNodes++;
		currTreeNode->next = treeNode;

		currTreeNode = treeNode;
		currTreeNode->sym = currSym->val;
		currTreeNode->tkn = NULL;
		currTreeNode->termOrNonTerm = currSym->termOrNonTerm;
		// treeNode->rule = rule;
		treeNode->inh_addr = NULL;
		currTreeNode->parent = parentNode;
		currTreeNode->children = NULL;
		currTreeNode->next = NULL;

		currSym = currSym->nextSym;
	}

	//Inserting in Stack in Reverse Order
	S = pop(S);

	parseTree* nextOfNodeToPush = NULL;

	node* temp = (node*)malloc(sizeof(node));
	do
	{
		currTreeNode = parentNode->children;

		while(currTreeNode->next!=nextOfNodeToPush)
		{
			currTreeNode = currTreeNode->next;
		}

		if(currTreeNode->sym!=EPSILON)
		{
			temp->val = currTreeNode->sym;
			temp->termOrNonTerm = currTreeNode->termOrNonTerm;
			S = push(S, temp, currTreeNode);
		}

		nextOfNodeToPush = currTreeNode;

	} while(currTreeNode!=parentNode->children);
}

stack* createAndInitialiseStack(parseTree* ptrToRootNode)
{	//This function initialise the stack with $ and mainFunction
	stack* S = (stack*)malloc(sizeof(stack));
	S->head = NULL;
	node* temp = (node*)malloc(sizeof(node));
	temp->val = DOLLAR;
	temp->termOrNonTerm = 0;
	S = push(S, temp, NULL);

	temp->val = mainFunction;
	temp->termOrNonTerm = 1;
	S = push(S, temp, ptrToRootNode);
	free(temp);

	return S;
}

stack* push(stack* S, node* ptr, parseTree* ptrToTreeNode)
{	//This function pushes given node on top of stack
	stackNode* stNode = (stackNode*)malloc(sizeof(stackNode));
	stNode->val = ptr->val;
	stNode->termOrNonTerm = ptr->termOrNonTerm;
	stNode->ptrToTreeNode = ptrToTreeNode;

	if(S->head==NULL)
	{
		S->head = stNode;
		stNode->next = NULL;
		return S;
	}
	stNode->next = S->head;
	S->head = stNode;
return S;
}

stack* pop(stack* S)
{	//This function pop the node from top of stack
	if(S->head==NULL)
	{
		return S;
	}
	stackNode* temp;
	temp = S->head->next;
	free(S->head);
	S->head = temp;
return S;
}

stackNode* top(stack* S)
{
	return (S->head);
}


void insertParserErr(token* currTkn, symbol val)
{	//This function insert the error in parserErrHead
	if (currTkn == NULL)
	{
		return;
	}

	syntacticErrExist = 1;
	
	errors* er = (errors*)malloc(sizeof(errors));
	er->lineNo = currTkn-> lineNo;
	char str[2048];
	strcpy(str, "The token ");
	strcat(str, currTkn->tkn);
	strcat(str, " for lexeme ");
	strcat(str, currTkn->lexeme);
	strcat(str, " doesn't match.");

	if(val>=noOfRules)
	{
		strcat(str, "The expected token here is ");
	 	strcat(str, enumToString(val));

	}

	strcpy(er->errStr, str);
	er->next = NULL;
	if(pErr->first==NULL)
	{
		pErr->first = er;
		pErr->last = er;
		return;
	}
	pErr->last->next = er;
	pErr->last = er;
}

void printSyntacticErrors()
{
	errors* er1 = lErr->first;
	errors* er2 = pErr->first;

	while(er1!=NULL&&er2!=NULL)
	{
		if(er1->lineNo <= er2->lineNo)
		{
			printf("\n%d: Lexical Error: %s\n", er1->lineNo, er1->errStr);
			er1 = er1->next;

		}
		else
		{
			printf("\n%d: Syntax Error: %s\n", er2->lineNo, er2->errStr);
			er2 = er2->next;
		}

	}
	while(er1!=NULL)
	{
		printf("\n%d: Lexical Error: %s\n", er1->lineNo, er1->errStr);
		er1 = er1->next;
	}

	while(er2!=NULL)
	{
		printf("\n%d: Syntax Error: %s\n", er2->lineNo, er2->errStr);
		er2 = er2->next;
	}
}
