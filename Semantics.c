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

//set the attribute of an array or boolean being put in the table
extern void setAttr(char * type, char * space){
  //create an entry type struct that will be set as the attribute for the name in the table
  struct EntryType * t = (struct EntryType *) malloc(sizeof(struct EntryType));
  //if the type passed in from yacc is array
  if(strcmp(type, "array") == 0){
    //set the attributes in the struct for an array
    int size = atoi(space);
    t->type = 1;
    t->size = size * 4;
  } else{
    //set type for boolean
    t->type = 2; 
  }
  //set the current attribute
  setCurrentAttr(table, (void*) t);
}


//generate the code for using the current value at an index in a array
struct ExprRes * doArrVal(char * name, struct ExprRes * Res1){
  struct ExprRes *res;
  //check if the var has been declared by looking in the symbol table
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  //assign the reg to an available register
  res->Reg = AvailTmpReg();
  struct InstrSeq * inst = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  //make the string used in a later instruction
  char* str1;
  char* str2;
  char* str3;
  str1 = "0(";
  str2 = TmpRegName(res->Reg);
  str3 = ")";
  char * str4 = (char *) malloc(1 + strlen(str1)+ strlen(str2) + strlen(str3));
  strcpy(str4, str1);
  strcat(str4, str2);
  strcat(str4, str3);
  
  //Append the instructions from the expresion passed in
  AppendSeq(inst, Res1->Instrs);
  //append the load address for the array name
  AppendSeq(inst, GenInstr(NULL,"la",TmpRegName(res->Reg), name,NULL));
  //append the sll for the index
  AppendSeq(inst, GenInstr(NULL,"sll",TmpRegName(Res1->Reg),TmpRegName(Res1->Reg),"2"));
  //a[[end the add for the index
  AppendSeq(inst, GenInstr(NULL,"add",TmpRegName(res->Reg),TmpRegName(res->Reg),TmpRegName(Res1->Reg)));
  //append the load word to set the reg to the value at the idex
  AppendSeq(inst, GenInstr(NULL,"lw",TmpRegName(res->Reg),str4,NULL));
  //set the instruction of the return res
  res->Instrs = inst;
  //set the name in the struct
  res->name = strdup(name);
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
  //set the name in the struct
  res->name = strdup(name);
  return res;
}

//create the mips instruction for an &&, append the first res, res2, and the AND
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

//create the mips instruction for an ||, append the first res, res2, and the AND
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
  //append the not sudo instruction 
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
  //append the sub instruction onto those
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
  //append the sub instruction onto those
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub", TmpRegName(reg), "$zero",TmpRegName(Res1->Reg)));
  //relese the registers because we used their results
  ReleaseTmpReg(Res1->Reg);
  Res1->Reg = reg;
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
  //relese the registers because we used their results
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

//create the mips code for a division
struct ExprRes * doDiv(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
  reg = AvailTmpReg();
  //append the two expressions being divided
  AppendSeq(Res1->Instrs,Res2->Instrs);
  //generate and append the division instruction
  AppendSeq(Res1->Instrs,GenInstr(NULL, "div", TmpRegName(reg), TmpRegName(Res1->Reg), TmpRegName(Res2->Reg)));
  //relese the registers because we used their result
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
  //generate and append the move high  instruction
  AppendSeq(Res1->Instrs,GenInstr(NULL,"MFHI", TmpRegName(reg), NULL, NULL));
  //relese the registers because we used their result
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
  //make labels
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
  AppendSeq(Res1->Instrs,GenInstr(NULL, "add", TmpRegName(total), TmpRegName(Res1->Reg), "$zero"));
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
  //relese the registers because we used their result
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  ReleaseTmpReg(total);
  ReleaseTmpReg(reg);
  Res1->Reg = total;
  free(Res2);
  return Res1;
}

//read input from user, set values of each var in list in order
struct InstrSeq * read(struct IdList * List) {
  struct InstrSeq * inst = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  //loop through the list of variables
  while(List != NULL){
    //load the 5 into $v0
    AppendSeq(inst,GenInstr(NULL,"li","$v0", "5", NULL));
    //do system call
    AppendSeq(inst,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    //store the input into the varible
    AppendSeq(inst,GenInstr(NULL,"sw","$v0", List->TheEntry->name, NULL));
    //advance the list
    List = List->Next;
  }
  return inst;
}

///read input from user, set values of each index of array
extern struct InstrSeq * readArr(char * name, struct ExprRes * Res1){
  struct InstrSeq * inst = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  int reg = AvailTmpReg();
  //build the string for later use in mips
  char* str1;
  char* str2;
  char* str3;
  str1 = "0(";
  str2 = TmpRegName(reg);
  str3 = ")";
  char * str4 = (char *) malloc(1 + strlen(str1)+ strlen(str2) + strlen(str3));
  strcpy(str4, str1);
  strcat(str4, str2);
  strcat(str4, str3);

  //append the passed in instructions
  AppendSeq(inst, Res1->Instrs);
  //append load adress for the name of array
  AppendSeq(inst, GenInstr(NULL,"la",TmpRegName(reg), name,NULL));
  //append the sll for the index
  AppendSeq(inst, GenInstr(NULL,"sll",TmpRegName(Res1->Reg),TmpRegName(Res1->Reg),"2"));
  //append the add for the index
  AppendSeq(inst, GenInstr(NULL,"add",TmpRegName(reg),TmpRegName(reg),TmpRegName(Res1->Reg)));
  //store 5 in $v0
  AppendSeq(inst, GenInstr(NULL,"li","$v0", "5", NULL));
  //append the syscall
  AppendSeq(inst, GenInstr(NULL,"syscall", NULL ,NULL, NULL));
  //store the vaule at the calculated address
  AppendSeq(inst, GenInstr(NULL, "sw", "$v0", str4, NULL));
  return inst;
}

//take in a list of expresions, print each one.
struct InstrSeq * print(struct ExprResList * ExprList) {
  struct InstrSeq * inst = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  //loop through the list
  while(ExprList != NULL){
    //create a instruction sequence
    struct InstrSeq * code = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
    //set its value to the instructions of the current expression
    code = ExprList->Expr->Instrs;
    //find the variable name in the table to set its current
    findName(table, ExprList->Expr->name);
    //create a struct of the current attribute 
    struct EntryType * ET = getCurrentAttr(table);
    //if the current attribute is null, print a int
    if(ET == NULL){
      //put correct system call code in $v
      AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
      //put the result to print in $a0
      AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(ExprList->Expr->Reg),NULL));
      //do system call
      AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    } else {
      //if here, print either a boolean or array value based on the type value
      //if the type is a boolean
      if(ET->type == 2){
	//make the labels
        char * label = GenLabel();
	char * end = GenLabel();
	//load the name into reg
	AppendSeq(code,GenInstr(NULL,"lw",TmpRegName(ExprList->Expr->Reg),ExprList->Expr->name,NULL));
	//if the value in the reg is zero, its a false boolean so branch
	AppendSeq(code,GenInstr(NULL,"beq","$zero",TmpRegName(ExprList->Expr->Reg),label));
	//code for printing a true boolean
	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
	AppendSeq(code,GenInstr(NULL,"la","$a0","true",NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
        //after printing, jump to end
	AppendSeq(code,GenInstr(NULL,"j",end,NULL,NULL));
	AppendSeq(code,GenInstr(label,NULL,NULL,NULL,NULL));
	//code for printing a false boolean
	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
	AppendSeq(code,GenInstr(NULL,"la","$a0","false",NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
	//print the end label
	AppendSeq(code,GenInstr(end,NULL,NULL,NULL,NULL));
	//if here, print a array value just like the int above
      } else {
	//put correct system call code in $v
	AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
	//find the variable name in the table to set its current
	AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(ExprList->Expr->Reg),NULL));
	//do system call
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
      }
    }
    //print a space
    AppendSeq(code,GenInstr(NULL,"li","$v0","11",NULL));
    AppendSeq(code,GenInstr(NULL,"li","$a0","32",NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    //append the code to the inst and release the reg
    AppendSeq(inst, code);
    ReleaseTmpReg(ExprList->Expr->Reg);
    ExprList = ExprList->Next;
  }   
  return inst;
}

//print out new lines, take in an expression which is the number of lines
struct InstrSeq * printlines(struct ExprRes * Expr) {
  int reg;
  struct InstrSeq * code;
  code = Expr->Instrs;
  //declaer the lables for the loop
  char * start;
  char * ending;
  //generate the labels
  start = GenLabel();
  ending = GenLabel();
 
  reg = AvailTmpReg();

  //add a 1 into the counter
  AppendSeq(code,GenInstr(NULL, "addi", TmpRegName(reg), "$zero", "0"));
  //print the start label
  AppendSeq(code,GenInstr(start, NULL, NULL, NULL, NULL));
  //branch to the end if the counter reaches the power
  AppendSeq(code,GenInstr(NULL, "bgt", TmpRegName(reg), TmpRegName(Expr->Reg), ending));
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
  //release the registers
  ReleaseTmpReg(Expr->Reg);
  ReleaseTmpReg(reg);
  free(Expr);
  return code;
}

//print blank spaces, take in the number of spaces which is an expression
struct InstrSeq * printspaces(struct ExprRes * Expr) {
  int reg;
  struct InstrSeq * code;
  code = Expr->Instrs;

  //declaer the lables for the loop
  char * start;
  char * ending;
  //generate the labels
  start = GenLabel();
  ending = GenLabel();

  reg = AvailTmpReg();
  //add a 1 into the counter
  AppendSeq(code,GenInstr(NULL, "addi", TmpRegName(reg), "$zero", "0"));
  //print the label
  AppendSeq(code,GenInstr(start, NULL, NULL, NULL, NULL));
  //branch to the end if the counter reaches the limit
  AppendSeq(code,GenInstr(NULL, "bge", TmpRegName(reg), TmpRegName(Expr->Reg), ending));
  //print a space
  AppendSeq(code,GenInstr(NULL,"li","$v0","11",NULL));
  AppendSeq(code,GenInstr(NULL,"li","$a0","32",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  //add one to the counter
  AppendSeq(code,GenInstr(NULL, "addi", TmpRegName(reg), TmpRegName(reg), "1"));
  //jump back to the start of the loop
  AppendSeq(code,GenInstr(NULL, "j", start, NULL, NULL));
  //print the ending label
  AppendSeq(code,GenInstr(ending, NULL, NULL, NULL, NULL));
  //release the registers
  ReleaseTmpReg(Expr->Reg);
  ReleaseTmpReg(reg);
  free(Expr);
  return code;
}

//generate the mips code to do an assignment '=' for ints
struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) {
  struct InstrSeq *code;
  //check if the name var has been delcared
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }
  //set the value to the instructions passed in
  code = Expr->Instrs;
  //genereate and append the store word expresion
  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));
  //release the registers
  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return code;
}

//generate the instructions for assigning a index in a array
extern struct InstrSeq * doArrAssign(char * name, struct ExprRes * Res1, struct ExprRes * Res2){
  struct InstrSeq * code = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  //check if the name var has been delcared
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }
  //make a temp reg
  int reg = AvailTmpReg();
  //make the string used in a later instruction
  char* str1;
  char* str2;
  char* str3;
  str1 = "0(";
  str2 = TmpRegName(reg);
  str3 = ")";
  char * str4 = (char *) malloc(1 + strlen(str1)+ strlen(str2) + strlen(str3));
  strcpy(str4, str1);
  strcat(str4, str2);
  strcat(str4, str3);

  //generate the instructions
  AppendSeq(code, Res1->Instrs);
  //load the address of the variable name
  AppendSeq(code,GenInstr(NULL,"la",TmpRegName(reg), name,NULL));
  //sll for the index
  AppendSeq(code,GenInstr(NULL,"sll",TmpRegName(Res1->Reg),TmpRegName(Res1->Reg), "2"));
  //add for the index
  AppendSeq(code,GenInstr(NULL,"add",TmpRegName(reg), TmpRegName(reg), TmpRegName(Res1->Reg)));
  //append the instrs in reg2 to the code, this is what is being stored
  AppendSeq(code, Res2->Instrs);
  //append the store word with the string generate above
  AppendSeq(code,GenInstr(NULL,"sw", TmpRegName(Res2->Reg),str4,NULL));
  //release registers
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);
  ReleaseTmpReg(reg);
  return code;
}

//generate the code for assigning a boolean a vaule
extern struct InstrSeq * doBoolAssign(char * name, char * val){
  //create the instr sequnce to return
  struct InstrSeq * code = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  struct ExprRes * Res;
  int reg = AvailTmpReg();
  
  //check if the name var has been delcared
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }
  //check if the vaule passed from yacc is true or false
  if(strcmp(val, "TRUE") == 0){
    //store 1 if true
    AppendSeq(code,GenInstr(NULL,"addi",TmpRegName(reg),"$zero", "1"));
    AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(reg), name, NULL));
  }else {
    //store 0 if false
    AppendSeq(code,GenInstr(NULL,"addi",TmpRegName(reg),"$zero", "0"));
    AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(reg), name, NULL));
  }
  //return the code
  return code;
}

//gen mips instruction for comparisions like == or >
extern struct ExprRes * doRel(struct ExprRes * Res1, struct ExprRes * Res2, char * op){
  struct ExprRes * Res;
  int reg = AvailTmpReg();
  //append the two instuction sequences being compared
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
  //set the return reg 
  Res->Reg = reg;	    
  Res->Instrs = Res1->Instrs;
  //free registers
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
  //make the label
  char * label = GenLabel();
  //append the beq to jump to label if false
  AppendSeq(Res->Instrs, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), label));
  //append the instrutions that will be executed if true
  seq2 = AppendSeq(Res->Instrs, seq);
  //append an instruction that just has a label
  AppendSeq(seq2, GenInstr(label, NULL, NULL, NULL, NULL));
  free(Res);
  return seq2;
}

//generate the mips code for an if else statment, work for checking conditinal is stored in bRes
//all the work for the body of the if statment is stored in seq
extern struct InstrSeq * doIfElse(struct ExprRes * Res, struct InstrSeq * seq,  struct InstrSeq * seq2) {
  struct InstrSeq * seq3;
  //make the labels
  char * elseLabel = GenLabel();
  char * end = GenLabel();

  //append the beq insturction that will jump to else if false
  AppendSeq(Res->Instrs, GenInstr(NULL, "beq", "$zero", TmpRegName(Res->Reg), elseLabel));
  //append if statment code
  seq3 = AppendSeq(Res->Instrs, seq);
  //jump to end to skip the else
  AppendSeq(seq3,GenInstr(NULL, "j", end, NULL, NULL));
  //print the else label
  AppendSeq(seq3, GenInstr(elseLabel, NULL, NULL, NULL, NULL));
  //add the code in the else section
  seq3 = AppendSeq(Res->Instrs, seq2);
  //print the end label
  AppendSeq(seq3, GenInstr(end, NULL, NULL, NULL, NULL));
  
  free(Res);
  return seq3;
}

//Generate mips code for while loop
extern struct InstrSeq * doWhile(struct ExprRes * Res, struct InstrSeq * seq) {
  struct InstrSeq * seq2 = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  //declare the lables for the loop
  char * start;
  char * ending;
  //generate the labels
  start = GenLabel();
  ending = GenLabel();
  
  //print the start label
  AppendSeq(seq2,GenInstr(start, NULL, NULL, NULL, NULL));
  AppendSeq(seq2, Res->Instrs);
  //branch to the end if the counter reaches the end condition
  AppendSeq(seq2,GenInstr(NULL, "beq", TmpRegName(Res->Reg), "$zero", ending));
  //append the body
  AppendSeq(seq2, seq);
  //jump back to the start of the loop
  AppendSeq(seq2,GenInstr(NULL, "j", start, NULL, NULL));
  //print the ending label
  AppendSeq(Res->Instrs,GenInstr(ending, NULL, NULL, NULL, NULL));
  free(Res);
  return seq2;
}

//Generate mips code for a for loop
extern struct InstrSeq * doFor(struct InstrSeq * seq, struct ExprRes * Res, struct InstrSeq * seq1, struct InstrSeq * seq2) {
  struct InstrSeq * seq3 = (struct InstrSeq *) malloc(sizeof(struct InstrSeq));
  //declaer the lables for the loop
  char * start;
  char * ending;
  //generate the labels
  start = GenLabel();
  ending = GenLabel();
  //append the passed in instrs
  AppendSeq(seq3, seq);
  //print the start label
  AppendSeq(seq3,GenInstr(start, NULL, NULL, NULL, NULL));
  AppendSeq(seq3, Res->Instrs);
  //branch to the end if the counter reaches the power
  AppendSeq(seq3,GenInstr(NULL, "beq", TmpRegName(Res->Reg), "$zero", ending));
  //append the passed in instrs
  AppendSeq(seq3, seq2);
  AppendSeq(seq3, seq1);
  //jump back to the start of the loop
  AppendSeq(seq3,GenInstr(NULL, "j", start, NULL, NULL));
  //print the ending label
  AppendSeq(Res->Instrs,GenInstr(ending, NULL, NULL, NULL, NULL));
  //ReleaseTmpReg(Res->Reg);
  free(Res);
  return seq3;
}


//Add an expression to the expresion list  
struct ExprResList * addElement(struct ExprRes * x, struct ExprResList *h) {
  struct ExprResList * h1 = (struct ExprResList *) malloc(sizeof(struct ExprResList));
  //set the list's expr to the paased in expr
  h1->Expr = x;
  //set the list's next to the paased in list
  h1->Next = h;
  //return the list
  return h1;
}

//add an variable name to the ID list
extern struct IdList * addVariable(char *x, struct IdList *h){
  //check if the name is in the table
  int find = findName(table, x);
  if(find == 1){
    //if it is, create an entry
    struct SymEntry * entry = table->current;
    //create a list
    struct IdList * h1 = (struct IdList *) malloc(sizeof(struct IdList));
    //set the list entry to the new entry
    h1->TheEntry = entry;
    //set the lists next to the passed in list
    h1->Next = h;
    //return the list
    return h1;
  } else {
    return NULL;
  }
}


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
  
  //iterate the symbol table
  hasMore = startIterator(table);
  while (hasMore) {
    //check if the current var has an attribute
    if (getCurrentAttr(table) == NULL) {
      //if not, its just an int so write out the name
      AppendSeq(code,GenInstr((char *)getCurrentName(table),".word","0",NULL,NULL));
    } else if (getCurrentAttr(table) != NULL){
      //if it does have an attribute, its a boolena or arrray
      struct EntryType * t = getCurrentAttr(table);
      //check the type in the attribute struct
      if(t->type == 1){
	//if the thype is 1 its a array
	char str[100];
	sprintf(str, "%d", t->size);
        AppendSeq(code,GenInstr((char *)getCurrentName(table),".space",str,NULL,NULL));
      } else if(t->type == 2){
	//if the type is 2 its an boolean
	AppendSeq(code,GenInstr((char *)getCurrentName(table),".word","0",NULL,NULL));
      }
    }
    //iterate the table
    hasMore = nextEntry(table);
  }
  //print out other info for .data
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));
  AppendSeq(code,GenInstr("true",".asciiz","\"true\"",NULL,NULL));
  AppendSeq(code,GenInstr("false",".asciiz","\"false\"",NULL,NULL));
  //write the instructions
  WriteSeq(code);
  return;
}
