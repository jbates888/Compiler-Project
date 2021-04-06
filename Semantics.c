#include <strings.h>
#include <stdlib.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"
#include <string.h>

extern SymTab* table;

//create the mips code for storing a Int
struct ExprRes * doIntLit(char * digits) {
  struct ExprRes *res;

  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  //assign the reg to an available register
  res->Reg = AvailTmpReg();
  //generatet the load integer instuction
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);
  return res;
}

//create mips code for using the current value of the varibale name
struct ExprRes * doRval(char * name) {
  struct ExprRes *res;

  //check if the var has been declared by looking in the symbol table
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  //assign the reg to an available register
  res->Reg = AvailTmpReg();
  //generatet the load word instruction
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);
  return res;
}

//create the mips instruction for an &&, append the first res, res2, and the AND, DOES NOT WORK IN IN STMT
extern struct ExprRes * doAnd(struct ExprRes * Res1, struct ExprRes * Res2) {
  struct ExprRes * Res;
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the two passed in expression, these are whats being added together
  AppendSeq(Res1->Instrs,Res2->Instrs);
  Res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"and", TmpRegName(reg), TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  //relese the registers because we used their results
  Res->Reg = reg;
  Res->Instrs =  Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);
  return Res;
}

//create the mips instruction for an ||, append the first res, res2, and the AND, DOES NOT WORK IN IN STMT
extern struct ExprRes * doOr(struct ExprRes * Res1, struct ExprRes * Res2) {
  struct ExprRes * Res;
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the two passed in expression, these are whats being added together
  AppendSeq(Res1->Instrs,Res2->Instrs);
  Res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"or", TmpRegName(reg), TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  //relese the registers because we used their results
  Res->Reg = reg;
  Res->Instrs =  Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);
  return Res;
}

//create the mips instruction for an !
extern struct ExprRes * doNor(struct ExprRes * Res1) {
  struct ExprRes * Res;
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  Res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sltu", TmpRegName(reg), "$zero",TmpRegName(Res1->Reg)));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"xori", TmpRegName(reg), TmpRegName(reg), "1"));
  //relese the registers because we used their results
  Res->Reg = reg;
  Res->Instrs =  Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  free(Res1);
  return Res;
}


//create the mips instruction for an add, append the first res, res2, and the +
struct ExprRes * doAdd(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the two passed in expression, these are whats being added together
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"Add", TmpRegName(reg), TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  //relese the registers because we used their results
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

//create the mips instruction for an subtract, append the first res, res2, and the -
struct ExprRes * doSub(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the two passed in expression, these are whats being added together
  AppendSeq(Res1->Instrs,Res2->Instrs);
  
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub", TmpRegName(reg), TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  
  //relese the registers because we used their results
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}


//generatee the mips instruction for an unary minus
struct ExprRes * doUSub(struct ExprRes * Res1) {
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub", TmpRegName(reg), "$zero",TmpRegName(Res1->Reg)));
  //relese the registers because we used their results
  ReleaseTmpReg(Res1->Reg);
  Res1->Reg = reg;
  //free(Res2);
  return Res1;
}


//create the mips code for a multiplication
struct ExprRes * doMult(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  reg = AvailTmpReg();
  //append the two expressions being multilpied
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //generate and append the multipliction instruction
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul", TmpRegName(reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

//create the mips code for a division WHY DOES THIS WORK?
struct ExprRes * doDiv(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  reg = AvailTmpReg();
  //append the two expressions being divided
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //generate and append the division instruction
  AppendSeq(Res1->Instrs,GenInstr(NULL, "div", TmpRegName(reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

//create the mips code for a modulo 
struct ExprRes * doMod(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  reg = AvailTmpReg();
  //append the two expressions being divided
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //generate and append the division instruction
  AppendSeq(Res1->Instrs,GenInstr(NULL,"div", TmpRegName(reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
  //generate and append the modulo  instruction
  AppendSeq(Res1->Instrs,GenInstr(NULL,"MFHI", TmpRegName(reg), NULL, NULL));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

//create the mips code for rasing a number to the power 
struct ExprRes * doPow(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  struct InstrSeq * seq2;
  int total;

  //declaer the lables for the loop
  char * start;
  char * ending;
  char * zero;

  start = GenLabel();
  ending = GenLabel();
  zero = GenLabel();
  
  reg = AvailTmpReg();
  total = AvailTmpReg();
  //append the two expressions passed in
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //add a 1 into the counter
  AppendSeq(Res1->Instrs,GenInstr(NULL, "addi", TmpRegName(reg), "$zero", "1"));
  //add the number being mulitplyed to the total
  AppendSeq(Res1->Instrs,GenInstr(NULL, "addi", TmpRegName(total), TmpRegName(Res1->Reg), "$zero"));
  //branch to zero case if the power is equal to zero
  AppendSeq(Res1->Instrs,GenInstr(NULL, "beq", "$zero", TmpRegName(Res2->Reg), zero));
  //print the label
  AppendSeq(Res1->Instrs,GenInstr(start, NULL, NULL, NULL, NULL));
  //branch to the end if the counter reaches the power
  AppendSeq(Res1->Instrs,GenInstr(NULL, "beq", TmpRegName(reg), TmpRegName(Res2->Reg), ending));
  //multiply the total by the res1, store the result in the total
  AppendSeq(Res1->Instrs,GenInstr(NULL, "mul", TmpRegName(total),TmpRegName(total),TmpRegName(Res1->Reg)));
  //add one to the counter
  AppendSeq(Res1->Instrs,GenInstr(NULL, "addi", TmpRegName(reg), TmpRegName(reg), "1"));
  //jump back to the start of the loop
  AppendSeq(Res1->Instrs,GenInstr(NULL, "j", start, NULL, NULL));
  //print hte label for the zero case
  AppendSeq(Res1->Instrs,GenInstr(zero, NULL, NULL, NULL, NULL));
  //make the total 1 if the exponenet passed in was a 0
  AppendSeq(Res1->Instrs,GenInstr(NULL, "addi", TmpRegName(total), "$zero", "1"));
  //print the ending label
  AppendSeq(Res1->Instrs,GenInstr(ending, NULL, NULL, NULL, NULL));
  
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  ReleaseTmpReg(total);
  ReleaseTmpReg(reg);
  
  Res1->Reg = total;
  free(Res2);
  return Res1;
}


//generate the instructions to print the value in the register storeing the result of the paassed in expresion in mips
struct InstrSeq * doPrint(struct ExprRes * Expr) {
  struct InstrSeq *code;
  code = Expr->Instrs;
  //put correct system call code in $v
  AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
  //put the result to print in $a0
  AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
  //do system call
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  //print a new line
  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return code;
}

struct InstrSeq * read(struct IdList * List) {
  struct InstrSeq *code;
  return code;
}

struct InstrSeq * print(struct ExprResList * ExprList) {
  struct InstrSeq *code;
  return code;
}

//print out new lines, take in an expression which is the number of lines
struct InstrSeq * printlines(struct ExprRes * Expr) {
  int reg;
  struct InstrSeq * code;
  code = Expr->Instrs;

  //declaer the lables for the loop
  char * start;
  char * ending;
  start = GenLabel();
  ending = GenLabel();

  reg = AvailTmpReg();

  //add a 1 into the counter
  AppendSeq(code,GenInstr(NULL, "addi", TmpRegName(reg), "$zero", "0"));
  //print the label
  AppendSeq(code,GenInstr(start, NULL, NULL, NULL, NULL));
  //branch to the end if the counter reaches the power
  AppendSeq(code,GenInstr(NULL, "bge", TmpRegName(reg), TmpRegName(Expr->Reg), ending));
  //print a new line
  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  //add one to the counter
  AppendSeq(code,GenInstr(NULL, "addi", TmpRegName(reg), TmpRegName(reg), "1"));
  //jump back to the start of the loop
  AppendSeq(code,GenInstr(NULL, "j", start, NULL, NULL));
  //print the ending label
  AppendSeq(code,GenInstr(ending, NULL, NULL, NULL, NULL));

  ReleaseTmpReg(Expr->Reg);
  ReleaseTmpReg(reg);
  free(Expr);
  return code;
  
}

//print blacnk spaces, take in the number of spaces which is an ecpression
struct InstrSeq * printspaces(struct ExprRes * Expr) {
  struct InstrSeq *code;
  code = Expr->Instrs;
  //put correct system call code in $v
  AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
  //put the result to print in $a0
  AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
  //do system call
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return code;
}

//generate the mips code to do an assignment '='
struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) {
  struct InstrSeq *code;

  //check if the name var has been delcared
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }
  code = Expr->Instrs;

  //genereate and append the store word expresion
  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));
  ReleaseTmpReg(Expr->Reg);
  free(Expr);

  return code;
}

//gen mips instruction for comparisions like == or >
extern struct ExprRes * doRel(struct ExprRes * Res1, struct ExprRes * Res2, char * op){
  struct ExprRes * Res;
  int reg = AvailTmpReg();
  AppendSeq(Res1->Instrs, Res2->Instrs);
  Res = (struct ExprRes *) malloc(sizeof(struct ExprRes));

  //check for each type of comparrison passed in 
  if(strcmp(op, "==") == 0){
    AppendSeq(Res1->Instrs, GenInstr(NULL,"seq",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  } else if (strcmp(op, "!=") == 0){
    AppendSeq(Res1->Instrs, GenInstr(NULL,"sne",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  } else if (strcmp(op, "<") == 0){
    AppendSeq(Res1->Instrs, GenInstr(NULL,"slt",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  }  else if (strcmp(op, ">") == 0){
    AppendSeq(Res1->Instrs, GenInstr(NULL,"sgt",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  } else if (strcmp(op, "<=") == 0){
    AppendSeq(Res1->Instrs, GenInstr(NULL,"sle",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  } else if (strcmp(op, ">=") == 0){
    AppendSeq(Res1->Instrs, GenInstr(NULL,"sge",TmpRegName(reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  }
  
  Res->Reg = reg;	    
  Res->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);
  return Res;
}

//generate the mips code for an if statment, work for checking conditinal is stored in bRes
//all the work for the body of the if statment is stored in seq

extern struct InstrSeq * doIf(struct ExprRes * Res, struct InstrSeq * seq) {
  struct InstrSeq * seq2;
  char * label = GenLabel();
  //append the instrutions that will be executed if true
  AppendSeq(Res->Instrs, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), label));
  seq2 = AppendSeq(Res->Instrs, seq);
  //append an instruction that just has a label
  AppendSeq(seq2, GenInstr(label, NULL, NULL, NULL, NULL));
  free(Res);
  return seq2;
}

//called for the top production "Prog"
//Take the linked list of instructions generated, and write them to a file
//genereate a data section based on whats in the symbol table
void Finish(struct InstrSeq *Code) {
  struct InstrSeq *code;
  //struct SymEntry *entry;
  struct Attr * attr;
  int hasMore;

  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL));
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));

  //iterate the symbol table
  hasMore = startIterator(table);
  while (hasMore) {
    AppendSeq(code,GenInstr((char *)getCurrentName(table),".word","0",NULL,NULL));
    hasMore = nextEntry(table);
  }

  WriteSeq(code);
  return;
}
