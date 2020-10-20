# apax
Named: Arbitrary Precision Arithmetic Expression, this `apax` repository 
comprises a makefile, c-modules with header files.
It makes the exectable `apa` and linkable `libapa.a` archive of relocatable object files.
It is implemented in original K&R C, where `int` was default for undeclared types.

Int was also the default for undeclared return values and for logical values (zero==FALSE).
Finally pointers were exchangable with ints, as was NULL and calloc's offsets

Changes required for compile without warnings include using <stdlib.h>  (in defn.h) for calloc's function prototype;

Using zero (0) in place of `NULL` where `int` type was needed, and returning `int` explicitly

It declares a maximal precision as 2^num_of_word_bits minus one, minus ASCII '0' (48). Values below ASCII '0' are used for housekeeping.

So this is not the 1989 vintage source because its been changed as above to let gcc [version 8.3.1 20191121 (Red Hat 8.3.1-5)] compile successfully without warnings. The original `makefile` was identical to `apa.mak` and it has been changed for the gcc archive creation rule and `ar` action.

APA(X) is a command line utility with a rudimentary text helper triggered by the '?'`<return>` character input

For those that also want to actually read the code, it may be useful to know it follows C and UNIX customs as in:

(a) Using `int` strings in the same fashioun as C employs `char` strings (stdio.h); and 

(b) Functions and Procedures always returning `int`, be they return status, a return count, a boolean result or a function integral return value

(c) The extended arithmetic register is implemented as a doubly linked list of integers

(d) The linked list is adapted to arithmetic expression stack tracers, variable names and historical values previously evaluated during a session.

(e) All register scans are unsorted linear search as only human input interaction was initially anticipated

(f) File and String I/O is anticipated and supported via `libapa.a` API

(g) Possible enhancement would be passing the integer string registers through a GPU for the actual arithmetic logic to support big computation

The utility supports any user chosen radix or precision, and also supports zero remainder division of rational numbers (`exact arithmetic`) A favourite demonstration is spitting out the Taylor Series value of the Transcendental `e` to the user's desired precision using the repeated fractions input of ones.
To support arbitrary radix and repeated fractions: `The place values are always printed right-justified and zero-padded to the space required for the maximum register value.` for any given radix or precision that has been selected. It supports less than a handful of MATHS functions which are amenable to ARITHMETIC (power, square root), and does a bit of Collections housekeeping, following TENSOR analysis semantics (enclosing square brackets, with commas delimiting columns and semicolons delimiting rows - With EBCDIC, curly braces replace square brackets) and does Contraction as per Einstein's notation.
