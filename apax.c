static char SccsId[] = "@(#) apax.c 1.11 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <defn.h>
#include <apax.h>
#include <fixr.h>
#include <fctr.h>
#include <cplx.h>
#include <fnct.h>
#include <objx.h>
#include <help.h>
#include <util.h>


NAMS *nams, *hist, *trace;
FILE *infile = (FILE*) 0, *outfile = (FILE*) 0;
STRG *instrg = (STRG*) 0;
int radix, *mxistr = (int*) 0, *mxir10s = (int*) 0,
   sglprc = 0, dblprc = 0, maxint = 1, maxjmp, tag = 0+'_';
int trace_level = 0;
char *outstr = NULL;
float global_float;
static char *rules[4] = {"LR","MA","DMA"};
static char *oprn_ascii  = " +-*/%:^=![,]|;'#@<>";
static char *oprn_ebcdic = " +-*/%:^=!{,}|;'#@<>";


char *fapa(currfile)
FILE *currfile;
{
FILE *tmpfile;
char *chr1ptr(), *tmpstr, *start;
NAMS *mknams();
STRS *strs, *mkstrs(), *scan();
int opts[2], count = 1;
trace = mknams(1);
tmpfile = infile, infile = currfile;
nams = mknams(1), hist = mknams(1);
precision();
opts[0] = 0, opts[1] = 2, default_prnt();
strs = scan(opts);
if(!strs) return '\0';
if(infile) printf("\n");
while(strs->prevstr) strs = strs->prevstr;
while(strs->nextstr){
   if(strs->nextstr->str){
      int ilen = intlen(strs->nextstr->str), flen = frclen(strs->nextstr->str);
      count = (strs->nextstr->form==1) ?
              count + fctrlen(ilen) + fctrlen(flen) + 1 :
              count + (ilen+flen)*poswidth(radix);
      }
   strs = strs->nextstr;
   }
tmpstr = outstr;
start = outstr = chr1ptr(count);
while(strs->prevstr) strs = strs->prevstr;
while(strs->nextstr){
   str_prnt(strs->nextstr);
   strs = strs->nextstr;
   }
infile = tmpfile, outstr = tmpstr;
return start;
}


char *sapa(strbuf)
STRG *strbuf;
{
char *chr1ptr(), *tmpstr, *start;
NAMS *mknams();
STRS *strs, *mkstrs(), *scan();
STRG *tmpstrg = instrg;
int opts[2], count = 1;
trace = mknams(1);
tmpstrg = instrg, instrg = strbuf;
nams = mknams(1), hist = mknams(1);
precision();
opts[0] = 0, opts[1] = 2, default_prnt();
strs = scan(opts);
if(!strs) return '\0';
if(instrg) printf("\n");
while(strs->prevstr) strs = strs->prevstr;
while(strs->nextstr){
   if(strs->nextstr->str){
      int ilen = intlen(strs->nextstr->str), flen = frclen(strs->nextstr->str);
      count = (strs->nextstr->form==1) ?
              count + fctrlen(ilen) + fctrlen(flen) + 1 :
              count + (ilen+flen)*poswidth(radix);
      }
   strs = strs->nextstr;
   }
tmpstr = outstr;
start = outstr = chr1ptr(count);
while(strs->prevstr) strs = strs->prevstr;
while(strs->nextstr){
   str_prnt(strs->nextstr);
   strs = strs->nextstr;
   }
outstr = tmpstr;
instrg = tmpstrg;
return start;
}

int precision()
{
int *inttos(), *cpystr(), *integer(), *clrstr(), twoint = 2;
float flt = 3.0;
double dbl = 3.0;
radix = 10;
flt = 1.0/flt;
dbl = 1.0/dbl;
while(twoint>maxint) maxint = twoint, twoint*=2;
maxint = twoint-1>maxint ? twoint-1 : maxint+1>maxint ? maxint+1 : maxint;
mxistr = inttos(maxint-'0'), mxir10s = cpystr(mxistr);
maxjmp = (maxint-'0'-1)/radix;
while(((flt = flt*10.0 - 3.0)>=0.0&&flt<1)) sglprc++;--sglprc;
while((dbl = dbl*10.0 - 3.0)>=0.0&&dbl<1) dblprc++;--dblprc;
return dblprc;
}


void
set_rule()
{
int *clrstr(), *rule;
char *istocs(), *cclrstr();
int tally[4];
tally[0] = tally[1] = tally[2] = tally[3] = 0;
if(rules[3]) free(cclrstr(rules[3]));
do{
   if(!infile||!instrg) printf("enter any of D,M,A,S in desired precedence: ");
   strent(&rule);
   rules[3] = istocs(rule);
   }
   while(invalid_rule(tally));
free(clrstr(rule));
}

int invalid_rule(tally)
int tally[];
{
int count = 0;
static int choice;
static char *rule_case[2] = {"DMAS","dmas"};
char *runrul = rules[3];
while(*runrul){
   switch(*runrul++){
      case 'D':case 'd': tally[choice=0]++;
         break;
      case 'M':case 'm': tally[choice=1]++;
         break;
      case 'A':case 'a': tally[choice=2]++;
         break;
      case 'S':case 's': tally[choice=3]++;
         break;
      default: err_prnt("invalid operation indicator");
         return -1;
      }
   while(count<4){
      if(tally[count]>1){
         fprintf(stderr,"%c/%c: %d ",
            rule_case[0][count],rule_case[1][count],tally[choice]);
         err_prnt("priorities or more for this operation is illegal");
         for(count=0;count<4;count++) tally[count] = 0;
         return ++count;
         }
      else count++;
      }
   count = 0;
   }
return 0;
}

void
active_rule(option)
int *option;
{
if(!infile||!instrg) outfile ? fprintf(outfile,"active precedence rule = %d  (%s)\n",
                             option[1],rules[option[1]]) :
      printf("active precedence rule = %d  (%s)\n",option[1],rules[option[1]]);
}

int isint(c) int c;{return c>='0';}

int isprtarg(c) int c;{return analnum(c)||c==(int)tag||c==(int)'.';}

int isprtopr(c) int c;{
return
c==(int)'^'||c==(int)','||c==(int)':'||c==(int)';'||
c==(int)'|'||c==(int)BEG||c==(int)END||c==(int)'-'||c==(int)'+'||
c==(int)'*'||c==(int)'/'||c==(int)'%'||c==(int)'='||c==(int)'!';}

int isopr(c) int c;{
char *val = ('0'==48)? oprn_ascii : oprn_ebcdic;
int idx = 0;while(*++val&&++idx!=c);return (int) *val;}

void
setopr(c) int *c;{
char *val = ('0'==48)? oprn_ascii : oprn_ebcdic;
int idx = 0;while(++idx,*++val) if(*val==*c){*c=idx;break;}}

int isopr_dm(c) int c;{return c>2&&c<6;}

int isopr_d(c) int c;{return c>3&&c<6;}

int isopr_m(c) int c;{return c==3;}

int isopr_as(c) int c;{return c>=1&&c<3;}

int isopr_a(c) int c;{return c==1;}

int isopr_s(c) int c;{return c==2;}

int isfct(c) int c;{return c==9;}

int isclx(c) int c;{return c==6;}

int isasgn(c) int c;{return c==8;}

int isobj(c) int c;{return c==10||c==12;}

int islft(c) int c;{return c==10;}

int isrgt(c) int c;{return c==12;}

int ishft(c) int c;{return c==7;}

int isnorm(c) int c;{return c==13;}

int iscol(c) int c;{return c==11;}

int isrow(c) int c;{return c==14;}

int issep(c) int c;{return c==11||c==14;}

int istrps(c) int c;{return c==15;}

int isdirs(c) int c;{return c==16;}

int isdirp(c) int c;{return c==17;}

int isbill(c) int c;{return c==18;}

int isbilr(c) int c;{return c==19;}

int ismrk(c) int c; {return issep(c)||isclx(c)||isnorm(c)||isobj(c);}

int isxpr(c) int c;{return c==(int)'('||c==(int)')';}

int isnot(c) int c;{return !isprtarg(c)&&!isprtopr(c)&&!isxpr(c);}

int diftyp(new,old) int new, old;{
return isprtarg(new)&&isopr(old)||isopr(new)||isxpr(new)||isasgn(new);}

int isnam(strs) STRS *strs;{int c; if(strs->str) c = *strs->str; else return 0;
if(strs->form!=1) return analpha(c)||c==(int)tag;else return 0;}

int isarg(strs) STRS *strs;{if(!strs) return 0;
if(strs->str&&(isopr(*strs->str)||isxpr(*strs->str))) return 0;
else if(radix!=10) return isint(*strs->str)||*strs->str=='.';
else if(strs->form!=1) return isnam(strs)||isdcml(strs->str);
else return isfctl(strs->str);}

int isfctl(str) int *str;{int *run=str, *ptadrs(), *pt = ptadrs(str);
int pos = (int)(pt-str);for(run--;++run<pt;) if(*run<'0'||*run-'0'>pos--){
fprintf(stderr,"bad value %d at position %d ",*run-'0',pos+1); return 0;}
if(*run) while(*++run) if(*run<'0'||*run-'0'>-(--pos)){
fprintf(stderr,"bad value %d at position %d ",*run-'0',pos+1); return 0;}
return 1;}

int isdcml(str) int *str;{if(!str) return 0;
for(str--;*++str;) if(!adigit(*str)&&*str!='.')return 0;return 1;}

int isnum(strs) STRS *strs;{return isarg(strs)&&!isnam(strs);}

int illarg(strs) STRS *strs;{int *arg = strs->str;
if(isopr(*arg)||isnam(strs)) return 0;
while(*++arg){ if(analpha(*arg)||*arg==(int)tag){
      err_prnt("begin name with letter or underscore");return 1;}}
return 0;
}

void
str_prnt(strs)
STRS *strs;
{
char *strchr();
int  tmpr;
if(!strs->str) return;
if(strs->form==1){
   outstr ?  (int) sprintf(strchr(outstr,'\0'),"!") :
   outfile ? fprintf(outfile,"!") :
             printf("!");
   }
if(isopr(*strs->str)) opr_prnt(strs->str);
else if(!strs->form&&isnam(strs)) idf_prnt(strs->str);
else if(strs->form==1) fct_prnt(strs->str);
else if(strs->form!=radix)
   tmpr = radix, radix = strs->form, fix_prnt(strs->str), radix = tmpr;
else fix_prnt(strs->str);
if(strs->shift){
   outstr ?  (int) sprintf(strchr(outstr,'\0'),"%c%+d",'^',strs->shift) :
   outfile ? fprintf(outfile,"%c%+d",'^',strs->shift) :
             printf("%c%+d",strs->form==1?'f':'e', strs->shift);
   }
}

void
mrk_prnt(mark)
int mark;
{
static char *marker = ",;[]";
   outstr ?  (int) sprintf(strchr(outstr,'\0'),"%c",marker[mark]) :
   outfile ? fprintf(outfile,"%c",marker[mark]) : printf("%c",marker[mark]);
}


void
fix_prnt(num)
int *num;
{
char *strchr();
while(*num){
   if(*num==(int)'.'){
     outstr ?  (int) sprintf(strchr(outstr,'\0'),"%c",*num) :
      outfile ? fprintf(outfile,"%c",*num) : printf("%c",*num);}
   else{ padchr(*num-'0',radix-1,'0');
      outstr ?  (int) sprintf(strchr(outstr,'\0'),"%d",*num-'0') :
      outfile ? fprintf(outfile,"%d",*num-'0') : printf("%d",*num-'0');}
   num++;
   }
}


void fct_prnt(num)
int *num;
{
int pos = intlen(num);
char *strchr();
while(*num){
   if(*num==(int)'.'){
      outstr ?  (int) sprintf(strchr(outstr,'\0'),"%c",*num) :
      outfile ? fprintf(outfile,"%c",*num) : printf("%c",*num);}
   else{ padchr(*num-'0',iabs(pos),'0');
      outstr ?  (int) sprintf(strchr(outstr,'\0'),"%d",*num-'0') :
      outfile ? fprintf(outfile,"%d",*num-'0') : printf("%d",*num-'0');}
   num++,pos--;
   }
}

void padchr(num,max,chr)
int num, max;
char chr;
{
char *strchr();
int count = poswidth(max) - poswidth(num);
while(count--) outstr ? (int) sprintf(strchr(outstr,'\0'),"%c",chr) :
               outfile ? fprintf(outfile,"%c",chr) : printf("%c",chr);
}

void
opr_prnt(opr) int *opr;{char *strchr(); for(--opr;*++opr;)
   outstr ?  (int) sprintf(strchr(outstr,'\0'),"%c",isopr(*opr)) :
   outfile ? fprintf(outfile,"%c",isopr(*opr)) : printf("%c",isopr(*opr));}

void
erd_prnt(num) int num;{fprintf(stderr,"%d",num);}

void
erc_prnt(chr) int chr;{fprintf(stderr,"%c",chr);}

void
err_prnt(msg) char *msg;{fprintf(stderr,"%s\n",msg);}

void
ers_prnt(nam) int *nam;{ for(--nam;*++nam;) fprintf(stderr,"%c",*nam);}

void
idf_prnt(nam) int *nam;{char *strchr();
for(--nam;*++nam;) outstr ?  (int) sprintf(strchr(outstr,'\0'),"%c",*nam) :
   outfile ? fprintf(outfile,"%c",*nam) : printf("%c",*nam);}

void
txt_prnt(txt) char *txt;{char *strchr();
outstr ?  (int) sprintf(strchr(outstr,'\0'),"%s",txt) :
   outfile ? fprintf(outfile,"%s",txt) : printf("%s",txt);}

int poswidth(num) int num;{int count = 1; while(num/=10) count++; return count;}

int numwidth(num) int num;{int count = 1; while(num/=radix) count++; return count;}

int fctrlen(maxpos) int maxpos;{int count = 0, width = 1, order = 1;
   while(order*10<maxpos) count += 9*width++*order,order *= 10;
   if(maxpos>order) count += (maxpos-order)*width; return count;}



STRS *scan(opts)
int *opts;
{
NAMS *track = trace, *tracexpr();
STRS *tmps, *strs, *mkstrs(), *setstrs();
CHRS *chrs, *mkchrs(), *pick(), *chrtos(), *clrchrs();
int c=0, error = 0;
static int xprno;
while(track->nextnam) track = track->nextnam;
if(!track->namno) xprno = 0;
strs = mkstrs(1);
track = tracexpr(strs,++xprno);
chrs = mkchrs(1);
while((!infile&&!instrg&&(c=getchar())!=EOF)
     ||(infile&&(c=fgetc(infile))!=EOF)
     ||(instrg&&(c=sgetc(instrg))!=EOF)){
   if(infile||instrg) putchar(c);
   if(c==(int)'?'){
      help(opts,chrs,&c);
      if(!infile&&!instrg) printf("(");
      }
   if(c==(int)'\n'||c==(int)' '||c=='?') continue;
   chrs = pick(opts,chrs,&strs,c);
   if(c==(int)')') break;
   }
if((infile||instrg)&&(c==')'||c==EOF)) putchar('\n');
preparse(&error,&strs,opts);
if(!error) parse(&strs,opts);
while(chrs->prevchr){
   chrs=chrs->prevchr;
   delchrs(chrs->nextchr);
   }
free(clrchrs(chrs));
delnams(track);
irreducible((STRS*)0, (int*)0);
return strs;
}


void help(opts,chrs,c)
int *opts, *c;
CHRS *chrs;
{
int option, radx_reset(), prec_reset(), algr_reset();
if(infile){
   option = fgetc(infile);
   printf("\n");
   }
else if(instrg) option = sgetc(instrg);
else option = getchar();
switch(option){
   case  '!': fctr_prnt(); *c = clrstrm(0+option); break;
   case  ':': clx_prnt(); *c = clrstrm(0+option); break;
   case  'a': option=clrstrm(0+option);
              if(algr_reset(opts)==(int)'y') set_algr(opts);
              *c = clrstrm(0+option); break;
   case  'b': busage_prnt(); *c = clrstrm(0+option); break;
   case  'c': option=clrstrm(0+option); if(tag_reset()==(int)'y') set_tag();
              *c = clrstrm(0+option); break;
   case  'd': default_prnt(); *c = clrstrm(0+option); break;
   case  'E': option=clrstrm(0+option);
              if(trace_reset()==(int)'y') set_trace();
              *c = clrstrm(0+option); break;
   case  'e': nams_prnt(trace); break;
   case  'f': fnct_prnt(); *c = clrstrm(0+option); break;
   case  'h': nams_prnt(hist); *c = clrstrm(0+option); break;
   case  'i': iusage_prnt(); *c = clrstrm(0+option); break;
   case  'l': lang_prnt(); *c = clrstrm(0+option); break;
   case  'o': boprs_prnt(); *c = clrstrm(0+option); break;
   case  'u': uoprs_prnt(); *c = clrstrm(0+option); break;
   case  'p': option=clrstrm(0+option);
              if(prec_reset(opts)==(int)'y') set_prec(opts);
              *c = clrstrm(0+option); break;
   case  'r': option=clrstrm(0+option); if(radx_reset()==(int)'y') set_radx();
              *c = clrstrm(0+option); break;
   case  't': chrs_prnt(chrs); break;
   case  'v': nams_prnt(nams); *c = clrstrm(0+option); break;
   case  'x': actions_prnt(); *c = clrstrm(0+option); break;
   default  : nopt_prnt(0+option); break;
   case '\n': help_prnt(); *c = option; break;
   }
}


CHRS *pick(opts,chrs,strs,c)
CHRS *chrs;
STRS **strs;
int  *opts, c;
{
static int last;
STRS *tmps, *scan(), *setstrs(), *padstrs();
CHRS *setchrs(), *chrtos();
if(isnot(c)){
   err_prnt("illegal character ignored");
   return chrs;
   }
if(isprtopr(c)) setopr(&c);
if(chrs->chrno&&diftyp(c,last)){
   *strs = setstrs(chrs,*strs);
   chrs = chrtos(chrs,(*strs)->str);
   if(illarg(*strs)){
      err_prnt("illegal argument deleted\n");
      *strs = (*strs)->prevstr;
      delstrs((*strs)->nextstr);
      }
   }
if(!isxpr(c)) chrs = setchrs(chrs,c);
if(c==(int)'('){
   tmps = scan(opts);
   while(tmps->prevstr&&tmps->prevstr->str) tmps = tmps->prevstr;
   if(tmps->prevstr!=(STRS*)0) delstrs(tmps->prevstr);
   else while(tmps->prevstr) tmps = tmps->prevstr;
   *strs = padstrs(*strs,tmps);
   }
last = c;
return chrs;
}

void
preparse(error,strs,opts)
STRS **strs;
int *error, *opts;
{
STRS  *tmps, *step(), *reduce();
int *str;
int sigdgt, rewind_strs = 1;
sigdgt = opts[0];
if(trace_level==1) nams_prnt(trace);
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if(literal_asgn((*strs)->nextstr)&&mixed(&(*strs)->nextstr->nextstr,sigdgt)
      &&binary(&(*strs)->nextstr->nextstr))
      *strs = reduce((*strs)->nextstr->nextstr,sigdgt,rewind_strs);
   *strs = step(*strs);
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if(isfct(*(*strs)->nextstr->str)&&(*strs)->nextstr->nextstr&&
      isnam((*strs)->nextstr->nextstr))  unary(strs,sigdgt);
   else if((*strs)->nextstr) *strs = step(*strs);
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if(isnam((*strs)->nextstr)&&
    (!(*strs)->nextstr->nextstr||
       !isasgn(*(*strs)->nextstr->nextstr->str))&&
      (!(*strs)->nextstr->nextstr||
       !issep(*(*strs)->nextstr->nextstr->str))&&
      (!(*strs)->nextstr->nextstr||
       !isobj(*(*strs)->nextstr->nextstr->str))){
          if(!replnam(&(*strs)->nextstr)){
             *error = 1;
             *strs = (*strs)->nextstr ? (*strs)->nextstr :
                      (*strs)->prevstr ? (*strs)->prevstr : *strs; return;
             }
      }
   else if((*strs)->nextstr) *strs = step(*strs);
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if(isfct(*(*strs)->nextstr->str)&&(*strs)->nextstr->nextstr)
      unary(strs,sigdgt);
   else if((*strs)->nextstr) *strs = step(*strs);
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if(ismrk(*(*strs)->nextstr->str)&&(*strs)->nextstr->nextstr&&
      ismrk(*(*strs)->nextstr->nextstr->str)){
                 if((isclx(*(*strs)->nextstr->str)&&
                    isobj(*(*strs)->nextstr->nextstr->str))||
                   (isobj(*(*strs)->nextstr->str)&&
                        isclx(*(*strs)->nextstr->nextstr->str))) *strs = step(*strs);
                 else mixed(&(*strs)->nextstr,sigdgt);
                 }
   else if((*strs)->nextstr) *strs = step(*strs);
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if((*strs)->nextstr->str&&ishft(*(*strs)->nextstr->str)&&
      (*strs)->nextstr->nextstr&&(*strs)->nextstr->nextstr->str&&
      isopr(*(*strs)->nextstr->nextstr->str)) mixed(&(*strs)->nextstr,sigdgt);
   if((*strs)->nextstr->str&&ishft(*(*strs)->nextstr->str)&&
      binary(&(*strs)->nextstr))
         *strs = reduce((*strs)->nextstr,sigdgt,rewind_strs);
   if((*strs)->nextstr) *strs = step(*strs);
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   *strs = (*strs)->nextstr;
   if((*strs)->str&&isobj(*(*strs)->str)){
      object(strs,opts);
      break;
      }
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   *strs = (*strs)->nextstr;
   if((*strs)->str&&isclx(*(*strs)->str)){
      while((*strs)->prevstr) *strs = (*strs)->prevstr;
      if(!(*strs)->str&&(*strs)->nextstr&&(*strs)->nextstr->str&&
         isopr(*(*strs)->nextstr->str)) unary(strs,sigdgt);
      complex(strs,opts);
      break;
      }
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
}


void parse(strs,opts)
int *opts;
STRS **strs;
{
STRS  *tmps, *step(), *reduce();
int *str;
int sigdgt, rewind_strs = 1;
sigdgt = opts[0];
while((*strs)->prevstr) *strs = (*strs)->prevstr;
if(opts[1]<=2){
   int priority = opts[1];
   while(priority--){
      reduce_inclsv(*strs,sigdgt,priority);
      while((*strs)->prevstr) *strs = (*strs)->prevstr;
      if(trace_level==6) nams_prnt(trace);
      }
   }
else if(*rules[3]){
   char *rule = rules[3];
   while(*rule){
      reduce_exclsv(*strs,sigdgt,rule++);
      while((*strs)->prevstr) *strs = (*strs)->prevstr;
      if(trace_level==6) nams_prnt(trace);
      }
   }
if(trace_level==5||trace_level==6) nams_prnt(trace);
while((*strs)->nextstr){
   if((*strs)->str&&isopr(*(*strs)->str)&&(*strs)->nextstr->str&&
      isopr(*(*strs)->nextstr->str)&&!isobj(*(*strs)->nextstr->str)&&
      mixed(strs,sigdgt)) while((*strs)->prevstr) *strs = (*strs)->prevstr;
   if(!(*strs)->str&&(*strs)->nextstr&&(*strs)->nextstr->str&&
      isopr(*(*strs)->nextstr->str)&&unary(strs,sigdgt))
      while((*strs)->prevstr) *strs = (*strs)->prevstr;
   if((*strs)->prevstr&&(*strs)->prevstr->str&&(*strs)->str&&
      isopr(*(*strs)->str)&&!ismrk( *(*strs)->str)&&binary(strs)){
      *strs = reduce(*strs,sigdgt,rewind_strs);
      while((*strs)->prevstr) *strs = (*strs)->prevstr;
      continue;
      }
   else if((*strs)->nextstr) *strs = step(*strs);
   else break;
   }
if((*strs)->prevstr&&(*strs)->prevstr->str){
   *strs = (*strs)->prevstr;
   if(isarg(*strs)&&isopr(*(*strs)->nextstr->str)&&unary(strs,sigdgt))
      while((*strs)->prevstr&&(*strs)->prevstr->str) *strs = (*strs)->prevstr;
   }
}


int literal_asgn(strs)
STRS *strs;
{
return isnam(strs)&&strs->nextstr&&
      strs->nextstr->str&&isfct(*strs->nextstr->str)&&
      strs->nextstr->nextstr&&strs->nextstr->nextstr->str&&
      isasgn(*strs->nextstr->nextstr->str);
}


STRS *reduce_oprn(strs,sigdgt)
STRS *strs;
int sigdgt;
{
STRS *reduce();
int rewind_strs = 0;
if((isopr(*strs->str)||isasgn(*strs->str))&&(mixed(&strs,sigdgt)
     ||binary(&strs))) return reduce(strs,sigdgt,rewind_strs);
else return strs;
}


void
reduce_exclsv(strs,sigdgt,rule)
STRS *strs;
int sigdgt;
char *rule;
{
STRS *reduce(), *step();
int rewind_strs = 0, (*isopr_x)();
switch(*rule){
   case'D':;case'd': isopr_x = isopr_d;break;
   case'M':;case'm': isopr_x = isopr_m;break;
   case'A':;case'a': isopr_x = isopr_a;break;
   case'S':;case's': isopr_x = isopr_s;break;
   default: erc_prnt(*rule),fprintf(stderr,": unknown operator initial\n");
   }
while(strs->nextstr){
   if(strs->str&&(isopr_x(*strs->str)||isasgn(*strs->str))&&(mixed(&strs,sigdgt)
     ||binary(&strs))) strs = reduce(strs,sigdgt,rewind_strs);
   if(strs->nextstr) strs = step(strs);
   }
}


void
reduce_inclsv(strs,sigdgt,priority)
STRS *strs;
int sigdgt, priority;
{
STRS *reduce(), *step();
int rewind_strs = 0, (*isopr_i[2])();
isopr_i[0] = isopr_dm, isopr_i[1] = isopr_d;
while(strs->nextstr){
   if(strs->str&&(isopr_i[priority](*strs->str)||isasgn(*strs->str))&&
      (mixed(&strs,sigdgt)||binary(&strs)))
      strs = reduce(strs,sigdgt,rewind_strs);
   if(strs->nextstr) strs = step(strs);
   }
}


STRS *step(strs)
STRS *strs;
{
return strs->nextstr;
}


STRS *reduce(strs,sigdgt,rewind_strs)
STRS *strs;
int sigdgt, rewind_strs;
{
int *trans(), *clrstr(), *str = (int*) 0, *rgtpad();
int count = 2;
str = trans(strs,sigdgt);
strs = strs->prevstr;
while(strs->nextstr&&count--) delstrs(strs->nextstr);
if(str){
   if(strs->str) free(clrstr(strs->str));
   strs->str = str;
   if((strs->nextstr&&strs->nextstr->str&&
       isopr(*strs->nextstr->str)&&!ismrk(*strs->nextstr->str))||
      (strs->prevstr&&strs->prevstr->str&&
       isopr(*strs->prevstr->str)&&!ismrk(*strs->prevstr->str)))
      format(strs,sigdgt?sigdgt+1:sigdgt);
   else{
      format(strs,sigdgt);
      if(pointrail(strs->str)) strs->str = rgtpad(strs->str,0+'0');
      }
   }
while(rewind_strs&&strs->prevstr) strs = strs->prevstr;
return strs;
}

int pointrail(str)
int *str;
{
int *ptadrs(), *strint();
return (int)(strint(str,0) - ptadrs(str)) == 1;
}

void
format(strs,sigdgt)
STRS *strs;
int sigdgt;
{
int diff, maxi = maxint;
int *high, *point, *hisgdt(), *ptadrs(), *cstois(),
     *shift(), *trim(), *fctmult();
if(sigdgt&&digits(strs->str)>sigdgt){
   point = ptadrs(strs->str), high = hisgdt(strs->str);
   if(point > high) diff =  (int) (point - high);
   else{
      diff = (int) (high - point);
      diff = strs->form==1 ? -diff : 1 - diff;
      }
   if(diff<0) maxi = -maxi;
   if(strs->shift&&iabs(maxi-diff)<iabs(strs->shift))
      err_prnt("shift value exceeds apa(x) maximum precision");
   else{
      strs->shift = strs->shift + diff;
      strs->str = trim(strs->form==1 ?
      fctmult(strs->str,shift(cstois(ONE),-diff)) : shift(strs->str,-diff));
      fixdgt(strs,sigdgt);
      }
   }
}


void
fixdgt(strs,sigdgt)
STRS *strs;
int sigdgt;
{
int *str = strs->str, *tmpc, *run, *lowone = (int*) 0, *ptadrs(), *fctadd(),
     *lftpad(), *clrstr(), *cstois(), *trim(), *add(), *cpystr();
int pos, *strint();
run = str;
while(*run&&sigdgt) if(*run++>='0') sigdgt--;
if(*run){
   tmpc = run;
   if(strs->form==1&&(pos=tmpc-ptadrs(str),*tmpc>=pos/2))
      lowone = cstois(ONE);
   else if(strs->form!=1&&(*tmpc-'0'>radix/2||
      (!(*tmpc-'0')%2&&*tmpc-'0'==radix/2))) lowone = cstois(ONE);
   *tmpc = 0, run = cpystr(str), *tmpc = '0',free(clrstr(str)),str = run;
   if(lowone){
      run = strint(str,0) - 1;
      if(*run==(int)'.') lowone = lftpad(lowone,0+'.');
      while(run-str){
         if(*--run==(int)'.') lowone = lftpad(lowone,0+'.');
         else lowone = lftpad(lowone,'0');
         }
      str = strs->form==1 ? fctadd(lowone,str) : add(lowone,str);
      if(strs->form!=1) str = trim(str);
      }
   strs->str = str;
   }
}

int digits(str)
int *str;
{
int count = 1, *run = str;
while(*run) if(*run++>='0') count++;
return count;
}


int replnam(strs)
STRS **strs;
{
STRS *tmps, *start, *insprevstrs();
int *cpystr();
NAMS *conx, *con_xpr(), *setnams();
start = *strs;
if(conx=con_xpr((*strs)->str)) tmps = conx->strs;
else nams = setnams((*strs)->str), tmps = nams->strs;
while(tmps->nextstr) tmps = tmps->nextstr;
while(tmps&&tmps->str){
   if(trace_level==2) nams_prnt(trace);
   if(irreducible(start,tmps->str)) return 0;
   *strs = insprevstrs(*strs);
   (*strs)->str = cpystr(tmps->str);
   (*strs)->form = tmps->form;
   (*strs)->shift = tmps->shift;
   tmps = tmps->prevstr;
   }
start = start->prevstr;
delstrs(start->nextstr);
return 1;
}

int irreducible(strs,val)
STRS *strs;
int *val;
{
int loop = 0;
static int cycles, loops;
STRS *start = strs;
static int *last_val, *last_nam;
int *clrstr(), *cpystr();
if(!strs&&!val){
   cycles = loops = 0;
   if(last_val) free(clrstr(last_val));last_val = 0;
   if(last_nam) free(clrstr(last_nam));last_nam = 0;
   return 0;
   }
if((last_val&&!istrcmp(strs->str,last_val))||
   (last_nam&&!istrcmp(val,last_nam))){
   if(++cycles>5){
      fprintf(stderr,"possibly irreducible cycle in substitution of ");
      ers_prnt(last_nam);
      fprintf(stderr," stopped after 5 detections\n");
      cycles = 0;
      return 1;
      }
   if(last_val) free(clrstr(last_val));last_val = 0;
   if(last_nam) free(clrstr(last_nam));last_nam = 0;
   }
if(last_val) free(clrstr(last_val));last_val = cpystr(val);
if(last_nam) free(clrstr(last_nam));last_nam = cpystr(strs->str);
while(strs){
   if(strs->str&&!istrcmp(strs->str,val)) loop = 1;
   strs = strs->prevstr;
   }
if(loops+=loop>5){
   fprintf(stderr,"possibly irreducible loop in substitution of ");
   ers_prnt(start->str);
   fprintf(stderr," stopped after 5 repetitions\n");
   loops = 0;
   return 1;
   }
return 0;
}


int unary(strs,sigdgt)
STRS **strs;
int sigdgt;
{
int c, *temp, *dstofs(), *clrstr(), *integer(),
        *fctrial(), *fcttod(), *ptadrs();
if(trace_level==7){
	printf("unary ");
	strs_prnt(*strs);
        }
if(isfct(*(*strs)->nextstr->str)&&(*strs)->nextstr->nextstr){
   if(isopr(*(*strs)->nextstr->nextstr->str)){
      (*strs)->nextstr->nextstr->form = 1;
      delstrs((*strs)->nextstr);
      return 1;
      }
   if(isnam((*strs)->nextstr->nextstr)){
      if(replfnct(&(*strs)->nextstr->nextstr,sigdgt)){
         delstrs((*strs)->nextstr);
         delstrs((*strs)->nextstr);
         printf("\n");
         return 1;
         }
      else{
         err_prnt("undefined function identifier turned into a variable");
         delstrs((*strs)->nextstr);
         return 1;
         }
      }
   }
while(isopr(c=(*(*strs)->nextstr->str))&&!ismrk(c)&&
   (*strs)->nextstr->nextstr&&isopr(c=(*(*strs)->nextstr->nextstr->str))&&
   !ismrk(c)) mixed(&(*strs)->nextstr,sigdgt);
switch(isopr(*(*strs)->nextstr->str)){
   default:  break;
   case '!':
      if((*strs)->nextstr->nextstr&&isnum((*strs)->nextstr->nextstr)){
         if((*strs)->nextstr->nextstr->form!=1){
            temp = dstofs((*strs)->nextstr->nextstr->str);
            free(clrstr((*strs)->nextstr->nextstr->str));
            (*strs)->nextstr->nextstr->str = temp;
            (*strs)->nextstr->nextstr->form = 1;
            }
         delstrs((*strs)->nextstr);
         return 1;
         }
      if(!(*strs)->nextstr->nextstr&&isnum(*strs)){
         if((*strs)->form==1){
            if((*strs)->shift) unshift(*strs);
            (*strs)->form = 0, temp = (*strs)->str,
            (*strs)->str = fcttod((*strs)->str,sigdgt?sigdgt+1:sigdgt),
            free(clrstr(temp));
            if(sigdgt) format(*strs,sigdgt);
            }
         else temp=(*strs)->str,(*strs)->str=fctrial((*strs)->str),
              free(clrstr(temp));
         delstrs((*strs)->nextstr);
         return 1;
         }
      break;
   case '+':
      if((*strs)->nextstr->nextstr){
         delstrs((*strs)->nextstr);
         return 1;
         }
   case '-': if((*strs)->nextstr->nextstr&&(*strs)->nextstr->nextstr->nextstr){
          switch(isopr(*(*strs)->nextstr->nextstr->nextstr->str)){
             default:  break;
             case '+':
                if((*strs)->nextstr->nextstr->nextstr->nextstr&&
                   isarg((*strs)->nextstr->nextstr->nextstr->nextstr)){
                   swapstrs(&(*strs)->nextstr->nextstr->nextstr->nextstr,
                            &(*strs)->nextstr->nextstr);
                   swapstrs(&(*strs)->nextstr,&(*strs)->nextstr->nextstr);
                   return 1;
                   }
                 break;
		 case '-':
		    /* Precedence of cascading unaries is highly ambiguous
		       is: (-1---2) equal to -(1-2) or -1-2 ??
		    Here: we reduce the mixed '--' to '+', but the binary '+'
                          takes precedence over any intended unary in '---'
		    */
		    unary(&(*strs)->nextstr->nextstr->nextstr,sigdgt);
		    if(isopr(*(*strs)->nextstr->nextstr->nextstr->str) == '-'){
		    	*(*strs)->nextstr->nextstr->nextstr->str = '+';
		    	setopr((*strs)->nextstr->nextstr->nextstr->str);
		        if((*strs)->nextstr->nextstr->nextstr->nextstr&&
			   (*strs)->nextstr->nextstr->nextstr->nextstr->str&&
			   isopr(*(*strs)->nextstr->nextstr->nextstr->nextstr->str))
			   delstrs((*strs)->nextstr->nextstr->nextstr->nextstr);
			}
		    return 0;
		 }
             }
   case ':': break;
   case BEG: case ';':case ',': return 1;
   case '*':;case '/':;case '%':;case '=':;case '^':
      if((*strs)->nextstr->nextstr){
	 delstrs((*strs)->nextstr);
         err_prnt("illegal unary operation deleted");
         }
      break;
   }
return 0;
}


void
unshift(strs)
STRS *strs;
{
int *shift();
if(strs->form==1){
   int tmpi;
   int *fctrl, *fctrial(), *clrstr(), *inttos(), *tmpn,
        *fctmuli(), *fctdivi(), *fctmult(), *fctdivd(),
        *tmps = inttos(iabs(strs->shift));
   tmpn = fctrial(tmps);
   if(greater(mxistr,tmpn)){
     tmpi = stor10i(tmpn);
      strs->str = strs->shift>0 ? fctmuli(strs->str,tmpi) :
                                  fctdivi(strs->str,tmpi);
      free(clrstr(tmpn));
      }
   else strs->str = strs->shift>0 ? fctmult(strs->str,tmpn) :
                                    fctdivd(strs->str,tmpn);
   free(clrstr(tmps));
   }
else strs->str = shift(strs->str,strs->shift);
strs->shift = 0;
}


int binary(strs)
STRS **strs;
{
if(trace_level==7){
	printf("binary ");
	strs_prnt(*strs);
        }
if((*strs)->prevstr->str&&isarg((*strs)->prevstr)&&
  ((*strs)->nextstr->str&&isarg((*strs)->nextstr)||
  isasgn(*(*strs)->str))) return 1;
return 0;
}


int mixed(strs,sigdgt)
STRS **strs;
int sigdgt;
{
STRS *tmps, *insprevstrs();
int *lftpad(), *cstois();
int divon = 0;
static char *oprs = "+-*/+%";
if(trace_level==7){
	printf("mixed ");
	strs_prnt(*strs);
        }
if((*strs)->nextstr&&(*strs)->nextstr->str&&isarg((*strs)->nextstr))
   return 0;
if(isopr(*(*strs)->str)&&isopr(*(*strs)->nextstr->str)){
   switch(isopr(*(*strs)->str)){
      case '+':case '-':
               switch(isopr(*(*strs)->nextstr->str)){
                   case '+': delstrs((*strs)->nextstr);
                             remixed(strs,sigdgt);
                             return 1;
                   case '-': *(*strs)->str =
                                 oprs[!fnctno(oprs,isopr(*(*strs)->str))];
                             setopr((*strs)->str);
                             delstrs((*strs)->nextstr);
                             remixed(strs,sigdgt);
                             return 1;
                   case ':': swapstrs(&(*strs),&(*strs)->nextstr);
                             remixed(strs,sigdgt);
                             return 1;
                   case '!': unary(strs,sigdgt);
                   case BEG:;case END:;case ',':;case ';': break;
                   default:  delstrs((*strs)->nextstr);
                             err_prnt("illegal unary operation deleted");
                             break;
                   }
      case '|': switch(isopr(*(*strs)->nextstr->str)){
                   case '|':
                      delstrs((*strs)->nextstr);
                      remixed(strs,sigdgt);
                      return 1;
                   default: break;
                   }
      default: switch(isopr(*(*strs)->str)){  /* Turbo C V1.0 bug avoidance */
         case '!': switch(isopr(*(*strs)->nextstr->str)){
                   case '!': err_prnt("illegal unary operation deleted");
                   default:  (*strs)->nextstr->form = 1;
                   case '=': *strs = (*strs)->nextstr;
                             delstrs((*strs)->prevstr);
                             remixed(strs,sigdgt);
                             return 1;
                   }
         case '^': switch(isopr(*(*strs)->nextstr->str)){
                   case '-': (*strs)->shift = 1;
                   case '+': delstrs((*strs)->nextstr);
                             remixed(strs,sigdgt);
                             return 1;
                   default:  delstrs((*strs)->nextstr);
                             err_prnt("illegal unary operation deleted");
                             break;
                   }
         case ';':switch(isopr(*(*strs)->nextstr->str)){
                   case ',':delstrs((*strs)->nextstr);
                      err_prnt("illegal unary operation deleted");
                      break;
                   case ';':;case BEG:
                      (*strs)->str = lftpad((*strs)->str,14);
                      delstrs((*strs)->nextstr);
                      remixed(strs,sigdgt);
                      return 1;
                   case END: *strs = (*strs)->nextstr;
                      return 1;
                   case '-': case '+': return unary(strs,sigdgt);
		   default: break;
                   }
         case ',':switch(isopr(*(*strs)->nextstr->str)){
                   case ';':delstrs((*strs)->nextstr);
                      err_prnt("illegal unary operation deleted");
                      break;
                   case ',':;case BEG:
                      (*strs)->str = lftpad((*strs)->str,11);
                      delstrs((*strs)->nextstr);
                      remixed(strs,sigdgt);
                      return 1;
                   case '-': case '+': return unary(strs,sigdgt);
		   default: break;
                   }
         default: switch(isopr(*(*strs)->str)){  /* Turbo C V1.0 bug */
            case BEG: switch(isopr(*(*strs)->nextstr->str)){
                   case BEG:
                      delstrs((*strs)->nextstr);
                      remixed(strs,sigdgt);
                      return 1;
                   case ';':case ',':  delstrs((*strs)->nextstr);
                      return 1;
                   default: break;
                   }
            case END: switch(isopr(*(*strs)->nextstr->str)){
                   case END:
                      delstrs((*strs)->nextstr);
                      remixed(strs,sigdgt);
                      return 1;
                   case ',':;case ';': *strs = (*strs)->nextstr;
                      delstrs((*strs)->prevstr);
                      remixed(strs,sigdgt);
                      return 1;
                   case ':': *strs = (*strs)->nextstr;
                      return 0;
                   default: break;
                   }
            case ':': switch(isopr(*(*strs)->nextstr->str)){
                   case ':': *(*strs)->str = '-',setopr((*strs)->str);
                      delstrs((*strs)->nextstr);
                      return 0;
                   case '[': *strs = (*strs)->nextstr;
                      return 0;
                   case '+': delstrs((*strs)->nextstr);
                      remixed(strs,sigdgt);
                      return 1;
		   case '-': return unary(strs,sigdgt);
		   default: break;
                   }
            default: switch(isopr(*(*strs)->str)){  /* Turbo C V1.0 bug */
               case '/':;case '%': divon = 1;
               case '*': switch(isopr(*(*strs)->nextstr->str)){
                   case ':': swapstrs(&(*strs),&(*strs)->nextstr);
                             swapstrs(&(*strs),&(*strs)->prevstr);
                             if(divon) tmps = insprevstrs(*strs),
                                tmps->form = (*strs)->form,
                                tmps->str = cstois(NEG);
                             *strs = (*strs)->nextstr;
                             remixed(strs,sigdgt);
                             return 1;
                   default:  switch(isopr(*(*strs)->nextstr->str)){
                      case '+': delstrs((*strs)->nextstr);
                            remixed(strs,sigdgt);
                            return 1;
                      case '-': swapstrs(&(*strs),&(*strs)->nextstr);
                            swapstrs(&(*strs),&(*strs)->prevstr);
                            *strs = (*strs)->nextstr;
                            remixed(strs,sigdgt);
                            return 1;
		      case '!': return unary(strs,sigdgt);
		      case ':': unary(strs,sigdgt);
			    *strs = (*strs)->nextstr;
                            break;
                      case ',':;case ';':
                            erc_prnt(isopr(*(*strs)->str));
                            err_prnt(": hanging operator deleted");
                            *strs = (*strs)->nextstr;
                            delstrs((*strs)->prevstr);
                            break;
                      case BEG:;case END:
                            break;
                      default:  delstrs((*strs)->nextstr);
                            err_prnt("illegal unary operation deleted");
                            break;
                      }
                   }
               case '=': break;
               default:  switch(isopr(*(*strs)->nextstr->str)){
                  case '+': delstrs((*strs)->nextstr);
                            remixed(strs,sigdgt);
                            return 1;
                  case '-': swapstrs(&(*strs),&(*strs)->nextstr);
                            swapstrs(&(*strs),&(*strs)->prevstr);
                            *strs = (*strs)->nextstr;
                            remixed(strs,sigdgt);
                            return 1;
		  case '!': return unary(strs,sigdgt);
		  case ':': unary(strs,sigdgt);
			    *strs = (*strs)->nextstr;
                            break;
                  case ',':;case ';':
                            erc_prnt(isopr(*(*strs)->str));
                            err_prnt(": hanging operator deleted");
                            *strs = (*strs)->nextstr;
                            delstrs((*strs)->prevstr);
                            break;
                  case BEG:;case END:
                  default:  delstrs((*strs)->nextstr);
                            err_prnt("illegal unary operation deleted");
                            break;
                  }
               }
            }
         }
      }
   }
return 0;
}

void
remixed(strs,sigdgt)
STRS **strs;
int sigdgt;
{if((*strs)->nextstr&&isopr(*(*strs)->nextstr->str)) mixed(strs,sigdgt);}


int *trans(strs,sigdgt)
STRS *strs;
int sigdgt;
{
STRS *tmpstrs, *insprevstrs();
int *tmps, *remd(), *modl(), *cpystr(), *cstois(),
   *str = 0, *shift(), *arith();
if(!strs->prevstr->str||!strs->nextstr) return str;
if(strs->form==1&&isopr(*strs->str)!='%'&&isopr(*strs->str)!='^'&&
   isopr(*strs->str)!='='&&(strs->nextstr->form!=1||strs->prevstr->form!=1)&&
   !setform(strs,sigdgt)){
      err_prnt("incorrect radix for factorial arithmetic");
      return str;
      }
switch(isopr(*strs->str)){
   int tmpform;
   case '^': if(cpyshft(strs)) str = cpystr(strs->prevstr->str); break;
   case '-': tmpform = strs->form;
      if(strs->prevstr->shift!=strs->nextstr->shift) equalize(strs);
      if(greater(strs->nextstr->str,strs->prevstr->str)){
         swapstrs(&strs->nextstr,&strs->prevstr);
         tmpstrs = insprevstrs(strs->prevstr);
         tmpstrs->str = cstois(NEG);
         tmpstrs->form = tmpform;
         }
      str = arith(strs,sigdgt?sigdgt+1:sigdgt);
      break;
   case '+': if(strs->prevstr->shift!=strs->nextstr->shift)
      equalize(strs);
      str = arith(strs,sigdgt?sigdgt+1:sigdgt); break;
   case '*':;case '/':
      if(strs->nextstr->shift&&!shiftable(strs,sigdgt)) break;
      str = arith(strs,sigdgt?sigdgt+1:sigdgt);
      break;
   case '%':
      if(radix!=10&&!dtordx(strs,sigdgt)) return str;
      if(strs->prevstr->form==1||strs->nextstr->form==1)
         err_prnt("fixed radix operation on factorial register");
       else if(strs->form==1){
         strs->form = 0;
         if(strs->nextstr->shift&&!shiftable(strs,sigdgt)) break;
         str = modl(cpystr(strs->prevstr->str),
               cpystr(strs->nextstr->str));
                 break;
         }
       else{
          if(strs->nextstr->shift&&!shiftable(strs,sigdgt)) break;
          str=remd(cpystr(strs->prevstr->str),
               cpystr(strs->nextstr->str),/*sigdgt?sigdgt+1:*/sigdgt);
          break;
          }
   case '=':
      if(isnam(strs->prevstr)){
         str = cpystr(strs->nextstr->str);
         strs->prevstr->form = strs->nextstr->form;
         cpynams(strs->prevstr->str,strs->nextstr);
         break;
         }
      else err_prnt("attempt to vary a constant");
   default:;
   }
return str;
}

int dtordx(strs,sigdgt)
STRS *strs;
int sigdgt;
{
STRS *tmps[2];
int *ptadrs(), *lftpad(), shift, *dstors(), *inttos(), *r10 = inttos(10),
   *cpystr(), *divd(), *mult(), *clrstr(), ix = -1;
tmps[0] = strs->prevstr, tmps[1] = strs->nextstr;
if(strs->form==1){
   err_prnt("incorrect radix for factorial arithmetic");
   return 0;
   }
while(++ix<2){
   shift = ((shift=istrln(ptadrs(tmps[ix]->str))-1)<0) ? 0 : shift;
   if(tmps[ix]->form==10||!tmps[ix]->form){
      tmps[ix]->str = dstors(lftshift(tmps[ix]->str,shift)),
      tmps[ix]->shift -= shift, tmps[ix]->form = radix,
      shift = tmps[ix]->shift, tmps[ix]->shift = 0;
      if(shift<0) while(shift++)
         tmps[ix]->str = divd(tmps[ix]->str,cpystr(r10),sigdgt);
      else if(shift>0)
         while(shift--) tmps[ix]->str = mult(tmps[ix]->str,cpystr(r10));
      }
   else if(tmps[ix]->form!=radix){
      if(tmps[ix]->form==1)
         err_prnt("cannot cope with two register formats within expression");
      else err_prnt("cannot cope with more than one radix within expression");
      free(clrstr(r10));
      return 0;
      }
   }
free(clrstr(r10));
return 1;
}


int cpyshft(strs)
STRS *strs;
{
int tmpform, maxi = maxint;
int *tmps, *clrstr(), *fintod();
tmpform =  strs->nextstr->form==1 ?
   tmps = fintod(strs->nextstr->str), tmpform = stor10i(tmps),
   free(clrstr(tmps)), tmpform : stor10i(strs->nextstr->str);
tmpform = strs->shift ? strs->shift = 0, -tmpform : tmpform;
if(tmpform<0) maxi = -maxi;
if(iabs(maxi-tmpform)<iabs(strs->prevstr->shift)){
   err_prnt("shift value exceeds maximum apa(x) precision");return 0;}
if(strs->shift) tmpform = -tmpform, strs->shift = 0;
strs->prevstr->shift += tmpform;
return 1;
}


int setform(strs,sigdgt)
STRS *strs;
int sigdgt;
{
int *tmps, *dectof(), *clrstr();
if(radix==10){
   if(strs->prevstr->form==0||strs->prevstr->form==10) unshift(strs->prevstr),
      tmps = strs->prevstr->str, strs->prevstr->str=dectof(tmps),
      format(strs->prevstr,sigdgt),free(clrstr(tmps)),strs->prevstr->form = 1;
   if(strs->nextstr->form==0||strs->nextstr->form==10) unshift(strs->nextstr),
      tmps = strs->nextstr->str, strs->nextstr->str=dectof(tmps),
      format(strs->nextstr,sigdgt),free(clrstr(tmps)),strs->nextstr->form = 1;
   return 1;
   }
return 0;
}


int shiftable(strs,sigdgt)
STRS *strs;
int sigdgt;
{
int *tmps, *fctmult(), *shift(), *cstois(), maxi = maxint;
int tmpi, min = 0, tmpshift = strs->nextstr->shift;
if(!(tmpi=strs->prevstr->shift))
   strs->prevstr->shift = tmpshift, strs->nextstr->shift = 0;
else if(tmpshift<0&&strs->prevstr->shift<0){
   maxi = -maxi;
   if(maxi-tmpshift<strs->prevstr->shift){ maxi = -maxi; return 0;}
   }
else if(tmpshift>0&&strs->prevstr->shift>0){
   if(maxi-tmpshift>strs->prevstr->shift) return 0;
   }
else if(strs->nextstr->form!=1){
   strs->prevstr->shift += tmpshift, strs->nextstr->shift = 0;
   if(sigdgt==2) return 0;}
else{
   if(iabs(tmpi)>iabs(tmpshift)){ min = tmpshift, strs->prevstr->shift = tmpi;
      if(sigdgt==3) return 0;}
   else{ min = tmpi, strs->prevstr->shift = tmpshift;
      if(sigdgt==4) return 0;}
   tmps = shift(cstois(ONE),min);
   if(sigdgt==5) return 0;
   strs->nextstr->str = fctmult(strs->nextstr->str,tmps);
   if(sigdgt==6) return 0;
   strs->nextstr->shift = 0;
   }
return 1;
}


void
equalize(strs)
STRS *strs;
{
int neg, pos, tmpshift;
int *cstois(), *shift(), *trim(), *fctmult();
if((neg=strs->prevstr->shift)<0&&(pos=strs->nextstr->shift)>0&&maxint+neg>pos)
   strs->nextstr->str = trim(strs->nextstr->form==1 ?
         fctmult(strs->nextstr->str,shift(cstois(ONE),pos-neg)) :
         shift(strs->nextstr->str,pos-neg));
else if((neg=strs->nextstr->shift)<0&&(pos=strs->prevstr->shift)>0&&maxint+
      neg>pos) strs->nextstr->str =  trim(strs->nextstr->form==1 ?
            fctmult(strs->nextstr->str,shift(cstois(ONE),neg-pos)) :
            shift(strs->nextstr->str,neg-pos));
else if(tmpshift=strs->nextstr->shift-strs->prevstr->shift)
      strs->nextstr->str =  trim(strs->nextstr->form==1 ?
      fctmult(strs->nextstr->str,shift(cstois(ONE),tmpshift)) :
                                 shift(strs->nextstr->str,tmpshift));
strs->nextstr->shift = strs->prevstr->shift;
}

int *arith(strs,sigdgt)
STRS *strs;
int sigdgt;
{
int *add(), *minus(), *mult(), *divd(), *remd(), *fctadd(), *clrstr(),
   *fctminus(), *fctmult(), *fctdivd(), *cpystr(), *dstors(),
   *str = 0, *dstofs(), *dectof(), *(*dopr[4])(), *(*fopr[4])(), *fintod();
static char *oprs = "+-*/";
int oprn = fnctno(oprs,isopr(*strs->str));
dopr[0] = add, dopr[1] = minus, dopr[2] = mult, dopr[3] = divd;
fopr[0] = fctadd, fopr[1] = fctminus, fopr[2] = fctmult, fopr[3] = fctdivd;
if(radix!=10&&!dtordx(strs,sigdgt)) return str;
if(strs->form==1) str = fopr[oprn]
   (cpystr(strs->prevstr->str),cpystr(strs->nextstr->str));
else if(strs->prevstr->form==1||strs->nextstr->form==1)
   err_prnt("fixed radix operation with factorial register");
else{
   str = (oprn!=3) ? dopr[oprn]
      (cpystr(strs->prevstr->str),cpystr(strs->nextstr->str)) :
                     dopr[oprn]
      (cpystr(strs->prevstr->str),cpystr(strs->nextstr->str),sigdgt);
   }
return str;
}


int fnctno(oprs,oprn)
char *oprs, oprn;
{
int count = 0;
while(*oprs&&*oprs++!=oprn) count++;
return count;
}

void
cpynams(name,strs)
int *name;
STRS *strs;
{
NAMS *tmpn, *setnams();
STRS *tmps, *cpystrs(), *clrstrs();
tmpn = setnams(name);
if(tmpn->strs){
   while(tmpn->strs->nextstr) delstrs(tmpn->strs->nextstr);
   free(clrstrs(tmpn->strs));
   }
tmps = tmpn->strs = cpystrs(strs);
reducible(name,tmps);
while(strs->nextstr){
   tmps->nextstr = cpystrs(strs->nextstr);
   tmps->nextstr->prevstr = tmps;
   tmps = tmps->nextstr;
   reducible(name,tmps);
   strs = strs->nextstr;
   }
}

void
reducible(lval,rval)
int *lval;
STRS *rval;
{
int *clrstr(), *cstois();
if(rval->str&&!istrcmp(lval,rval->str)){
   ers_prnt(rval->str);
   fprintf(stderr,":  irreducible identifier made 0\n");
   free(clrstr(rval->str));
   rval->str = cstois(ZERO);
   }
}
