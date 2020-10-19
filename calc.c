static char SccsId[] = "@(#) calc.c 1.5 93/02/12 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */


#include <defn.h>
#include <calc.h>
#include <apax.h>
#include <help.h>
#include <util.h>


extern NAMS *nams, *hist, *trace;
extern FILE *infile, *outfile;
extern STRG *instrg;
extern int *outstr;
extern int sglprc, dblprc;

main(narg,argv)
int narg;
char *argv[];
{
NAMS *mknams();
STRS *strs, *scan();
int opts[2], count = 0;
trace = mknams(1);
nams = mknams(1);
hist = mknams(1);
check_files(narg,argv);
if(!infile) intro_prnt();
precision(), opts[0] = 0, opts[1] = 2, default_prnt();
if(!infile) printf("(");
while(1){
   strs = scan(opts);
   if(!strs) continue;
   if(!strs->str) break;
   if(infile) outfile ? fprintf(outfile,"\n") : printf("\n");
   strs_prnt(strs);
   while(strs->prevstr&&strs->prevstr->str) strs = strs->prevstr;
   history(strs,++count);
   }
gbye_prnt();
return 1;
}

void
check_files(narg,argv)
int narg;
char **argv;
{
int *tmpc;
if(narg>=2&&strcmp(argv[1],"stdin")&&!(infile=fopen(argv[1],"r")))
   fprintf(stderr,"File name \"%s\" not found. Use keyboard inputs.\n",argv[1]);
if(narg>=3){
   if(outfile=fopen(argv[2],"r")){
      fprintf(stderr,"File name \"%s\" already exists.\n",argv[2]);
      printf("enter the letter 'o' to overwrite file \"%s\",\n",argv[2]);
      printf("      the letter 'a' to append to file \"%s\"\n",argv[2]);
      printf("      any other letter to exit this program: ");
      fclose(outfile);
      if(infile) kbd_strent(&tmpc);
      else {
         fprintf(stderr,"Remove or rename %s to rerun; Bye!\n",argv[2]);
         exit(1);
         }
      switch(*tmpc){
         case 'o': outfile = fopen(argv[2],"w");break;
         case 'a': outfile = fopen(argv[2],"a");break;
         default: exit(2);
         }
      free(tmpc);
      }
   else outfile = fopen(argv[2],"w");
   }
if(narg>3){
   int *tmpc;
   fprintf(stderr,"More than two command-line arguments provided.\n");
   fprintf(stderr,"This program is invoked with the following general command line:\n");
   fprintf(stderr,"\t%s  [ input_filename ]  [ output_filename ] ]\n",argv[0]);
   fprintf(stderr,"(To specify an output file and no input file use input_filename = ");
   fprintf(stderr,"\"stdin\").\nIgnore argument \"%s\" onwards, and continue ? (y/n): ",
            argv[3]);
   strent(&tmpc);
   switch(*tmpc){
      case 'y': break;
      default: exit(3);
      }
   free(tmpc);
   }
}

