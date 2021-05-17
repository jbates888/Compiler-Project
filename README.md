# Compiler Project

# This program is a complier for a C like programming languge that outputs MIPS code made with lex and yacc. 

# Features: 
* Int and Boolean types
* +, -, *, /, %
* ^ (exponentiation)
* <, <=, ==, >=, >, !=
* !, &&, ||
* Read ints from users
* Print lines, spaces, or a list of variables
* While loops
* For loops
* Single dimension arrays
* Boolean expressions

# How to install
1. Download zip and exctract files or clone 
2. git clone https://github.com/jbates888/Compiler-Project.git

# How to run
1. CD to project directory
2. lex lex1.y
3. yacc -d ExprEval.y
4. gcc -o comp lex.yy.c y.tab.c SymTab.c Semantics.c CodeGen.c IOMngr.c main.c
5. ./comp test1 listing.lst asmCode.asm (test1 is a file with the c like language to be complied)
6. view asmCode.asm to see mips output and the listing.lst to view the input
