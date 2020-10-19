static char SccsId[] = "@(#) fnct.c 1.4 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */


#include <defn.h>
#include <fnct.h>
#include <apax.h>
#include <fixr.h>
#include <fctr.h>
#include <util.h>


static char *rules[4] = {"LR","MA","DMA"};
static char *fnct_nams[FNCS] = {"exp","log","pow"};

int *add(), *minus(), *fctadd(), *fctminus(), *(*pm[2][2])(),
     *mult(), *divd(), *fctmult(), *fctdivd(), *(*md[2][2])(),
     *muli(), *divi(), *fctmuli(), *fctdivi(), *(*mdi[2][2])();


int
replfnct(strs,sigdgt)
STRS **strs;
int sigdgt;
{
int fnct_index;
int xtrdgt = 0;
int *cstois();
int *constone = cstois(ONE);
STRS *size = NULL, *sign = NULL;
int *str = NULL, *(*fnct[3])(), *clrstr(), *ptadrs(), *xexp(),
     *xlog(), *xpow(), *correct();
fnct[0]=xexp,fnct[1]=xlog,fnct[2]=xpow;
pm[0][0] = add, pm[0][1] = minus, pm[1][0] = fctadd, pm[1][1] = fctminus,
   md[0][0] = mult, md[0][1] = divd, md[1][0] = fctmult, md[1][1] = fctdivd,
   mdi[0][0] = muli, mdi[0][1] = divi, mdi[1][0] = fctmuli, mdi[1][1]=fctdivi;
if(!found(strs,&size,&sign)) return 0;
switch(xfnctno((*strs)->str)){
   case -1: return 0;
   case 1: if(sign){err_prnt("log: negative domain error");return 0;}break;
   case 2: if(sign){err_prnt("pow: negative base error");return 0;}
      if(issep(*size->nextstr->str)){
         extern int maxint;
         STRS *tmps = size->nextstr, *tmpsz = NULL, *tmpsn = NULL;
         int *inttos(), *mxistr = inttos(maxint-48), *(*arg[2])(),
              *neutral(), *fintod();
         arg[0] = neutral, arg[1] = fintod;
         if(!found(&size->nextstr,&tmpsz,&tmpsn)) return 0;
         if(!zero(ptadrs(tmpsz->str))||
             greater((str=arg[tmpsz->form](tmpsz->str)),mxistr)){
                fnct[2] = xlog, xtrdgt = sigdgt/2;
             if(str&&tmpsz->form) free(clrstr(str));
             }
         else sign = tmpsn;
         free(clrstr(mxistr)),delstrs(tmps);
         }
      else{ err_prnt("pow: two arguments needed");return 0;}
   default:;
   }
fnct_index = xfnctno((*strs)->str);
if(fnct_index==-1) return 0;
str = fnct[fnct_index](sign!=NULL,size,sigdgt+xtrdgt);
switch(xfnctno((*strs)->str)){
   case -1: return 0;
   case 2:  if(fnct[2]==xlog){
               int *fpow();
               if(greater(constone,size->str)) sign = size->prevstr;
               free(clrstr(size->str)), size->str = str;
               if(!(str = fpow(&sign,&size,sigdgt))) return 0;
               str = correct(str,sigdgt);
               }
            if(sign) delstrs(sign);
            break;
   case 0:  if(sign) delstrs(sign);break;
   default:;
   }
if(str) free(clrstr(size->str)), size->str = str;
return 1;
}


int *xpow(sign,size,sigdgt)
STRS *size;
int sign, sigdgt;
{
STRS *expnt;
int count = 1;
int *cstois();
CHRS *chrs, *sequence(), *clrchrs();
int *clrstr(), *cpystr(), *value = cstois(ONE), *powr,
     *(*arg[2])(), *fintod(), *neutral(), *correct();
arg[0] = neutral, arg[1] = fintod;
expnt = sign ? size->nextstr->nextstr : size->nextstr;
chrs = sequence(strtoi(powr=arg[expnt->form](expnt->str)));
if(expnt->form) free(clrstr(powr));
value = size->form||sign?
        md[size->form][sign](value,cpystr(size->str),sigdgt):
        md[size->form][sign](value,cpystr(size->str));
powr = (chrs->chr==1) ? chrs=chrs->prevchr, cpystr(value) : cstois(ONE);
while(chrs->chr){
   value =  correct(size->form ?
      md[size->form][0](cpystr(value),value,sigdgt):
      md[size->form][0](cpystr(value),value),sigdgt);
   if(chrs->chr==++count){
      powr = correct(size->form ?
         md[size->form][0](powr,cpystr(value),sigdgt) :
         md[size->form][0](powr,cpystr(value)),sigdgt);
      chrs = chrs->prevchr;
      }
   }
while(chrs->nextchr) delchrs(chrs->nextchr);free(clrchrs(chrs));
free(clrstr(value));
delstrs(expnt);
return correct(powr,sigdgt);
}


int *fpow(sign,size,sigdgt)
STRS **sign, **size;
int sigdgt;
{
int *str, *xexp(), *clrstr(), *cpystr();
STRS *tmpsz = NULL, *tmpsn = NULL;
if(!found(size,&tmpsz,&tmpsn)) return 0;
else if((*size)->form!=tmpsz->form) convert(!(*size)->form?*size:tmpsz),
             err_prnt("xpow: mixed formats turned into factorial");
str = md[(*size)->form][0](cpystr((*size)->str),cpystr(tmpsz->str));
if(*sign&&tmpsn) delstrs(*sign), delstrs(tmpsn), *sign = tmpsn = NULL;
else if(tmpsn) *sign = tmpsn;
delstrs(*size), free(clrstr(tmpsz->str)), tmpsz->str = str, *size = tmpsz;
return xexp(*sign!=NULL,*size,sigdgt);
}


void
convert(strs)
STRS *strs;
{
int *dectof(), *str = dectof(strs->str), *clrstr();
free(clrstr(strs->str)), strs->str = str, strs->form = 1;
}


int found(strs,size,sign)
STRS **strs, **size, **sign;
{
STRS *tmps = *strs;
while((*strs)->nextstr&&isnam((*strs)->nextstr))
   if(!replnam(&(*strs)->nextstr)) return 0;
while(tmps&&tmps->nextstr&&!isarg(tmps->nextstr)||
   tmps&&tmps->nextstr&&isnam(tmps->nextstr)){
   if(tmps->nextstr->str&&isopr(*tmps->nextstr->str)=='-'){
      *sign = tmps->nextstr;
      while(tmps->nextstr->nextstr&&isnam(tmps->nextstr->nextstr))
         if(tmps->nextstr->nextstr&&!replnam(&tmps->nextstr->nextstr)) return 0;
      *size = tmps->nextstr->nextstr;
      }
   if(tmps->nextstr&&tmps->nextstr->str) tmps = tmps->nextstr;
   else break;
   }
if(!*size) *size = tmps->nextstr;
return 1;
}


int *xlog(sign,size,sigdgt)
STRS *size;
int sign, sigdgt;
{
int count = -1;
int *cstois(), *int1ptr();
int *constone = cstois(ONE);
STRS *mkstrs(), *tmpstrs, *insprevstrs();
int *term, *tmpt, *tmps, *large, *small, *cpystr(),
     *answ = NULL, *clrstr(), *incorrect(), *correct();
if(zero(size->str)){err_prnt("log: zero domain error");return NULL;}
if(greater(size->str,constone)) large = cpystr(size->str), small = cstois(ONE);
else if(greater(constone,size->str)) large = cstois(ONE), small = cpystr(size->str),
     (tmpstrs = insprevstrs(size))->str = int1ptr(2), *tmpstrs->str = 2,
     tmpstrs->form = size->form;
else answ = cstois(ZERO);
if(answ) return answ; else answ = cstois(ZERO);
term = pm[size->form][1](cpystr(large),cpystr(small));
tmps = pm[size->form][0](large,small);
term = md[size->form][1](term,tmps,sigdgt);
tmpt = cpystr(term);
term = correct(md[size->form][0](cpystr(term),term),sigdgt);
while(count+=2){
   tmps = size->form ?
      mdi[size->form][1](cpystr(tmpt),count) :
      mdi[size->form][1](cpystr(tmpt),count,strlen(tmpt)+sigdgt);
   tmps = correct(tmps,sigdgt);
   answ = pm[size->form][0](cpystr(tmps),answ);
   if(insigdgts(tmps)>sigdgt) break;
   free(clrstr(tmps));
   tmpt = correct(size->form ? md[size->form][0](tmpt,cpystr(term),sigdgt):
                                md[size->form][0](tmpt,cpystr(term)),sigdgt);
   }
free(clrstr(tmps)), free(clrstr(tmpt)), free(clrstr(term));
return correct(mdi[size->form][0](answ,2),sigdgt);
}


int *xexp(sign,size,sigdgt)
STRS *size;
int sign, sigdgt;
{
int *cstois();
int count = 0, sumsgn = 0;
int *sumf, *sumi, *exp0, *intg, *frac, *answ, *clrstr(), *cpystr(), *rgtpad(),
     *integer(), *fraction(), *correct(), *incorrect(), *neutral();
intg = integer(size->str), frac = fraction(size->str),
       sumf = NULL, sumi = NULL, exp0 = cstois("10.");
if(!zero(frac)){
   int *tmps, *term = cstois(ONE); sumf = cstois(ONE);
   while(++count,1){
      tmps = size->form ? md[size->form][0](term,cpystr(frac),sigdgt) :
                           md[size->form][0](term,cpystr(frac));
      term = size->form ? mdi[size->form][1](tmps,count) :
                     mdi[size->form][1](tmps,count,strlen(tmps)+sigdgt);
      if(insigdgts(term)>sigdgt) break;
      sumf = pm[size->form][(sumsgn+=sign)%2](sumf,cpystr(term));
      term = correct(term,sigdgt);
      }
   if(tmps=incorrect(sumf,term)) count=tmps-sumf;
   free(clrstr(term));
   }
if(!zero(intg)){
   CHRS *chrs, *sequence(), *clrchrs();
   int *str, *tmps, *value = cstois(ONE), *prod, *rgtshift(),
        *(*arg[2])(), *fintod(), *(*typ[2])(), *dectof(), *fcttod();
   arg[0] = neutral, arg[1] = fintod, typ[0] = fcttod, typ[1] = cpystr;
   count = count ? count : sigdgt + 2;
   while(count--) exp0 = rgtpad(exp0,'1');count = 1;
   chrs = sequence(strtoi(arg[size->form](intg)));
   value = size->form?
           md[size->form][0](value,typ[size->form](exp0,sigdgt),sigdgt):
           md[size->form][0](value,typ[size->form](exp0,sigdgt));
   prod = (chrs->chr==1) ? chrs=chrs->prevchr, cpystr(value) : cstois(ONE);
   while(chrs->chr){
      value =  correct(size->form ?
         md[size->form][0](cpystr(value),value,sigdgt):
         md[size->form][0](cpystr(value),value),sigdgt);
      if(chrs->chr==++count){
         prod = correct(size->form ?
            md[size->form][0](prod,cpystr(value),sigdgt) :
            md[size->form][0](prod,cpystr(value)),sigdgt);
         chrs = chrs->prevchr;
         }
      }
   while(chrs->nextchr) delchrs(chrs->nextchr);free(clrchrs(chrs));
   sumi = prod;
   }
return !sumi&&!sumf? cstois(ONE):!sumf? correct(sumi,sigdgt) : !sumi ?
   correct(sumf,sigdgt) : size->form||sign ?
   correct(md[size->form][sign](sumf,sumi,sigdgt),sigdgt) :
   correct(md[size->form][sign](sumf,sumi),sigdgt);
}

insigdgts(term)
int *term;
{
int *ptadrs();
int count = 0;
term = ptadrs(term);
while(count++,!(*++term-48));
return count;
}


int *correct(term,sigdgt)
int *term;
int sigdgt;
{
int *ptadrs(), *cpystr(), *clrstr(), *tmpt, *tmps;
tmpt = ptadrs(term);
if(!*tmpt) return term;
while(*++tmpt&&!(*tmpt-48));
while(*tmpt&&sigdgt--) tmpt++;
if(!*tmpt) return term;
*tmpt = 0, tmps = cpystr(term), *tmpt = 48, free(clrstr(term));
return tmps;
}


int *incorrect(sumf,term)
int *sumf, *term;
{
int *ptadrs(), *tmps, *tmpt;
tmps = ptadrs(sumf), tmpt = ptadrs(term);
if(!*tmpt) return NULL;
while(tmps++,!(*++tmpt-48));
return tmps[-1] ? tmps : --tmps;
}


int *neutral(str) int *str; {return str;}

CHRS *sequence(target)
int target;
{
CHRS *mkchrs(), *setchrs(), *chrs = mkchrs(1);
int count, times, itern = 0;
while(target){
   count = 1, times = 1, itern++;
   while(count*2<=target) count*=2,times++;
   chrs = setchrs(chrs,times);
   target -= count;
   }
return chrs;
}

int xfnctno(name)
int *name;
{
int count = -1, *cstois();
while(fnct_nams[++count]&&istrcmp(name,cstois(fnct_nams[count])));
return count==FNCS-1 ? -1 : count;
}

