# apax
Arbitrary Precision Arithmetic Expression
Comprises makefile, c-modules with headers.
Makes exectable 'apa' and linkable 'libapa.a'.
Originally K&R C, where int was default for undeclared types.
Int was also the default for undeclared return values and for logical values (zero==FALSE).
Finally pointers were exchangable with ints, as was NULL and calloc's offsets.
Changes required for compile without warnings include using <stdlib.h>  (in defn.h);
Using zero (0) in place of NULL where int type was needed, and returning int explicitly.
It declares a maximal precision as 2^wordbits minus one, minus ASCII '0' (48.
