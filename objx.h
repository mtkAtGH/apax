void object(STRS **strs, int *opts);

void operate(OBJO *objo, STRS **strs,int *opts);

void dual_action(OBJO *objo, STRS **strs,int *opts);

void attach_objects(OBJO *objo, STRS **strs);

void simplify_args(OBJO *objo, STRS **strs);

void direct_sum(OBJO *objo, STRS **strs);

STRS *zero_at_depth(OBJT *objt, int depth);

void zero_args(STRS *catch);

STRS *cpyallstrs(STRS *strs);

void idx_refer(OBJT *objt, STRS **strs,int depth);

void left_replicate(OBJO *objo, STRS **strs, int *opts);

void unmixobj(STRS **strs, int *opts);

STRS *remstrs(STRS **strs);

STRS *objt_replicate(OBJT *objt);

void combine_objects(OBJO *objo,STRS **strs);

int checksizes(OBJT *obj1, OBJT *obj2);

void rightaction(OBJO *objo, STRS **strs);

OBJT *setobj(STRS **strs);

OBJT *reference(OBJT *objt, STRS **strs, STRS *name);

void sym_refer(OBJT *objt, STRS **strs, int depth);

STRS *ele_refer(OBJT *objt);

int **symbolic(OBJT *objt);

void maskcopy(OBJT *objt);

void obj_show(OBJT *objt);

int get_choice(void);

void list_choices(void);

void obj_prnt(OBJT *objt);

void dim_prnt(OBJT *objt);

void pmt_prnt(OBJT *objt);

void msk_prnt(OBJT *objt);

void sym_prnt(OBJT *objt);

void idx_prnt(OBJT *objt);

void ele_prnt(OBJT *objt);

void left_implant(STRS *insert, STRS *strs);

void implant(STRS *insert, STRS *strs);

STRS *isolate(STRS **cht, int pos);

int  setmskidx(OBJT *objt, STRS *strs);

int goodidx(STRS **mskcht);

int *masktype(STRS **chrt, int size);

NAMS *findnam(STRS *strs);

OBJT *classify(STRS *strs);

void typify(STRS **chart, int count, int *type);

int locate(OBJT *objt);

int **indicate(OBJT *objt);

void genutype(OBJT *objt);

STRS **chart(STRS *strs,int *count);

OBJT *clrobjt(OBJT *objt);

int **clrsym(int **symbol);

OBJT *obj1ptr(int size);

OBJO *mkobjo(int nmem);

OBJO *objo1ptr(int size);
