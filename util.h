int istrcmp(int *str1,int *str2);

int *strint(int *str, int chr);

int istrln(int *str);

int adigit(int num);

int alower(int num);

int analpha(int num);

int anupper(int num);

int analnum(int num);

void xpr(FILE *currfile,int *str);

NAMS *tracexpr(STRS *strs, int count);

void history(STRS *strs, int count);

void set_trace(void);

void set_radx(void);

void set_tag(void);

void set_prec(int *option);

void set_algr(int *option);

int strent(int **str);

int kbd_strent(int **str);

int *cstois(char *str);

char *istocs(int *str);

STRS *insprevstrs(STRS *strs);

STRS *insnextstrs(STRS *strs);

void delnams(NAMS *tmpn);

void delstrs(STRS *strs);

void delchrs(CHRS *chrs);

int *lftcut(int *str);

int *rgtcut(int *str);

int *lftpad(int *str, int chr);

int *rgtpad(int *str, int chr);

STRS *padstrs(STRS *bots, STRS *tops);

CHRS *chrtos(CHRS *chrs, int *str);

CHRS *setchrs(CHRS *chrs, int c);

CHRS *clrchrs(CHRS *chrs);

STRS *clrstrs(STRS *strs);

NAMS *clrnams(NAMS *tmpn);

char *cclrstr(char *str);

int *clrstr(int *str);

void swapstr(int **one, int **other);

void swapstrs(STRS **one, STRS **other);

NAMS *setnams(int *name);

NAMS *newnam(int *name);

NAMS *oldnam(int *str);

NAMS *con_xpr(int *num);

STRS *setstrs(CHRS *chrs,STRS *strs);

int clrstrm(int option);

int radx_reset(void);

int trace_reset(void);

int tag_reset(void);

int prec_reset(int *opts);

int algr_reset(int *opts);

int *cpystr(int *str);

void cpystrsval(STRS *strs, STRS *copy);

STRS *cpystrs(STRS *strs);

void chr_prnt(CHRS *chrs);

void nam_prnt(NAMS *tmpn);

void strs_prnt(STRS *strs);

void nams_prnt(NAMS *list);

void chrs_prnt(CHRS *chrs);

char *chr1ptr(int nmem);

int *int1ptr(int nmem);

int **int2ptr(int nmem);

int ***int3ptr(int nmem);

CHRS *mkchrs(int size);

NAMS *mknams(int size);

STRS *mkstrs(int size);

CHRS *chrs1ptr(int size);

NAMS *nams1ptr(int size);

STRS *strs1ptr(int size);

STRS **strs2ptr(int size);

STRS ***strs3ptr(int size);

int sgetc(STRG *strg);

STRG *sopen(char *string);

STRG *sclose(STRG *strg);

STRG *srewind(STRG *strg);

void free(void*);

/* 
 * Calloc prototype now drawn from <stdlib.h> to silence  gcc warnings
 *
  void *calloc(int, int
 * 
 *  */   

void exit( int exit_status);
