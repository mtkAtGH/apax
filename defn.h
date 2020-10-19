/* static char SccsId[] = "@(#) defn.h 1.6 92/11/09 (c)MTK"; */

/*
         APA(X): ARBITRARY PRECISION ARITHMETIC (EXPRESSION)

                (C) Copyright Malome T. Khomo 1989
                       All rights reserved.                   */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define FNCS  4
#define ZERO  "0"
#define FZERO "0.0"
#define ONE   "1"
#define POS   "\001"
#define NEG   "\002"
#define MUL   "\003"
#define DIV   "\004"
#define REM   "\005"
#define CLX   "\006"
#define SFT   "\007"
#define ASN   "\010"
#define FCT   "\011"
#define LFT   "\012"
#define COL   "\013"
#define RGT   "\014"
#define NRM   "\015"
#define ROW   "\016"
#define BEG   ('0'==48) ? '[' : '{'
#define END   ('0'==48) ? ']' : '}'

typedef struct strgs STRS;   /*  STRing Stack    */
struct strgs{
int form;         /*   format: 1=factorial, others are fixed radix (0=10) */
int *str;         /*   current string                                     */
int shift;        /*   shift: +ve value left, -ve right from point        */
STRS *nextstr;    /*   pointer to next     string                         */
STRS *prevstr;    /*   pointer to previous string                         */
};

typedef struct strjnt SJNT;          /*   String stack JoiNT    */
struct strjnt{ STRS *lft, *rgt;};    /*   left and right side   */


typedef struct strmrk SMRK;               /*   String stack MaRKer         */
struct strmrk{ STRS *mrk, *lft, *rgt;};   /*   mark, left and right side   */


typedef struct chars CHRS;   /* CHaRacter Stack */
struct chars{
int chr;                 /*   current character              */
int chrno;               /*   number of current character    */
CHRS *nextchr;           /*   pointer to next     character  */
CHRS *prevchr;           /*   pointer to previous character  */
};


typedef struct names NAMS;   /* NAMe Stack      */
struct names{
int *nam;                /*   current name              */
STRS *strs;              /*   current value             */
int namno;               /*   number of current name    */
NAMS *nextnam;           /*   pointer to next  name     */
NAMS *prevnam;           /*   pointer to previous name  */
};

typedef struct objct OBJT;    /*   type name for OBJjecT attributes  */
struct objct{
   STRS **cht;       /*  chart of object markers in linear array
                         Example:
                           object [1,2;3,4] has chart [,;,]      */

   int typ[2];       /*  Degrees of the contra- and co-variant indices
                         Occasionally, type will refer to the index of
                         this size-two array. Type 0 = columns (contra-
                         variant index separated by ',') and type 1 = rows
                         (covariant index separated with ';').
                         Examples:
                            typ (0,0) is a scalar
                            typ (1,0) is a row vector
                            typ (2,0) is a row of row vectors
                            typ (0,1) is a column vector
                            typ (0,2) is a column of column vectors
                            typ (1,1) is a matrix
                            typ (2,1) is a row of matrices
                            typ (1,2) is a column of matrices
                            typ (2,2) is a matrix of matrices.     */

   int *dim[2];      /*  coordinate dimensions. For each type dim[type] holds
                         an integer array of size (degree) = typ[type] whose
                         elements dim[type][position] are the dimension for
                         that index.
                         Examples:
                            dim (1) has index sizes [1;] and is a 2-col. vector
                            dim (;1) has index sizes [;1] and is a 2-row vector
                            dim (5;5) has index sizes [5;5] and is a 6x6 matrix
                            dim (2,4;6) is 5 columns of 7row x 3col matrices
                            dim (1;2,3) is 4 rows of 3row x 2col matrices. */

   int *pmt[2];      /*  permutation of coordinates. For each type pmt[type]
                         holds an integer array of size (degree) = typ[type]
                         whose elements pmt[type][position] are the global
                         sequence number (i.e. the order of appearance) for
                         that index.
                         Examples:
                            [1,2;3,4] has index permutations [0;1]
                            [1;2,3;4] has index permutations [1;0]
                            [1,2;3,4,,5,6;7,8] has index permutations [0,2;1]
                            [1,2,,3,4;5,6,,7,8] has permutations [0,1;2]. */

   int *idx[2];      /*  index of object element in coordinate form
                         This is an index object that specifies a subset of
                         the object being referenced. Its member values may
                         range from 0 to the maximum as defined by the
                         dim[type][position] for its elements. A symbolic value
                         means the entire range for that index element. If two
                         indices of different type use the same symbol, a
                         contraction will be made if their sizes are equal
                         The index is written as a list of comma-separated
                         contravariant indices separated by a semi-colon from
                         a comma-separated list of covariant indices. When the
                         indices are all covariant or all contravariant the
                         semi-colon may be omitted. That is, for typ (0,n) and
                         typ (n,0) objects where n>0, no ';' is needed.
                         Example: If a is an object variable.
			    a:  [5,10;15,20;;6,12;18,24]
                            a[0;0,0] gives 5
                            a[1;0,1] gives 12
                            a[1;1,1] gives 24
                            a[1;0,i] gives [10;12]
                            a[1;j,i] gives [10;20;;12;24]
                            a[k;j,i] gives [5,10;15,20;;6,12;18,24]
                         or if b is a row vector
                            b:  [1,2,3,4]
                            b[3] = b[3;] gives 4
                         or if c is a column of column vectors c:  [5;6;;7;8]
                            c[0,1] = c[;0,1] gives 7
                            c[1,i] = c[;1,i] gives [6;8]                   */

   int **sym;        /*  list of index symbols. The structure of sym
                         is as follows:
                         sym holds an array of int2ptr pointers.
                         The array size is determined by the count
                         of symbols found in the mask (msk) attribute
                         described below. Count + 1 pointers are allocated
                         The first one holds three int1ptr pointers. The
                         values given are:
                            **sym = sym[0][0] = symbol count
                            sym[0][1] = contract
                            sym[0][2] =  0 currently unused.
                         Contract and transpose are 1 if mask has index them, or
                         zero otherwise. The remaining int2ptr pointers each
                         hold descriptions of index symbols in a size three
                         int1ptr array designated as follows:
                            sym[count][0] = pmt[type][position]
                            sym[count][1] = type
                            sym[count][2] = position as in msk[type][position].
                          position is constrained by degree - typ[type].          */

   STRS **msk[2];    /*  mask of index as extracted from index object
                         After the indexed object is parsed the index portion
                         is destroyed. The raw index elements are kept in the
                         mask for use in selecting the indexed sub-object.
                         Examples:
                            index [1,2;3,4] has mask (1,2;3,4)
                            index [1;j] has mask (1;j)                      */

   int siz;          /*  total number of elements                     */
   int pos;          /*  position of object element in linear form    */
   };

typedef struct objopr OBJO;  /* typename for OBJect Operator  */
struct objopr{
   STRS *opr;         /*  object operator     */
   OBJT *lftobj;      /*  left object         */
   OBJT *rgtobj;      /*  right object        */
   };

typedef struct char_stream STRG; /* typename for FILE-like character stream */
struct char_stream{
   char *start;		/* beginning of stream */
   char *curr;		/* current character in stream */
   };
