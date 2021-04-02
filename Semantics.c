#include <strings.h>
#include <stdlib.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"


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

//create the mips instruction for an less than, NOTWORKING
struct ExprRes * doLessThan(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the two passed in expression, these are whats being added together
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add", TmpRegName(reg), TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  //relese the registers because we used their results
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}


//create the mips instruction for an add, append the first res, res2, and the +
struct ExprRes * doAdd(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  //assign the reg to an available register
  reg = AvailTmpReg();
  //append the two passed in expression, these are whats being added together
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //append the add instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add", TmpRegName(reg), TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
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

//create the mips instruction for an unary minus
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

//boolean expresion result for if statments '=='
struct BExprRes * doBExpr(struct ExprRes * Res1, struct ExprRes * Res2) {
  struct BExprRes * bRes;
  //append the two instruction sequences that are being compared
  AppendSeq(Res1->Instrs, Res2->Instrs);
  bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
  bRes->Label = GenLabel();
  //append branch not equal instruction
  AppendSeq(Res1->Instrs, GenInstr(NULL, "bne", TmpRegName(Res1->Reg), TmpRegName(Res2->Reg), bRes->Label));
  bRes->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);
  return bRes;
}

//generate the mips code for an if statment, work for checking conditinal is stored in bRes
//all the work for the body of the if statment is stored in seq
struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
  struct InstrSeq * seq2;
  //append the instrutions that will be exicuted if true
  seq2 = AppendSeq(bRes->Instrs, seq);
  //append an instruction that just has a label
  AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
  free(bRes);
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
