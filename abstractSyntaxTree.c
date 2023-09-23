//Deepak Guneja
//2015A7PS0083P

#include "abstractSyntaxTree.h"

void createAST(parseTree* treeNode)
{
    // printf("%s\n", enumToString(treeNode->sym));
    if(treeNode->sym == arithmeticExpression)
    {
    	parseTree* child = treeNode->children;
    	while(child!=NULL)
    	{
    		if (child->sym == arithmeticExpression2)
    		{
    			child->inh_addr = (ASTnode*)treeNode->children->addr;
    			createAST(child);
				treeNode->addr = (ASTnode*)child->addr;
				return;
    		}

    		createAST(child);
    		child = child->next;
    	}
    	return;
    }

    else if(treeNode->sym == arithmeticTerm)
    {
    	parseTree* child = treeNode->children;
    	while(child!=NULL)
    	{
    		if (child->sym == arithmeticTerm2)
    		{
    			child->inh_addr = (ASTnode*)treeNode->children->addr;
				createAST(child);
				treeNode->addr = (ASTnode*)child->addr;
				return;
    		}

    		createAST(child);
    		child = child->next;
    	}
    	return;
    }

    else if (treeNode->sym == arithmeticExpression2  && treeNode->children->sym != EPSILON)
    {
        parseTree* child = treeNode->children;
        while(child!=NULL)
        {
            if (child->sym == arithmeticExpression2)
            {
                parseTree* child1 = treeNode;   //arithmeticExpression2
                parseTree* child2 = treeNode->children->next;   //arithmeticTerm.addr
                
                ASTnode* nd = (ASTnode*)treeNode->children->addr; //operator_lowPrecedence
                nd->children = (ASTnode*)child1->inh_addr;    //arithmeticExpression2_1.inh_addr
                nd->children->next = (ASTnode*)child2->addr; //arithmeticTerm.addr

                child->inh_addr = nd; //arithExprsn2_2.inh_addr = nd

                createAST(child);
                treeNode->addr = (ASTnode*)child->addr;
                return;
            }

            createAST(child);
            child = child->next;
        }
        // newNodeArithEx2(treeNode);
        return;
    }

    else if (treeNode->sym == arithmeticTerm2 && treeNode->children->sym != EPSILON)
    {
        parseTree* child = treeNode->children;
        while(child!=NULL)
        {
            if (child->sym == arithmeticTerm2)
            {
                parseTree* child1 = treeNode;   //arithmeticTerm2_1
                parseTree* child2 = treeNode->children->next;   //factor.addr
                
                ASTnode* nd = (ASTnode*)treeNode->children->addr; //operator_highPrecedence
                nd->children = (ASTnode*)child1->inh_addr;    //arithmeticTerm2_1.inh_addr
                nd->children->next = (ASTnode*)child2->addr; //factor.addr

                child->inh_addr = nd; //arithTerm2_2.inh_addr = nd

                createAST(child);
                treeNode->addr = (ASTnode*)child->addr;
                return;
            }

            createAST(child);
            child = child->next;
        }
        // newNodeArithTerm2(treeNode);
        return;
    }

    parseTree* child = treeNode->children;
    while(child!=NULL)
    {
        createAST(child);
        child = child->next;
    }

    if (treeNode->termOrNonTerm)
    {
        int ruleNo = treeNode->rule->ruleNo;
        // printf("%d\n", ruleNo);
        switch(ruleNo)
        {
            case 1: newNodeMain(treeNode);		//Only for MAIN 
                break;
            case 2: siblings1(treeNode);	//has 2 children and need to make them as siblings
                break;            
            case 3: assign1(treeNode);		//assign a single child
                break; 
            case 4: treeNode->addr = NULL;
                break;
            case 5: assign1(treeNode);		//assign a single child
                break;          
            case 6: assign1(treeNode);		//assign a single child
                break;      
            case 7: assign1(treeNode);		//assign a single child
                break;
            case 8: assign1(treeNode);		//assign a single child
                break;            
            case 9: assign1(treeNode);		//assign a single child
                break;
            case 10: assign1(treeNode);		//assign a single child
                break;
            case 11: assign1(treeNode);		//assign a single child
                break;
            case 12: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 13: newNodeFunction(treeNode);	//Only for FUNCTION
                break;
            case 14: siblings2(treeNode);	//Only for parameter_list
                break;
            case 15: treeNode->addr = NULL;
                break;
            case 16: assign2(treeNode);		//Single Terminal Child
                break;
            case 17: assign2(treeNode);		//Single Terminal Child
                break;
            case 18: assign2(treeNode);		//Single Terminal Child
                break;
            case 19: assign2(treeNode);		//Single Terminal Child
                break;
            case 20: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 21: treeNode->addr = NULL;
                break;
            case 22: newNodeDeclaration(treeNode);	//Only for Declaration
                break;
            case 23: siblings3(treeNode);	//If one children is terminal and other is NT
                break;
            case 24: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 25: treeNode->addr = NULL;
                break;
            case 26: newNodeAssignSingleVar(treeNode);	//Only for ASSIGN_SINGLE_VAR
                break;
            case 27: newNodeAssignListVar(treeNode);	//Only for ASSIGN_LIST_VAR
                break;
            case 28: assign2(treeNode);		//Single Terminal Child
                break;
            case 29: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 30: assign1(treeNode);		//assign a single child
                break;
            case 31: assign1(treeNode);		//assign a single child
                break;
            case 32: assign1(treeNode);		//assign a single child
                break;
            case 33: assign1(treeNode);		//assign a single child
                break;
            case 34: assign1(treeNode);		//assign a single child
                break;
            case 35: newNodeSize(treeNode);	//Only for SIZE
                break;
            case 36: newNodeIf(treeNode);	//Only for IF
                break;
            case 37: newNodeElse(treeNode);	//Only for ELSE
                break;
            case 38: treeNode->addr = NULL;
                break;
            case 39: siblings1(treeNode);	//has 2 children and need to make them as siblings
                break;
            case 40: treeNode->addr = NULL;
                break;
            case 41: newNodeIoRead(treeNode);	//Only for IOREAD
                break;
            case 42: newNodeIoPrint(treeNode);	//Only for IOPRINT
                break;
            case 43: newNodeFuncall(treeNode);	//Only for FUNCALL
                break;
            case 44: siblings1(treeNode);	//has 2 children and need to make them as siblings
                break;
            case 45: treeNode->addr = NULL;
                break;
            case 46: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 47: treeNode->addr = NULL;
                break;
            // case 48: assign4(treeNode);		//assign addr and inh_addr for second child, only for arithmeticExpression
                // break;
            // case 49: newNodeArithEx2(treeNode);	//Only for arithmeticExpression2
                // break;
            case 50: treeNode->addr = (ASTnode*)treeNode->inh_addr;
                break;
            // case 51: assign5(treeNode);		//assign addr and inh_addr for second child, only for arithmeticTerm
                // break;
            // case 52: newNodeArithTerm2(treeNode);	//Only for arithmeticTerm2
            //     break;
            case 53: treeNode->addr = (ASTnode*)treeNode->inh_addr;
                break;
            case 54: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 55: assign1(treeNode);		//assign a single child
                break;
            case 56: assign2(treeNode);		//Single Terminal Child
                break;
            case 57: assign2(treeNode);		//Single Terminal Child
                break;
            case 58: assign2(treeNode);		//Single Terminal Child
                break;
            case 59: assign2(treeNode);		//Single Terminal Child
                break;
            case 60: newNodeBool1(treeNode);	//Only for booleanExpreesion rule1
                break;
            case 61: newNodeBool2(treeNode);	//Only for booleanExpreesion rule2
                break;
            case 62: newNodeNot(treeNode);	//Only for NOT
                break;
            case 63: assign2(treeNode);		//Single Terminal Child
                break;
            case 64: assign2(treeNode);		//Single Terminal Child
                break;
            case 65: assign2(treeNode);		//Single Terminal Child
                break;
            case 66: newNodeVarMatrixElem(treeNode);	//Only for var matrixElement
                break;
            case 67: assign2(treeNode);		//Single Terminal Child
                break;
            case 68: assign2(treeNode);		//Single Terminal Child
                break;
            case 69: assign2(treeNode);		//Single Terminal Child
                break;
            case 70: assign1(treeNode);		//assign a single child
                break;
            case 71: newNodeMatrixElem(treeNode);	//Only for matrixElement
                break;
            case 72: treeNode->addr = NULL;
                break;
            case 73: newNodeMatrix(treeNode);	//Only for MATRIX
                break;
            case 74: siblings4(treeNode);	//Only for ROW
                break;
            case 75: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 76: treeNode->addr = NULL;
                break;
            case 77: siblings3(treeNode);	//has 2 children and need to make them as siblings
                break;
            case 78: assign3(treeNode);		//has many children, but only single non terminal child, so only need to assign to that child
                break;
            case 79: treeNode->addr = NULL;
                break;
            case 80: assign2(treeNode);		//Single Terminal Child
                break;
            case 81: assign2(treeNode);		//Single Terminal Child
                break;
            case 82: assign2(treeNode);		//Single Terminal Child
                break;
            case 83: assign2(treeNode);		//Single Terminal Child
                break;
            case 84: assign2(treeNode);		//Single Terminal Child
                break;
            case 85: assign2(treeNode);		//Single Terminal Child
                break;
            case 86: assign2(treeNode);		//Single Terminal Child
                break;
            case 87: assign2(treeNode);		//Single Terminal Child
                break;
            default:
            	break;
        }
    }
}

// Child's parent and next would be decide in parent

ASTnode* initASTnode()
{
    ASTnode* nd = (ASTnode*)malloc(sizeof(ASTnode));
    nd->parent = NULL;
    nd->next = NULL;
    nd->children = NULL;
    // int symValid;   //if the node has valid symbol or not: symValid = 1
    // ASTnodeSym sym;
    nd->tkn = NULL; //if symbol is not valid, means it is a terminal so pointer to its
    nd->symTablePtr = NULL;  //pointer to entry in symbol symTablePtr,i.e. pointer to field
    nd->code = NULL; //pointer to intermediate code generation
    nd->addr = NULL;   //addr for intermediate code generation
    nd->trueLabel = NULL;  //these all are for if-else intermediate code generation
    nd->falseLabel = NULL;
    nd->nextLabel = NULL;
    // varType t;
    return nd;
}

void newNodeMain(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
	nd->parent = NULL;	//This is main, so its parent and next is made NULL here, but for others their parent would be decided in not their call, but of their parent itself
	nd->next = NULL;
    nd->symTablePtr = NULL;	
	parseTree* child = treeNode->children;
	while(child->sym != stmtsAndFunctionDefs)	//till we get stmtsAndFunctionDefs
	{
		child = child->next;
	}
	nd->children = (ASTnode*)child->addr;
	// child->addr->parent = nd;
	nd->symValid = 1;
	nd->sym = _MAIN;
	nd->tkn = NULL;

	treeNode->addr = nd;
}

void newNodeFunction(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = FUNCTION_DEF;
    nd->tkn = NULL;
	while(child!=NULL)
	{
		if(child->sym == parameter_list)
		{
            ASTnode* temp = initASTnode();
            noOfASTnodes++;
            temp->symTablePtr = NULL;
            // temp->parent = nd;
            nd->children = temp;
            temp->children = (ASTnode*)child->addr;
            currChild = temp;
            temp->symValid = 1;
            temp->sym = OUT_PARAM;
            temp->tkn = NULL;
            child = child->next->next->next;

            //FUNID starts
            currChild->next = initASTnode();
            noOfASTnodes++;
            currChild->next->symTablePtr = NULL;
			// temp->parent = nd;
            currChild = (ASTnode*)currChild->next;
			currChild->children = NULL;
			currChild->symValid = 0;
			currChild->tkn = child->tkn;
            child = child->next->next;

            //Out Param starts
            currChild->next = initASTnode();
            noOfASTnodes++;
            currChild->next->symTablePtr = NULL;
            currChild = (ASTnode*)currChild->next;
            currChild->children = (ASTnode*)child->addr;
            currChild->symValid = 1;
            currChild->sym = IN_PARAM;
            currChild->tkn = NULL;
            child = child->next->next;

            //stmtsAndFunctionDefs starts
            currChild->next = (ASTnode*)child->addr;	//stmtsAndFunctionDefs.addr

            break;
		}

		child = child->next;
	}

	treeNode->addr = nd;
}	

void newNodeDeclaration(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = DECL;
    nd->tkn = NULL;
	while(child!=NULL)
	{
		if(child->sym == type)
		{
			nd->children = (ASTnode*)child->addr;
			currChild = (ASTnode*)child->addr;
			// currChild->parent = nd;
		}
		else if (child->sym == var_list)
		{
			currChild->next = (ASTnode*)child->addr;
			// currChild->parent = nd;
			break;
		}

		child = child->next;
	}
	treeNode->addr = nd;
}

void newNodeAssignSingleVar(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = ASSIGN_SINGLE_VAR;
    nd->tkn = NULL;
	while(child!=NULL)
	{
		if(child->sym == leftHandSide_singleVar)
		{
			nd->children = (ASTnode*)child->addr;
			currChild = (ASTnode*)child->addr;
			// currChild->parent = nd;
		}
		else if (child->sym == rightHandSide_type1)
		{
			currChild->next = (ASTnode*)child->addr;
			// currChild->parent = nd;
			break;
		}

		child = child->next;
	}

	treeNode->addr = nd;
}

void newNodeAssignListVar(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = ASSIGN_LIST_VAR;
    nd->tkn = NULL;
    while(child!=NULL)
    {
        if(child->sym == leftHandSide_listVar)
        {
            ASTnode* temp = initASTnode();
            noOfASTnodes++;
            temp->next = NULL;
            temp->symTablePtr = NULL;
            nd->children = temp;
            temp->children = (ASTnode*)child->addr;
            temp->symValid = 1;
            temp->sym = LIST_VAR;
            temp->tkn = NULL;
            currChild = temp;
            // currChild->parent = nd;
        }
        else if (child->sym == rightHandSide_type2)
        {
            currChild->next = (ASTnode*)child->addr;
            // currChild->parent = nd;
            break;
        }

        child = child->next;
    }

    treeNode->addr = nd;
}

void newNodeSize(parseTree* treeNode)
{
    ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children->next;    //ID as child
    nd->symValid = 1;
    nd->sym = SIZEOP;
    nd->tkn = NULL;

    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    // temp->parent = nd;
    nd->children = temp;
    temp->children = NULL;
    temp->symValid = 0;
    temp->tkn = child->tkn;

    treeNode->addr = nd;
}

void newNodeIf(parseTree* treeNode)
{
    ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = IFSTMT;
    nd->tkn = NULL;
    while(child!=NULL)
    {
        if(child->sym == booleanExpression)
        {
            nd->children = (ASTnode*)child->addr;
            currChild = (ASTnode*)child->addr;
            // currChild->parent = nd;
        }
        else if(child->sym == stmt)
        {
            ASTnode* temp = initASTnode();
            noOfASTnodes++;
            temp->next = NULL;
            temp->symTablePtr = NULL;
            currChild->next = (ASTnode*)temp;
            currChild = temp;
            // temp->parent = nd;
            temp->children = (ASTnode*)child->addr;
            // child->addr->parent = temp;
            temp->symValid = 1;
            temp->sym = STMTS;
            temp->tkn = NULL;

            //otherstmts starts
            child = child->next;
            temp->children->next = (ASTnode*)child->addr;
        }
        else if (child->sym == elsePart)
        {
            currChild->next = (ASTnode*)child->addr;
            // child->addr->parent = nd;
            break;
        }

        child = child->next;
    }
        treeNode->addr = nd;
}

void newNodeElse(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children->next;    //stmt
    nd->symValid = 1;
    nd->sym = ELSESTMT;
    nd->tkn = NULL;

    nd->children = (ASTnode*)child->addr;

    //otherstmts starts
    child = child->next;
    nd->children->next = (ASTnode*)child->addr;

    treeNode->addr = nd;
}

void newNodeIoRead(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children->next->next;  //ID as child
    nd->symValid = 1;
    nd->sym = IOREAD;
    nd->tkn = NULL;

    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    // temp->parent = nd;
    nd->children = temp;
    temp->children = NULL;
    temp->symValid = 0;
    temp->tkn = child->tkn;

    treeNode->addr = nd;
}

void newNodeIoPrint(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children->next->next;  //ID as child
    nd->symValid = 1;
    nd->sym = IOPRINT;
    nd->tkn = NULL;

    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    // temp->parent = nd;
    nd->children = temp;
    temp->children = NULL;
    temp->symValid = 0;
    temp->tkn = child->tkn;

    treeNode->addr = nd;
}

void newNodeFuncall(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;  //FUNID as child
    nd->symValid = 1;
    nd->sym = FUNCALL;
    nd->tkn = NULL;

    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    // temp->parent = nd;
    temp->symTablePtr = NULL;
    temp->next = NULL;
    temp->children = NULL;
    nd->children = temp;
    temp->symValid = 0;
    temp->tkn = child->tkn;

    //inputParameterList starts
    child = child->next->next;
    temp->next = (ASTnode*)child->addr;

    treeNode->addr = nd;
}

void newNodeArithEx2(parseTree* treeNode)
{
	treeNode->addr = (ASTnode*)treeNode->children->next->next->addr; // arithExprsn2_1.addr = arithExprsn2_2.addr
}

void newNodeArithTerm2(parseTree* treeNode)
{
    treeNode->addr = (ASTnode*)treeNode->children->next->next->addr; // arithTerm2_1.addr = arithTerm2_2.addr
}

void newNodeBool1(parseTree* treeNode)
{
	parseTree* child1 = treeNode->children->next;	//booleanExpreesion1
	parseTree* child2 = treeNode->children->next->next->next->next->next;	//booleanExpreesion2
	
	ASTnode* nd = (ASTnode*)treeNode->children->next->next->next->addr;	//logicalOp
	nd->children = (ASTnode*)child1->addr;
	nd->children->next = (ASTnode*)child2->addr;

	treeNode->addr = nd;
}

void newNodeBool2(parseTree* treeNode)
{
	parseTree* child1 = treeNode->children;	//constrainedVars1
	parseTree* child2 = treeNode->children->next->next;	//constrainedVars2
	
	ASTnode* nd = (ASTnode*)treeNode->children->next->addr;	//relationalOp
	nd->children = (ASTnode*)child1->addr;
	nd->children->next = (ASTnode*)child2->addr;

	treeNode->addr = nd;
}

void newNodeNot(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children->next->next;  //booleanExpreesion
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = BOOL_NOT;
    nd->tkn = NULL;
    nd->children = (ASTnode*)child->addr;

    treeNode->addr = nd;
}

void newNodeVarMatrixElem(parseTree* treeNode)
{
	ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    temp->symValid = 0;
    temp->tkn = treeNode->children->tkn;
    temp->children = (ASTnode*)treeNode->children->next->addr;

    treeNode->addr = temp;
}

void newNodeMatrixElem(parseTree* treeNode)
{
	ASTnode* temp1 = initASTnode();
    noOfASTnodes++;
    // temp->parent = nd;
    temp1->symTablePtr = NULL;
    temp1->next = NULL;
    parseTree* child = treeNode->children->next;	//NUM1
    temp1->children = NULL;
    temp1->symValid = 0;
    temp1->tkn = child->tkn;

    //NUM2 starts
    child = child->next->next;
    ASTnode* temp2 = initASTnode();
    noOfASTnodes++;
    temp2->next = NULL;
    temp2->symTablePtr = NULL;
    // temp->parent = nd;
    temp2->children = NULL;
    temp2->symValid = 0;
    temp2->tkn = child->tkn;
    temp1->next = (ASTnode*)temp2;

	treeNode->addr = temp1;
}

void newNodeMatrix(parseTree* treeNode)
{
	ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children->next;    //rows
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = MATRIXDECL;
    nd->tkn = NULL;

	nd->children = (ASTnode*)child->addr;

	treeNode->addr = nd;
}

void siblings1(parseTree* treeNode)	//has 2 children and need to make them as siblings
{
    ASTnode* child1 = (ASTnode*)treeNode->children->addr;
    ASTnode* child2 = (ASTnode*)treeNode->children->next->addr;
    child1->next = child2;
    treeNode->addr = child1;
}

void siblings2(parseTree* treeNode)	//Only for parameter_list
{
    ASTnode* child1 = initASTnode();
    noOfASTnodes++;
    child1->next = NULL;
    child1->symTablePtr = NULL;
    child1->symValid = 1;
    child1->sym = TYPE_ID;
    child1->tkn = NULL;
    child1->children = (ASTnode*)treeNode->children->addr;	//type.addr

    //ID starts
    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    child1->children->next = temp;
    temp->children = NULL;
    temp->symValid = 0;
    temp->tkn = treeNode->children->next->tkn;

    //New node created, sibling starts
    ASTnode* child2 = (ASTnode*)treeNode->children->next->next->addr; //child2 of AST, not ParseTree => remainingList.addr
    child1->next = child2;

    treeNode->addr = child1;
}

void siblings3(parseTree* treeNode)     //If one children is terminal and other is NT
{
    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    temp->children = NULL;
    temp->symValid = 0;
    temp->tkn = treeNode->children->tkn;
    // printf("%s\n", temp->tkn->lexeme);
    temp->next = (ASTnode*)treeNode->children->next->addr;
    // printf("%s\n", treeNode->children->next->tkn->lexeme);
    treeNode->addr = temp;
}

void siblings4(parseTree* treeNode) //Only for rows
{
    ASTnode* nd = initASTnode();
    noOfASTnodes++;
    nd->next = NULL;
    nd->symTablePtr = NULL;
    parseTree* child = treeNode->children;    //row
    ASTnode* currChild;
    nd->symValid = 1;
    nd->sym = ROW;
    nd->tkn = NULL;

    nd->children = (ASTnode*)child->addr;
    
    nd->next = (ASTnode*)child->next->addr;   //rows2

    treeNode->addr = nd;
}

void assign1(parseTree* treeNode)		//assign a single child
{
    treeNode->addr = (ASTnode*)treeNode->children->addr;
}

void assign2(parseTree* treeNode)		//Single Terminal Child
{
    ASTnode* temp = initASTnode();
    noOfASTnodes++;
    temp->next = NULL;
    temp->symTablePtr = NULL;
    temp->children = NULL;
    temp->symValid = 0;
    temp->tkn = treeNode->children->tkn;
    treeNode->addr = temp;
}

void assign3(parseTree* treeNode)		//has many children, but only single non terminal child, so only need to assign to that child
{
    parseTree* child = treeNode->children;
    while(child->termOrNonTerm != 1)    //tiil get a Non Terminal
    {
        child = child->next;
    }
    treeNode->addr = (ASTnode*)child->addr;
}

void printASTInOrder(ASTnode* ast)
{
    if (ast == NULL)
    {
        return;
    }

    ASTnode* child = ast->children;
    printASTInOrder(child);
    if (ast->tkn == NULL)
    {
        if (ast->parent != NULL)
        {
            if (ast->parent->tkn == NULL)
            {
                printf("(Dummy Node): %s   Parent: %s\n", enumToString2(ast->sym), enumToString2(ast->parent->sym));
            }
            else
            {
                 printf("(Dummy Node): %s   Parent: %s\n", enumToString2(ast->sym), ast->parent->tkn->lexeme);               
            }
            
        }
    }
    else
    {
        if (ast->parent != NULL)
        {
            if (ast->parent->tkn == NULL)
            {
                printf("Node: %s   Parent: %s\n", ast->tkn->lexeme, enumToString2(ast->parent->sym));
            }
            else
            {
                printf("Node: %s   Parent: %s\n", ast->tkn->lexeme, ast->parent->tkn->lexeme);
            }
        }
    }
    if (child == NULL)
    {
        return;
    }
    child = child->next;
    while(child!=NULL)
    {
        printASTInOrder(child);
        child = child->next;
    }
}

//root Node parent pointer already made NULL

void makeParentPtrInAST(ASTnode* ast)
{
    if (ast == NULL)
    {
        return;
    }

    ASTnode* child = ast->children;
    while(child!=NULL)
    {
        child->parent = ast;
        makeParentPtrInAST(child);
        child = child->next;
    }

}

void printASTPostOrder(ASTnode* ast)
{
    if(ast == NULL)
    {
        return;
    }
    ASTnode* child = ast->children;
    while(child!=NULL)
    {
        printASTPostOrder(child);
        child = child->next;
    }
    if (ast->tkn == NULL)
    {
        printf("%s\n", enumToString2(ast->sym));
    }
    else
    {
       printf("%s\n", ast->tkn->lexeme);
    }
}

char* enumToString2(ASTnodeSym val)
{
    return convert2[val].str;
}

char* enumToString3(varType t)
{
    return convert3[t].str;
}

varType stringToEnum3(char* str)
{
    for (int i = 0; i < sizeof(convert3)/sizeof(convert3[0]); ++i)
    {
        if(strcmp(str, convert3[i].str)==0) 
        {
            return convert3[i].t;
        }
    }

    printf("Error in VarTypes Name: %s! Change it in abstractSyntaxTreeDef.h\n", str);

}