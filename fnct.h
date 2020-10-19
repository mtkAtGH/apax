int replfnct(STRS **strs, int sigdgt);

int *xpow(int sign, STRS *size, int sigdgt);

int *fpow(STRS **sign, STRS **size, int sigdgt);

void convert(STRS *strs);

int found(STRS **strs, STRS **size, STRS **sign);

int *xlog(int sign, STRS *size, int sigdgt);

int *xexp(int sign, STRS *size, int sigdgt);

int insigdgts(int *term);

int *correct(int *term, int sigdgt);

int *incorrect(int *sumf, int *term);

int *neutral( int *str);

CHRS *sequence(int target);

int xfnctno(int *name);
