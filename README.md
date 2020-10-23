# apax
Named: Arbitrary Precision Arithmetic Expression, this `apax` repository 
comprises a makefile, c-modules with header files.
It makes the exectable `apa` and linkable `libapa.a` archive of relocatable object files.
It is implemented in original K&R C, where `int` was default for undeclared types.

Int was also the default for undeclared return values and for logical values (zero==FALSE).
Finally pointers were exchangable with ints, as was NULL and calloc's offsets

Changes required for compile without warnings include using <stdlib.h>  (in defn.h) for calloc's function prototype;

Using zero (0) in place of `NULL` where `int` type was needed, and returning `int` explicitly

It declares a maximal precision of 2^num_of_bits_in_word minus one, minus ASCII '0' (48). Values below ASCII '0' are used for housekeeping.

So this is not the 1989 vintage source because its been changed as above to let gcc [version 8.3.1 20191121 (Red Hat 8.3.1-5)] compile successfully without warnings. The original `makefile` was identical to `apa.mak` and it has been changed for the gcc archive `libapa.a` library creation rule `ar`.

APA(X) is a command line utility `apa`, with a rudimentary text helper triggered by the '`?<return>`' character input. Its an interactive interpretter (`?l`) for arithmetic expressions (`?x`) according to a default algebraic precedence that the user may change  (`?a`). It supports 12 unary operators (`?u`) and 15 binary operators (`?o`).

The utility supports any user chosen radix  (`?r`) or precision (`?p`), and also supports zero remainder division of rational numbers using factorial format (`?!`). A favourite demonstration is spitting out the Taylor Series value of the Transcendental Number `e` to the user's desired precision using the repeated fractions input of ones.
To support arbitrary radix and repeated fraction expressions: `... place values are always printed right-justified and zero-padded to the space required for the maximum register value`, for any selected radix or precision. There are less than a handful of MATHS functions which are amenable to ARITHMETIC (power, factorial, exponent), and does a bit of coordinate collections housekeeping, following TENSOR semantics (with enclosing square brackets, commas delimiting columns and semicolons delimiting rows - With EBCDIC, curly braces replace square brackets) and does contraction of covariant and contravariant indices as per Einstein's tensor notation.

For those that also want to actually read the code, it may be useful to know it follows C and UNIX custom as in:

(a) Using `int` strings in the same fashion as C employs `char` strings (stdio.h); and 

(b) Procedures always return `int`, be they return status, a return count, a boolean result or a function with integral return value

(c) The extended precision arithmetic register is implemented in a doubly linked list of integers

(d) The linked list is adapted to expression stack tracers  (`?[eE]`), variable names  (`?v`) and historical values (`?h`) for previously evaluated values.

(e) All register scans are unsorted linear search as only terminal keyboard input was initially anticipated.

(f) File and String I/O is anticipated and supported via `libapa.a` expression string and pipe streams API (`?b`).

(g) Possible enhancement would be passing the stream of integer register strings through a GPU for arithmetic logic operations to support big computation
