static char SccsId[] = "@(#) help.c 1.11 93/02/17 (c)MTK";
/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <defn.h>
#include <help.h>
#include <apax.h>
#include <fixr.h>

static char *rules[4] = {"LR","MA","DMA"};
extern int tag, trace_level;

void
default_prnt()
{
extern FILE *infile, *outfile;
extern int maxint;
outfile ?
   fprintf(outfile,"apa(x) maximum precision (RAM permitting:) = %d digits.\n",
   maxint-'0'),
   fprintf(outfile,"default precision = 0(variable), precedence = 2(DMA), "),
   fprintf(outfile,"radix = 10\nYour system uses %s. ",
          '0'==48?"ASCII":"EBCDIC") :
   printf("apa(x) maximum  (RAM permitting:) = %d digits.\n",
          maxint-'0'),
   printf("default precision = 0(variable), precedence = 2(DMA), "),
   printf("radix = 10\nYour system uses %s. ", '0'==48?"ASCII":"EBCDIC");
tag_prnt();
outfile ?
   fprintf(outfile,"Reset it with ?c if your keyboard does not have it.\n")
   : printf("Reset it with ?c if your keyboard does not have it.\n"),
     printf("Type an arithmetic expression, end it with ')' and 'Return'\n");
}

void
lang_prnt()
{
printf("Language definition:\n");
printf("Inputs in (x) are statements consisting of expression markers and ");
printf("a series\nof tokens making up an expression. Expression tokens are ");
printf("identifiers, constants\nand operators. Identifiers are alphanumeric");
printf(" strings starting with a letter\n(tags included). Constants");
printf(" are numeric strings with at most one decimal\npoint. An expression");
printf(" has at least one constant or identifier, and zero or more\noperato");
printf("rs acting on them. See ?o for  operators and ?x for their actions.\n");
printf("The answer is normally a constant expression, i.e. has no identifier");
printf("s, and is\nstored in a historical list of constant expressions. ");
printf("It can be reused with\nidentifier '%cn%c', where 'n' is its ",tag,tag);
printf("numerical position in the list. Identifiers\nof this form are always");
printf(" presumed historical so that the user-defined variable %cn%c",tag,tag);
printf(" shall be\nhidden by the variable for the n'th constant expression.\n");
}


void
actions_prnt()
{
printf("Actions:\n");
printf("APA(X) interprets algebraic statements entered from a keyboard");
printf(" or \nread from a file specified in the  command-line or created by ");
printf("an apa-calling\nprogram. The answer is sent to the monitor or a ");
printf("file if specified in the\ncommand line or into a numeric string ");
printf("accessible to the calling program.\n");
printf("Operator precedence in (x) is in the following order:\n");
printf("Begin Expression       '('    The outermost '(' is not required.\n");
printf("End Expression         ')'    Expressions may be nested.\n");
printf("Literal assignment     '!='   copies identifiers literally.\n");
printf("Function Evaluation    '!F'   where 'F' is a function identifier.\n");
printf("Variable Evaluation    'V'    where 'V' is a variable identifier.\n");
printf("Operator modification  '!O'   where 'O' is an arithmetic operator.\n");
printf("Factorial prefix       '!C'   where 'C' is a factorial constant.\n");
printf("Shift-evaluation       '^'    Shift value assigned to argument.\n");
printf("Object Expansion:      [,;]'  vectors, matrices, tensors etc.\n");
printf("Complex evaluation     ':'    see ?: for precedence.\n");
printf("Arithmetic calculation 'O'    where O is any of +,-,*,/ and %,\n");
printf("                              in some set precedence (see ?a).\n");
printf("Factorial postfix      'C!'   where 'C' is a constant.\n");
printf("Value assignment       '=',   copies evaluation result.\n");
printf("A factorial arithmetic operation will convert decimal operands ");
printf("into factorial.\n");
}


void
radx_prnt()
{
extern FILE *infile, *outfile;
extern int radix, maxint;
printf("Radix:\n");
printf("The basis of the fixed-radix number system i.e. other than the ");
printf("factorial \nformat may be changed from the decimal default value ");
printf("of 10. Valid values range \nfrom 2 to %d.",maxint-'0'-1);
printf(" A number is written as a string of radix registers, each\nvalue is");
printf(" right-justified and zero-padded in the space required to hold the\n");
printf("value radix-1.\nWARNING: Inputs must be radix 10. The apparent prec");
printf("ision increases roughly \nwith the log of the radix but the computa");
printf("tion speed drops more drastically,\nespecially at high radices.\n");
if(!infile) outfile ? fprintf(outfile,"current setting = %d\n",radix)
                    :  printf("current setting = %d\n",radix);
}

void
tag_prnt()
{
extern FILE *outfile;
outfile ? fprintf(outfile,"'%c' is your tag character.\n",tag)
        :  printf("'%c' is your tag character.\n",tag);
}

void
trace_prnt()
{
extern FILE *outfile;
printf("Trace Level:\n");
printf("The tracing of expression evaluations can be set to focus on");
printf(" specific \nphases of execution. Set the level to:\n");
printf("0:\tfor no continuous tracing\n");
printf("1:\tfor tracing preparser initial conditions\n");
printf("2:\tfor expansion of variables\n");
printf("3:\tfor expansion of objects\n");
printf("4:\tfor expansion of complex expressions\n");
printf("5:\tfor parsing of arithmetic expressions\n");
printf("6:\tfor parsing of arithmetic expressions by precedence.\n");
printf("7:\tfor parsing of infix operator context.\n");
outfile ? fprintf(outfile,"The trace level is %d.\n",trace_level)
        :  printf("The trace level is %d.\n",trace_level);
}


void
prec_prnt(option)
int *option;
{
extern FILE *infile, *outfile;
printf("Precision Handling:\n");
printf("Precision may be fixed at some value or may be allowed to vary.\n");
printf("If it is variable (set with value zero), the effective precision is");
printf("\ninitially determined by the argument lengths in the input ");
printf("expression.\nVariable precision may increase by one on each ");
printf("addition, up to a factor\nof two on each decimal division, and by ");
printf("as much as is needed for an exact\nanswer on each factorial ");
printf("division. No rounding is done. Fixed precision\nis maintained by ");
printf("rounding.\n");
if(!infile) outfile ? fprintf(outfile,"current setting = %d ",iabs(option[0]))
                    :  printf("current setting = %d ",iabs(option[0]));
if(!infile){
   if(!option[0]) outfile ? fprintf(outfile,"(variable)\n") :
                                     printf("(variable)\n");
   else outfile ? fprintf(outfile,"(fixed)\n") : printf("(fixed)\n");
   }
outfile ? fprintf(outfile,"\n") : printf("\n");
}

void
fctr_prnt()
{
printf("The Factorial Format:\n");
printf("The factorial register uses a  '.' to separate integer and");
printf(" fraction portions \nof a number; just as with decimal (register)");
printf(" points. The place-value of each \nfactorial register position ");
printf("equals the factorial of the position number as \ncounted from ");
printf("point position zero in the integer direction , and equals its\n");
printf("reciprocal in the negative direction, except that place value '1/1!'");
printf(" is \nexcluded to prevent duplication of whole fractional place ");
printf("values which carry \nover to the next place left. The maximum regist");
printf("er value equals the magnitude\nof its position. ");
printf("The place values are always printed right-justified and zero-\npad");
printf("ded to the space required for the maximum register value.\n   For ");
printf("input a '!' prefix converts a decimal string into factorial ");
printf("format \naccording to these rules, except that the end-register ");
printf("position value need not \nbe right-justified. A '!' postfix converts");
printf(" a factorial register into decimal up to the active number of ");
printf("significant digits if necessary.\n");
}


void
algr_prnt(option)
int *option;
{
extern FILE *infile, *outfile;
printf("Algebraic precedence rules for arithmetic operators:\n");
printf("0 :  Equal, Left to right in order of appearance (LR).\n");
printf("1 :  Multiplicatives before additives(MA), LR within each.\n");
printf("2 :  Division before multiplication before additives(DMA),");
printf(" LR within each.\n");
printf("3 :  Other combinations of zero or more of D,M,A,S, in the desired ");
printf("precedence.\n");
active_rule(option);
printf("\n");
}


void
uoprs_prnt()
{
printf("Unary operators include the following:\n");
printf("    '('   (prefix)    open expression\n");
printf("    ')'   (postfix)   close expression\n");
printf("    '['   (prefix)    begining of object marker\n");
printf("    ']'   (postfix)   end of object marker\n");
printf("    ';'   (prefix)    covariant object (column index) marker\n");
printf("    '+'   (prefix)    positive\n");
printf("    '-'   (prefix)    negative\n");
printf("    ':'   (prefix)    complex\n");
printf("    '!'   (prefix)    function or factorial register indicator\n");
printf("    '!'   (postfix)   factorial if decimal register argument\n");
printf("    '!'   (postfix)   convert to decimal if factorial register\n");
printf("    '?'    (prefix)   query\n");
}

void
boprs_prnt()
{
printf("Binary operators include the following:\n");
printf("    ','    contravariant (column) separator\n");
printf("    ';'    covariant (row) separator\n");
printf("    '+'    plus\n");
printf("    '-'    minus\n");
printf("    '*'    multipy,\n");
printf("    '/'    divide\n");
printf("    '^'    shift:  x^n = x*pow(radix,n), and is written  'xen' or\n");
printf("                  !x^n = x*pow(|n|!,sign(n)1),        as '!xfn' but\n");
printf("           left as  ^n when written to a file or character string\n");
printf("    '%%'    remainder, (tractable under fixed precision)\n");
printf("    '!%%'   modulus,   (integral reminder to integer division)\n");
printf("    '!+'   factorial plus\n");
printf("    '!-'   factorial minus\n");
printf("    '!*'   factorial multipy,\n");
printf("    '!/'   factorial divide (note: remainder is always zero)\n");
printf("    '='    assign after replacing variables with active values\n");
printf("    '!='   assign literally (before variable substitution)\n");
}



void
nopt_prnt(option)
int option;
{
printf("invalid query option '%c'\n",option);
}

void
iusage_prnt()
{
printf("Usage in the interactive calculator mode:\n");
printf("\napa %cinput_file_name %coutput_file_name%c%c\n",BEG,BEG,END,END);
printf("\nFrom the operating system this command invokes the interactive ");
printf("calculator mode.\nSpecifying an input file causes input to be read ");
printf("from it. Specifying an output\nfile causes the interpreted ");
printf("results, precision and precedence information to\nbe sent to that ");
printf("file. Other interactive outputs will still go to the monitor.\n");
printf("To specify an output file without an input file the term \"stdin\"");
printf(" must be used\nfor the input_file_name option. Apa prompts the ");
printf("user with the start-of-\nexpression indicator '('. The input 'x' ");
printf("is ended with the corresponding end-of-\nexpression indicator ')'. ");
printf("The session ends whenever this (x) is empty i.e. ().\n");
}


void
busage_prnt()
{
printf("Usage in batch mode(from a C program) :\n");
printf("Using FILE* streams:\n");
printf("1: For expressions in file stream, open a FILE pointer with fopen(),\n");
printf("   storing the return value in variable 'file_ptr'.\n");
printf("2: Format the expression with function fprintf().\n");
printf("3: Reset the file with rewind(file_ptr) and\n");
printf("   interpret the expression with function fapa(file_ptr).\n");
printf("Using character buffer STRG* streams:\n");
printf("1: Format the expression with function sprintf().\n");
printf("2: Open a 'pseudo-stream' with STRG *sopen(buffer); char* buffer;\n");
printf("   storing the return value in variable 'STRG *pstrm_ptr;'.\n");
printf("   Define STRG thus:\n\t'typedef struct string_stream{\n\t\tchar* start;");
printf("\n\t\tchar *curr;\n\t\t} STRG;'\n");
printf("   Do not alter the contents of *pstrm_ptr. It merely emulates FILE\n");
printf("   by supporting the primitives sopen(), srewind(STRG*) sclose(STRG*)\n");
printf("   and 'sgetc(STRG*)'. You need only use sopen() and sclose().\n");
printf("3. Interpret the expression with function sapa(pstrm_ptr).\n");
printf("In either case, fapa() and sapa() return a charcater string result.\n");
printf("You have to link the compiled calling program with  the library libapa.a.\n");
printf("NOTE. All apa(x) functions use the 'C'calling convention.\n");
}

void
fnct_prnt()
{
printf("Functions:\n");
printf("Three functions are available:\n");
printf("      (1) exp(x) the natural exponent\n");
printf("      (2) log(x) the natural logarithm\n");
printf("      (3) pow(x,y) the power function.\n");
printf("They are invoked with a '!' preceding the function name. The\n");
printf("function arguments may be integer or floating point. Their\n");
printf("register formats may be fixed radix or factorial. Be aware\n");
printf("that variable precision and exact arithmetic are not used.\n");
printf("The best results are obtained with fixed precision. Complex\n");
printf("arguments are not supported and produce unpredictable results.\n");
}

void
help_prnt()
{
printf("Follow the '?' with one of the following and the 'enter' key:\n");
printf("!     factorial format\n");
printf(":     complex operator\n");
printf("a     algebraic precedence of arithmetic operators\n");
printf("b     usage in batch mode\n");
printf("c     the non-alphabetical tag character\n");
printf("d     start-up defaults and machine characteristics\n");
printf("e     trace of current expression\n");
printf("E     continuous trace of current expression at a trace level\n");
printf("f     functions\n");
printf("h     history, i.e. previously interpreted expressions\n");
printf("i     usage in interactive mode\n");
printf("l     (x) language definition.\n");
printf("o     binary operators\n");
printf("p     precision setting\n");
printf("r     radix setting\n");
printf("t     trace of current token\n");
printf("u     unary operators\n");
printf("v     variable names and current values\n");
printf("x     operator actions in (x)\n");
}

void
clx_prnt()
{
printf("The Complex Operator:\n");
printf("The behaviour of the complex operator ':' is context-dependent:\n");
printf("0: It is a unary marking imaginaries    :b,\n");
printf("or a binary separating them from reals a:b;\n");
printf("Unless 1: It adds up imaginaries      :b:d = :b+d;\n");
printf("Unless 2: It reduces another unary      :: = -  or :+ = :\n");
printf("or is displaced by another unary        o: = :o, o = + or -;\n");
printf("Unless 3: It gathers imaginaries   a:boc:d = aoc:bo:d, o = + or -\n");
printf("whilst respecting other unaries  a:-boc:-d = aoc:-bo:-d etc.;\n");
printf("Unless 4: It reduces a binary         :aod = :(aod),  o = * or /\n");
printf("whilst respecting any unaries        :ao-d = :-(aod) etc;\n");
printf("Unless 5: It is displaced by a binary a*:d = :a*d\n");
printf("and also conjugated by a binary       a/:d = :-a/d;\n");
printf("Unless 6: It left-expands a binary   a:boc = aoc:boc, o = * or /,\n");
printf("whether real or imaginary           a:b*:d = a*:d:b*:d;\n");
printf("Unless 7: It right-expands a binary  a*c:d = a*c+a*:d,\n");
printf("whether real or imaginary           :b*c:d = :b*c+:b*:d.\n");
printf("or both                            a:b*c:d = a:b*c+a:b*:d;\n");
printf("Unless 8: It can invert a binary   a:b/c:d = a:b*c/|c:d|:-d/|c:d|,\n");
printf("where |c:d| is the squared modulus.\n");
printf("Its precedence is higher than that of the arithmetic operators, expa");
printf("nding them\nby the \"DMA\" rule, but reducing them by proximity afte");
printf("r expansion, regardless\nof the active precedence rule.\n");
}

void
intro_prnt()
{
extern FILE *infile, *outfile;
cpyrgt_prnt();
if(!infile) printf("\n\nuse ? for queries\n");
}

void
gbye_prnt()
{
printf("\nEnd of session.\n\n");
cpyrgt_prnt();
}

void
cpyrgt_prnt()
{
printf("    apa(x): Arbitrary Precision Arithmetic (Expression)\n\n    ");
printf("(C)Copyright  Malome T. Khomo, 1989, 1993. All rights reserved.\n");
}
