

struct ExprRes * doIntLit(char * digits) {
  struct ExprRes *res;

  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);
  return res;
}

struct ExprRes * doRval(char * name) {
  struct ExprRes *res;

  if (!FindName(table, name)) {
    WriteIndicator(GetCurrentColumn());
    WriteMessage("Undeclared variable");
  }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);
  return res;
}

struct ExprRes * doAdd(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",
				  TmpRegName(reg),
				  TmpRegName(Res1->Reg),
				  TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes * doMult(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;

  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
				  TmpRegName(reg),
				  TmpRegName(Res1->Reg),
				  TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct InstrSeq * doPrint(struct ExprRes * Expr) {
  struct InstrSeq *code;
  code = Expr->Instrs;

  AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
  AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
  AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
  AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return code;
}

struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) {
  struct InstrSeq *code;

  if (!FindName(table, name)) {
    WriteIndicator(GetCurrentColumn());
    WriteMessage("Undeclared variable");
  }
  code = Expr->Instrs;

  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));
  ReleaseTmpReg(Expr->Reg);
  free(Expr);

  return code;
}

struct BExprRes * doBExpr(struct ExprRes * Res1, struct ExprRes * Res2) {
  struct BExprRes * bRes;
  AppendSeq(Res1->Instrs, Res2->Instrs);
  bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
  bRes->Label = GenLabel();
  AppendSeq(Res1->Instrs,
	    GenInstr(NULL, "bne", TmpRegName(Res1->Reg),
		     TmpRegName(Res2->Reg), bRes->Label));
  bRes->Instrs = Res1->Instrs;
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  free(Res1);
  free(Res2);
  return bRes;
}

struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
  struct InstrSeq * seq2;
  seq2 = AppendSeq(bRes->Instrs, seq);
  AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
  free(bRes);
  return seq2;
}

void Finish(struct InstrSeq *Code) {
  struct InstrSeq *code;
  struct SymEntry *entry;
  struct Attr * attr;
  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL));
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));
  hasMore = startIterator(table);
  while (hasMore) {
    AppendSeq(code,GenInstr((char *)getCurrentName(table),
			     “.word","0",NULL,NULL));
 hasMore = nextEntry(table);
 }

 WriteSeq(code);

}
