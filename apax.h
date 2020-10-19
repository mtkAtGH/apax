char *fapa( FILE *currfile);

char *sapa( STRG *strbuf);

int precision(void);

void set_rule(void);

int invalid_rule( int *tally);

void active_rule( int *option);

int isopr(int c);

isint( int c);

isprtarg( int c);

isprtopr( int c);

isopr( int c);

void setopr( int *c);

isopr_dm( int c);

isopr_d( int c);

isopr_m( int c);

isopr_as( int c);

isopr_a( int c);

isopr_s( int c);

isfct( int c);

isclx( int c);

isasgn( int c);

isobj( int c);

islft( int c);

isrgt( int c);

ishft( int c);

isnorm( int c);

iscol( int c);

isrow( int c);

issep( int c);

istrps( int c);

isdirs( int c);

isdirp( int c);

isbill( int c);

isbilr( int c);

ismrk( int c); 

isxpr( int c);

isnot( int c);

diftyp( int new, int old);

isnam( STRS *strs);

isarg( STRS *strs);

isfctl( int *str);

isdcml( int *str);

isnum( STRS *strs);

illarg( STRS *strs);

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

literal_asgn( STRS *strs);

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

