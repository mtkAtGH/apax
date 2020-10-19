static char SccsId[] = "@(#) objx.c 1.8 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */


#include <defn.h>
#include <apax.h>
#include <fixr.h>
#include <fctr.h>
#include <objx.h>
#include <cplx.h>
#include <util.h>

extern NAMS *nams, *trace;
extern int trace_level;

void
object(strs,opts)
STRS **strs;
int *opts;
{
OBJT *objt, *setobj(), *clrobjt();
OBJO *mkobjo(), *objo = mkobjo(1);
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   *strs = (*strs)->nextstr;
   if((*strs)->str&&islft(*(*strs)->str)){
      if(!(objt = setobj(strs))) return;
      else if(objt->sym){
         free(clrobjt(objt)), objt = (OBJT*) NULL;
         objo->lftobj = objo->rgtobj = (OBJT*) NULL;
         objo->opr = (STRS*) NULL;
         while((*strs)->prevstr) *strs = (*strs)->prevstr;
         }
      if(!objo->opr) objo->rgtobj = objt, objt = NULL;
      else if(objo->rgtobj){
         objo->lftobj = objo->rgtobj;
         objo->rgtobj = objt, objt = NULL;
         }
      else objo->rgtobj = objt, objt = NULL;
      if((*strs)->prevstr&&(*strs)->prevstr->str&&
         isopr(*(*strs)->prevstr->str)&&!(ismrk(*(*strs)->prevstr->str&&
            !isclx(*(*strs)->nextstr->str))))
            objo->opr = (*strs)->prevstr;
      }
   operate(objo,strs,opts);
   if((*strs)->str&&isrgt(*(*strs)->str)){
      if((*strs)->nextstr&&(*strs)->nextstr->str&&
         isopr(*(*strs)->nextstr->str)&&!(ismrk(*(*strs)->nextstr->str&&
         !isclx(*(*strs)->nextstr->str)))){
         objo->opr = (*strs)->nextstr;
         objo->lftobj = objo->rgtobj;
         objo->rgtobj = NULL;
         }
      if(!(*strs)->nextstr)
         if(objo->rgtobj) free(clrobjt(objo->rgtobj));
      }
   }
}

void
operate(objo,strs,opts)
OBJO *objo;
STRS **strs;
int *opts;
{
if(objo->lftobj&&objo->opr&&objo->rgtobj)
   dual_action(objo,strs,opts);
else if(objo->opr&&objo->rgtobj){
   rightaction(objo,strs);
   delstrs(objo->opr);
   objo->opr = NULL;
   }
}

void
dual_action(objo,strs,opts)
OBJO *objo;
STRS **strs;
int *opts;
{
OBJT *clrobjt();
switch(isopr(*objo->opr->str)){
   case '+':case '-':case ':':
      combine_objects(objo,strs);
      delstrs(objo->opr);
      objo->opr = NULL;
      free(clrobjt(objo->lftobj));
      objo->lftobj = NULL;
      if(trace_level==3) nams_prnt(trace); 
      break;
   case '@':
      *strs = objo->lftobj->cht[0]->prevstr;
      left_replicate(objo,strs,opts);
      unmixobj(strs,opts);
      objo->opr = NULL;
      free(clrobjt(objo->lftobj));
      free(clrobjt(objo->rgtobj));
      objo->lftobj = objo->rgtobj = NULL;
      if(trace_level==3) nams_prnt(trace); 
      break;
   case '#':
          attach_objects(objo,strs);
   default:;
   }
}

void
attach_objects(objo,strs)
OBJO *objo;
STRS **strs;
{
int pos = 0, siz = 0;
siz = checksizes(objo->lftobj,objo->rgtobj);
if(siz<1){
   err_prnt("restructuring objects into type (1;1) or less");
   simplify_args(objo,strs);
   }
direct_sum(objo,strs);
}

void
simplify_args(objo,strs)
OBJO *objo;
STRS **strs;
{
OBJT *objs[2];
int runobj;
objs[0] =  objo->lftobj, objs[1] = objo->rgtobj;
for(objs[0]->pos=objs[1]->pos=0;
    objs[0]->pos<=objs[0]->siz&&objs[1]->pos<=objs[1]->siz;objs[0]->pos++,objs[1]->pos++)
      if(issep(*objs[0]->cht[objs[0]->pos]->str)&&issep(*objs[1]->cht[objs[1]->pos]->str))
         if(objs[0]->siz>objs[1]->siz)
            *objs[1]->cht[objs[1]->pos]->str=*objs[0]->cht[objs[0]->pos]->str;
         else
            *objs[0]->cht[objs[0]->pos]->str=*objs[1]->cht[objs[1]->pos]->str;
}


void
direct_sum(objo,strs)
OBJO *objo;
STRS **strs;
{
int *int1ptr(), *clrstr();
int basesize, position;
STRS *tmps, *mkstrs(), *cpyallstrs(), *padstrs(), *zero_at_depth(),
     *copy = (STRS*) NULL, *catch = (STRS*) NULL;
*strs = objo->lftobj->cht[0];
catch = zero_at_depth(objo->lftobj,1);
catch->str = int1ptr(2);
*catch->str = objo->lftobj->typ[0] ?
                 (objo->lftobj->pmt[0][0] ? 14 : 11):
                 (objo->lftobj->pmt[1][0] ? 11 : 14);
tmps = zero_at_depth(objo->rgtobj,1);
tmps->str = int1ptr(2);
*tmps->str = objo->rgtobj->typ[0] ?
                (objo->rgtobj->pmt[0][0] ? 14 : 11):
                (objo->rgtobj->pmt[1][0] ? 11 : 14);
copy = tmps->nextstr;
tmps->nextstr = (STRS*) NULL;
copy->prevstr = (STRS*) NULL;
padstrs(copy,tmps);
basesize = objo->lftobj->typ[0] ?
              *objo->lftobj->dim[0] + 1 :
              *objo->lftobj->dim[1] + 1;
if(objo->lftobj->typ[0]&&objo->lftobj->typ[1]&&
   objo->rgtobj->typ[0]&&objo->rgtobj->typ[1]){
   for(position=0;position<objo->lftobj->siz;position+=basesize){
      if(position+basesize < objo->lftobj->siz){
         left_implant(cpyallstrs(catch),objo->lftobj->cht[position+basesize]);
         implant(cpyallstrs(copy),objo->rgtobj->cht[position]);
         }
      else{
         left_implant(catch,objo->lftobj->cht[position+basesize]);
         implant(copy,objo->rgtobj->cht[position]);
	 }
      }
   *objo->opr->str = objo->rgtobj->pmt[0][0] ? 11 : 14;
   }
else *objo->opr->str = objo->lftobj->typ[0] ?
                          (objo->lftobj->pmt[0][0] ? 14 : 11):
                          (objo->lftobj->pmt[1][0] ? 11 : 14);
delstrs(objo->opr->prevstr);
delstrs(objo->opr->nextstr);
}

STRS *zero_at_depth(objt,depth)
OBJT *objt;
int depth;
{
STRS *mkstrs(), *newstrs = (STRS*) NULL;
int **index = objt->idx, **indicate();
newstrs = mkstrs(1);
objt->pos = 0;
index = indicate(objt);
idx_refer(objt,&newstrs,depth);
while(newstrs->prevstr) newstrs = newstrs->prevstr;
zero_args(newstrs);
return newstrs;
}

void
zero_args(catch)
STRS *catch;
{
int *cstois(), *clrstr();
while(catch->prevstr) catch = catch->prevstr;
while(catch->nextstr){
   catch = catch->nextstr;
   if(catch->str&&!issep(*catch->str)){
      free(clrstr(catch->str));
      catch->str = cstois(ZERO);
      }
   }
}

STRS *cpyallstrs(strs)
STRS *strs;
{
STRS *tmps, *mkstrs(), *copy, *tmpc, *insnextstrs();
tmps = strs;
tmpc = copy = mkstrs(1);
cpystrsval(tmps,tmpc);
while(tmps->nextstr){
   tmps = tmps->nextstr;
   tmpc = insnextstrs(tmpc);
   cpystrsval(tmps,tmpc);
   }
return copy;
}

void
idx_refer(objt,strs,depth)
OBJT *objt;
int depth;
STRS **strs;
{
STRS *tmps, *tail, *insnextstrs(), *ele_refer();
int runi, rund, *int1ptr(), *rgtpad();
int stop = 0, runidx, runtyp, runnum;
if(depth){
   for(runidx=0;runidx<objt->typ[0]+objt->typ[1];runidx++){
      for(runtyp=0;runtyp<2;runtyp++){
         for(runnum=0;runnum<objt->typ[runtyp];runnum++){
            if(objt->pmt[runtyp][runnum]==depth - 1){
               stop = 1;
               break;
               }
            }
         if(stop) break;
         }
      if(stop) break;
      }
   for(runi=0;
      runi<=objt->dim[runtyp][runnum];
      runi++){
      objt->idx[runtyp][runnum] = runi;
      idx_refer(objt,strs,depth-1);
      if(depth==1){
         if(objt->idx[runtyp][runnum]==runi){
            objt->pos = locate(objt);
            tail = tmps = ele_refer(objt);
            while(tail->nextstr) tail = tail->nextstr;
            implant(tmps,*strs);
            *strs = tail;
            if(runi!=objt->dim[runtyp][runnum]){
               *strs = insnextstrs(*strs);
               (*strs)->str = int1ptr(2);
               (*strs)->str = rgtpad((*strs)->str,runtyp?14:11);
               }
            }
         }
      else if(runi!=objt->dim[runtyp][runnum]){
         *strs = insnextstrs(*strs);
         (*strs)->str = int1ptr(2);
         rund = objt->pmt[runtyp][runnum]/2;
         do{(*strs)->str = rgtpad((*strs)->str,runtyp?14:11);
            }while(rund-->0);
         }
      }
   }
}

void
left_replicate(objo,strs,opts)
OBJO *objo;
STRS **strs;
int *opts;
{
int direct_prod = 17, regular_prod = 3, pos;
STRS *insprevstrs(), *cpystrs(), *objt_replicate(),
         *isolate(), *remstrs(), *tmps, *begn, *news, *padstrs();
if(*objo->opr->str == direct_prod) *objo->opr->str = regular_prod;
news = NULL;
for(pos=0;pos<objo->lftobj->siz;pos++){
   tmps = cpystrs(objo->opr);
   implant(tmps,isolate(objo->lftobj->cht,pos));
   implant(objt_replicate(objo->rgtobj),tmps);
   news = news ? padstrs(news,remstrs(&objo->lftobj->cht[pos])) :
                 remstrs(&objo->lftobj->cht[pos]);
   while(tmps->prevstr) tmps = tmps->prevstr;
   begn = insprevstrs(tmps);
   object(&tmps,opts);
   if(tmps==begn) tmps=tmps->nextstr, delstrs(tmps->prevstr);
   else delstrs(begn);
   news = padstrs(news,tmps);
   }
tmps = news;
while(news->str){
   if(issep(*news->str)&&news->prevstr&&news->nextstr&&
      news->prevstr->str&&news->nextstr->str&&
          islft(*news->nextstr->str)&&isrgt(*news->prevstr->str)){
          if(iscol(*news->str)&&istrln(news->str)==
                 objo->lftobj->typ[0]+objo->rgtobj->typ[0]){
                 delstrs(news->nextstr);
                 delstrs(news->prevstr);
                 }
          if(isrow(*news->str)&&istrln(news->str)==
                 objo->lftobj->typ[1]+objo->rgtobj->typ[1]){
                 delstrs(news->nextstr);
                 delstrs(news->prevstr);
                 }
          }
   if(news->prevstr) news = news->prevstr;
   else break;
   }
delstrs(tmps);
left_implant(news,*strs);
do{if(objo->opr->nextstr) delstrs(objo->opr->nextstr);}
while(objo->opr->nextstr&&objo->opr->nextstr!=
          objo->rgtobj->cht[objo->rgtobj->siz+1]);
delstrs(objo->opr);
}


void
unmixobj(strs,opts)
STRS **strs;
int *opts;
{
while((*strs)->prevstr) *strs = (*strs)->prevstr;
while((*strs)->nextstr){
   if((*strs)->nextstr->str&&ismrk(*(*strs)->nextstr->str)&&
      (*strs)->nextstr->nextstr&&(*strs)->nextstr->nextstr->str&&
      ismrk(*(*strs)->nextstr->nextstr->str)){
      if((isclx(*(*strs)->nextstr->str)&&
         isobj(*(*strs)->nextstr->nextstr->str))||
         (isobj(*(*strs)->nextstr->str)&&
         isclx(*(*strs)->nextstr->nextstr->str)))
            *strs = (*strs)->nextstr;
      else mixed(&(*strs)->nextstr,*opts);
      }
   else if((*strs)->nextstr) *strs = (*strs)->nextstr;
   }
while((*strs)->prevstr)  *strs = (*strs)->prevstr;
}


STRS *remstrs(strs)
STRS **strs;
{
STRS *cutout, *next, *prev;
if(!*strs) return NULL;
cutout = *strs;
next = (*strs)->nextstr;
prev = (*strs)->prevstr;
if(prev) prev->nextstr = next;
if(next) next->prevstr = prev;
cutout->nextstr = NULL;
cutout->prevstr = NULL;
return cutout;
}

STRS *objt_replicate(objt)
OBJT *objt;
{
STRS *strs, *tmps, *objs, *cpystrs();
objs = *objt->cht;
strs = tmps = cpystrs(objs);
do{
   if(objs->nextstr){
      tmps->nextstr = cpystrs(objs->nextstr);
      tmps->nextstr->prevstr = tmps;
      tmps = tmps->nextstr;
      objs = objs->nextstr;
      }
   else break;
   }while(!isrgt(*objs->str));
return strs;
}

void
combine_objects(objo,strs)
OBJO *objo;
STRS **strs;
{
STRS *isolate();
int pos = 0, siz = 0;
if((siz = checksizes(objo->lftobj,objo->rgtobj))>0){
   rightaction(objo,strs);
   while(pos<siz){
      objo->lftobj->pos = pos;
      implant(isolate(objo->lftobj->cht,pos),objo->rgtobj->cht[pos]);
      delstrs(objo->lftobj->cht[pos++]);
      }
   delstrs(objo->lftobj->cht[pos]);
   }
else fprintf(stderr,"cannot '%c'\n",isopr(*objo->opr->str));
}

int checksizes(obj1,obj2)
OBJT *obj1, *obj2;
{
int runtype, runnum;
if(obj1->siz!=obj2->siz){
   err_prnt("object sizes unequal");
   return 0;
   }
for(runtype=0;runtype<2;runtype++){
   if(obj1->typ[runtype]!=obj2->typ[runtype]){
      err_prnt("object types dissimilar");
      return -1;
      }
   for(runnum=0;runnum<obj1->typ[runtype];runnum++){
      if(obj1->dim[runtype][runnum]!=obj2->dim[runtype][runnum]){
         err_prnt("dimensions of object coordinates different");
         return -2;
         }
      if(obj1->pmt[runtype][runnum]!=obj2->pmt[runtype][runnum]){
         err_prnt("permutations of object coordinates differ");
         return -3;
         }
      }
   }
return obj1->siz;
}

void
rightaction(objo,strs)
OBJO *objo;
STRS **strs;
{
STRS *cpystrs();
switch(isopr(*objo->opr->str)){
   case '+':case '-':case ':':
      objo->rgtobj->pos = 0;
      while(objo->rgtobj->pos<objo->rgtobj->siz)
         implant(cpystrs(objo->opr),objo->rgtobj->cht[objo->rgtobj->pos++]);
      if(trace_level==3) nams_prnt(trace); 
      break;
   case '*':
      objo->rgtobj->pos = 0;
      while(objo->rgtobj->pos<objo->rgtobj->siz){
         implant(cpystrs(objo->opr),objo->rgtobj->cht[objo->rgtobj->pos]);
         if(objo->opr->prevstr&&isarg(objo->opr->prevstr))
            implant(cpystrs(objo->opr->prevstr),
                    objo->rgtobj->cht[objo->rgtobj->pos]);
         objo->rgtobj->pos++;
         }
      if(objo->opr->prevstr&&isarg(objo->opr->prevstr))
         delstrs(objo->opr->prevstr);
         if(objo->opr->prevstr&&objo->opr->prevstr->str&&
            isopr(*objo->opr->prevstr->str)) *strs = objo->opr->prevstr;
      if(trace_level==3) nams_prnt(trace); 
      break;
   case '=':
      if(isnam(objo->opr->prevstr)){
         objo->opr->prevstr->form = objo->opr->nextstr->form;
         cpynams(objo->opr->prevstr->str,objo->opr->nextstr);
         delstrs(objo->opr->prevstr);
         }
      break;
default:;
   }
}


OBJT *setobj(strs)
STRS **strs;
{
int **symbolic();
NAMS *onam = NULL, *findnam();
STRS *base = (STRS *) NULL, *runstr,
     *name, *insnextstrs();
OBJT *classify(), *objt, *reference();
name = (STRS*) NULL;
if((*strs)->prevstr&&(*strs)->prevstr->str&&isnam((*strs)->prevstr)){
   name = (*strs)->prevstr;
   onam = findnam((*strs)->prevstr);
   if(onam) base = onam->strs;
   if(base&&!islft(*base->str)){
      err_prnt("error: attempting to index a non-object");
      return (OBJT *) NULL;
      }
   }
if(!base) base = *strs;
objt = classify(base);
if(onam&&objt->siz!=1){
   if(!setmskidx(objt,*strs)) return (OBJT*) NULL;
   maskcopy(objt);
   objt->pos = locate(objt);
   objt->sym = symbolic(objt);
#ifdef DBG_OBJT
   sym_prnt(objt);
   obj_show(objt);
#endif
   return reference(objt,strs,name);
   }
if(!objt&&**objt->sym){
   err_prnt("object not properly marked");
   return (OBJT *) NULL;
   }
return objt;
}


OBJT *reference(objt,strs,name)
OBJT *objt;
STRS **strs, *name;
{
STRS *tmps = (STRS*) NULL, *ele_refer();
OBJT *clrobjt();
if(!objt->sym){
   *strs = name;
   delstrs(name->nextstr);
   delstrs(name->nextstr);
   tmps = ele_refer(objt);
   implant(tmps,name);
   *strs = tmps;
   delstrs(name);
   free(clrobjt(objt));
   return (OBJT*) NULL;
   }
else{
   if(objt->typ[0]+objt->typ[1]<2){
      *strs = name;
      delstrs(name->nextstr);
      delstrs(name->nextstr);
      sym_refer(objt,strs,**objt->sym);
      free(clrobjt(objt));
      objt = (OBJT*) NULL;
      }
   else sym_refer(objt,strs,**objt->sym);
   delstrs(name);
   return objt;
   }
}

void
sym_refer(objt,strs,depth)
OBJT *objt;
int depth;
STRS **strs;
{
STRS *tmps, *tail, *insnextstrs(), *ele_refer();
int runi, rund, *int1ptr(), *rgtpad(), contract = objt->sym[0][1];
if(depth){
   for(runi=0;
      runi<=objt->dim[objt->sym[depth][1]][objt->sym[depth][2]];
      runi++){
      objt->idx[objt->sym[depth][1]][objt->sym[depth][2]] = runi;
      sym_refer(objt,strs,depth-1);
      if(depth==1){
         if(!contract||(contract&&
            objt->idx[objt->sym[2][1]][objt->sym[2][2]]==runi)){
            objt->pos = locate(objt);
            tail = tmps = ele_refer(objt);
            while(tail->nextstr) tail = tail->nextstr;
            implant(tmps,*strs);
            *strs = tail;
            if(runi!=objt->dim[objt->sym[depth][1]][objt->sym[depth][2]]){
               *strs = insnextstrs(*strs);
               (*strs)->str = int1ptr(1);
               (*strs)->str = contract ? rgtpad((*strs)->str,1) :
                              rgtpad((*strs)->str,objt->sym[depth][1]?14:11);
               }
            }
         }
      else if(runi!=objt->dim[objt->sym[depth][1]][objt->sym[depth][2]]){
         *strs = insnextstrs(*strs);
         (*strs)->str = int1ptr(1);
         if(contract&&depth==2) (*strs)->str = rgtpad((*strs)->str,1);
         else{
            rund = objt->sym[depth][2] - contract;
            do{(*strs)->str = rgtpad((*strs)->str,objt->sym[depth][1]?14:11);
               }while(rund-->0);
            }
         }
      }
   }
}


STRS *ele_refer(objt)
OBJT *objt;
{
STRS *tmps, *cpystrs(), *copy, *tmpc, *insnextstrs();
tmps = objt->cht[objt->pos];
copy = tmpc = (STRS*) NULL;
while(tmps->nextstr!=objt->cht[(objt->pos+1)%(objt->siz)]){
   if(!tmps->nextstr||isrgt(*tmps->nextstr->str)) break;
   tmps = tmps->nextstr;
   if(!tmpc) copy = tmpc = cpystrs(tmps);
   else{
      tmpc = insnextstrs(tmpc);
      cpystrsval(tmps,tmpc);
      }
   }
return copy;
}

int **symbolic(objt)
OBJT *objt;
{
int type, runnum, **symbols, *int1ptr(), **int2ptr(),
    numsym = 0, contract = 0, runeye, pmt[2], unused = 0;
symbols = (int **) NULL;
if(objt->msk){
   for(type=0;type<2;type++)
      for(runnum=0;runnum<objt->typ[type];runnum++)
         if(!adigit(*objt->msk[type][runnum]->str)) numsym++;
   for(runnum=0;runnum<objt->typ[0];runnum++)
      for(runeye=0;runeye<objt->typ[1];runeye++)
         if(!adigit(*objt->msk[0][runnum]->str)&&
            !istrcmp(objt->msk[0][runnum]->str,objt->msk[1][runeye]->str)){
            contract++;
            if(contract==1){
               if(objt->dim[0][runnum]!=objt->dim[1][runeye]){
                  err_prnt("cannot contract indices of unequal dimension");
                  return (int**) NULL;
                  }
               pmt[0] = objt->pmt[0][runnum];
	       pmt[1] = objt->pmt[1][runeye];
               }
            }
   if(contract>1) contract = 1;
   }
if(numsym){
   symbols = int2ptr(numsym+1);
   symbols[0] = int1ptr(3);
   **symbols = numsym;
   symbols[0][1] = contract;
   symbols[0][2] = unused; /* This symbol attribute field is unused */
   numsym = 0;
   for(type=0;type<2;type++)
      for(runnum=0;runnum<objt->typ[type];runnum++)
         if(!adigit(*objt->msk[type][runnum]->str)){
            symbols[++numsym] = int1ptr(3);
            symbols[numsym][0] = objt->pmt[type][runnum];
            symbols[numsym][1] = type;
            symbols[numsym][2] = runnum;
            }
   if(**symbols>1){
      int sorted, **runsym, *tmpsym;
      do{runsym = &symbols[1];
         sorted = 1;
         for(runnum=1;runnum<**symbols;runnum++,runsym++)
            if(**runsym>*runsym[1]){
               tmpsym = *runsym;
               *runsym = runsym[1];
               runsym[1] = tmpsym;
               sorted = 0;
               }
         }while(!sorted);
      if(contract==1)
         for(type=0;type<2;type++){
            tmpsym = symbols[pmt[type]+1];
            symbols[pmt[type]+1] = symbols[type+1];
            symbols[type+1] = tmpsym;
            }
      }
   }
return symbols;
}

void
maskcopy(objt)
OBJT *objt;
{
int type, runtyp, *int1ptr(), **int2ptr(), **index = objt->idx;
if(objt->msk)
   for(type=0;type<2;type++){
      if(objt->typ[type]) index[type] = int1ptr(objt->typ[type]);
      for(runtyp=0;runtyp<objt->typ[type];runtyp++)
         if(objt->msk[type][runtyp]&&objt->msk[type][runtyp]->str){
            if(adigit(*objt->msk[type][runtyp]->str))
               index[type][runtyp] = stor10i(objt->msk[type][runtyp]->str);
            else index[type][runtyp] = 0;
            }
      }
}

#ifdef DBG_OBJT
obj_show(objt)
OBJT *objt;
{
int runtype, runnum, choice = 1;
printf("\n");
if(!objt){
   err_prnt("no object available");
   return;
   }
while(choice){
   switch(choice){
      case 1: list_choices();
              choice = get_choice();
              break;
      case 2: printf("chart ");
              for(runnum=0;runnum<=objt->siz;runnum++)
                 str_prnt(objt->cht[runnum]);
              printf("\n");
              choice = get_choice();
              break;
      case 3: printf("type (%d,%d)\n",objt->typ[0],objt->typ[1]);
              choice = get_choice();
              break;
      case 4: dim_prnt(objt);
              choice = get_choice();
              break;
      case 5: pmt_prnt(objt);
              choice = get_choice();
              break;
      case 6: msk_prnt(objt);
              choice = get_choice();
              break;
      case 7: idx_prnt(objt);
              choice = get_choice();
              break;
      case 8: printf("size %d\n",objt->siz);
              choice = get_choice();
              break;
      case 9: printf("position %d\n",objt->pos);
              choice = get_choice();
              break;
      case 10: obj_prnt(objt);
               choice = get_choice();
               break;
      case 11: ele_prnt(objt);
               choice = get_choice();
               break;
      case 12: sym_prnt(objt);
               choice = get_choice();
               break;
      default: list_choices();
               choice = get_choice();
               break;
      }
   }
}

int get_choice()
{
int num = 1;
printf("show ?: ");
scanf("%d",&num);
getchar();
return num;
}

void
list_choices()
{
printf("0\t: stop showing object attributes\n");
printf("1\t: selection menu\n");
printf("2\t: chart\n");
printf("3\t: type\n");
printf("4\t: dimensions\n");
printf("5\t: permutation of indices\n");
printf("6\t: index mask\n");
printf("7\t: element index\n");
printf("8\t: size\n");
printf("9\t: element position\n");
printf("10\t: all object attributes\n");
printf("11\t: element value\n");
printf("12\t: symbolic indices\n");
}


void
obj_prnt(objt)
OBJT *objt;
{
printf("type (%d,%d)\n",objt->typ[0],objt->typ[1]);
dim_prnt(objt);
pmt_prnt(objt);
msk_prnt(objt);
idx_prnt(objt);
sym_prnt(objt);
printf("size %d\n",objt->siz);
printf("position %d\n",objt->pos);
ele_prnt(objt);
}

dim_prnt(objt)
OBJT *objt;
{
int runtype, runnum;
printf("dimension (");
for(runtype=0;runtype<2;runtype++){
   for(runnum=0;runnum<objt->typ[runtype];runnum++){
      printf("%d",objt->dim[runtype][runnum]);
      if(runnum!=objt->typ[runtype]-1) printf(",");
      }
   runtype ? printf(")\n") : printf(";");
   }
}



pmt_prnt(objt)
OBJT *objt;
{
int runtype, runnum;
printf("permutation (");
for(runtype=0;runtype<2;runtype++){
   for(runnum=0;runnum<objt->typ[runtype];runnum++){
      printf("%d",objt->pmt[runtype][runnum]);
      if(runnum!=objt->typ[runtype]-1) printf(",");
      }
   runtype ? printf(")\n") : printf(";");
   }
}

msk_prnt(objt)
OBJT *objt;
{
int runtype, runnum;
if(objt->msk){
   printf("mask (");
   for(runtype=0;runtype<2;runtype++){
      for(runnum=0;runnum<objt->typ[runtype];runnum++){
         str_prnt(objt->msk[runtype][runnum]);
         if(runnum!=objt->typ[runtype]-1) printf(",");
         }
      runtype ? printf(")\n") : printf(";");
      }
   }
}

void
sym_prnt(objt)
OBJT *objt;
{
int runsym;
if(objt->sym){
   if(!*objt->sym) printf("symbols: not yet set\n");
   else{
      printf("%d symbols; %d contraction\n",
             **objt->sym,objt->sym[0][1]);
      for(runsym=1;runsym<=**objt->sym;runsym++)
         printf("%d %d %d\n",objt->sym[runsym][0],
                objt->sym[runsym][1],objt->sym[runsym][2]);
      }
   }
else printf("symbols: none\n");
}

void
idx_prnt(objt)
OBJT *objt;
{
int runtype, runnum;
if(objt->idx){
   printf("index (");
   for(runtype=0;runtype<2;runtype++){
      for(runnum=0;runnum<objt->typ[runtype];runnum++){
         printf("%d",objt->idx[runtype][runnum]);
         if(runnum!=objt->typ[runtype]-1) printf(",");
         }
      runtype ? printf(")\n") : printf(";");
      }
   }
}

void
ele_prnt(objt)
OBJT *objt;
{
STRS *tmps, *stop = objt->cht[objt->pos+1];
tmps = objt->cht[objt->pos]->nextstr;
while(tmps){
   if(tmps->str&&issep(*tmps->str)) break;
   str_prnt(tmps);
   if(!tmps->nextstr) break;
   if(tmps->nextstr&&tmps->nextstr->str&&isrgt(*tmps->nextstr->str)) break;
   if(tmps->nextstr==stop) break;
   if(!(tmps = tmps->nextstr)) break;;
   }
printf("\n");
}
#endif


void
left_implant(insert,strs) /* Warning!! see implant warning */
STRS *insert, *strs;
{
STRS *tail;
if(!strs){
   err_prnt("left_implant into null STRS ignored\n");
   return;
   }
tail = insert;
while(tail->nextstr) tail = tail->nextstr;
if(strs->prevstr){
   strs->prevstr->nextstr = insert;
   insert->prevstr = strs->prevstr;
   }
else if(!strs->str){
   implant(insert,strs);
   return;
   }
tail->nextstr = strs;
strs->prevstr = tail;
}


void
implant(insert,strs)  /* Warning!! does not check for insert==head */
STRS *insert, *strs;
{
STRS *tail;
tail = insert;
while(tail->nextstr) tail = tail->nextstr;
if(strs->nextstr){
   strs->nextstr->prevstr = tail;
   tail->nextstr = strs->nextstr;
   }
insert->prevstr = strs;
strs->nextstr = insert;
}

STRS *isolate(cht,pos)
STRS **cht;
int pos;
{
STRS *cutout = cht[pos]->nextstr;
cutout->prevstr = NULL;
cht[pos]->nextstr = cht[pos+1];
cht[pos+1]->prevstr->nextstr = NULL;
cht[pos+1]->prevstr = cht[pos];
return cutout;
}

int
setmskidx(objt,strs)
OBJT *objt;
STRS *strs;
{
int runnum, runtyp, type, msksiz, *msktyp, *masktype();
STRS **chart(), **mskcht, **runcht, **strs2ptr(), *isolate(),
     ***mskidx = objt->msk;
mskcht = chart(strs,&msksiz);
if(!mskcht) return 0;
msktyp = masktype(mskcht,msksiz);
if(!goodidx(mskcht)){
   err_prnt("undefined operation on index element");
   return 0;
   }
if((msktyp[0]==objt->typ[0]&&msktyp[1]==objt->typ[1])){
   for(runnum=type=0;type<2;type++){
      if(msktyp[type]) mskidx[type] = strs2ptr(msktyp[type]);
      for(runtyp=0;runtyp<msktyp[type];runtyp++,runnum++){
         mskidx[type][runtyp] = isolate(mskcht,runnum);
         if(issep(*mskcht[runnum]->str)) delstrs(mskcht[runnum]);
         }
      }
   free(mskcht);
   free(msktyp);
   }
else if((msktyp[0]>0&&objt->typ[0]==0&&msktyp[1]==0&&objt->typ[1]>0)){
   mskidx[1] = strs2ptr(msktyp[0]); /* we have type (0;1) to index with [n] */
   for(runnum=runtyp=0;runtyp<msktyp[0];runtyp++,runnum++){
      mskidx[1][runtyp] = isolate(mskcht,runnum);
      if(issep(*mskcht[runnum]->str)) delstrs(mskcht[runnum]);
      }
   free(mskcht);
   free(msktyp);
   }
else{
   err_prnt("incorrect index mask for referred object");
   return 0;
   }
return 1;
}

int goodidx(mskcht)
STRS **mskcht;
{
while(!isrgt(*(*mskcht)->str))
   if(!isarg((*mskcht++)->nextstr)) return 0;
return 1;
}


int *masktype(chrt,size)
STRS **chrt;
int size;
{
int *int1ptr(), *msktyp = int1ptr(2), runnum, type;
type = runnum = msktyp[0] = msktyp[1] = 0;
if(isarg((*chrt)->nextstr)) msktyp[type] += 1;
while(runnum++<=size){
   if(iscol(*(*chrt)->str)) msktyp[type] += 1;
   if(isrow(*(*chrt)->str)){
      type++;
      if(isarg((*chrt)->nextstr)) msktyp[type] += 1;
      }
   chrt++;
   }
return msktyp;
}


NAMS *findnam(strs)
STRS *strs;
{
NAMS *conx, *con_xpr(), *oldnam();
if(conx=con_xpr(strs->str)) return conx;
return oldnam(strs->str);
}


OBJT *classify(strs)
STRS *strs;
{
OBJT *obj1ptr(), *objt = obj1ptr(1);
STRS **chart(), **runc;
int **indicate(), depth[2], runnum;
objt->cht = chart(strs,&objt->siz);
if(!objt->cht) return (OBJT *) NULL;
typify(objt->cht,objt->siz,objt->typ);
genutype(objt);
return objt;
}

void
typify(chart,count,type)
STRS **chart;
int count, *type;
{
int runnum;
runnum = type[0] = type[1] = 0;
while(runnum++<=count){
   if(iscol(*(*chart)->str)) type[0] = max(type[0],istrln((*chart)->str));
   if(isrow(*(*chart)->str)) type[1] = max(type[1],istrln((*chart)->str));
   chart++;
   }
}

int locate(objt)
OBJT *objt;
{
int  **index = objt->idx, *type = objt->typ,
     **dimen = objt->dim, **permt = objt->pmt;
static char *term[2] = {"column","row"};
int runnum, runeye, count = type[0]+type[1], posval = 1, position = 0, runtype;
for(runtype=0;runtype<2;runtype++)
   for(runnum=0;runnum<type[runtype];runnum++){
      if(index[runtype][runnum]>dimen[runtype][runnum])
         fprintf(stderr,"%s ",term[runtype]),
         err_prnt("index out of bounds on high side");
      if(index[runtype][runnum]<0)
         fprintf(stderr,"%s ",term[runtype]),
         err_prnt("index out of bounds on low side");
      }
for(runeye=0;runeye<=count;runeye++)
   for(runtype=0;runtype<2;runtype++)
      for(runnum=0;runnum<type[runtype];runnum++)
         if(permt[runtype][runnum]==runeye){
            position += posval * index[runtype][runnum];
            posval *= (dimen[runtype][runnum] + 1);
            }
return position;
}

int **indicate(objt)
OBJT *objt;
{
int position = objt->pos, *type = objt->typ, **dimen = objt->dim,
    **permt = objt->pmt, runnum, runeye, count = type[0]+type[1],
    maxpos = 1, runtype, *int1ptr(), **index = objt->idx;
for(runtype=0;runtype<2;runtype++){
   if(type[runtype]&&!index[runtype])
      index[runtype] = int1ptr(type[runtype]);
   if(type[runtype]&&!permt[runtype])
      permt[runtype] = int1ptr(type[runtype]);
   for(runnum=0;runnum<type[runtype];runnum++)
      maxpos *= (dimen[runtype][runnum] + 1);
   }
if(position>maxpos){
   err_prnt("position out of bounds on high side");
   return (int **) NULL;
   }
else if(position<0){
   err_prnt("position out of bounds on low side");
   return (int **) NULL;
   }
for(runtype=0;runtype<2;runtype++)
   for(runnum=0;runnum<=count;runnum++)
      for(runeye=0;runeye<type[runtype];runeye++)
         if(permt[runtype][runeye]==runnum){
            index[runtype][runeye] = position%(dimen[runtype][runeye]+1);
            position /= (dimen[runtype][runeye] + 1);
            }
return index;
}

void
genutype(objt)
OBJT *objt;
{
STRS **chart = objt->cht;
int count = objt->siz, *type = objt->typ;
int **int2ptr(), *int1ptr(), runtype,
    maxcol, maxrow, colson, rowson, runnum = 0, *runcol,
    *runrow, *colseq, *rowseq, length, depth[2], seqnum = 0;
for(runtype=0;runtype<2;runtype++)
   if(type[runtype]){
      objt->dim[runtype] = int1ptr(type[runtype]);
      objt->pmt[runtype] = int1ptr(type[runtype]);
      }
maxrow = maxcol = depth[0] = depth[1] = 0;
runcol = objt->dim[0] - 1, runrow = objt->dim[1] - 1;
colseq = objt->pmt[0] - 1, rowseq = objt->pmt[1] - 1;
while(runnum++<=count){
   if(iscol(*(*chart)->str)){
      length = istrln((*chart)->str);
      if(length>depth[0]){
         depth[0] = length;
         colson = 1, rowson = 0;
         *++runcol = 1;
         *++colseq = seqnum++;
         if(++maxcol!=depth[0]){
            err_prnt("a column separator is out of order ");
            err_prnt("indexing may not work correctly");
            }
         }
      else if(length==depth[0]&&colson)  *runcol += 1;
      }
   if(isrow(*(*chart)->str)){
      length = istrln((*chart)->str);
      if(length>depth[1]){
         depth[1] = length;
         rowson = 1, colson = 0;
         *++runrow = 1;
         *++rowseq = seqnum++;
         if(depth[1]!=++maxrow){
            err_prnt("a row separator is out of order ");
            err_prnt("indexing may not work correctly.");
            }
         }
      else if(length==depth[1]&&rowson) *runrow += 1;
      }
   chart++;
   }
}

STRS **chart(strs,count)
STRS *strs;
int *count;
{
STRS *runs = strs, **chrt = 0, **strs2ptr(), **runc;
*count = 0;
while(runs){
   if(runs->str&&issep(*runs->str)) *count += 1;
   if(runs->str&&isrgt(*runs->str)){
      *count += 1;
      chrt = strs2ptr(*count+1);
      break;
      }
   runs = runs->nextstr;
   }
if(!chrt){
      err_prnt("Could not set mask index chart\n");
      return  (STRS**) NULL;
      }
runs = strs, runc = chrt;
*runc++ = strs;
while(runs){
   if(runs->str&&issep(*runs->str)) *runc++ = runs;
   if(runs->str&&isrgt(*runs->str)){
      *runc = runs;
      break;
      }
   runs = runs->nextstr;
   }
return (!runs) ? (STRS **) NULL : chrt;
}

OBJT *clrobjt(objt)
OBJT *objt;
{
STRS *clrstrs();
int type, runt, **clrsym();
if(objt->cht) free(objt->cht);
objt->cht = 0;
for(type=0;type<2;type++){
   if(objt->msk)
      for(runt=0;runt<objt->typ[type];runt++)
         if(objt->msk[type]&&objt->msk[type][runt])
            free(clrstrs(objt->msk[type][runt]));
   if(objt->dim[type]) free(objt->dim[type]);
   if(objt->pmt[type]) free(objt->pmt[type]);
   if(objt->idx[type]) free(objt->idx[type]);
   if(objt->msk[type]) free(objt->msk[type]);
   }
if(objt->sym) free(clrsym(objt->sym)), objt->sym = (int**) NULL;
objt->typ[0] = objt->typ[1] = objt->siz = objt->pos = 0;
return objt;
}

int **clrsym(symbol)
int **symbol;
{
int run;
while(**symbol){
   for(run=0;run<3;run++) symbol[**symbol][run] = 0;
   free(symbol[**symbol]);
   **symbol -= 1;
   }
symbol[**symbol][0] = symbol[**symbol][1] = 0;
free(symbol[**symbol]);
return symbol;
}

OBJT *obj1ptr(size)
int size;
{
OBJT *ptr;
ptr = (OBJT *) calloc(size,sizeof(OBJT));
if(!ptr){
   err_prnt("insufficient storage space for object structure");
   exit(1);
   }
return ptr;
}

OBJO *mkobjo(nmem)
int nmem;
{
OBJO *tmpoo, *objo1ptr();
tmpoo = objo1ptr(nmem);
tmpoo->opr = (STRS*) NULL;
tmpoo->lftobj = (OBJT*) NULL;
tmpoo->rgtobj = (OBJT*) NULL;
return tmpoo;
}


OBJO *objo1ptr(size)
int size;
{
OBJO *ptr;
ptr = (OBJO *) calloc(size,sizeof(OBJO));
if(!ptr){
   err_prnt("insufficient storage space for object operator");
   exit(1);
   }
return ptr;
}

