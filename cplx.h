void complex(STRS **strs,int *opts);

void unmixclx(STRS **strs,int *opts);

void expand(STRS **strs,int *opts);

STRS *lftexp(SMRK *lf, SMRK *op, SJNT *j2);

void rgtexp(SJNT *j1, SMRK *op,SMRK *en, SMRK *rg,STRS **strs,int *opts);

STRS *divlex(SMRK *op,SMRK *rg, STRS **strs,int *opts);

STRS *modlex(SMRK *op, SMRK *rg, int *opts);

STRS *square(STRS *sarg,int *opts);

void collect(STRS **strs,int *opts);

void swapsjnt(SJNT *jnt1, SJNT *jnt2);
