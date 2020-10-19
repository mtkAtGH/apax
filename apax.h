char *fapa( FILE *currfile);

char *sapa( STRG *strbuf);

int precision(void);

void set_rule(void);

int invalid_rule( int *tally);

void active_rule( int *option);

int isopr(int c);

int isint( int c);

int isprtarg( int c);

int isprtopr( int c);

int isopr( int c);

void setopr( int *c);

int isopr_dm( int c);

int isopr_d( int c);

int isopr_m( int c);

int isopr_as( int c);

int isopr_a( int c);

int isopr_s( int c);

int isfct( int c);

int isclx( int c);

int isasgn( int c);

int isobj( int c);

int islft( int c);

int isrgt( int c);

int ishft( int c);

int isnorm( int c);

int iscol( int c);

int isrow( int c);

int issep( int c);

int istrps( int c);

int isdirs( int c);

int isdirp( int c);

int isbill( int c);

int isbilr( int c);

int ismrk( int c); 

int isxpr( int c);

int isnot( int c);

int diftyp( int new, int old);

int isnam( STRS *strs);

int isarg( STRS *strs);

int isfctl( int *str);

int isdcml( int *str);

int isnum( STRS *strs);

int illarg( STRS *strs);

void str_prnt( STRS *strs);

void mrk_prnt( int mark);


void fix_prnt( int *num);


void fct_prnt( int *num);

void padchr( int num, int max, char chr);

void opr_prnt( int *opr);

void erd_prnt( int num);

void erc_prnt( int chr);

void err_prnt( char *msg);

void ers_prnt( int *nam);

void idf_prnt( int *nam);

void txt_prnt( char *txt);

int poswidth( int num);

int numwidth( int num);

int fctrlen( int maxpos);

int isprtopr( int c);

STRS *scan( int *opts);

void help( int *opts, CHRS *chrs, int *c);


CHRS *pick( int  *opts, CHRS *chrs, STRS **strs, int c);

void preparse(  int *error, STRS **strs, int *opts);

void parse(  STRS **strs, int *opts);

int literal_asgn( STRS *strs);

STRS *reduce_oprn( STRS *strs, int sigdgt);

void reduce_exclsv( STRS *strs, int sigdgt, char *rule);

void reduce_inclsv( STRS *strs, int sigdgt, int priority);

STRS *step( STRS *strs);


STRS *reduce( STRS *strs, int sigdgt, int rewind_strs);

int pointrail( int *str);

void format( STRS *strs, int sigdgt);

void fixdgt( STRS *strs, int sigdgt);

int digits( int *str);

NAMS *mknams(int num);

int replnam( STRS **strs);

int irreducible( STRS *strs, int *val);


int unary( STRS **strs, int sigdgt);


void unshift( STRS *strs);


int binary( STRS **strs);


int mixed( STRS **strs, int sigdgt);

void remixed( STRS **strs, int sigdgt);

int *trans( STRS *strs, int sigdgt);

int dtordx( STRS *strs, int sigdgt);

int cpyshft( STRS *strs);

int setform( STRS *strs, int sigdgt);

int shiftable( STRS *strs, int sigdgt);

void equalize( STRS *strs);

int  *arith( STRS *strs, int sigdgt);

int fnctno( char *oprs, char oprn);

void cpynams( int *name, STRS *strs);

void reducible( int *lval, STRS *rval);

