static char SccsId[] = "@(#) cplx.c 1.6 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <defn.h>
#include <apax.h>
#include <cplx.h>
#include <util.h>

extern NAMS *trace;
extern int trace_level;

void complex(strs,opts)
STRS **strs;
int *opts;
{
expand(strs,opts);
unmixclx(strs,opts);
collect(strs,opts);
while((*strs)->prevstr) *strs = (*strs)->prevstr;
parse(strs,opts);
}

void unmixclx(strs,opts)
STRS **strs;
int *opts;
{
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   *strs = (*strs)->nextstr;
   if((*strs)->str&&isclx(*(*strs)->str)&&(*strs)->nextstr&&
   (*strs)->nextstr->str&&isclx(*(*strs)->nextstr->str))
          mixed(strs,*opts);
   }
while((*strs)->prevstr)  *strs = (*strs)->prevstr;
}

void expand(strs,opts)
STRS **strs;
int *opts;
{
STRS *lftexp(), *reduce_oprn();
SMRK opr, lft, rgt, end;
SJNT jnt1, jnt2, jnt3, jnt4;
opr.mrk = opr.lft = opr.rgt = end.mrk = end.lft = end.rgt =
lft.mrk = lft.lft = lft.rgt = rgt.mrk = rgt.lft = rgt.rgt =
          jnt1.lft = jnt1.rgt = jnt2.lft = jnt2.rgt = (STRS*) 0;
while((*strs)->nextstr){
   *strs = (*strs)->nextstr;
   if((*strs)->str&&isopr_dm(*(*strs)->str)){
      if(!opr.mrk){
         opr.mrk = *strs,opr.lft = (*strs)->prevstr,opr.rgt = (*strs)->nextstr;
         end.mrk = *strs,end.lft = (*strs)->prevstr,end.rgt = (*strs)->nextstr;
         if(!lft.mrk&&!jnt1.rgt){
            jnt1.rgt = *strs;
            while(!isarg(jnt1.rgt)) jnt1.rgt = jnt1.rgt->prevstr;
            jnt1.lft = jnt1.rgt->prevstr;
            }
         }
      if(!rgt.mrk)
         end.mrk = *strs,end.lft = (*strs)->prevstr,end.rgt = (*strs)->nextstr;
      }
   if((*strs)->str&&isclx(*(*strs)->str)&&!opr.mrk){
      lft.mrk = *strs, lft.rgt = (*strs)->nextstr;
      if((*strs)->prevstr&&(*strs)->prevstr->str&&
         isarg((*strs)->prevstr)) lft.lft = (*strs)->prevstr;
      if(lft.lft&&lft.lft->str&&isarg(lft.lft))
           jnt1.lft = lft.lft->prevstr, jnt1.rgt = lft.lft;
      else jnt1.lft = (*strs)->prevstr, jnt1.rgt = *strs;
      }
   if((*strs)->str&&isclx(*(*strs)->str)&&opr.mrk){
      rgt.mrk = *strs, rgt.rgt = (*strs)->nextstr;
      if((*strs)->prevstr&&(*strs)->prevstr->str&&
         isarg((*strs)->prevstr)) rgt.lft = (*strs)->prevstr;
      }
   if(opr.mrk&&isarg(*strs)&&(!(*strs)->nextstr||
      ((*strs)->nextstr->str&&isopr_as(*(*strs)->nextstr->str)||
      isopr_dm(*(*strs)->nextstr->str)))){
      if(rgt.mrk&&rgt.lft&&rgt.rgt){
         jnt2.lft = *strs, jnt2.rgt = (*strs)->nextstr;
         rgtexp(&jnt1,&opr,&end,&rgt,strs,opts);
         *strs = jnt1.lft, jnt1.lft = jnt1.rgt = (STRS*) 0;
         if(trace_level==4) nams_prnt(trace); 
         }
      else if((!rgt.lft||!rgt.rgt)&&(lft.mrk||rgt.mrk)){
         if(lft.mrk&&lft.lft&&lft.rgt){
            jnt2.lft = *strs, jnt2.rgt = (*strs)->nextstr;
            *strs = lftexp(&lft,&opr,&jnt2);
            *strs = reduce_oprn(*strs,*opts);
            if(trace_level==4) nams_prnt(trace); 
            }
         *strs = reduce_oprn(opr.mrk,*opts);
         if(trace_level==4) nams_prnt(trace); 
         if((*strs)->nextstr&&(*strs)->nextstr->str&&
            isopr_dm(*(*strs)->nextstr->str)) *strs = jnt1.lft;
          else jnt1.lft = jnt1.rgt = (STRS*) 0;
         }
      opr.mrk = opr.lft = opr.rgt = end.lft = end.mrk = end.rgt
              = rgt.lft = rgt.mrk = rgt.rgt = lft.lft = lft.mrk = lft.rgt
               = jnt2.lft = jnt2.rgt = (STRS*) 0;
      }
   }
while((*strs)->prevstr) *strs = (*strs)->prevstr;
}


STRS *lftexp(lf,op,j2)
SMRK *lf, *op;
SJNT *j2;
{
STRS *rn, *new, *strs, *insprevstrs();
rn = op->mrk;
new = strs = insprevstrs(lf->mrk);
cpystrsval(rn,strs);
while(rn->nextstr!=j2->rgt){
   rn = rn->nextstr;
   strs = insprevstrs(lf->mrk);
   cpystrsval(rn,strs);
   }
return new;
}

void rgtexp(j1,op,en,rg,strs,opts)
SJNT *j1;
SMRK *op, *en, *rg;
STRS **strs;
int *opts;
{
STRS *new;
int *cstois();
STRS *rn, *divlex(), *insprevstrs();
if(op&&op->mrk&&op->mrk->str&&isopr_d(*op->mrk->str)){
   *op->mrk->str = 0+'*', setopr(op->mrk->str);
   *strs = insprevstrs(divlex(op,rg,strs,opts));
   (*strs)->form = op->mrk->form, (*strs)->str = cstois(NEG);
   }
rn = j1->rgt;
*strs = insprevstrs(rg->mrk);
cpystrsval(rg->mrk,*strs);
*(*strs)->str = 0+'+',setopr((*strs)->str);
while(rn!=en->rgt){
   *strs = insprevstrs(rg->mrk);
   cpystrsval(rn,*strs);
   rn = rn->nextstr;
   }
}

STRS *divlex(op,rg,strs,opts)
SMRK *op, *rg;
STRS **strs;
int *opts;
{
STRS *cp, *rn, *tmps, *modlex(), *reduce_oprn(), *cpystrs(),
     *insprevstrs(), *insnextstrs();
cp = modlex(op,rg,opts);
for(*strs=rg->mrk;!isarg(*strs);*strs=(*strs)->prevstr);
tmps = insnextstrs(*strs);cpystrsval(op->mrk,tmps);
*tmps->str = 0+'/',setopr(tmps->str);
*strs = insnextstrs(tmps);cpystrsval(cp,*strs);
*strs = reduce_oprn(tmps,*opts);
for(*strs=rg->mrk;!isarg(*strs)&&(*strs)->nextstr;
   *strs=(*strs)->nextstr);
tmps = insnextstrs(*strs);cpystrsval(op->mrk,tmps);
*tmps->str = 0+'/',setopr(tmps->str);
*strs = insnextstrs(tmps);cpystrsval(cp,*strs);*strs = tmps->prevstr;
*strs = reduce_oprn(tmps,*opts);
return *strs;
}



STRS *modlex(op,rg,opts)
SMRK *op, *rg;
int *opts;
{
int *cstois();
STRS *cp, *rn, *strs, *tmps, *square(), *reduce_oprn(), *padstrs(),
     *cpystrs(), *insprevstrs(), *insnextstrs();
for(cp=rg->mrk;!isarg(cp);cp=cp->prevstr);
strs = square(cp,opts);
for(cp=rg->mrk;!isarg(cp);cp=cp->nextstr);
cp = padstrs(strs,square(cp,opts));
strs = insnextstrs(strs);
strs->str = cstois(POS),strs->form = op->mrk->form;
return reduce_oprn(strs,*opts);
}


STRS *square(sarg,opts)
STRS *sarg;
int *opts;
{
int *cstois();
STRS *mkstrs(), *cpystrs(), *reduce_oprn();
SMRK tris;
tris.mrk = mkstrs(1);
tris.mrk->str = cstois(MUL);
tris.mrk->form = sarg->form;
tris.mrk->prevstr = tris.lft = cpystrs(sarg);
tris.lft->nextstr = tris.mrk;
tris.mrk->nextstr = tris.rgt = cpystrs(sarg);
tris.rgt->prevstr = tris.mrk;
return reduce_oprn(tris.mrk,*opts);
}


void collect(strs,opts)
STRS **strs;
int *opts;
{
int *cpystr();
STRS *insnextstrs(), *reduce_oprn(), *pivot = (STRS*) 0, *tmpstr; 
SJNT jnt1, jnt2, jnt3, jnt4, prev_jnt;
jnt1.lft = jnt1.rgt = jnt2.lft = jnt2.rgt = jnt3.lft
   = jnt3.rgt = jnt4.lft = jnt4.rgt = (STRS*) 0;
while((*strs)->nextstr){
   if(!jnt1.lft&&!jnt1.rgt){
      while((*strs)->nextstr){
         *strs = (*strs)->nextstr;
         if((*strs)->str&&isclx(*(*strs)->str)) break;
         }  /* ^^ find the first complex operator (complex_op1) ^^ */
      jnt1.lft = (*strs)->prevstr, jnt1.rgt = *strs;
      }  /* ^^ mark the left joint of complex_op1 [1] ^^ */
   else if((*strs)->str&&issep(*(*strs)->str)||isobj(*(*strs)->str)){
     collect(strs,opts);
         return;
     }
   else jnt1.lft = jnt1.rgt->prevstr;
   while((*strs)->nextstr){
      *strs = (*strs)->nextstr;
      if((*strs)->str&&issep(*(*strs)->str)||isobj(*(*strs)->str)){
         collect(strs,opts);
         return;
         }
      if((*strs)->str&&isarg(*strs)&&(*strs)->nextstr&&
         (*strs)->nextstr->str&&isopr_as(*(*strs)->nextstr->str)&&
         (*strs)->nextstr->nextstr&&(*strs)->nextstr->nextstr->str&&
         !isclx(*(*strs)->nextstr->nextstr->str)) break;
      if((*strs)->str&&(isarg(*strs)||isopr_as(*(*strs)->str))&&
         (*strs)->nextstr&&(*strs)->nextstr->str&&
         isclx(*(*strs)->nextstr->str)){
         *(*strs)->nextstr->str=0+'+',setopr((*strs)->nextstr->str),
         (*strs)->nextstr->form=(*strs)->form;
         if((*strs)->str&&isopr_as(*(*strs)->str))
            *strs = reduce_oprn(*strs,*opts);
         else *strs = reduce_oprn((*strs)->nextstr,*opts);
         if(trace_level==4) nams_prnt(trace); 
         *strs = (*strs)->prevstr;
         } /* ^^ add up imaginaries ^^  */
      } /* ^^ reduce contiguous imaginaries into one imaginary ^^ */
   if(((*strs)->str&&issep(*(*strs)->str))||
      ((*strs)->str&&isobj(*(*strs)->str))){
      collect(strs,opts);
      return;
      }
   if((*strs)->nextstr) pivot = insnextstrs(*strs);
      /* ^^ to pivot reals to the left of compex_op1 [1] ^^ */
   else break; /*  << no more tokens to parse  <<  */
   if(pivot){
      jnt2.lft = jnt3.rgt = pivot; /* <<    imaginary    <<  */
      jnt3.lft = *strs;            /* << [3]pivot[2]     <<  */
      jnt2.rgt = pivot->nextstr;   /* <<    real(s)      <<  */
      while((*strs)->nextstr){
         *strs = (*strs)->nextstr;
         if((*strs)->str&&issep(*(*strs)->str)||
            (*strs)->str&&isobj(*(*strs)->str)){
            collect(strs,opts);
            return;
            }
         if((*strs)->str&&isclx(*(*strs)->str)){
            jnt4.lft = (*strs)->prevstr;
            jnt4.rgt = *strs;  /* <<  [2]reals[4]complex_op2 << */
            if(prev_jnt.lft){
               (insnextstrs(*strs))->str = cpystr(prev_jnt.lft->str);
               prev_jnt.lft = prev_jnt.rgt = (STRS*) 0;
               }
            break;
            }  /*  ^^  stop at the end of reals ^^  */
         jnt4.lft = *strs;
         jnt4.rgt = (*strs)->nextstr;
         if(jnt4.lft->str&&isopr_s(*jnt4.lft->str)&&
            jnt4.rgt->str&&isarg(jnt4.rgt)){
            prev_jnt.lft = jnt4.lft;
            prev_jnt.rgt = jnt4.rgt;
            }
         }    /* ^^ find the end of reals while moving joint [4] ^^  */
      swapsjnt(&jnt1,&jnt2); /* <<  after swaps, end result is     << */
      swapsjnt(&jnt3,&jnt4); /* << [2]reals[1]pivot[4]imaginary[3] << */
      delstrs(pivot);         /* <<  [2]reals[1][4]imaginary[3]    << */
      pivot = (STRS*) 0;
      if(jnt4.rgt){
         free(clrstr(jnt4.rgt->str));
         jnt4.rgt->str = cpystr(jnt2.rgt->str);
         jnt4.rgt->form = jnt2.rgt->form;
         jnt4.rgt->shift = jnt2.rgt->shift;
         } /*  ^^copy the outer operator into the complex2's spot^^ */
      }
   }
}



void swapsjnt(jnt1,jnt2)
SJNT *jnt1, *jnt2;
{
SJNT tmpj;
tmpj.lft = tmpj.rgt = (STRS*) 0;
tmpj.lft = jnt1->lft;
tmpj.rgt = jnt1->rgt;
if(jnt1->lft) jnt1->lft->nextstr = jnt2->rgt;
if(jnt1->rgt) jnt1->rgt->prevstr = jnt2->lft;
if(jnt2->rgt) jnt2->rgt->prevstr = tmpj.lft;
if(jnt2->lft) jnt2->lft->nextstr = tmpj.rgt;
}
