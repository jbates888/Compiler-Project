/* Semantic Records */
struct IdList {
  struct SymEntry * TheEntry;
  struct IdList * Next;
};
struct ExprRes {
  int Reg;
  char * name;
  int Type;
  struct InstrSeq * Instrs;
};
struct ExprResList {
  struct ExprRes *Expr;
  struct ExprResList * Next;
};
struct BExprRes {
  char * Label;
  struct InstrSeq * Instrs;
};
struct EntryType {
  char * type;
  int size;
};

/* Semantics Actions */
extern void setAttr(char * type, char * space);
extern void setBoolAttr();
extern struct ExprRes * doIntLit(char * digits);
extern struct ExprRes * doArrVal(char * name, struct ExprRes * Res1);
extern struct ExprRes * doRval(char * name);
extern struct InstrSeq * doAssign(char * name, struct ExprRes * Res1);
extern struct InstrSeq * doArrAssign(char * name, struct ExprRes * Res1, struct ExprRes * Res2);
extern struct InstrSeq * doBoolAssign(char * name, char * val);
extern struct ExprRes * doAnd(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doOr(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doAdd(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doSub(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doUSub(struct ExprRes * Res1);
extern struct ExprRes * doMult(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doDiv(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doMod(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doPow(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes * doNor(struct ExprRes * Res1);
extern struct InstrSeq * doPrint(struct ExprRes * Expr);
extern struct InstrSeq * readArr(char * name, struct ExprRes * Res1);
extern struct InstrSeq * read(struct IdList * List);
extern struct InstrSeq * print(struct ExprResList * ExprList);
extern struct InstrSeq * printlines(struct ExprRes * Expr);
extern struct InstrSeq * printspaces(struct ExprRes * Expr);
extern struct ExprRes * doRel(struct ExprRes * Res1, struct ExprRes * Res2, char * op);
extern struct InstrSeq * doIf(struct ExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doIfElse(struct ExprRes * Res, struct InstrSeq * seq, struct InstrSeq * seq2);
extern struct InstrSeq * doWhile(struct ExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doFor(struct InstrSeq * seq, struct ExprRes * Res, struct InstrSeq * seq1, struct InstrSeq * seq2);
extern struct ExprResList * addElement(struct ExprRes * x, struct ExprResList *h);
extern struct IdList * addVariable(char * x, struct IdList *h);
extern void Finish(struct InstrSeq *Code);

char * cpy;
