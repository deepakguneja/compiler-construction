BIRLA INSTITUTE OF TECHNOLOGY AND SCIENCE, PILANI
DEPARTMENT OF COMPUTER SCIENCE AND INFORMATION SYSTEMS
Compiler Construction (CS F363)
II Semester 2017-18
Compiler Project (Stage-2 Submission)
Coding Details 
(April 20, 2018)
Instruction: Write the details precisely and neatly. Places where you do not have anything to mention, please write NA for Not Applicable.
ID Number: 2015A7PS0083P
Name:  Deepak Guneja

Mention the names of the Submitted files ( Include Stage-1 and Stage-2 both) 
1: Grammar.txt	2: abstractSyntaxTree.c		3: abstractSyntaxTree.h
4: abstractSyntaxTreeDef.h	5: allKeywords.txt		6: codeGenerator.c
	7: codeGenerator.h	8: driver.c	9: hashTableLexer.c	10:hashTableLexer.h
	11: header.h	12: intermediateCodeGen.c	13: intermediateCodeGen.h
	14: intermediateCodeGenDef.h	15: lexer.c	16: lexer.h
	17: lexerDef.h	18: makefile	19: parser.c	  20: parser.h	
	21: parserDef.h	22: readPrint.asm		23: symbolTableAndTypeChecker.c
	24: symbolTableAndTypeChecker.h		25: symbolTableAndTypeCheckerDef.h
	26: codingDetails.docx	27-30 : c1.txt - c4.txt	31-50: testcase1.txt – testcase20.txt
Total number of submitted files: 50 (All files should be in ONE folder named exactly as your ID)
Have you compressed the folder as specified in the submission guidelines? (yes/no): yes

Status of Code development: Mention 'Yes' if you have developed the code for the given module, else mention 'No'.
Lexer (Yes/No): Yes
Parser (Yes/No): Yes
Abstract Syntax tree (Yes/No): Yes
Symbol Table (Yes/ No): Yes
Type checking Module (Yes/No): Yes
Semantic Analysis Module (Yes/ no): Yes (reached LEVEL 4  as per the details uploaded)
Code Generator (Yes/No): Yes
Execution Status:
Code generator produces code.asm (Yes/ No): Yes
code.asm produces correct output using NASM for testcases (C#.txt, #:1-3): Yes
Semantic Analyzer produces semantic errors appropriately (Yes/No): Yes
Type Checker reports type mismatch errors appropriately (Yes/ No): Yes
Symbol Table is constructed (yes/no): Yes and printed appropriately (Yes /No): Yes
AST is constructed (yes/ no): Yes and printed (yes/no): yes
Name the test cases out of 9 as uploaded on the course website for which you get the segmentation fault (testcase#.txt ; # 1-6 and c@.txt ; @:1-3): None


Data Structures (Describe in maximum 2 lines and avoid giving C definition of it)
AST node structure : parent, children, next, varType t, token* tkn, ASTnodeSym sym, symTablePtr, code, addr, trueLabel, falseLabel, nextLabel
Symbol Table structure: varType t, char* lexeme, char* funcScopeName, next pointer, idPtr (if entry is id then pointer to this struct idPtr) else funcPtr (if entry is funID)
Matrix type expression structure: parent: MATRIXDECL (a dummy node)
Its children are ROW → ROW →ROW … (where ROW is a dummy node)
and each ROW has children NUM1→ NUM2→ NUM3...
Input parameters type structure: linked list of children having parent as IN_PARAM (a dummy node) and each parameter is child, where each child is (type, id)
Output parameters type structure: linked list of children having parent as OUT_PARAM (a dummy node) and each parameter is child, where each child is (type, id)
Structure for maintaining the three address code(if created) : Created 4 address code, a quadruple which has structure (result, op, arg1, arg2), where result, arg1 and arg2 are ASTnode and op is the operator to be done.

Semantic Checks: Mention your scheme NEATLY for testing the following major checks (in not more than 5-10 words)[ Hint: You can use simple phrases such as 'symbol table entry empty', 'symbol table entry already found populated', 'traversal of linked list of parameters and respective types' etc.]
Variable not Declared : symbol Table entry empty
Multiple declarations:  symbol table entry already found populated
Number and type of input and output parameters: traversal of list in the symbol table entry of that function, and verifying input and output parameters
assignment of value to the output parameter in a function: used a flag, which is set to 0 if output parameter not initialised
function call semantics: checking for that funID in symbolTable and then if exists, traversal the list in the symbol table entry of that function, and verifying input and output parameters
type checking : traversing AST only once and populating symbol table and checking type by bottom up
return semantics: checking the number and type of output parameters of the function called, by looking at its symbol table entry
Recursion : checking the symbol table stack and finding if the function is already loaded
module overloading: symbol table entry already found populated
'If' semantics : type checking the boolean expression by bottom up traversing AST
Matrix semantics and type checking of matrix type variables: calculating size of a row by traversing AST, and if it does not match previous row size, giving error otherwise update size in symbolTable. 
And if 2 matrices then checking their size in their symbol table and if equal then doing the given operation
register allocation (your manually selected heuristic) : AX, BX for general purpose, and EBP for stack
Scope of variables and their visibility : by looking at stack of symbol table and checking if they exist in which symbol table
Compilation Details:
Makefile works (yes/No): yes
Code Compiles (Yes/ No): yes
Mention the .c files that do not compile: NA
Any specific function that does not compile: NA
Ensured the compatibility of your code with the specified  gcc version(yes/no): yes
Driver Details: Does it take care of the options specified earlier?(yes/no): yes
Specify the language features your compiler  is not able to handle (in maximum one line): NA
Are you availing the lifeline (Yes/No): Yes
Write exact command you expect to be used for executing the code.asm using NASM simulator [We will use these directly while evaluating your NASM created code]
: If the output file is code.asm →

nasm -f elf code.asm		[Press Enter]
ld -m elf_i386 code.o -o code 		[Press Enter]
./code		[Press Enter]

Strength of your code(Strike off where not applicable): (a) correctness  (b) completeness  (c) robustness (d) Well documented  (e) readable  (f) strong data structure  (f) Good programming style (indentation, avoidance of goto stmts etc) (g) modular (h) space  and time efficient  
Any other point you wish to mention: NA
Declaration: I,  Deepak Guneja,  declare that I have put my genuine efforts in creating the compiler project code and have submitted the code developed by me. I have not copied any piece of code from any source. If my code is found plagiarized in any form or degree, I understand that a disciplinary action as per the institute rules will be taken against me and I will accept the penalty as decided by the department of Computer Science and Information Systems, BITS, Pilani.
Sign:Deepak Guneja 
ID: 2015A7PS0083P                             
Name: Deepak Guneja       
Date: 21/4/18
-------------------------------------------------------------------------------------------------------------------------------------------------
/*not to exceed three pages*/
					
