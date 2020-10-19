static char SccsId[] = "@(#) fixr.c 1.2 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <defn.h>
#include <fixr.h>
#include <util.h>
#include <apax.h>
#include <help.h>


extern int radix, *mxir10s, *mxistr, sglprc, dblprc, maxint, maxjmp;
extern float global_float;

int *add(str1,str2)
int *str1, *str2;
{
int space, sum = 0, cary = 0;
int tailen, hedlen, *strint(), *ans, *int1ptr(), *lftpad(), *clrstr();
register int *reg1, *reg2, *reg3;
tailen = frclen(str1)-frclen(str2);
hedlen = intlen(str1)-intlen(str2);
space = sumlen(str1,str2);
ans = int1ptr(space+1);
reg1 = strint(str1,0);
reg2 = strint(str2,0);
reg3 = &ans[space];
while(tailen>0&&tailen>istrln(reg1)) *--reg3 = *--reg1;
while(tailen<0&&-tailen>istrln(reg2)) *--reg3 = *--reg2;
while(reg1-str1&&reg2-str2){
   if(reg1[-1]==0+'.'||reg2[-1]==0+'.'){
      *--reg3 = 0+'.';
      --reg1, --reg2;
      }
   else{
      cary = (sum = (*--reg1)-'0'+(*--reg2)-'0'+cary)>radix-1;
      *--reg3 = sum%radix +  '0';
      }
   }
while(hedlen>0&&str1!=reg1){
   cary = (sum = (*--reg1)-'0'+cary)>radix-1;
   *--reg3 = sum%radix +  '0';
   }
while(hedlen<0&&str2!=reg2){
   cary = (sum = (*--reg2)-'0'+cary)>radix-1;
   *--reg3 = sum%radix +  '0';
   }
if(cary) ans = lftpad(ans,cary+'0');
free(clrstr(str1));
free(clrstr(str2));
return ans;
}

int *minus(x,y)
int *x, *y;
{
int *subt();
return subt(y,x);
}



int *subt(first,second) /*  positive difference only!!  */
int *first, *second;
{
int *result, *lowone, *run, *trim();
int *strint(), *lftcut(), *cstois(), *lftpad(), *rgtpad(), *rgtshift();
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
run = first;
while(*run){
   if(!(*run==0+'.')) *run = radix - *run + 2*'0' - 1;
   run++;
   }
run = result = add(first,second);
run = strint(result,0) - 1;
if(*run==0+'.') lowone = rgtpad(lowone,0+'.');
while(run-result>0){
   if( *--run==0+'.') lowone = lftpad(lowone,0+'.');
   else if(istrln(run)<istrln(result))
      lowone = lftpad(lowone,'0');
   }
return trim(lftcut(add(lowone,result)));
}



int *mult(one,other)
int *one, *other;
{
int *answer, *submul(), *rgtpad(), *ptadrs(), *inttos(), *cstois(),
     *str, *cpystr(), *trim(), *clrstr(), *intgr = (int*) 0, *muli();
int pos, intg, *tmpv = (int*) 0;
answer = cstois(FZERO);
if(zero(ptadrs(one))&&zero(ptadrs(other))&&
   greater(mxistr,one)&&greater(mxistr,other)){
   if(greater(one,other)) intgr = one, str = other;
   else intgr = other, str = one;
   }
else if(zero(ptadrs(one))&&greater(mxistr,one)) intgr = one, str = other;
else if(zero(ptadrs(other))&&greater(mxistr,other)) intgr = other, str = one;
if(intgr){
   intg = strtoi(intgr);
   if(intg&&intg<=maxjmp&&greater((tmpv=inttos((maxint-'0')/intg)),str)){
      free(clrstr(intgr)), free(clrstr(tmpv));
      return muli(str,intg);
      }
   if(tmpv) free(clrstr(tmpv));
   }
if(sumdgt(one)>sumdgt(other)) swapstr(&one,&other);
if(chrpos(one,0+'.')==-1) one = rgtpad(one,0+'.');
pos = chrpos(one,0);
while(--pos>=0){
   if(one[pos]==0+'.') continue;
   answer = add(submul(pos,cpystr(one),other),answer);
   }
free(clrstr(one)),free(clrstr(other));
return trim(answer);
}


int *submul(pos,one,other)
int pos;
int *one, *other;
{
int *dgtptr, *subans, *lftcut(), *strint(), *point = strint(one,0+'.');
int *cpystr(), *clrstr(), *cstois(), *lftshift(), *rgtshift(), *muli(), *add();
int *tmps, *diff, count, times, totjmp;
subans = cstois(ZERO);
dgtptr = &one[pos];
times = *dgtptr - '0';
if(times&&maxjmp&&times>=maxjmp) do{
   tmps = muli(cpystr(other),maxjmp), totjmp = maxjmp;
   while(maxint/maxjmp>totjmp&&times>totjmp*maxjmp)
      tmps = muli(tmps,maxjmp), totjmp *= maxjmp;
   while(maxjmp>2&&maxint/2>totjmp&&times>totjmp*2)
      tmps = muli(tmps,2), totjmp *= 2;
   subans = add(subans,tmps), times -= totjmp;
   }while(times&&maxjmp&&times>=maxjmp);
if(times&&times<maxjmp) subans=add(subans,muli(cpystr(other),times));
else while(times--) subans = add(subans,cpystr(other));
if(dgtptr>point) subans = rgtshift(subans,(int)(dgtptr-point));
else if(point>dgtptr) subans = lftshift(subans,(int)(point-dgtptr)-1);
return subans;
}


int *divd(str1,str2,sigdgt)
int *str1, *str2;
int sigdgt;
{
int *ans, *divrem(), *str0 = 0, *clrstr(), *ptadrs(), *inttos(), *divi();
int *hisgdt();
int ansdgt = ordhsd(str2) - ordhsd(str1) + *hisgdt(str2) > *hisgdt(str1);
if(sigdgt&&ansdgt>0) sigdgt += ansdgt + digits(str2) + digits(str1);
if(zero(ptadrs(str2))&&greater(mxistr,str2)){
   int intg = strtoi(str2);
   free(clrstr(str2));
      return divi(str1,intg,sigdgt);
   }
ans = divrem(&str0,str1,str2,sigdgt);
if(str0) free(clrstr(str0));
return ans;
}

int *remd(str1,str2,sigdgt)
int *str1, *str2;
int sigdgt;
{
int *ans, *divrem(), *str0 = 0, *clrstr();
ans = divrem(&str0,str1,str2,sigdgt);
if(ans) free(clrstr(ans));
return str0;
}


int *divrem(str0,str1,str2,sigdgt)
int **str0, *str1, *str2;
int sigdgt;
{
int od1, od2, order, point, times, jump, cnt, *tmps, *muli();
int *pt1, *pt2, *hi1, *hi2, *res, *clrstr(), tmpo;
int *lftpad(), *rgtpad(), *cpystr(), *lftshift(), *rgtshift();
int *hisgdt(), *ptadrs(), *int1ptr(), *subt(), *trim(), *strint();
if(!sigdgt)  sigdgt = max(sigdgt,2*max(istrln(str1),istrln(str2)));
if(radix<8) sigdgt *= 2;
pt1 = ptadrs(str1), pt2 = ptadrs(str2);
hi1 = hisgdt(str1), hi2 = hisgdt(str2);
if(hi2==strint(str2,0)){
   if(hi1==strint(str1,0)){
      err_prnt("zero/zero undefined");
      }
   else err_prnt("division by zero");
   return 0;
   }
if(hi1==strint(str1,0)) return cpystr(*str0 = str1);
od1 = ((od1=pt1-hi1)>0) ? --od1 : od1;
od2 = ((od2=pt2-hi2)>0) ? --od2 : od2;
point = ((order = od1 - od2)>=0) ? 1 : 0;
res = int1ptr(1);
if(order>0) str2 = lftshift(str2,order);
if(order<0){
   str2 = rgtshift(str2,-order);
   if(order==-1) res = lftpad(res,0+'.');
   else res = rgtshift(res,-order-1);
   if(greater(str2,str1)) res = rgtpad(res,'0');
   }
if(greater(str2,str1)) sigdgt++;
while(sigdgt--){
   times = 0;
   if(!greater(str2,str1)){
      while(!greater(str2,str1)){
         if(radix>10){
            hi1 = hisgdt(str1), hi2 = hisgdt(str2);
            switch(ordhsd(str1)-ordhsd(str2)){
               case 1: jump = radix/(*hi2)*(*hi1);
                  if(jump>0) jump--;
                  if(hi2[1]>jump) jump -= hi2[1]/jump;
                  break;
               case 0: jump = *hi1/(*hi2);
                  if(jump>0) jump--;
               default:;
               }
            if(jump) while(jump){
               while(!greater(str1,(tmps = muli(cpystr(str2),jump)))&&jump>10)
                  jump /= 10, free(clrstr(tmps));
               if(!greater(tmps,str1)){
                  while(!greater(tmps,str1))
                     str1 = subt(cpystr(tmps),str1), times += jump;
                  free(clrstr(tmps)), jump = 0;
                  }
               else str1 = subt(cpystr(str2),str1), times++,
                  free(clrstr(tmps)), jump = 0;
               }
            else str1 = subt(cpystr(str2),str1), times++;
            }
         else str1 = subt(cpystr(str2),str1), times++;
         }
      res = rgtpad(res,times+'0');
      }
   else if(!zero(str1)){
      if(point){
         res = rgtpad(res,0+'.');
         point = 0;
         }
      str2 = rgtshift(str2,1);
      if(greater(str2,str1)) res = rgtpad(res,times+'0');
      sigdgt++;
      }
   else break;
   }
if(order>0) res = lftshift(res,order);
*str0 = str1;
free(clrstr(str2));
return trim(res);
}


int *muli(str,num)
int *str;
int num;
{
int prod, cary = 0;
register int *reg, *run;
int *int1ptr(), *lftpad(), *ans, *trim(), *clrstr(), *tmpv,
      *inttos(), *mult(), *add(), *divmod(),
      *tmpr, *cstois(), *ptadrs(), *strint();
if(num<0) fprintf(stderr,"muli: %d sign changed\n",num), num = iabs(num);
if(radix>10&&num>maxjmp) return mult(str,inttos(num));
if(!num){
   ans = chrpos(str,0+'.')>-1 ? cstois(FZERO) : cstois(ZERO);
   free(clrstr(str));
   return ans;
   }
if(zero(ptadrs(str))&&greater(mxistr,str)){
   if(greater(tmpv=inttos((maxint-'0')/num),str)||equal(tmpv,str)){
      free(clrstr(tmpv)), prod = strtoi(str), free(clrstr(str));
      return inttos(prod*num);
      }
   free(clrstr(tmpv));
   }
ans = int1ptr(1), run = strint(str,0);
while((run--)-str){
   if(*run==0+'.') ans = lftpad(ans,0+'.');
   else if(*run-'0'<(maxint-'0')/num&&(*run-'0')*num<maxint-'0'-cary)
      prod = (*run-'0')*num, prod += cary,
      cary = prod/radix, ans = lftpad(ans,prod%radix+'0');
   else tmpv = mult(inttos(num),inttos(*run-'0')),
      tmpv=add(tmpv,inttos(cary)),
      tmpv = divmod(&tmpr,tmpv,inttos(radix)), cary = strtoi(tmpv),
      ans = lftpad(ans,strtoi(tmpr)+'0'),free(clrstr(tmpr)),free(clrstr(tmpv));
   }
while(cary) prod = cary, cary = prod/radix, ans = lftpad(ans,prod%radix+'0');
free(clrstr(str));
return trim(ans);
}

int *divi(str,num,sigdgt)
int *str;
int num, sigdgt;
{
int val, cary = 0, point = 0;
register int *reg, *run = str;
int *int1ptr(), *rgtpad(), *ans = int1ptr(1), *cpystr(),
     *tmpr, *tmpv, *muli(), *idivmod(), *add(),
     *inttos(), *trim(), *clrstr();
if(!sigdgt)  sigdgt = max(sigdgt,2*max(istrln(str),poswidth(num)));
if(radix<8) sigdgt *= 2;
if(!num) err_prnt("divi: division by zero"),exit(1);
if(num<0) num = iabs(num), err_prnt("divi: sign changed");
while(*run){
   if(*run==0+'.') point = 1, ans = rgtpad(ans,0+'.'), run++;
   else if(!cary) val = *run++ - '0', cary = val%num, val /= num,
      ans = rgtpad(ans,val+'0');
   else if(cary&&radix<(maxint-'0')/cary&&radix*cary<maxint-*run)
      val = (cary*radix - '0' ) + *run++,
      cary = val%num, val /= num, ans = rgtpad(ans,val+'0');
   else if(cary&&radix<(maxint-'0')/cary&&num>1)
      tmpv = add(inttos(cary*radix),inttos(*run++-'0')),
      tmpv = idivmod(&cary,tmpv,num),
      ans = rgtpad(ans,strtoi(tmpv)+'0'), free(clrstr(tmpv));
   else tmpv = muli(inttos(cary),radix), tmpv = add(tmpv,inttos(*run++-'0')),
      tmpv = idivmod(&cary,tmpv,num),
      ans = rgtpad(ans,strtoi(tmpv)+'0'), free(clrstr(tmpv));
   sigdgt--;
   }
if(!point) ans = rgtpad(ans,0+'.'), sigdgt++;
while(sigdgt-->0&&cary){
   if(cary&&radix<(maxint-'0')/cary)
      val = (cary*radix)/num, cary = (cary*radix)%num, ans = rgtpad(ans,val+'0');
   else tmpv = muli(inttos(cary),radix),
      tmpv = idivmod(&cary,tmpv,num),
      ans = rgtpad(ans,strtoi(tmpv)+'0'), free(clrstr(tmpv));
   }
free(clrstr(str));
return trim(ans);
}


int *idivmod(rem,str,num)
int *rem, *str, num;
{
int val, cary = 0;
int *rgtpad(), *inttos(),
     *tmpv, *integer(), *cstois(), *clrstr(), *muli(), *add(), *divmod(),
     *tmpr, *int1ptr(), *ans = int1ptr(1), *cpystr();
register int *reg, *run = integer(str);
free(clrstr(str)), str = run;
if(!num) err_prnt("idivmod: division by zero\n"),exit(1);
if(num<0) num = iabs(num), err_prnt("idivmod: sign changed");
if(greater((tmpv=inttos(num)),str))
   {free(clrstr(tmpv));*rem = strtoi(str),free(clrstr(str));
   return cstois(ZERO);}
if(greater(mxistr,str))
   {free(clrstr(tmpv));val=strtoi(str);free(clrstr(str));*rem = val%num;
   return inttos(val/num);}
free(clrstr(tmpv));
while(*run){
   if(!cary) val = *run++ - '0', cary = val%num, val /= num,
      ans = rgtpad(ans,val+'0');
   else if(cary&&radix<(maxint-'0')/cary&&radix*cary<maxint-*run)
      val = (cary*radix - '0' ) + *run++, cary = val%num, val /= num,
      ans = rgtpad(ans,val+'0');
   else if(cary&&radix<(maxint-'0')/cary&&num>1)
      tmpv = add(inttos(cary*radix),inttos(*run++-'0')),
      tmpv = divmod(&tmpr,tmpv,inttos(num)), cary = strtoi(tmpr),
      free(clrstr(tmpr)), val = strtoi(tmpv),
      ans = rgtpad(ans,val+'0'), free(clrstr(tmpv));
   else tmpv = muli(inttos(cary),radix), tmpv = add(tmpv,inttos(*run++-'0')),
      tmpv = divmod(&tmpr,tmpv,inttos(num)), cary = strtoi(tmpr),
      free(clrstr(tmpr)), val = strtoi(tmpv),
      ans = rgtpad(ans,val+'0'), free(clrstr(tmpv));
   }
free(clrstr(str));
*rem = cary;
return ans;
}


int *idiv(str1,str2)
int *str1, *str2;
{
int mod;
int *ans, *idivmod(), *divmod(), *str0 = 0, *clrstr(),
     *tmps, *inttos(), *ptadrs();
if(zero(ptadrs(str2))&&greater(mxistr,str2)){
   ans = idivmod(&mod,str1,strtoi(str2));
   free(clrstr(str2));
   return ans;
   }
ans = divmod(&str0,str1,str2);
if(str0) free(clrstr(str0));
return ans;
}


int *modl(str1,str2)
int *str1, *str2;
{
int *ans, *divmod(), *str0 = 0, *clrstr();
ans = divmod(&str0,str1,str2);
if(ans) free(clrstr(ans));
return str0;
}


int *divmod(str0,str1,str2)    /* divide first by second  */
int **str0, *str1, *str2;
{
int times, order = 1, jump, *tmps, tmpo, *muli(), *hisgdt(), *hi1, *hi2, cnt;
int *res, *rgtpad(), *cpystr(), *lftshift(), *clrstr(),
     *rgtshift(),*int1ptr(),*subt(), *trim(), *cstois();
if(zero(str2)){
   err_prnt("division by zero");
   return 0;
   }
if(greater(str2,str1)){
   *str0 = cpystr(str1);
   return cstois(ZERO);
   }
else if(!greater(str1,str2)){
   *str0 = cstois(ZERO);
   return cstois(ONE);
   }
while(greater(str1,(res = lftshift(cpystr(str2),1))))
   free(clrstr(str2)), str2 = res, order++;
res = int1ptr(1);
while(!zero(str1)&&order--){
   times = 0;
   while(!greater(str2,str1)){
      if(radix>10){
         hi1 = hisgdt(str1), hi2 = hisgdt(str2);
         switch(ordhsd(str1)-ordhsd(str2)){
            case 1: jump = radix/(*hi2)*(*hi1);
               if(jump>0) jump--;
               if(hi2[1]>jump) jump -= hi2[1]/jump;
               break;
            case 0: jump = *hi1/(*hi2);
               if(jump>0) jump--;
            default:;
            }
         if(jump) while(jump){
            while(!greater(str1,(tmps = muli(cpystr(str2),jump)))&&jump>10)
               jump /= 10, free(clrstr(tmps));
            if(!greater(tmps,str1)){
               while(!greater(tmps,str1))
                  str1 = subt(cpystr(tmps),str1), times += jump;
               free(clrstr(tmps)), jump = 0;
               }
            else str1 = subt(cpystr(str2),str1), times++,
               free(clrstr(tmps)), jump = 0;
            }
         else str1 = subt(cpystr(str2),str1), times++;
         }
      else str1 = subt(cpystr(str2),str1), times++;
      }
   res = rgtpad(res,times+'0');
   str2 = rgtshift(str2,1);
   }
*str0 = str1;
free(clrstr(str2));
return  trim(res);
}


int *fctrial(decin)
int *decin;
{
int decint;
int *fctrl, *ptadrs(), *integer(), *inttos(), *clrstr(),
     *cpystr(), *mult(), *muli(), *trim(), *cstois();
if(!zero(ptadrs(decin))){
   decin = integer(decin);
   err_prnt("factorial: fraction discarded");
   }
else decin = cpystr(decin);
fctrl = cstois(ONE);
while(greater(decin,mxistr))
   fctrl = mult(fctrl,cpystr(decin)), decin = subt(cstois(ONE),decin);
decint = strtoi(decin);
while(decint>1) fctrl = muli(fctrl,decint--);
free(clrstr(decin)), decin=integer(fctrl), free(clrstr(fctrl)),
fctrl=decin;
return trim(fctrl);
}


int *ptadrs(num)
int *num;
{
int *strint(), *poindx;
return ((poindx=strint(num,0+'.'))==(int*)0) ? strint(num,0) : poindx;}


int ordhsd(num)
int *num;
{
int *ptadrs(), *pt = ptadrs(num), *hisgdt(), *hi = hisgdt(num), order = 0;
order = ((order=(int)(pt-hi))>0) ? --order : (order =(int) (hi-pt), -order);
return order;
}

int greater(str1,str2)
int *str1, *str2;
{
int tlen, hlen;
register int *ptr1, *ptr2;
int *end1, *end2, *strint();
ptr1 = str1, ptr2 = str2;
end1 = strint(str1,0), end2 = strint(str2,0);
tlen = frclen(str1) - frclen(str2);
hlen = intlen(str1) - intlen(str2);
if(hlen>0) {while(hlen--) if(*ptr1++>'0') return 1;}
else if(hlen<0) while(hlen++) if(*ptr2++>'0') return 0;
while(ptr1-end1&&ptr2-end2){
   if(*ptr1>*ptr2) return 1;
   if(*ptr1<*ptr2) return 0;
   ptr1++,ptr2++;
   }
if(tlen>0) while(tlen--) if(*ptr1++>'0') return 1;
return 0;
}

int equal(str1,str2) int *str1,*str2;{
return !greater(str1,str2)&&!greater(str2,str1);}

int zero(num)
int *num;
{
num--;
while(*++num){
   if(*num==0+'.') continue;
   if(*num>'0') return 0;
   }
return 1;
}


int *shift(num,count)
int *num;
int count;
{
int *lftshift(), *rgtshift();
return !count ? num :  (count>0) ? lftshift(num,count) : rgtshift(num,-count);
}


int *rgtshift(num,count)
int *num;
int count;
{
int *ptr, *prev, *lftpad(), *rgtpad(), *strint();
while(count--){
   ptr = strint(num,0+'.');
   switch(chrpos(num,0+'.')){
      case -1:
         num = rgtpad(num,0+'.');
         count++;
         break;
      case 0:
         num = lftpad(num,'0');
         count++;
         break;
      default:
         prev = ptr - 1;
         *ptr = *prev;
         *prev = 0+'.';
         }
      }
return num;
}


int *lftshift(num,count)
int *num;
int count;
{
int *ptr, *next, *rgtpad(), *strint();
while(count--){
   ptr = strint(num,0+'.');
   switch(frclen(num)){
      case 0:  num = rgtpad(num,'0');
               break;
      case 1:  num = rgtpad(num,'0');
               count++;
               break;
      default: next = ptr + 1;
               *ptr = *next;
               *next = 0+'.';
      }
   }
return num;
}

int sumdgt(str)
int *str;
{
int sum = 0;
while(*str){
   if(adigit(*str)) sum += *str - '0';
   str++;
   }
return sum;
}


int intlen(flt)
int *flt;
{
int retval;
retval = chrpos(flt,0+'.');
return (retval<0) ? chrpos(flt,0) : retval;
}


int frclen(flt)
int *flt;
{
int retval;
retval = chrpos(flt,0+'.');
return  (retval==-1) ? 0 : chrpos(flt,0) - retval;
}



int chrpos(str,chr)
int *str, chr;
{
int *strint(), *chridx;
chridx = strint(str,chr);
return (chridx) ? (int) (chridx - str) : -1;
}



int sumlen(str1,str2)
int *str1, *str2;
{
return
    max(iabs(intlen(str1)),iabs(intlen(str2))) +
    max(iabs(frclen(str1)),iabs(frclen(str2)));
}

int *trim(num)
int *num;
{
int *lftrim(), *rgtrim();
return rgtrim(lftrim(num));
}


int *lftrim(num)
int *num;
{
int *lftcut(), *strint();
if(strint(num,0)==num) return num;
if(intlen(num))
   while(*num=='0'&&(frclen(num)>1||intlen(num)>1)) num = lftcut(num);
return num;
}



int *rgtrim(num)
int *num;
{
int *run, *tmp, *str, *top, *ptadrs(), *clrstr(), *cpystr(), *strint();
if((tmp=ptadrs(num))<(run=top=strint(num,0))){
   while(--run>tmp+1&&*run=='0') *run = 0;
   str = cpystr(num); while(run!=top) *run++ = '0'; free(clrstr(num));
   return str;
   }
else return num;
}


int stor10i(str)         /* convert string to radix 10 integer  */
int *str;              /* usage: integer = strtoi(str); */
{
int *run, *ptadrs();
int powr=1, buff=0;
if(!str) return buff;
run = ptadrs(str);
while(1){
   if(*run>'0'-1){
      buff += (*run-'0') * powr;
      powr *= 10;
      }
   else if(*run==0+'-') buff *= -1;
   if(run--==str) break;
   }
return(buff);
}


int *dstors(dstr)
int *dstr;
{
int mod, dec, *int1ptr(), *istr = int1ptr(1), stor10i(), *lftpad(), *idivmod();
while(greater(dstr,mxir10s))
   dstr = idivmod(&mod,dstr,radix), istr = lftpad(istr,mod+'0');
dec = stor10i(dstr);
while(dec) mod = dec%radix, dec = dec/radix, istr = lftpad(istr,mod+'0');
return istr;
}

int strtoi(str)         /* convert string to integer  */
int *str;              /* usage: integer = strtoi(str); */
{
int *run, *ptadrs();
int powr=1, buff=0;
if(!str) return buff;
run = ptadrs(str);
while(1){
   if(*run>'0'-1){
      buff += (*run-'0') * powr;
      powr *= radix;
      }
   else if(*run==0+'-') buff *= -1;
   if(run--==str) break;
   }
return(buff);
}


int *inttos(value)
int value;
{
int tmpv;
int *ans, *int1ptr(), *cpystr(), *lftpad();
ans = int1ptr(1);
while(value) tmpv = value, ans = lftpad(ans,tmpv-(value/=radix)*radix+'0');
return (*ans) ? ans : lftpad(ans,'0');
}

char *intocs(value)
int value;
{
int *inttos(), *istr = inttos(value), *clrstr();
char *istocs(), *cstr = istocs(istr);
free(clrstr(istr));
return cstr;
}


int *dbltos(value)
double value;
{
double tmpv;
int *ans, *exponent, *shift(), *strint(), *cstois();
char *chrp, *chr1ptr(), *cclrstr();
if(!dblprc) precision();
chrp = chr1ptr(3+dblprc+7);
sprintf(chrp,"%-*.*e\0",dblprc+9,dblprc,value);
ans = cstois(chrp), free(cclrstr(chrp));
*(exponent = strint(ans,0+'e')) = 0;exponent++;
return shift(ans,stor10i(exponent));
}

char *dbtocs(value)
double value;
{
int *dbltos(), *istr = dbltos(value), *clrstr();
char *istocs(), *cstr = istocs(istr);
free(clrstr(istr));
return cstr;
}


int *flttos()
{
int exp, *strint(), *cstois();
float tmpv, value = global_float;
int *ans, *exponent, *shift();
char *cptr, *chr1ptr(), *cclrstr();
if(!sglprc) precision();
cptr = chr1ptr(3+sglprc+7);
sprintf(cptr,"%-*.*e\0",sglprc+9,sglprc,value);
ans = cstois(cptr), free(cclrstr(cptr));
*(exponent = strint(ans,0+'e')) = 0;exponent++;
exp = stor10i(exponent);
return shift(ans,exp);
}

char *fltocs()
{
int *flttos(), *istr = flttos(), *clrstr();
char *istocs(), *cstr = istocs(istr);
free(clrstr(istr));
return cstr;
}


int *fraction(str)
int *str;
{
int *cpystr(), *ptadrs();
return cpystr(ptadrs(str));
}

int *integer(str)
int *str;
{
int *point, *intgr, *cpystr(), *ptadrs();
if(*(point = ptadrs(str))) *point = 0, intgr = cpystr(str), *point = 0+'.';
else intgr = cpystr(str);
return intgr;
}


int min(int1,int2)
int int1, int2;
{
return (int1<=int2) ? int1 : int2;
}


int max(int1,int2)
int int1, int2;
{
return (int1>=int2) ? int1 : int2;
}

int iabs(num)
int num;
{
return (num<0) ? -num : num;
}

int *hisgdt(num)
int *num;
{
num--;
while(*++num<='0'&&*num);
return num;
}

