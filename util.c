static char SccsId[] = "@(#) util.c 1.5 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <defn.h>
#include <util.h>
#include <apax.h>
#include <fixr.h>
#include <help.h>

extern NAMS *nams, *hist, *trace;
extern FILE *infile, *outfile;
extern STRG *instrg;
extern int radix, *outstr, sglprc,
           dblprc, maxint, *mxistr, maxjmp, tag, trace_level;


int istrcmp(str1,str2)
int *str1, *str2;
{
while(*str1&&*str2&&*str1==*str2) str1++,str2++;
return (0+*str1)-*str2;
}


int *strint(str,chr)
int *str, chr;
{
while(*str&&*str!=chr) str++;
return !chr||*str ? str : (int*) 0;
}


int istrln(str)
int *str;
{
int num = 0;
while(*str++) num++;
return num;
}


int adigit(num) int num; {return num>='0'&&num<='9';}

int alower(let) int let;
{return (let>='a'&&let<='i')||(let>='j'&&let<='r')||(let>='s'&&let<='z');}

int anupper(let) int let;
{return (let>='A'&&let<='I')||(let>='J'&&let<='R')||(let>='S'&&let<='Z');}

int analpha(let) int let; {return alower(let)||anupper(let);}

int analnum(chr) int chr; {return analpha(chr)||adigit(chr);}


void
xpr(currfile,str)
FILE *currfile;
int *str;
{
fprintf(currfile,"%s",str);
}


NAMS *tracexpr(strs,count)
STRS *strs;
int count;
{
int *inttos(), tmpr = radix;
NAMS *runt, *tmpt, *mknams();
runt = trace;
while(runt->nextnam) runt = runt->nextnam;
tmpt = mknams(1);
tmpt->namno = runt->namno + 1;
radix = 10, tmpt->nam = inttos(count), radix = tmpr;
tmpt->strs = strs;
tmpt->prevnam = runt;
runt->nextnam = tmpt;
return tmpt;
}



void
history(strs,count)
STRS *strs;
int count;
{
int *inttos(), tmpr = radix;
NAMS *runh, *tmph, *mknams();
runh = hist;
while(runh->nextnam) runh = runh->nextnam;
tmph = mknams(1);
tmph->namno = runh->namno + 1;
radix = 10, tmph->nam = inttos(count), radix = tmpr;
while(strs->prevstr) delstrs(strs->prevstr);
tmph->strs = strs;
tmph->prevnam = runh;
runh->nextnam = tmph;
}



void
set_trace()
{
int *tmpstr, *clrstr(), *inttos();
if(!infile&&!instrg) printf("enter trace level: ");
if(strent(&tmpstr)&&isdcml(tmpstr)){
   trace_level = stor10i(tmpstr);
   if(trace_level<0||trace_level>7)
      err_prnt("setting out of bounds, reset to 0"), trace_level = 0;
   }
else err_prnt("trace level reset ignored");
free(clrstr(tmpstr));
}


void
set_radx()
{
int *tmpstr, *clrstr(), *inttos();
if(!infile&&!instrg) printf("enter radix: ");
if(strent(&tmpstr)&&isdcml(tmpstr)){
   radix = stor10i(tmpstr);
   if(radix<2||radix>maxint-'0'-1)
      err_prnt("setting out of bounds, reset to 10"), radix = 10;
   free(clrstr(mxistr)), mxistr = inttos(maxint-'0');
   maxjmp = (maxint-'0'-1)/radix;
   }
else err_prnt("radix reset ignored");
free(clrstr(tmpstr));
}

void
set_tag()
{
int *tmpstr, *clrstr(), *inttos();
if(!infile&&!instrg) printf("enter one of '~', '$', '&' and '_': ");
if(strent(&tmpstr)){
   tag = *tmpstr;
   if(!(tag=='~'||tag=='$'||tag=='&'||tag=='_'))
      err_prnt("setting not permitted, reset to '_'"), tag = '_';
   }
else err_prnt("tag character reset ignored");
free(clrstr(tmpstr));
}




void
set_prec(option)
int *option;
{
int *tmpstr, *clrstr();
if(!infile&&!instrg) printf("enter precision: ");
if(strent(&tmpstr)&&isdcml(tmpstr)){
   *option = stor10i(tmpstr);
   if(*option<0) err_prnt("negative setting made positive"), *option = -*option;
   }
else err_prnt("precision reset ignored");
free(clrstr(tmpstr));
}



void
set_algr(option)
int *option;
{
int *tmpstr, *clrstr();
if(!infile&&!instrg) printf("select precedence rule number[0 to 3]: ");
if(strent(&tmpstr)&&(*tmpstr<=0+'3'&&*tmpstr>=0+'0')){
   option[1] = stor10i(tmpstr);
   if(option[1]<0) option[1] = 0;
   if(option[1]>2){
      if(option[1]>3) option[1] = 3;
      set_rule();
      }
   }
else err_prnt("precedence reset ignored");
free(clrstr(tmpstr));
}


int strent(str)
int **str;
{
int c, slen;
int *int1ptr();
CHRS *chrs, *mkchrs(), *setchrs(), *chrtos(), *clrchrs();
chrs = mkchrs(1);
while((!infile&&!instrg&&(c=getchar())!='\n')||(infile&&(c=fgetc(infile))!='\n')
     ||(instrg&&(c=sgetc(instrg))!='\n')) chrs = setchrs(chrs,c);
slen = chrs->chrno;
*str = int1ptr(chrs->chrno+1);
chrs = chrtos(chrs,*str);
while(chrs->prevchr){
   chrs = chrs->prevchr;
   delchrs(chrs->nextchr);
   }
free(clrchrs(chrs));
return slen;
}

int kbd_strent(str)
int **str;
{
int c, slen;
int *int1ptr();
CHRS *chrs, *mkchrs(), *setchrs(), *chrtos(), *clrchrs();
chrs = mkchrs(1);
while((c=getchar())!='\n')
     chrs = setchrs(chrs,c);
slen = chrs->chrno;
*str = int1ptr(chrs->chrno+1);
chrs = chrtos(chrs,*str);
while(chrs->prevchr){
   chrs = chrs->prevchr;
   delchrs(chrs->nextchr);
   }
free(clrchrs(chrs));
return slen;
}



int *cstois(str)
char *str;
{
int *int1ptr(), *irun, *istr;
irun = istr = int1ptr(strlen(str)+1);
while(*irun++=(int)*str++);
return istr;
}


char *istocs(str)
int *str;
{
int nmem = istrln(str);
char *chr1ptr(), *crun, *cstr = chr1ptr(nmem+1);
crun = cstr;
while(*crun++=(char)*str++);
return cstr;
}


STRS *insprevstrs(strs)
STRS *strs;
{
STRS *tmpstrs, *mkstrs();
tmpstrs = mkstrs(1);
if(!strs) return tmpstrs;
if(strs->prevstr) strs->prevstr->nextstr = tmpstrs;
tmpstrs->prevstr = strs->prevstr;
strs->prevstr = tmpstrs;
tmpstrs->nextstr = strs;
return tmpstrs;
}




STRS *insnextstrs(strs)
STRS *strs;
{
STRS *tmpstrs, *mkstrs();
tmpstrs = mkstrs(1);
if(!strs) return tmpstrs;
if(strs->nextstr) strs->nextstr->prevstr = tmpstrs;
tmpstrs->nextstr = strs->nextstr;
strs->nextstr = tmpstrs;
tmpstrs->prevstr = strs;
return tmpstrs;
}


void
delnams(tmpn)
NAMS *tmpn;
{
NAMS *clrnams();
if(tmpn->nextnam) tmpn->nextnam->prevnam = tmpn->prevnam;
if(tmpn->prevnam) tmpn->prevnam->nextnam = tmpn->nextnam;
free(clrnams(tmpn));
}


void
delstrs(strs)
STRS *strs;
{
STRS *clrstrs();
if(strs->nextstr) strs->nextstr->prevstr = strs->prevstr;
if(strs->prevstr) strs->prevstr->nextstr = strs->nextstr;
free(clrstrs(strs));
}


void
delchrs(chrs)
CHRS *chrs;
{
if(chrs->nextchr)
   chrs->nextchr->prevchr = chrs->prevchr;
if(chrs->prevchr)
   chrs->prevchr->nextchr = chrs->nextchr;
free(chrs);
}


int *lftcut(str)
int *str;
{
int *cut, *run, *ran, *int1ptr();
run = cut = int1ptr(istrln(str));
ran = str + 1;
while(*run++ = *ran++);
free(str);
return cut;
}


int *rgtcut(str)
int *str;
{
int *strint();
int *cut;
cut = strint(str,0)-1;
*cut = 0;
return str;
}


int *lftpad(str,chr)
int *str;
int chr;
{
int *padded, *run, *int1ptr();
int slen;
slen = istrln(str);
padded = int1ptr(slen+2);
run = &padded[slen+1];
while(*str++);
while(run>=&padded[1]) *run-- = *--str;
*padded = chr;
free(str);
return padded;
}


int *rgtpad(str,chr)
int *str, chr;
{
int *padded, *run, *ran, *int1ptr();
run = padded = int1ptr(istrln(str)+2);
ran = str;
while(*run++ = *ran++);
*--run = chr,*++run=0;
free(str);
return padded;
}



STRS *padstrs(bots,tops)
STRS *bots, *tops;
{
while(bots->nextstr) bots = bots->nextstr;
while(tops->prevstr) tops = tops->prevstr;
bots->nextstr = tops;
tops->prevstr = bots;
while(tops->nextstr) tops = tops->nextstr;
return tops;
}


CHRS *chrtos(chrs,str)
CHRS *chrs;
int *str;
{
CHRS *tmpchrs, *clrchrs();
int count = 0;
str[chrs->chrno] = 0;
while(chrs->chrno){
   count += (chrs->chr==0+'.');
   if(count>1){
      err_prnt("illegal argument specification");
      exit(1);
      }
   str[chrs->chrno-1] = chrs->chr;
   tmpchrs = chrs;
   chrs = chrs->prevchr;
   free(clrchrs(tmpchrs));
   }
return chrs;
}

CHRS *setchrs(chrs,c)
CHRS *chrs;
int c;
{
CHRS *tmpchrs, *mkchrs();
tmpchrs = mkchrs(1);
chrs->nextchr = tmpchrs;
tmpchrs->chr = c;
tmpchrs->chrno = chrs->chrno + 1;
tmpchrs->prevchr = chrs;
return tmpchrs;
}


CHRS *clrchrs(chrs)
CHRS *chrs;
{
chrs->chr = 0;
chrs->chrno = 0;
chrs->prevchr = (CHRS*) 0;
chrs->nextchr = (CHRS*) 0;
return chrs;
}


STRS *clrstrs(strs)
STRS *strs;
{
int *clrstr();
if(strs->str) free(clrstr(strs->str));
strs->form =  0;
strs->str = (int*) 0;
strs->shift =  0;
strs->prevstr = (STRS*)  0;
strs->nextstr = (STRS*)  0;
return strs;
}


NAMS *clrnams(tmpn)
NAMS *tmpn;
{
tmpn->nam = 0;
tmpn->namno = 0;
tmpn->prevnam = (NAMS*) 0;
tmpn->nextnam = (NAMS*) 0;
return tmpn;
}


char *cclrstr(str)
char *str;
{
char *run;
run = str - 1;
while(*++run) *run = 0;
return str;
}

int *clrstr(str)
int *str;
{
int *run;
run = str - 1;
while(*++run) *run = 0;
return str;
}


void
swapstr(one,other)
int **one, **other;
{
int *temp;
temp = *other;
*other = *one;
*one = temp;
}


void
swapstrs(one,other)
STRS **one, **other;
{
int *tmpstr;
int tmpform, tmpshift;
tmpform = (*one)->form;
tmpstr = (*one)->str;
tmpshift = (*one)->shift;
(*one)->form = (*other)->form;
(*one)->str = (*other)->str;
(*one)->shift = (*other)->shift;
(*other)->form = tmpform;
(*other)->str = tmpstr;
(*other)->shift = tmpshift;
}




NAMS *setnams(name)
int *name;
{
NAMS *tmpnams, *oldnam(), *newnam();
return (tmpnams = oldnam(name)) ?
         tmpnams : newnam(name);
}



NAMS *newnam(name)
int *name;
{
NAMS *tmpnams, *mknams();
STRS *mkstrs();
int *cpystr(), *cstois();
nams->nextnam = tmpnams = mknams(1);
tmpnams->nam = cpystr(name);
tmpnams->strs = mkstrs(1);
tmpnams->strs->str = cstois(ZERO);
tmpnams->prevnam = nams;
return tmpnams;
}



NAMS *oldnam(str)
int *str;
{
while(nams->prevnam) nams = nams->prevnam;
while(1){
   if(nams->nam&&!istrcmp(str,nams->nam)) return nams;
   if(!nams->nextnam) break;
   nams = nams->nextnam;
   }
return (NAMS*) 0;
}


NAMS *con_xpr(num)
int *num;
{
NAMS *tmph = hist;
int *start = num, *end, hold;
if(!*num) return (NAMS*) 0;
if(*start!=0+tag) return (NAMS*) 0;
while(*++num&&adigit(*num));
if(*(end = num)!=0+tag) return (NAMS*) 0;
if(end[1]) return (NAMS*) 0;
*end = 0;
while(tmph->prevnam) tmph = tmph->prevnam;
while(1){
   if(tmph->nam&&!istrcmp(&start[1],tmph->nam)){
   return tmph;
   }
   if(!tmph->nextnam) break;
   tmph = tmph->nextnam;
   }
*end = 0+tag;
err_prnt("warning: the constant expression history list may hide this variable");
return (NAMS*) 0;
}


STRS *setstrs(chrs,strs)
CHRS *chrs;
STRS *strs;
{
int *int1ptr();
STRS *tmpstrs, *mkstrs();
tmpstrs = mkstrs(1);
strs->nextstr = tmpstrs;
tmpstrs->str = int1ptr(chrs->chrno+1);
tmpstrs->prevstr = strs;
return tmpstrs;
}



int clrstrm(option)
int option;
{
return (option!='\n') ? (infile) ? fgetc(infile) : (instrg) ? sgetc(instrg) : getchar() : option;
}


int radx_reset()
{
static int answer;
int *tmpstr, *clrstr();
radx_prnt();
if(!infile&&!instrg) printf("reset radix ? (y/n) ");
strent(&tmpstr);
answer = *tmpstr;
free(clrstr(tmpstr));
return answer;
}

int trace_reset()
{
static int answer;
int *tmpstr, *clrstr();
trace_prnt();
if(!infile&&!instrg) printf("reset trace level ? (y/n) ");
strent(&tmpstr);
answer = *tmpstr;
free(clrstr(tmpstr));
return answer;
}

int tag_reset()
{
static int answer;
int *tmpstr, *clrstr();
tag_prnt();
if(!infile&&!instrg) printf("reset tag character ? (y/n) ");
strent(&tmpstr);
answer = *tmpstr;
free(clrstr(tmpstr));
return answer;
}



int prec_reset(opts)
int *opts;
{
static int answer;
int *tmpstr, *clrstr();
prec_prnt(opts);
if(!infile&&!instrg) printf("reset precision ? (y/n) ");
strent(&tmpstr);
answer = *tmpstr;
free(clrstr(tmpstr));
return answer;
}



int algr_reset(opts)
int *opts;
{
static int answer;
int *tmpstr, *clrstr();
algr_prnt(opts);
if(!infile&&!instrg) printf("reset precedence rule? (y/n)");
strent(&tmpstr);
answer = *tmpstr;
free(clrstr(tmpstr));
return answer;
}



int *cpystr(str)
int *str;
{
int *copied, *run, *int1ptr();
run = copied = int1ptr(istrln(str)+1);
while(*run++ = *str++);
return copied;
}



void
cpystrsval(strs,copy)
STRS *strs, *copy;
{
int *cpystr();
copy->form = strs->form;
copy->str = cpystr(strs->str);
copy->shift = strs->shift;
}




STRS *cpystrs(strs)
STRS *strs;
{
int *cpystr();
STRS *tmpstrs, *mkstrs();
tmpstrs = mkstrs(1);
tmpstrs->form = strs->form;
tmpstrs->str = cpystr(strs->str);
tmpstrs->shift = strs->shift;
return tmpstrs;
}



void
chr_prnt(chrs)
CHRS *chrs;
{
if(chrs->chr) printf("%c",chrs->chr);
}





void
nam_prnt(tmpn)
NAMS *tmpn;
{
int *strint();
STRS *tmps;
tmps = tmpn->strs;
if(tmpn->nam) idf_prnt(tmpn->nam);
txt_prnt(":  ");
while(tmps){
   str_prnt(tmps);
   tmps = tmps->nextstr;
   }
}




void
strs_prnt(strs)
STRS *strs;
{
STRS *tmps;
tmps = strs;
while(tmps->prevstr) tmps=tmps->prevstr;
if(tmps->str) str_prnt(tmps);
while(tmps->nextstr){
   tmps = tmps->nextstr;
   str_prnt(tmps);
   }
printf("\n");
if(!infile) printf("(");
}




void
nams_prnt(list)
NAMS *list;
{
NAMS *tmpn;
tmpn = list;
while(tmpn->prevnam) tmpn=tmpn->prevnam;
while(tmpn->nextnam){
   tmpn = tmpn->nextnam;
   nam_prnt(tmpn);
   outfile ? fprintf(outfile,"\n") : printf("\n");
   }
}




void
chrs_prnt(chrs)
CHRS *chrs;
{
CHRS *tmpc;
tmpc = chrs;
while(tmpc->prevchr) tmpc = tmpc->prevchr;
while(tmpc->nextchr){
   tmpc = tmpc->nextchr;
   chr_prnt(tmpc);
   }
printf("\n");
}




char *chr1ptr(nmem)
int nmem;
{
char *ptr;
if((ptr = (char *) calloc(nmem,sizeof(char)))==NULL){
   err_prnt("insufficient storage space for character string");
   exit(1);
   }
return ptr;
}



int *int1ptr(nmem)
int nmem;
{
int *ptr;
if((ptr = (int *) calloc(nmem,sizeof(int)))==(int*)0){
   err_prnt("insufficient storage space for integer array");
   exit(1);
   }
return ptr;
}




int **int2ptr(nmem)
int nmem;
{
int **ptr;
if((ptr = (int **) calloc(nmem,sizeof(int*)))==(int**)0){
   err_prnt("insufficient storage space for array of integer pointers");
   exit(1);
   }
return ptr;
}



int ***int3ptr(nmem)
int nmem;
{
int ***ptr;
if((ptr = (int ***) calloc(nmem,sizeof(int**)))==(int***)0){
   err_prnt("insufficient storage space for array of integer pointer arrays");
   exit(1);
   }
return ptr;
}




CHRS *mkchrs(size)
int size;
{
CHRS *chrs, *chrs1ptr();
chrs = chrs1ptr(size);
chrs->chr = 0;
chrs->chrno = 0;
chrs->nextchr = (CHRS*) 0;
chrs->prevchr = (CHRS*) 0;
return chrs;
}




NAMS *mknams(size)
int size;
{
NAMS *tmpn, *nams1ptr();
tmpn = nams1ptr(size);
tmpn->nam = (int*) 0;
tmpn->strs = (STRS*) 0;
tmpn->namno = 0;
tmpn->nextnam = (NAMS*) 0;
tmpn->prevnam = (NAMS*) 0;
return tmpn;
}




STRS *mkstrs(size)
int size;
{
STRS *strs, *strs1ptr();
strs = strs1ptr(size);
strs->form = 0;
strs->str = (int*) 0;
strs->shift = 0;
strs->nextstr = (STRS*) 0;
strs->prevstr = (STRS*) 0;
return strs;
}



CHRS *chrs1ptr(size)
int size;
{
CHRS *ptr;
if((ptr = (CHRS *) calloc(size,sizeof(CHRS)))==(CHRS*)0){
   err_prnt("insufficient storage space for character stack");
   exit(1);
   }
return ptr;
}




NAMS *nams1ptr(size)
int size;
{
NAMS *ptr;
if((ptr = (NAMS *) calloc(size,sizeof(NAMS)))==(NAMS*)0){
   err_prnt("insufficient storage space for string variable stack");
   exit(1);
   }
return ptr;
}





STRS *strs1ptr(size)
int size;
{
STRS *ptr;
if((ptr = (STRS *) calloc(size,sizeof(STRS)))==(STRS*)0){
   err_prnt("insufficient space for string stack");
   exit(1);
   }
return ptr;
}




STRS **strs2ptr(size)
int size;
{
STRS **ptr;
if((ptr = (STRS **) calloc(size,sizeof(STRS *)))==(STRS**)0){
   err_prnt("insufficient space for string stack pointer");
   exit(1);
   }
return ptr;
}


STRS ***strs3ptr(size)
int size;
{
STRS ***ptr;
if((ptr = (STRS ***) calloc(size,sizeof(STRS **)))==(STRS***)0){
   err_prnt("insufficient space for array of string stack pointer array");
   exit(1);
   }
return ptr;
}

int sgetc(strg)
STRG *strg;
{
if(strg->curr) return *strg->curr++;
else return EOF;
}

STRG *sopen(string)
char *string;
{
STRG *strg;
if(!string) return (STRG*) 0;
strg = (STRG*) calloc( 1, sizeof(STRG));
if(strg) strg->curr = strg->start = string;
return strg;
}

STRG *sclose(strg)
STRG *strg;
{
if(strg) free(strg);
strg = (STRG*) 0;
return strg;
}

STRG *srewind(strg)
STRG *strg;
{
if(strg) strg->curr = strg->start;
return strg;
}
