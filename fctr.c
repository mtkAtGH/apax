static char SccsId[] = "@(#) fctr.c 1.2 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <defn.h>
#include <fctr.h>
#include <fixr.h>
#include <apax.h>
#include <util.h>
#include <help.h>


extern int radix, *mxistr, sglprc, dblprc, maxint;
extern float global_float;

int *fctadd(str1,str2)
int *str1, *str2;
{
int *strint();
int space, sum = 0, cary = 0;
int tailen, hedlen, pos;
register int *reg1, *reg2, *reg3;
int  *ans, *int1ptr(), *lftpad(), *clrstr();
tailen = frclen(str1)-frclen(str2);
hedlen = intlen(str1)-intlen(str2);
pos = max(frclen(str1),frclen(str2)) - 1;
space = sumlen(str1,str2);
ans = int1ptr(space+1);
reg1 = strint(str1,0);
reg2 = strint(str2,0);
reg3 = &ans[space];
while(tailen>0&&tailen>istrln(reg1))  *--reg3 = *--reg1,--pos;
while(tailen<0&&-tailen>istrln(reg2)) *--reg3 = *--reg2,--pos;
while(reg1-str1&&reg2-str2){
   if(reg1[-1]=='.'||reg2[-1]=='.'){
      *--reg3 = 0+'.';
      --reg1, --reg2, --pos;
      }
   else{
      cary = (sum = (*--reg1)-'0'+(*--reg2)-'0'+cary)>iabs(pos);
      *--reg3 = sum%(iabs(pos--) + 1) +  '0';
      }
   }
while(hedlen>0&&str1!=reg1){
   cary = (sum = (*--reg1)-'0'+cary)>iabs(pos);
   *--reg3 = sum%(iabs(pos--) + 1) +  '0';
   }
while(hedlen<0&&str2!=reg2){
   cary = (sum = (*--reg2)-'0'+cary)>iabs(pos);
   *--reg3 = sum%(iabs(pos--) + 1) +  '0';
   }
if(cary) ans = lftpad(ans,cary+'0');
free(clrstr(str1));
free(clrstr(str2));
return ans;
}


int *fctminus(x,y)
int *x, *y;
{
int *fctsubt();
return fctsubt(y,x);
}


int *fctsubt(first,second) /*  positive difference only!!  */
int *first, *second;
{
int *strint();
int pos;
int *result, *lowone, *run, *trim();
int *lftcut(), *cstois(), *lftpad(), *rgtpad(), *rgtshift();
lowone = cstois(ONE);
second = trim(second);
while(intlen(first)<intlen(second)) first = lftpad(first,'0');
while(frclen(first)<frclen(second)){
   if(frclen(first)<1){
      first = rgtpad(first,0+'.');
      continue;
      }
   first = rgtpad(first,'0');
   }
pos = intlen(first);
run = first;
while(*run){
   if(!(*run=='.')) *run = iabs(pos) - *run + 2*'0';
   run++,pos--;
   }
run = result = fctadd(first,second);
run = strint(result,0) - 1;
if(*run=='.') lowone = rgtpad(lowone,0+'.');
while(run-result>0){
   if( *--run=='.') lowone = lftpad(lowone,0+'.');
   else if(istrln(run)<istrln(result))
      lowone = lftpad(lowone,'0');
   }
return trim(lftcut(fctadd(lowone,result)));
}


int *fctmult(str1,str2)
int *str1, *str2;
{
int *ptadrs(), *cstois(), *lftshift(), *fctimul(), *fctidiv();
int order, temp;
if(zero(ptadrs(str1))||zero(ptadrs(str2))) return fctimul(str1,str2);
if((order=frclen(str1))>(temp=frclen(str2))) order=temp, swapstr(&str1,&str2);
if(--order>0){
   str1 = fctimul(str1,lftshift(cstois(ONE),order));
   str2 = fctimul(str1,str2);
   return fctidiv(str2,lftshift(cstois(ONE),order));
   }
return fctimul(str1,str2);
}


int *fctdivd(str1,str2)
int *str1, *str2;
{
int *fctimul(), *fctidiv(), *cstois(), *lftshift(), *ptadrs();
int order = 0;
if(!zero(ptadrs(str2))&&(order=frclen(str2)-1)){
   str1 = fctimul(str1,lftshift(cstois(ONE),order));
   str2 = fctimul(str2,lftshift(cstois(ONE),order));
   }
return fctidiv(str1,str2);
}


int *fctimul(str1,str2)
int *str1, *str2;
{
int *strint();
int  pos, abspos, val, cari, intg;
int *intgr, *fintod(), *fctrl, *fctrun, *ans, *cary, *muli(), *add(),
     *tmps = 0, *trim(), *ptadrs(), *int1ptr(), *cpystr(), *clrstr(),
     *lftpad(), *inttos(), *integer(), *cstois(), *idivmod(),
      *divi(), *fctmuli();
if(zero(ptadrs(str2))){
   if(zero(ptadrs(str1))&&greater(str1,str2)){
      intgr = fintod(tmps=str1), pos = frclen(str2) - 1,
      fctrl=str2, fctrun = strint(str2,0);
      }
   else intgr = fintod(tmps=str2),
   pos = frclen(str1) - 1,
   fctrl=str1, fctrun=strint(str1,0);
   }
else if(!tmps&&zero(ptadrs(str1))) intgr = fintod(str1),
   pos = frclen(tmps=str2) - 1,
   fctrl=str2, fctrun = strint(str2,0);
else err_prnt("fctimul: no integral factor"), exit(1);
if(greater(mxistr,intgr)){
   intg = strtoi(intgr);
   free(clrstr(intgr));
   return fctmuli(fctrl,intg);
   }
ans = int1ptr(1), cary = cstois(ZERO);
while((fctrun--)-fctrl){
   if(!pos){ ans = lftpad(ans,*fctrun), pos--;continue;}
   abspos = iabs(pos--) + 1;
   tmps = muli(cpystr(intgr),*fctrun-'0');
   tmps = add(cpystr(tmps),cary);
   cary = idivmod(&val,tmps,abspos);
   ans = lftpad(ans,val+'0');
   }
while(greater(cary,mxistr)){
   if(!pos){ ans = lftpad(ans,0+'.'), --pos;continue;}
   abspos = iabs(pos--) + 1;
   cary = idivmod(&val,cary,abspos);
   ans = lftpad(ans,val+'0');
   }
cari = strtoi(cary);
while(cari){
   if(!pos){ ans = lftpad(ans,0+'.'), --pos;continue;}
   abspos = iabs(pos--) + 1;
   val = cari%abspos, cari /= abspos;
   ans = lftpad(ans,val+'0');
   }
free(clrstr(str1)),free(clrstr(str2)),free(clrstr(cary));
return trim(ans);
}


int *fctidiv(str1,str2)
int *str1, *str2;
{
int  pos, val, intg, abspos, posval, prod, cari;
int *tmps, *intgr, *fintod(), *fctrl, *fctrun, *cary, *clrstr(), *add(),
     *ans, *value, *trim(), *ptadrs(), *int1ptr(), *cpystr(), *muli(),
     *rgtpad(), *divmod(), *inttos(), *cstois(),
     *fctdivi();
if(!zero(ptadrs(str2))) err_prnt("fctidiv: non-integer denominator"), exit(1);
intgr = fintod(str2),pos = intlen(str1), fctrl=str1;
if(greater(mxistr,intgr)){
   intg = strtoi(intgr);
   free(clrstr(intgr));
   free(clrstr(str2));
   return fctdivi(fctrl,intg);
   }
ans = int1ptr(1),cary = cstois(ZERO);
while(*fctrl){
   if(!pos){ans = rgtpad(ans,0+'.'), pos--, fctrl++;continue;}
   abspos = iabs(pos--) + 1;
   posval = *fctrl++-'0';
   tmps = muli(cary,abspos);
   tmps = add(tmps,inttos(posval));
   value = divmod(&cary,tmps,cpystr(intgr));
   ans = rgtpad(ans,strtoi(value) + '0');
   free(clrstr(value));
   }
while(!zero(cary)){
   if(!pos){ans = rgtpad(ans,0+'.'), pos--;continue;}
   abspos = iabs(pos--) + 1;
   tmps = muli(cary,abspos);
   value = divmod(&cary,tmps,cpystr(intgr));
   ans = rgtpad(ans,strtoi(value) + '0'),
   free(clrstr(value));
   }
free(clrstr(str1)),free(clrstr(str2));
return trim(ans);
}


int *fctmuli(str,num)
int *str;
int num;
{
int *strint();
int val, cary = 0, pos = frclen(str) - 1, abspos;
int *int1ptr(), *ans, *run = strint(str,0),
     *tmpv, *lftpad(), *trim(), *clrstr(), *divi(),
     *muli(), *add(), *cpystr(), *inttos(), *idivmod(), *cstois();
if(!num){
   ans = chrpos(str,'.')>-1 ? cstois(FZERO) : cstois(ZERO);
   free(clrstr(str));
   return ans;
   }
ans = int1ptr(1);
if(num<0) num = iabs(num), err_prnt("fctmuli: sign changed");
while((run--)-str){
   if(!pos){ ans = lftpad(ans,0+'.'),--pos;continue;}
   abspos = iabs(pos--) + 1;
   if(num&&*run-'0'<(maxint-'0')/num&&(*run-'0')*num<maxint-'0'-cary)
      val = num*(*run-'0')+cary, cary = val/abspos, val %= abspos;
   else tmpv = muli(inttos(num),*run-'0'),tmpv = add(tmpv,inttos(cary)),
      tmpv = idivmod(&val,tmpv,abspos),
      cary = strtoi(tmpv),
      free(clrstr(tmpv));
   ans = lftpad(ans,val+'0');
   }
while(cary){
   if(!pos){ ans = lftpad(ans,0+'.'),--pos;continue;}
   abspos = iabs(pos--) + 1;
   val = cary%abspos, cary /= abspos;
   ans = lftpad(ans,val+'0');
   }
free(clrstr(str));
return trim(ans);
}

int *fctdivi(str,num)
int *str;
int num;
{
int val, cary = 0, pos = intlen(str), abspos;
int *run, *int1ptr(), *ans = int1ptr(1), *rgtpad(), *trim(), *clrstr(),
     *tmpv, *muli(), *add(), *inttos(), *cpystr(), *idivmod();
if(!num) err_prnt("fctdivi: division by zero\n"),exit(1);
if(num<0) num = iabs(num), err_prnt("fctdivi: sign changed");
run = str;
while(*run){
   if(!pos){ ans = rgtpad(ans,0+'.'),pos--, run++;continue;}
   abspos = iabs(pos--) + 1;
   if(cary&&abspos<(maxint-'0')/cary&&abspos*cary<maxint-*run)
      val = cary*abspos + *run++ - '0', cary = val%num, val/=num;
   else tmpv = muli(inttos(cary),abspos),
      tmpv = add(tmpv,inttos(*run++-'0')),
      tmpv = idivmod(&cary,tmpv,num),
      val = strtoi(tmpv),
      free(clrstr(tmpv));
   ans = rgtpad(ans,val+'0');
   }
while(cary){
   if(!pos){ ans = rgtpad(ans,0+'.'),pos--;continue;}
   abspos = iabs(pos--) + 1;
   if(cary&&(maxint-'0')/cary>abspos)
      val = cary * abspos, cary = val % num, val /= num;
   else tmpv = muli(inttos(cary),abspos),
      tmpv = idivmod(&cary,tmpv,num),
      val = strtoi(tmpv),
      free(clrstr(tmpv));
   ans = rgtpad(ans,val+'0');
   }
free(clrstr(str));
return trim(ans);
}

int *intofs(value)       /* sign ignored */
int value;
{
int *str, *lftpad(), *int1ptr();
int count = 2;
str = int1ptr(1);
value = iabs(value);
while(value){
   str = lftpad(str,value%count+'0');
   value /= count++;
   }
if(!*str) str = lftpad(str,'0');
return str;
}


int *dstofs(dstr)
int *dstr;
{
int *strint();
int *fstr, *str, *end, *run, *drun, *ptadrs(), *clrstr(),
     *int1ptr(), *rgtpad(), *lftpad();
int pos = 1, order = 1, width = 1, count = 0;
fstr = int1ptr(1);
str = int1ptr(1);
if(*(drun=ptadrs(dstr))){
   fstr = lftpad(fstr,0+'.');
   do{
      if(!count){
         str = lftpad(str,'0');
         count = 9*order*width++;
         order *= 10;
         }
      run = str;
      while(*run&&*++drun){
         *run++ = *drun;
         count--;
         }
      while(*run) *run++ = 0;
      if(strtoi(str)>pos++)
         ers_prnt(dstr),
         fprintf(stderr,":  illegal factorial register\n");
      if(*str) fstr = rgtpad(fstr,strtoi(str)+'0');
      }while(*drun);
   free(clrstr(str));
   }
pos = 1, order = 1, width =1, count = 0;
str = int1ptr(1);
drun = ptadrs(dstr);
do{
   if(!count){
      str = lftpad(str,'0');
      end = strint(str,0);
      count = 9*order*width++;
      order *= 10;
      }
   run = end;
   while(run-str&&drun-dstr){
      *--run = *--drun;
      count--;
      }
   while(run-str) *--run = '0';
   if(strtoi(str)>pos++)
      ers_prnt(dstr),
      fprintf(stderr,":  illegal factorial register\n");
   if(*str) fstr = lftpad(fstr,strtoi(str)+'0');
   }while(drun-dstr);
free(clrstr(str));
return fstr;
}


int *dectof(decml)
int *decml;
{
int *fctrl, *point, *frctn, *intgr, *str, *ptadrs(), *muli(), *idivmod(),
     *lftpad(), *rgtpad(), *int1ptr(), *cpystr(), *integer(), *fraction(),
     *clrstr();
int pos = 1, mod;
fctrl = int1ptr(1);
if(frctn = fraction(decml)){
   fctrl = lftpad(fctrl,0+'.');
   while(frctn&&!zero(frctn)){
      frctn = muli(frctn,++pos);
      fctrl = rgtpad(fctrl,strtoi(str=integer(frctn))+'0');
      if(str) free(clrstr(str));
      frctn = fraction(str=frctn);
      if(str) free(clrstr(str));
      }
   }
pos = 1, intgr = integer(decml);
while(intgr&&!zero(intgr)){
   intgr = idivmod(&mod,intgr,++pos);
   fctrl = lftpad(fctrl,mod+'0');
   }
if(frctn) free(clrstr(frctn));
if(intgr) free(clrstr(intgr));
return fctrl;
}


int *fintod(fctin)
int *fctin;
{
int *decin, *add(), *muli(), *inttos(), *cstois();
int pos = intlen(fctin);
decin = cstois(ZERO);
while(pos) decin = muli(add(decin,inttos(*fctin++-'0')),pos--);
return decin;
}

int *fcttod(factrl,sigdgt)
int *factrl;
int sigdgt;
{
int *fctrl, *fintod(), *cpystr(), *lftshift(), *fctimul(), *divd(), *ptadrs();
int order = frclen(factrl) - 1, *cstois();
if(order>0&&!zero(ptadrs(factrl))){
   int *fmul, *dec1, *dec2;
   fctrl = cpystr(factrl);
   fmul = fctimul(fctrl,lftshift(cstois(ONE),order));
   dec1 = fintod(fmul);
   dec2 = fintod(lftshift(cstois(ONE),order));
   dec1 = divd(dec1,dec2,sigdgt);
   return dec1;
   }
return fintod(factrl);
}


