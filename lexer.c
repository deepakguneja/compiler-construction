//Deepak Guneja
//2015A7PS0083P

#include"lexer.h"
#include"lexerDef.h"

tokenHead* tokenList(char* sourceCode, hashNode* hashTable) {
	//This function gives the token of the sourcecode
	FILE* fp;
	fp = fopen(sourceCode, "r");
	if(fp==NULL)
	{
		printf("Give the correct path for Source Code\n");
		exit(1);
	}

	char* buff1 = (char*)malloc(8193*sizeof(char));
	char* buff2 = (char*)malloc(8193*sizeof(char));

	int cnt = fread(buff1, sizeof(char), 8192, fp);

	buff1[8192] = '|'; //this '|' will be treated as End of Buffer
	buff2[8192] = '|';

	if(ferror(fp)) {
		printf("Error in Reading Source Code\n");
	}

	state currState = start;

	int lexemeBegin=0, forward=0;

	int currBuff = 0;
	char* currBuffptr = buff1;
	char* otherBuffptr = buff2;

	int lexerErr = -1;
	int lineNo = 1;

	tokenHead* allTkns = (tokenHead*)malloc(sizeof(tokenHead));
	allTkns->first = NULL;
	allTkns->last = NULL;

	char* tkn = (char*)malloc(15*sizeof(char));
	int buffFlag = 1; //means lexemeBegin and forward are in the same buffer

	int lastBuff = 0; //the buffer read is last or not, i.e. further data left in source code or not: 0 indicates not last buff

	if(cnt<8192) {
		lastBuff = 1;
	}

	char* str = (char*)malloc(1024*sizeof(char));

	while((!lastBuff)||(lastBuff&&forward<cnt)) {

		lexemeBegin = forward;

		int flag = 1;
		int idLen = 0;
		int strLen = 0;
		int funIdLen = 0;
		buffFlag = 1;
		while(flag) {

			if(forward==8192) { //8192 is the size of the buffer
				if(currState==start) {
					buffFlag = 1;
					lexemeBegin = 0;
				}
				else {
					buffFlag = (buffFlag+1)%2;
				}

				if(currBuff==0) {
					cnt = fread(buff2, sizeof(char), 8192, fp);
					currBuffptr = buff2;
					otherBuffptr = buff1;
				}
				else {
					cnt = fread(buff1, sizeof(char), 8192, fp);
					currBuffptr = buff1;
					otherBuffptr = buff2;
				}

				if(cnt==0) {
					fclose(fp);
					free(buff1);
					free(buff2);
					free(tkn);
					free(str);
					return allTkns;
				}

				if(cnt<8192) {
					lastBuff = 1;
				}

				currBuff = (currBuff+1)%2;

				if(ferror(fp)) {
					printf("Error in Reading Source Code\n");
				}
				forward = 0;

				buff1[8192] = '|'; //this '|' will be treated as End of Buffer
				buff2[8192] = '|';
			}

			char ch = currBuffptr[forward];

			switch(currState) {

				case start:
							if(ch=='=') {
								currState = assignFin;
							}

							else if(ch=='#') {
								currState = comment;
							}

							else if(ch=='_') {
								currState = funcStart;
								funIdLen++;
							}

							else if((ch>=65&&ch<=90)||(ch>=97&&ch<=122)) {
								currState = idStart;
								idLen++;
							}

							else if(ch>=48&&ch<=57) {
								currState = numStart;
							}

							else if(ch==34) { //34 is ASCII of "
								currState = strStart;
								strLen++;
							}

							else if(ch==91) { //91 is ASCII of [
								currState = leftSq;
							}

							else if(ch==93) { //93 is ASCII of ]
								currState = rightSq;
							}

							else if(ch==40)  { //40 is ASCII of (
								currState = opPar;
							}

							else if(ch==41) { //41 is ASCII of )
								currState = clPar;
							}

							else if(ch==';') {
								currState = semiCol;
							}

							else if(ch==',') {
								currState = comma;
							}

							else if(ch=='+') {
								currState = plus;
							}

							else if(ch=='-') {
								currState = minus;
							}

							else if(ch=='*') {
								currState = mul;
							}

							else if(ch=='/') {
								currState = divOp;
							}

							else if(ch=='@') {
								currState = sz;
							}

							else if(ch=='.') {
								currState = boolStart;
							}

							else if(ch=='<') {
								currState = lessThan;
							}

							else if(ch=='>') {
								currState = greaterThan;
							}

							else if(ch=='\n') {
								lineNo++;
								currState = start;
								flag = 0;
								break;
							}

							else if(ch=='\t'||ch==' '||ch=='\r'||ch=='\v') {
								currState = start;
								flag = 0;
								break;
							}

							else {
								lexerErr = 0;	//unknown symbol
								forward--;
								currState = errState;
							}
							break;

				case assignFin:
							if(ch=='=') {
								currState = eqFin;
							}
							else if(ch=='/') {
								currState = neInt1;
							}
							else {
								strcpy(tkn, "ASSIGNOP");
								insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
								flag = 0;
								forward--;
								currState = start;
							}

							break;


				case eqFin:
							strcpy(tkn, "EQ");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;
							break;

				case neInt1:
							if(ch!='=') {
								lexerErr = 1; //unknown pattern for not equal to
								// forward--;
								currState = errState;
							}
							else {
								currState = neFin;
							}

							break;

				case neFin:
							strcpy(tkn, "NE");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;
							break;

				case comment:
							if(ch=='\n') {
								flag = 0;
								forward--;
								currState = start;
							}
							else {
								currState = comment;
							}
							break;

				case funcStart:
							if((ch>=65&&ch<=90)||(ch>=97&&ch<=122))	{
								currState = funcFin;
							}
							else {
								lexerErr = 1; //unknown pattern in function Id
								currState = errState;
							}

							break;

				case funcFin:
							if((ch>=65&&ch<=90)||(ch>=97&&ch<=122)||(ch>='0'&&ch<='9'))	{
								currState = funcFin;
								funIdLen++;
								if (funIdLen>6000)
								{
									// lexerErr = 2;
									currState = start; //FunId length greater than 6000
									printf("Errrrrrr! This much length not allowed. Compile Again!\n\n\n");
									break;
								}
							}
							else {
								strcpy(tkn, "FUNID");
								insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0,  buffFlag, hashTable);
								flag = 0;
								forward--;
								currState = start;
							}

							break;

				case idStart:
							if(ch>='0'&&ch<='9') {
								currState = idFin;
								idLen++;
							}

							if((ch>=65&&ch<=90)||(ch>=97&&ch<=122)) {
								currState = idStart;
								idLen++;
							}

							if(idLen>20) {
								lexerErr = 2;
								forward--;
								currState = errState; //id length greater than 20
								break;
							}

							if(!((ch>=65&&ch<=90)||(ch>=97&&ch<=122)||(ch>='0'&&ch<='9')))	{
								strcpy(tkn, "ID");
								insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
								flag = 0;
								forward--;
								currState = start;
							}

							break;

				case idFin:
							strcpy(tkn, "ID");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case numStart:
							if(ch=='.') {
								currState = rnumInt1;
							}
							else if(ch>='0'&&ch<='9') {
								currState = numStart;
							}
							else {
								strcpy(tkn, "NUM");
								insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 1, buffFlag, hashTable);
								flag = 0;
								forward--;
								currState = start;
							}

							break;

				case rnumInt1:
							if(!(ch>='0'&&ch<='9')) {
								
								if(ch=='a'||ch=='n'||ch=='o')
								{
									forward --;	//because if 16.and. then till 16 we want, so need to backtrack '.' and 'a'
									strcpy(tkn, "NUM");
									insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 1, buffFlag, hashTable);
									flag = 0;
									forward--;
									currState = start;
									break;
								}

								lexerErr = 1;
								currState = errState; //error in Real Number;
							}
							else {
								currState = rnumInt2;
							}

							break;

				case rnumInt2:
							if(!(ch>='0'&&ch<='9')) {
								lexerErr = 1;
								currState = errState; //error in Real Number;
							}
							else {
								currState = rnumFin;
							}

							break;

				case rnumFin:
							strcpy(tkn, "RNUM");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 1, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case strStart:
							if(!(ch==' '||ch=='"'||(ch>=97&&ch<=122))) {
								lexerErr = 3;
								currState = errState; //unknown symbol in string
							}

							else {
								currState = strInt1;
								strLen++;
							}

							break;

				case strInt1:
							if(ch==' '||(ch>=97&&ch<=122)) {
								currState = strInt1;
								strLen++;
							}

							if(ch==34) {
								currState = strFin;
								strLen++;
							}

							if(strLen>20) {
								lexerErr = 4;
								currState = errState; //str length greater than 20
								break;
							}

							if(!((ch>=97&&ch<=122)||ch=='"'||ch==' ')) {
								lexerErr = 3;
								currState = errState; //invalid charaacter in string
								break;
							}

							break;

				case strFin:
							// if(strLen==0) {
							// 	lexerErr = 7;
							// 	currState = errState; //length 0 string not allowed
							// 	break;
							// }

							strcpy(tkn, "STR");
							insertTknAtLast(allTkns, tkn, lexemeBegin+1, forward-1, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							//Do not include "" in the string
							flag = 0;
							forward--;
							currState = start;

							break;

				case leftSq:
							strcpy(tkn, "SQO");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case rightSq:
							strcpy(tkn, "SQC");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case opPar:
							strcpy(tkn, "OP");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case clPar:
							strcpy(tkn, "CL");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case semiCol:
							strcpy(tkn, "SEMICOLON");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case comma:
							strcpy(tkn, "COMMA");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case plus:
							strcpy(tkn, "PLUS");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case minus:
							strcpy(tkn, "MINUS");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case mul:
							strcpy(tkn, "MUL");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case divOp:
							strcpy(tkn, "DIV");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case sz:
							strcpy(tkn, "SIZE");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case boolStart:
							if(ch=='a') {
								currState = andInt1;
							}

							else if(ch=='o') {
								currState = orInt1;
							}

							else if(ch=='n') {
								currState = notInt1;
							}

							else {
								lexerErr = 1;
								currState = errState; //unknown pattern in boolean
							}

							break;

				case andInt1:
							if(ch=='n') {
								currState = andInt2;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case andInt2:
							if(ch=='d') {
								currState = andInt3;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case andInt3:
							if(ch=='.') {
								currState = andFin;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case andFin:
							strcpy(tkn, "AND");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case orInt1:
							if(ch=='r') {
								currState = orInt2;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case orInt2:
							if(ch=='.') {
								currState = orFin;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;


				case orFin:
							strcpy(tkn, "OR");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case notInt1:
							if(ch=='o') {
								currState = notInt2;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case notInt2:
							if(ch=='t') {
								currState = notInt3;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case notInt3:
							if(ch=='.') {
								currState = notFin;
							}

							else {
								lexerErr = 1;
								currState = errState; //invalid symbol
							}

							break;

				case notFin:
							strcpy(tkn, "NOT");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case lessThan:
							if(ch=='=') {
								currState = lessEqual;
							}

							else {
								strcpy(tkn, "LT");
								insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
								flag = 0;
								forward--;
								currState = start;
							}

							break;

				case lessEqual:
							strcpy(tkn, "LE");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case greaterThan:
							if(ch=='=') {
								currState = greaterEqual;
							}

							else {
								strcpy(tkn, "GT");
								insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
								flag = 0;
								forward--;
								currState = start;
							}

							break;

				case greaterEqual:
							strcpy(tkn, "GE");
							insertTknAtLast(allTkns, tkn, lexemeBegin, forward, currBuffptr, otherBuffptr, lineNo, 0, buffFlag, hashTable);
							flag = 0;
							forward--;
							currState = start;

							break;

				case errState:

							if(lexerErr==0)
							{
								char errSym[2];
								errSym[0] = ch;
								errSym[1] = '\0';
								strcpy(str, "Error unknown symbol ");
								strcat(str, errSym);
								flag = 0;
								currState = start;
							}
										
							else if(lexerErr==1)
							{
								char *errLexme;
								errLexme = giveErrLexeme(lexemeBegin, forward, currBuffptr, otherBuffptr, buffFlag);
								strcpy(str, "Unknown pattern ");
								strcat(str, errLexme);
								flag = 0;
								currState = start;
								forward--;
							}
										
							else if(lexerErr==2)
							{
								if((ch>=65&&ch<=90)||(ch>=97&&ch<=122))
								{
									currState = errState;
									lexerErr = 2;
								}
								else if(ch>='0'&&ch<='9')
								{
									char *errLexme;
									errLexme = giveErrLexeme(lexemeBegin, forward, currBuffptr, otherBuffptr, buffFlag);
									strcpy(str, "Identifier is longer than prescribed length ");
									strcat(str, errLexme);
									flag = 0;
									currState = start;
								}
								else
								{
									char *errLexme;
									errLexme = giveErrLexeme(lexemeBegin, forward, currBuffptr, otherBuffptr, buffFlag);
									strcpy(str, "Identifier is longer than prescribed length ");
									strcat(str, errLexme);
									flag = 0;
									currState = start;
									forward--;
								}
							}
										
							else if(lexerErr==3)
							{
								if(ch!='"')
								{
									currState = errState;
									lexerErr = 3;
								}
								else
								{
									char *errLexme;
									errLexme = giveErrLexeme(lexemeBegin, forward, currBuffptr, otherBuffptr, buffFlag);
									strcpy(str, "Error in string ");
									strcat(str, errLexme);
									strcat(str, "\"");
									flag = 0;
									currState = start;
								}
							}
								
							else if(lexerErr==4)
							{
								if(ch!='"')
								{
									currState = errState;
									lexerErr = 4;
								}
								else
								{
									char *errLexme;
									errLexme = giveErrLexeme(lexemeBegin, forward, currBuffptr, otherBuffptr, buffFlag);
									strcpy(str, "String is longer than prescribed length ");
									strcat(str, errLexme);
									strcat(str, "\"");

									flag = 0;
									currState = start;
								}
							}		

							if(!flag)
							{
								insertLexerErr(str, lineNo);	
							}
								
							break;

				default:
							break;
			}

			forward++;
		}
	}

	fclose(fp);
	free(buff1);
	free(buff2);
	free(tkn);
	free(str);
	return allTkns;
}

char* giveErrLexeme(int lexemeBegin, int forward, char* currBuffptr, char* otherBuffptr, int buffFlag)
{
	//This function is used in Error Handling to report the lexeme which contains error
	char* lexeme;
	int lenStr, i;

	if(!buffFlag) {

		lenStr = (8192-lexemeBegin) + forward;
		lexeme = (char*)malloc((lenStr+1)*sizeof(char));
		int j = 0;
		for(i=lexemeBegin; i<8192; i++) {
			lexeme[j] = otherBuffptr[i];
			j++;
		}
		for(i=0; i<forward; i++) {
			lexeme[j] = currBuffptr[i];
			j++;
		}
		lexeme[j] = '\0';
	}
	else {
		lenStr = forward - lexemeBegin;
		lexeme = (char*)malloc((lenStr+1)*sizeof(char));
		int j = 0;
		for(i=lexemeBegin; i<forward; i++) {
			lexeme[j] = currBuffptr[i];
			j++;
		}
		lexeme[j] = '\0';
	}

	return lexeme;
}


void insertTknAtLast(tokenHead* allTkns, char* tkn, int lexemeBegin, int forward, char* currBuffptr, char* otherBuffptr, int lineNo, int tknValFlag, int buffFlag, hashNode* hashTable) {

	token* currTkn;
	int i;
	if(allTkns->first==NULL) {

			allTkns->first = (token*)malloc(sizeof(token));
			currTkn = allTkns->first;
			allTkns->last = currTkn;
	}

	else {

		allTkns->last->next = (token*)malloc(sizeof(token));
		currTkn = allTkns->last->next;
		allTkns->last = currTkn;
	}

	int lenStr;

	strcpy(currTkn->tkn, tkn);

	if(!buffFlag) {

		lenStr = (8192-lexemeBegin) + forward;
		currTkn->lexeme = (char*)malloc((lenStr+1)*sizeof(char));
		int j = 0;
		for(i=lexemeBegin; i<8192; i++) {
			(currTkn->lexeme)[j] = otherBuffptr[i];
			j++;
		}
		for(i=0; i<forward; i++) {
			(currTkn->lexeme)[j] = currBuffptr[i];
			j++;
		}
		(currTkn->lexeme)[j] = '\0';
	}
	else {
		lenStr = forward - lexemeBegin;
		currTkn->lexeme = (char*)malloc((lenStr+1)*sizeof(char));
		int j = 0;
		for(i=lexemeBegin; i<forward; i++) {
			(currTkn->lexeme)[j] = currBuffptr[i];
			j++;
		}
		(currTkn->lexeme)[j] = '\0';
	}

	if(tknValFlag) {
		tokenVal value;
		if(strcmp(tkn, "NUM")==0) {
			int val = 0;
			for(i=0; i<lenStr; i++) {
				if((currTkn->lexeme)[i]=='\0') {
					break;
				}
				val = val*10 + (int)((currTkn->lexeme)[i]-'0');
			}
			value.intVal = val;
			currTkn->value = value;
		}

		if(strcmp(tkn, "RNUM")==0) {
			double val = 0;
			for(i=0; i<lenStr; i++) {
				if((currTkn->lexeme)[i]=='.') {
					i++;
					break;
				}
				val = val*10 + (double)((currTkn->lexeme)[i]-'0');
			}
			double dec;
			dec = ((double)((currTkn->lexeme)[i]-'0'))*10 + (double)((currTkn->lexeme)[i+1]-'0');
			dec = dec/100;
			val = val + dec;
			value.doubleVal = val;
			currTkn->value = value;
		}
	}

	if(strcmp(tkn, "ID")==0) {
		keyword* node = lookupHashTable(hashTable, currTkn->lexeme);
		if(node!=NULL) {
			strcpy(currTkn->tkn, node->tkn);
		}
	}
	else if(strcmp(tkn, "FUNID")==0) {
		keyword* node = lookupHashTable(hashTable, currTkn->lexeme);
		if(node!=NULL) {
			strcpy(currTkn->tkn, node->tkn);
		}
	}
	currTkn->lineNo = lineNo;
	currTkn->next = NULL;
}

token* getNextToken(token* currTkn) {
	return currTkn->next;
}

void printToken(token* currTkn) {

	printf("< %s, %s, %d, %lf, %d >\n", currTkn->tkn, currTkn->lexeme, (currTkn->value).intVal, (currTkn->value).doubleVal, currTkn->lineNo);
}

void printTokenList(tokenHead* allTkns)
{
	token* currTkn = allTkns->first;
	while(currTkn!=NULL)
	{
		// printf("< %s %s %d >\n", currTkn->tkn, currTkn->lexeme, currTkn->lineNo);
		printf("%d   %s   %s\n", currTkn->lineNo, currTkn->lexeme, currTkn->tkn);
		currTkn = currTkn->next;
	}
}

void freeTkn(tokenHead* tkns)
{
	token* currTkn = tkns->first;
	free(tkns);
	while(currTkn!=NULL)
	{
		free(currTkn->lexeme);
		token* temp = currTkn;
		currTkn = currTkn->next;
		free(temp);
	}

}

void insertLexerErr(char* str, int lineNo)
{
	//This function inserts the error in lexerErrHead

	lexicalErrExist = 1;

	errors* er = (errors*)malloc(sizeof(errors));
	er->lineNo = lineNo;
	strcpy(er->errStr, str);
	er->next = NULL;
	if(lErr->first==NULL)
	{
		lErr->first = er;
		lErr->last = er;
		return;
	}
	lErr->last->next = er;
	lErr->last = er;
}

void printLexerErrs()
{
	errors* er = lErr->first;
	while(er!=NULL)
	{
		printf("%d: Lexical Error: %s\n", er->lineNo, er->errStr);
		er = er->next;
	}
}

void removeTheComments(char* sourceCode)
{
	//This function removes the comments from sourceCode
	FILE* fp;
	fp = fopen(sourceCode, "r");

	if(fp==NULL)
	{
		printf("Give the correct path for Source Code\n");
		exit(1);
	}

	char* buff1 = (char*)malloc(8193*sizeof(char));
	char* buff2 = (char*)malloc(8193*sizeof(char));

	int cnt = fread(buff1, sizeof(char), 8192, fp);

	buff1[8192] = '|'; //this '|' will be treated as End of Buffer
	buff2[8192] = '|';

	if(ferror(fp)) {
		printf("Error in Reading Source Code\n");
	}

	state currState = start;

	int lexemeBegin=0, forward=0;

	int currBuff = 0;
	char* currBuffptr = buff1;
	char* otherBuffptr = buff2;

	int lexerErr = -1;
	int lineNo = 1;

	int buffFlag = 1; //means lexemeBegin and forward are in the same buffer

	int lastBuff = 0; //the buffer read is last or not, i.e. further data left in source code or not: 0 indicates not last buff

	if(cnt<8192) {
		lastBuff = 1;
	}

	while((!lastBuff)||(lastBuff&&forward<cnt)) {

		lexemeBegin = forward;

		int flag = 1;
		buffFlag = 1;

		while(flag) {

			if(forward==8192) { //8192 is the size of the buffer
				if(currState==start) {
					buffFlag = 1;
					lexemeBegin = 0;
				}
				else {
					buffFlag = (buffFlag+1)%2;
				}

				if(currBuff==0) {
					cnt = fread(buff2, sizeof(char), 8192, fp);
					currBuffptr = buff2;
					otherBuffptr = buff1;
				}
				else {
					cnt = fread(buff1, sizeof(char), 8192, fp);
					currBuffptr = buff1;
					otherBuffptr = buff2;
				}

				if(cnt==0) {
					fclose(fp);
					free(buff1);
					free(buff2);
					return;
				}

				if(cnt<8192) {
					lastBuff = 1;
				}

				currBuff = (currBuff+1)%2;

				if(ferror(fp)) {
					printf("Error in Reading Source Code\n");
				}
				forward = 0;

				buff1[8192] = '|'; //this '|' will be treated as End of Buffer
				buff2[8192] = '|';
			}

			char ch = currBuffptr[forward];

			switch(currState) {

				case start:
							if(ch=='#') {
								currState = comment;
								break;
							}
							printf("%c", ch);
							flag = 0;
							break;

				case comment:

							if(ch=='\n') {
								flag = 0;
								currState = start;
							}
							else {
								currState = comment;
							}
							break;
				default:
							break;
			}

			forward++;
		}
	}

	fclose(fp);
	free(buff1);
	free(buff2);
}