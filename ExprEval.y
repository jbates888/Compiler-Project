%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOMngr.h"
#include "SymTab.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
extern int yyerror(char *);
void dumpTable();

extern SymTab *table;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct ExprResList * ExprResList;
  struct IdList * IdList;
}

%type <string> Id
%type <ExprRes> Factor
%type <ExprRes> Term
%type <ExprRes> Expr
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <ExprRes> BExpr
%type <ExprRes> RExpr
%type <ExprRes> Expo
%type <ExprResList> ExList
%type <IdList> IDList

%token Ident 
%token IntLit 
%token Int
%token Write
%token WriteLines
%token WriteSpaces
%token Read
%token IF
%token ELSE
%token WHILE
%token FOR
%token EQ
%token TRUE
%token FALSE
%token Boolean

%%

Prog           :  Declarations StmtSeq                                       {Finish($2); } ;
Declarations   :  Dec Declarations                                           { };
Declarations   :                                                             { };
Dec            :  Int Id {enterName(table, $2); }';'                         {};
Dec            :  Int Id {enterName(table, $2); } '[' IntLit {cpy = strdup(yytext);} ']' ';'   {setAttr("array",cpy);};
Dec            :  Boolean Id {enterName(table, $2); }';'                     {setAttr("boolean", "");};
StmtSeq        :  Stmt StmtSeq                                               {$$ = AppendSeq($1, $2); } ;
StmtSeq        :                                                             {$$ = NULL;} ;
Stmt           :  Read '(' Id '[' Expr ']' ')' ';'                           {$$ = readArr($3, $5); };
Stmt           :  Read '(' IDList ')' ';'                                    {$$ = read($3); };
Stmt           :  Write '(' ExList ')' ';'                                   {$$ = print($3); };
Stmt           :  WriteLines '(' Expr ')' ';'                                {$$ = printlines($3); };
Stmt           :  WriteSpaces '(' Expr ')' ';'                               {$$ = printspaces($3); };
Stmt           :  Id '=' BExpr ';'                                           {$$ = doAssign($1, $3);} ;
Stmt           :  Id '[' BExpr ']' '=' BExpr ';'                             {$$ = doArrAssign($1, $3, $6);} ;
Stmt           :  Id '=' TRUE ';'                                            {$$ = doBoolAssign($1, "TRUE");} ;
Stmt           :  Id '=' FALSE ';'                                           {$$ = doBoolAssign($1, "FALSE");} ;
Stmt           :  WHILE '(' BExpr ')' '{' StmtSeq '}'                        {$$ = doWhile($3, $6);}
Stmt           :  FOR '(' Stmt  BExpr ';' Stmt ')' '{' StmtSeq '}'           {$$ = doFor($3, $4, $6, $9);}
Stmt           :  IF '(' BExpr ')' '{' StmtSeq '}' ELSE '{' StmtSeq '}'      {$$ = doIfElse($3, $6, $10);};
Stmt           :  IF '(' BExpr ')' '{' StmtSeq '}'                           {$$ = doIf($3, $6);};
IDList         :  Id                                                         {$$ = addVariable($1, NULL);};
IDList         :  Id ',' IDList                                              {$$ = addVariable($1, $3);};
ExList         :  BExpr                                                      {$$ = addElement($1, NULL);};
ExList         :  BExpr ',' ExList                                           {$$ = addElement($1, $3);};
BExpr          :  BExpr '&''&' RExpr                                         {$$ = doAnd($1, $4); } ;
BExpr          :  BExpr '|''|' RExpr                                         {$$ = doOr($1, $4); } ;
BExpr          :  RExpr                                                      {$$ = $1;}; 
RExpr          :  Expr EQ Expr                                               {$$ = doRel($1, $3, "==");};
RExpr          :  Expr '!' '=' Expr                                          {$$ = doRel($1, $4, "!=");};
RExpr          :  Expr '<' Expr                                              {$$ = doRel($1, $3, "<");};
RExpr          :  Expr '>' Expr                                              {$$ = doRel($1, $3, ">");};
RExpr          :  Expr '<' '=' Expr                                          {$$ = doRel($1, $4, "<=");};
RExpr          :  Expr '>' '=' Expr                                          {$$ = doRel($1, $4, ">=");};
RExpr          :  Expr                                                       {$$ = $1;}; 
Expr           :  Expr '+' Term                                              {$$ = doAdd($1, $3); } ;
Expr           :  Expr '-' Term                                              {$$ = doSub($1, $3); } ;
Expr           :  Term                                                       {$$ = $1; } ;
Term           :  Term '*' Factor                                            {$$ = doMult($1, $3); } ;
Term           :  Term '/' Factor                                            {$$ = doDiv($1, $3); } ;
Term           :  Term '%' Factor                                            {$$ = doMod($1, $3); } ;
Term           :  Factor                                                     {$$ = $1; } ;
Factor         :  Expo '^' Factor                                            {$$ = doPow($1, $3); } ;
Factor         :  Expo                                                       {$$ = $1;}; 
Expo           :  '!' Expo                                                   {$$ = doNor($2); } ;
Expo           :  '-' Expo                                                   {$$ = doUSub($2) ;};
Expo           :  '(' BExpr ')'                                              {$$ = $2;};
Expo           :  IntLit                                                     {$$ = doIntLit(yytext); };
Expo           :  Id                                                         {$$ = doRval($1); };
Expo           :  Id '[' BExpr ']'                                           {$$ = doArrVal($1, $3); };
Id             :  Ident                                                      {$$ = strdup(yytext);}


%%

int yyerror(char *s)  {
  writeIndicator(getCurrentColumnNum());
  writeMessage("Illegal Character in YACC");
  return 1;
}
