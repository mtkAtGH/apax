.pl 60 \" prevent over/underflow on OL400
.PL \" notify nroff that -ms has begun
.nr PO 1.5i \" set global page offset to 1.5in.
.de F0 \" give font 0 escape sequence to post-processor
\!^[&l0O^[(8U ^[(s 0p 10h 12v 0s 0b 3T
..
.de F1 \" give font 1 escape sequence to post-processor
\!^[&l0O^[(8U ^[(s 0p 16.66h 8.5v 0s 0b 0T
..
.de MB
.br
.ev 1
.nf
.ne
.nh
.di XX
..
.de ME
.br
.di
.if \\n(dn>=\\n(.t .BP \" modified .bp if doesn't fit
.in +25
.F1
.XX
.F0
.in -25
.ev
.hy
.ec
.fi
..
.de BP
.br
.bp
..
.F0 \" force default to font 0
.FS
APA(X) is a trademark of Precis Computing Products
.FE
.ce  2
THE APA(X) USER'S GUIDE TO BETTER
ACCURACY IN COMPUTING

.PP
This is a story about how one can generate incorrect results out of a
computer without ever having posed incorrect problems; and how to avoid
situations where that can happen. ... What??? Yes, you can provide the
computer with a perfectly formed problem and it could return a perfectly
wrong result. Why? Read on to find out why and to protect against it.

.SH
"A Computer Is Like a Broom..."

.PP
Thats what the blind man would say when equating accuracy to cleanliness.
You see, you don't really have to be guilty of human error to create
calculation errors out of a computing machine. True, Garbage In - Garbage
Out is an understandable situation, but what is this thing about Non-
Garbage In - But Garbage Out? Can a computer really be that evil?

Actually computers have no motives and cannot therefore be evil. What is
evil is the incompleteness of human technology and the false comfort that
people lapse into after observing that their invention seems to do the job.

Here is an example not related to computing. Supposing that your mission
is to keep the floor clean. You begin by picking all the objects that
you can see and stop after satisfying yourself that there is nothing left
on the floor. Now you happen to notice that if you used a broom and pan
it would take you a fraction of the time to do the same job. So you
adopt the convenience and save yourself a lot of time.  Some time later
you notice that the broom has worn down. What happened to the bristles?
Well, in exchange for the convenience you LITTERED your floor with bristle
scrapings that you could not quite see. The irony is that the broom was
meant to HELP you keep the floor clean but it in fact did the opposite.

If that goes against what you believe, let us say - brooms do clean; but
only to a point. In this case, they  sweep out objects that are bigger than
bristles. They cannot catch smaller objects very well, and they actually
throw in some smaller ones themselves. To clean out the smaller pieces you
will need something like a buffing cloth. No people in their right minds
would use a broom to polish the floor. But because we are not thinking of
it at the time, it is easy to forget that brooms and buffers create dust...

So why are we pursuing perfection in computing? Actually we are not. We
just want to keep up with our degree of cleanliness. Computers do very
well when the 'fluff' does not bother us; but when fluff is a problem, we
need to be able to replace the computing 'broom' with a 'brush' or 'buffer'.
That is what we hope APA(X) can do, and we also hope to have fun in the
process because computing should be fun.



.SH
Turning Your Computer into a Calculator

.PP
We are going to use APA(X) to show how a computer behaves when it is doing
the right thing and when it fails to do so, and we will show how to use
APA(X) to avoid the latter.

A computer is a calculating machine. Unfortunately computer makers do not
provide calculator services. Through programs it can perform many varied
tasks, and it just does not seem fair to make it work only like a calculator.
The task of doing so is left to the providers of software for your machine.
So we shall assume that you have an executable copy of APA(X) or someting
like it to make it behave like a calculator. The name APA(X) stands for
Arbitrary Precision Arithmetic (Expression). The expression is abbreviated
with an 'X' and it is demarcated with parentheses. This is deliberate as you
will now see why.

You invoke the calculator mode under APA(X) by simply typing the first
three letters of its abbreviated name - apa. APA provides the arithmetic '('
prompt and you provide the expression 'X)' to complete the APA(X) action.

In the very first instance APA(X) displays its name, version number and
copyright notice and some lines of text describing how to query APA(X), the
maximum precision that APA(X) can maintain if given adequate storage capacity,
the default settings and how to give it an arithmetic problem.

Later it simply prompts you with the open parenthesis and lets you enter
whatever you wish to be evaluated.

Finally, it gives the APA(X) prompt '(',for the expression which you must
provide. You will also have to terminate the expression with a ')' to inform
APA(X) to go ahead and compute the expression. Here is a sample output
from a computer with 32-bit architecture:


.MB
-------------------------------------------------------------------------
|									|
|apa									|
|    apa(x): Arbitrary Precision Arithmetic (Expression) Version 1.0	|
|									|
|    (C)Copyright  Malome T. Khomo, 1989. All rights reserved.		|
|									|
|									|
|use ? for queries							|
|apa(x) maximum precision (RAM permitting:) = 2147483599 digits.	|
|default precision = 0(variable), precedence = 2(DMA), radix = 10	|
|Your system uses ASCII. '_' is your tag character.			|
|Reset it with ?c if your keyboard does not have it.			|
|Type in an arithmetic expression, end it with ')' and 'return'		|
|(									|
-------------------------------------------------------------------------
.ME


From this point on APA(X) will only print answers to your inputs and
the prompt for the next arithmetic expression. If you are done all you
have to do is give APA(X) a null expression, that is a ')' with no 'X'
and APA exits to return you to the operating system of your computer:

.MB
-------------------------------------------------------------------------
|(1/13)		(your expression)					|
|.0769		(the apa answer)					|
|()		(null expression)					|
|									|
|End of session.							|
|									|
|    APA(X): Arbitrary Precision Arithmetic (Expression) Version 1.0	|
|									|
|    (C)Copyright  Malome T. Khomo, 1989. All rights reserved.		|
|									|
-------------------------------------------------------------------------
.ME

A point to note is that APA(X) does not associate any special meaning to
the [Enter] or [Return] key. If you end your expression with it without
the ')', APA(X) will assume you have not ended the expression. It will
ignore the new-line characters and wait for more input terminated with
a ')'. It will not provide another prompt. This may seem unnatural to those
of you who are accustomed to operating system prompts. The difference is
that new-lines are for the benefit of the operating system and are being
ignored by APA(X). If you enter the ')' and do not press [Enter] or [Return],
APA(X) will not know that you've done so until it is informed by the operating
system which finds out only AFTER you press the [Enter] or [Return] key.
If this all sounds confusing, avoid having to worry about it by always
ending your expressions with ')', irrespective of the number of lines the
expression spans.


.SH
Mimicking the Hapless Computer with APA(X)

.PP
Most computers use fixed-sized registers.  These are the same registers that
you have seen on those big table-top adding machines that they used to have in
banks and other prestigious institutions. The register is a panel with little
windows through which digits are displayed. The number displayed or stored
by the register is the value you can read through the holes provided. When
one says that the register has fixed size, they mean the number of windows
provided with that machine is fixed. Some adding machines had six positions
and had a decimal point embossed between the second and third position.
These were specialized to counting money, as with cash registers. Other more
expensive machines had eight or twelve positions, but the point is that the
number on any such machine was fixed.

Computers are the same way. They do not count with digits but with binary
digits - bits, and one of the identifiers of a computer's architecture is
the size of its register. As opposed to a 32-bit machine output shown above,
a machine with 16-bit architecture would have caused APA(X) to produce the
following line instead:

.MB
-------------------------------------------------------------------------
|...									|
|apa(x) maximum precision (RAM permitting:) = 32719 digits.		|
|...									|
-------------------------------------------------------------------------
.ME

A century ago when Charles Babbage was building his first Analytic Engine -
the precursor to today's computers, one of the decisions he had to make
was what register size to use. He felt that in order to perform every-day
calculations without loss of accuracy in the answer he would need a register
with 21 decimal positions. In today's binary terms that was 69-bits, more
than twice the size of present-day 32-bit architectures.  So how do we
stand with the numerous 16-bit machines of the IBM-PC/AT era? In short, not
very well, but let us see exactly where.

Within the world of computing one has to decide on how to represent numbers.
They come in two forms. Integers or floating point. With integers you simply
read the register as you would a sequence of numbers, with zero being the
smallest value, and other values being any whole number that can be contained
by the register. On an AT class machine the maximum integer is 65536, and if
sign is used the range of signed integers is -32768 to +32768.

Notice that you cannot express an integer million on 16-bit machine. How
does one go about expressing arbitrary values? Through floating-point
representation. Under this scheme a number is expressed as a fraction,
that is a value between 0 and 1, and an exponent or mantissa which tells
by how much to shift the decimal point in the fraction to express the
correct magnitude of the value. So 1,000,000 is simply .1e+7, and .0000001
is .1e-6.

Now the fraction and mantissa have to split the same register pie to come up
with a representation, causing the fraction to have much fewer values that
it can represent accurately. Many combinations of fraction/mantissa size
splits are possible, further there is the question of which bit to use for
the fraction's sign and which for the mantissa's.  The potential for chaos
is so great that the IEEE has set a standard for splitting the register; in
fact, IEEE encourages the use of larger registers,  32-bit or even 64-bit.
Virtually all computer manufacturers adhere to the standard.

When the floating point value uses only one register it is called single
precision. When two registers are used - in the bid to be at least as
accurate as the integers - it is called double precision. On 16-bit machines
double precision gives a fraction with around seven significant digits. 
Occasionally, specialized floating point hardware may increase this to
around eleven, or use of the IEEE standard may make the machine use a 32-bit
or 64-bit register, but seven is what the original machine could do so that's
where we shall begin with mimicking our hapless computer.


.SH
Setting Fixed Precision
.PP
To query and set the precision under which APA(X) is operating enter ?p for
the following response:

.MB
-------------------------------------------------------------------------
|									|
|(?p									|
|Precision Handling:							|
|Precision may be fixed at some value or may be allowed to vary.	|
|If it is variable (set with value zero), the effective precision is	|
|initially determined by the argument lengths in the input expression.	|
|Variable precision may increase by one on each addition, up to a factor|
|of two on each decimal division, and by as much as is needed for an	|
|exact answer on each factorial division. No rounding is done. Fixed	|
|precision is maintained by rounding.					|
|current setting = 0 (variable)						|
|									|
|reset precision ? (y/n) y						|
|enter precision: 7							|
|(									|
|									|
-------------------------------------------------------------------------
.ME

So now we have our AT class machine precision settings. Let us tour the
landscape in search of the pitfalls. Actually, the emphasis is misplaced.
Considering that there are an uncountable infinity of numerical values
in existence, and that we can only represent 10,000,000, we have as a
landscape of 10 million needle-points to stand on, overwhelmed by a vast
and bottomless pit-like space to fall into. There is no searching to do,
any move from the register values is a certain plunge, so how do the
computers do it? How do they give us the confidence to which we have all
submitted? By building a false bottom to bridge the gaps between the
register values with the ROUNDING mechanism.

In rounding you legislate that if a value is less than half-way from the
current register value to the next valid register value, use the current
register value. If it is half-way or further, then use the next value.
This piece of legislation enables us to estimate ALL the real numbers with
the machine register values.

.MB
-------------------------------------------------------------------------
|									|
|(1/3)			(one divided by three)				|
|.3333333		(a recursive answer)				|
|(1%3)			(remainder of one divided by three)		|
|.1e-8			(the remainder - a loss of accuracy)		|
|(3*1/3)		(three thirds is unity)				|
|1.0			(correct, thanks to rounding)			|
|(									|
|									|
-------------------------------------------------------------------------
.ME

But rounding does not eliminate the loss of accuracy. Accuracy is maintained
when rounding can catch errors before they get truncated by a register. If
we were to evaluate 3*.1/3 instead of 3*1/3, something else happens:

.MB
-------------------------------------------------------------------------
|									|
|(.1/3)			(floating decimal point .1)			|
|.333333e-1		(only six significant digits in .0333333)	|
|(3*.1/3)								|
|.999999e-1		(no rounding took place)			|
|(									|
|									|
-------------------------------------------------------------------------
.ME

When entering the value .1 we have suggested to APA(X) a format for filling
out the register value. When we divide that value by three we generate the
result in the same format .0333333 which has truncated one significant digit.
Accuracy cannot be restored after that. We could have used the fixed point
format to avoid truncation:

.MB
-------------------------------------------------------------------------
|									|
|(1^-1/3)		(fixed decimal point 1.0e-1)			|
|.3333333e-1		(no truncation)					|
|(3*1^-1/3)								|
|1.0e-1			(rounded to right answer)			|	
|(9876543*1/9876533)	(The action is from the 6th digit)		|
|(1.0			(rounded to closest incorrect answer)		|
|									|
-------------------------------------------------------------------------
.ME

APA(X) has a degree of flexibility in the way a number can be presented
to our hypothetical computer. Actual computers do not have this flexibility.
A number is  either integer or floating point, and all floating points are
converted to the fixed point before processing. The loss of accuracy due
to truncation as a result of floating decimal computation will not occur.
Other truncations will still occur with alarming regularity:

.MB
-------------------------------------------------------------------------
|									|
|(1*123456789)		(number too long)				|
|.1234568e+9		(truncated and rounded)				|
|(									|
|									|
-------------------------------------------------------------------------
.ME

With a real computer, integer manipulation would have caused a more drastic
truncation to simply the value 1234567, and not the rounded floating point
estimate shown above.

Before we leave the subject of architectural limits to computing we should
caution that the seven-digit limit used here is theoretical. Actual computers
have tended to use specialized hardware that increases that limit. If you
were to test the precision limits of an AT-class machine by programming
you would actually find it good up to seventeen digits because its floating
point unit uses wider registers to do the actual calculations.


.SH
Minimizing Error in Calculation

.PP
We have shown two ways in which a computer creates gross arithmetic errors.
In truncation of long sequences of digits, and in rounding an answer when
it cannot represent the exact value. 

When one calculation is done, the effect of this problem can be assessed
well enough and it can be compensated for by a variety of conventions. When
calculations are being performed by a computer many times over out of sight
of human inspectors, then the potential for grossly damaging results is
great. A program such as APA(X) can provide the mechanism for avoiding chaos
by two means; by extending fixed-point precision and by using a variable
floating-point precision.

Extending Fixed-Point Precision

The first obvious one is - avoid truncation by extending precision. If you
know from the nature of your work that the input numbers tend to have a
certain maximum precision, then simply change the fixed-point precision
to a value above that to ensure rounding to an acceptably correct value.

APA(X) can extend precision to almost as far a your machine's integers can
go; but you must have the core memory for APA(X) to manipulate such numbers
in. Usually, that is what runs out first. In operating systems that implement
virtual memory, it takes a bit more before memory runs out, but the limits
are usually reached before the maximum integer size limit is reached. It may
be that the trend will continue in the near future because disk storage capacity
increases have tended to be followed closely after by the broadening of the
machine architectures.

Computing is a finite resource activity; What you gain in higher precision
you pay for in longer computing times. There is an exponential relation
between the time it takes and the precision, so one must settle for an
optimum. The raw machine gives you maximum speed and the worst precision.
This is not quite true, it is possible to set precision to less than
the machine's best, in which case APA(X) will consume resources and not
improve precision, but you WILL pay in speed for improvements in precision.

.SH
Variable Floating Point Precision

.PP
APA(X) also provides a variable or free-form precision which is in effect
when you set your precision to the value zero. What happens in this case
is that before any arithmetic operation, APA(X) examines the arguments
and determines a precision for the outputs.  This is what APA(X) does by
default. The precision handling rules displayed by the ?p query indicated
that the precision of an answer will tend to increase with long arithmetic
expressions. Occasionally a result will have insignificant leading zeroes
which APA(X) will trim. In those instances, the precision may drop. But
round answers are the exception rather than the rule. In certain situations
it can be profitable to seek out round answers and we shall demonstrate
how to do so, but for now, let us look at variable precision. For the next
example let us assume that the APA(X) precision is already set to zero for
variable precision:


.MB
-------------------------------------------------------------------------
|									|
|(1/3)									|
|.33			(precision increased by one)			|
|(1234/2345)								|
|.52622			(precision increased by one)			|
|(.1234/2345)								|
|.000052622		(precision increased by five)			|
|(									|
|									|
-------------------------------------------------------------------------
.ME

Note in the last case that even though precision increased by five, the number
of significant digits increased by only one. The whole idea behind the way
precision is handled is not to give a false sense of accuracy. (The guiding
philosophy is that even though the Laws of Arithmetic dictate the increase
by many significant digits, there is a limit to their significance, especially
when the initial input values were acquired by some empirical measurement
process.) The number of significant digits in the result is maintained to the
same order of magnitude as that of the inputs. When the expression has a
series of operations the number of significant digits increases slowly with
the number of operations:

.MB
-------------------------------------------------------------------------
|									|
|(1234+2345/3456*4567)							|
|4332.84651			(total increase of six)			|
|(1234+.2345/3456*4567)							|
|1234.309884651			(total increase of nine)		|
|(									|
|									|
-------------------------------------------------------------------------
.ME

For those of you who want to double-check the result with hand-calculations,
remember - the active precedence rule is DMA - which means you do all the
divisions first from left to right, the mutiplications next and the additives
(plus and minus) last. The ?a query explains the available rules and how
reset them.

Here we see that dividing a fraction by a whole number brought about the
larger increase in precision. It is not possible to differentiate the gain
in significant digits from the gain in precision because all the digits in
this result are significant. This is a feature of true floating point that
is not exercised in fixed-point arithmetic. When there is the lee-way to
align all operands by decimal position before summing, as we normally do
with pen and paper, we have the opportunity of preserving all significant
digits. The procedure can lead to excesses that fixed precision arithmetic
cannot cope with:

.MB
-------------------------------------------------------------------------
|									|
|(.0000000001+10000000000)						|
|10000000000.0000000001		(precision is sum of operands')		|
|(1^10+1^-10)			(same inputs in fixed-point format)	|
|1.00000000000000000001e+10	(same result - all zeroes significant)	|
|(									|
|									|
-------------------------------------------------------------------------
.ME

Under variable precision, inputs that are in fixed-point format do not
affect the handling of significant digits. We see here that all the
significant digits were maintained, but because the exponential shifts, had
been activated  we ended up wasting space accounting for the shift in the
decimal point.

The manner in which APA(X) calculates has been deliberately made the same as
the way we are accustomed to when working with pen and paper. It is hoped
that this approach makes it more intuitive than raw computer arithmetic.

.SH
Maintaining Accuracy with Round Numbers

.PP
The other way to preserve accuracy is by working with round numbers. It is
not normally possible to know how to take advantage of round numbers, but
we do know that whenever a value is a multiple of 10 with leading or zeroes,
it can be written more briefly by using the exponential shift. We saw in
the above example that 10000000000 is the same as 1^10 exactly. This is
because in the decimal system multiples and factors of ten are round.
Historically, this has made it easy to count with our hands. In fact, the
decimal digit is an abstraction of our fingers.

No combination of digits will ever let us represent fractions such as 1/3
exactly. I will turn the precision up to show that this is a common problem
with all prime numbers that are not factors of 10 (viz 5):

.MB
-------------------------------------------------------------------------
|									|
|(?p									|
|Precision Handling:							|
|Precision may be fixed at some value or may be allowed to vary.	|
|If it is variable (set with value zero), the effective precision is	|
|initially determined by the argument lengths in the input expression.	|
|Variable precision may increase by one on each addition, up to a factor|
|of two on each decimal division, and by as much as is needed for an	|
|exact answer on each factorial division. No rounding is done. Fixed	|
|precision is maintained by rounding.					|
|current setting = 0 (variable)						|
|									|
|reset precision ? (y/n) y						|
|enter precision: 64							|
|(1/3)			(my favorite prime unrelated to 10)		|
|.3333333333333333333333333333333333333333333333333333333333333333	|
|(1/7)									|
|.1428571428571428571428571428571428571428571428571428571428571428	|
|(1/13)									|
|.7692307692307692307692307692307692307692307692307692307692307692e-1	|
|(									|
|									|
-------------------------------------------------------------------------
.ME

Now supposing you belonged to a company of three partners, and the articles
of association guaranteed three partners only for the life of the partnership.
Suddenly, the splitting of shares is more rounded in thirds than in factors
or multiples of ten. Which would be more convenient, using digits or a
different base of computation? This is where RADIX comes to play. APA(X) sets
a default radix of 10, but it enables us to change it to any other value at
will. Let us see how the partnership of three fares with splitting revenues
of values 1 to 9. First we observe the split using the default radix of 10:

.MB
-------------------------------------------------------------------------
|									|
|(1/3)			(cannot split into exact shares)		|
|.3333333333333333333333333333333333333333333333333333333333333333	|
|(2/3)			(cannot split into exact shares)		|
|.6666666666666666666666666666666666666666666666666666666666666667	|
|(3/3)									|
|1.			(a round number for an exact share)		|
|(4/3)			(cannot split into exact shares)		|
|.1333333333333333333333333333333333333333333333333333333333333333e+1	|
|(5/3)			(cannot split into exact shares)		|
|.1666666666666666666666666666666666666666666666666666666666666667e+1	|
|(6/3)									|
|2.			(a round number for an exact share)		|
|(7/3)			(cannot split into exact shares)		|
|.2333333333333333333333333333333333333333333333333333333333333333e+1	|
|(8/3)			(cannot split into exact shares)		|
|.2666666666666666666666666666666666666666666666666666666666666667e+1	|
|(9/3)									|
|3.			(a round number for an exact share)		|
|(
|									|
-------------------------------------------------------------------------
.ME

We made exact splits thrice. Actually zero also splits exactly, but I do not
think any of those partners care for a share of that value.  The reason there
were three even splits was that there are three multiples of number 3 between
1 and 9. If there were 7 partners there would only be one exact split. Thirteen
partners would never see an even split in the decimal system. Let us see how
the partners fare with a base 3 radix. The APA(X) radix is changed  with the
?r query:

.MB
-------------------------------------------------------------------------
|									|
|(?r									|
|Radix:									|
|The base of a fixed radix number system ie. other than the factorial	|
|format may be changed from the decimal default value of 10. Its values	|
|range from 2 to 2147483598. A number is written as a string of radix	|
|registers, each value is right justified and zero padded in the space	|
|required to hold the value radix minus one.				|
|WARNING: Inputs must be radix 10. The apparent precision increases	|
|roughly with the log of the radix but the computation speed drops more	|
|drastically, especially at high radices.				|
|current setting = 10							|
|reset radix ? (y/n) y							|
|enter radix: 3								|
|(									|
|									|
-------------------------------------------------------------------------
.ME

The ?r query mentions a factorial register. What is this? We shall explain
it in the next section on exact computing. For now, let us work with round
numbers. Here's how partnership earnings split into shares under base three:

.MB
-------------------------------------------------------------------------
|									|
|(1/3)									|
|.1									|
|(2/3)									|
|.2									|
|(3/3)									|
|1.									|
|(4/3)									|
|1.1									|
|(5/3)									|
|1.2									|
|(6/3)									|
|2.									|
|(7/3)									|
|2.1									|
|(8/3)									|
|2.2									|
|(9/3)									|
|10.									|
|(									|
|									|
-------------------------------------------------------------------------
.ME

With a simple change of radices we have made all the splits into three
shares exact. Our active precision was still 64 but the round results needed
much fewer digits. This principle can be applied for other splits. The main
criterion for choosing a radix is whether or not there exists a special number
into which all other values will be reasonably expected to be divided by or
divided into. The situation occurs more frequently than normally thought, but
is takes some consideration before finding it.

In scientific experiments certain configurations give rise to quantum states
for which only certain multiples of a basic unit make sense. In those
instances, the lowest common denominator of the values is such a radix that
enables exact computation of arithmetic expressions involving those quantum
changes.

In engineering situations, say in control engineering, the mechanical or
electrical system to be controlled may have finite states. Such systems can be
expressed elegantly as finite rings - the set of numbers that repeat themselves
after so many values. The units natural to that system are also good candidates
for exact computation with a set radix.

You would normally try to find a prime number for a radix, but that is not
essential. The number 10 for instance is not a prime but is used anyway. This
brings us to the final feature in our guide to accuracy in computing.

.SH
Guaranteeing Round Numbers with Exact Arithmetic

.PP
The use of round numbers poses an interesting possibility for  minimizing
errors. In the radix 3 example everything worked well when we were dividing
by three. What happens in other divisions?:

.MB
-------------------------------------------------------------------------
|									|
|(1/2)									|
|.1111111111111111111111111111111111111111111111111111111111111111	|
|(1/3)									|
|.1									|
|(1/4)									|
|.2020202020202020202020202020202020202020202020202020202020202021e-11	|
|(1/5)									|
|.1210121012101210121012101210121012101210121012101210121012101210e-11	|
|(1/6)									|
|.1111111111111111111111111111111111111111111111111111111111111111e-11	|
|(1/7)									|
|.1021201021201021201021201021201021201021201021201021201021201022e-11	|
|(1/8)									|
|.1010101010101010101010101010101010101010101010101010101010101010e-11	|
|(1/9)									|
|.01									|
|(									|
|									|
-------------------------------------------------------------------------
.ME

Only 1/3 and 1/9 produces round answers. The reset is just as bad if not
worse than radix 10 which does much better with 9 significant characters -
0 to 9. So how do we improve our chances?

The answer lies in using as many radices as needed to maintain exactness.
The examples we have looked at so far use a fixed radix scheme. What this
means is that each register position has the value of the radix to the power
of that position. For fixed radix systems the position values are as follows:

.MB
-----------------------------------------------------------------------------
|					the point spot			    |
|					     V				    |
|	<---		The integer side   -|.|- the fraction side	--> |
|									    |
|	...	3	2	1	0	-1	-2 	-3	... |
|									    |
|	...	1000	100	10	1	.1	.01	.001	... |
|									    |
-----------------------------------------------------------------------------
.ME

To make the radix variable, the scheme for varying radices is to give positions
the value of their factorials. A factorial is the result of multiplying a number
with all the numbers that precede it starting from 1. The position values are
given thus:

.MB
-----------------------------------------------------------------------------
|			the point spot					    |
|				   V					    |
|	<---	The integer side -|.|- 	the fraction side		--> |
|									    |
|	...	3	2	1	-2	-3	 -4		... |
|									    |
|	...	1*2*3	1*2	1    1/(1*2)  1/(1*2*3)	1/(1*2*3*4)	... |
|									    |
-----------------------------------------------------------------------------
.ME

The arithmetic symbol for factorial is the '!'. So 1! equals 1, 2! equals 2,
3! equals 6, 4! equals 24, 5! equals 120 and so on. APA(X) allows you to
evaluate factorials using the same natural notation. Let us  use the default
decimal radix system to illustrate this. We do not show the ?r query output
since it is the same as the one we saw when we changed from the default to 3.
Here are some factorial evaluations:

.MB
-------------------------------------------------------------------------
|(5!)									|
|120									|
|(6!)									|
|720									|
|(7!)									|
|5040									|
|(									|
-------------------------------------------------------------------------
.ME

Factorials grow rapidly and take long to calculate for large integers. Most
hand-held calculators reach their precision limits with 69!. They stop
computing and declare a 'Register Overflow Error'. Your computer would do
the same thing had it provided a keyboard function for factorials. They
don't, so you will just have to take our word that it would behave like
its hand-held cousin. APA(X) can compute larger factorials. At the value
100!, you can actually notice that it takes a few seconds to return the 158
digit integer result. Try it on your machine, it could take longer or shorter.

Now that we know what factorials mean let us query APA(X) on the factorial
register format that it referred to in the ?r query:

.MB
------------------------------------------------------------------------------
|(?!									     |
|The Factorial Format:							     |
|The factorial register uses a  '.' to separate integer and fraction portions|
|of a number; just as with decimal (register) points. The place-value of each| 
|factorial register position equals the factorial of the position number as  | 
|counted from point position zero in the integer direction , and equals its  |
|reciprocal in the negative direction, except that place value '1/1!' is     |
|excluded to prevent duplication of whole fractional place values which carry| 
|over to the next place left. The maximum register value is the magnitude    |
|of its physical position. The place values are always printed right-	     |
|justified and zero-padded to the space required for the maximum register    |
|value.									     |
|   For input a '!' prefix converts a decimal string into factorial format   |
|according to these rules, except that the end-register position value need  |
|not be right-justified. A '!' postfix converts a factorial register into    |
|decimal up to the active number of significant digits if necessary.	     |
|(									     |
|									     |
------------------------------------------------------------------------------
.ME

The phrase 'zero-padded and right justified' appears in both the ?r and ?!
queries. What it means is that a register position with say value 200 will
be represented like this:

.MB
	---------------------------------------------------------
	|							|
	|	000  001  002      011  012	 198	199	|
	|	---, ---, ---, ... ---, ---, ... ---,	---	|
	|	200  200  200      200  200 	 200	200	|
	|							|
	---------------------------------------------------------
.ME

So that we can distinguish the first value 1/200 from the second value 11/200 
and the last value 111/200 in a number such as 001011111. Let us demonstrate
factorial values; remember, a factorial register value is indicated with a '!'
prefix. Arithmetic operations on factorial register are also indicated with
the '!' prefix: !+,!-,!* and !/. To make life easier, we have made factorial
arithmetic operations recognize ordinary decimal numbers and convert them for
you before calculating the result of the operation. In the following example
we use decimal inputs for the factorial arithmetic operations:

.MB
-------------------------------------------------------------------------
|(1!/3)			(again my favorite problem)			|
|!.02			(Whoops! a round answer!)			|
|(									|
-------------------------------------------------------------------------
.ME

What does this round answer mean? The factorial number !.02 expands to

.MB
	-------------------------------------------------
	|						|
	|    	.	0/(1*2)  + 	2/(1*2*3)	|
	|						|
	-------------------------------------------------
.ME

Suddenly we have an exact representation of 1/3. But let us not celebrate
prematurely. How about the other prime numbers so beloved to sorcerers and
their ilk:

.MB
-------------------------------------------------------------------------
|(1!/7)									|
|!.003206								|
|(1!/13)								|
|!.001412548050012							|
|(1!/17)								|
|!.00120236809000902070016						|
|(1!/23)								|
|!.00101210310000610140509101803100022					|
|(									|
-------------------------------------------------------------------------
.ME

All the medieval magic numbers have been tamed. Too bad! But this is good
news for our parnership accountants who can still maintain an exact split of
revenues into shares if the number of partners changes. At the end of the
accounting period they simply convert the exact shares into decimals because
actual money can only be split into legal denominations such as cents:

.MB
-------------------------------------------------------------------------
|(!010203040506070809876543210.123456789010203!)			|
|7182224590861286.9999997541360388582610804833027055249277471		|
|(									|
-------------------------------------------------------------------------
.ME

The company accountant can decide on the proper way to disburse the fraction
of cents that cannot be paid out in legal currency denominations. No need
to set up random profit and loss accounts to catch arithmetical inaccuracies. 

As with factorials, the computations get long quickly. The value 1!/4001
generates an exact answer that takes quite a few lines to write down, but it is
EXACT. If you are an ordinary person like ourselves, you would not know off
-hand whether 4001 is a prime number; so under decimal radix 10 you simply jack
up the precision and hope 1/4001 returns the exact answer. You will tell that
it is exact if increases in the precision do not change existing significant
digits or add new ones. Well if you do this as we have just tried, you will go
to a precision of 100, 200, 400, 600 and then to 1000 and still keep getting
more significant digits. So you cannot really tell if you have an exact  answer
with tentative attempts. It is best that you go ahead and use the factorial
registers and get a one-time exact answer (or of-course the APA(X) equivalent
of an 'Overflow Error' when we run out of core memory).

We will demonstrate the way to input values in factorial register format with
another number that stays magical even to present-day mathematicians. They call
them transcendental numbers. One of them emerges in geometry and is called pi,
the greek letter for an angle that you cannot really see because it results in
a straight line. Circles are formed with angles of 2-pi. The other one is the
base of natural logarithms, the number denoted by e. The exponent has the
property that it and its derivative are the same. If this does not make sense
to you do not worry about it. Just know that many mathematicians make a good
living because of these two transcendental numbers.

The one we will show here is the number e. It just so happens that there is
no way to represent transcendental numbers exactly. Worse still, there is
no magical way of writing out its value without doing a series of calculations.
Calculator and computer makers get around this by storing a value in their
machines' memory permanently, and simply recall it for use. 

From the guts of the computer on which I demonstrated APA(X) there are the
following lines among many constants stored:

.MB
-------------------------------------------------------------------------
|#define M_PI    	3.14159265358979323846				|
|#define M_E            2.7182818284590452354				|
-------------------------------------------------------------------------
.ME

M_E is the value for e. My machine has 32-bit architecture. Notice that they
only use about 20 significant digits. By the way, the text just shown is
copyrighted by Sun Microsystems Incorporated who manufactured this said
computer. The value of e can be obtained by the power series:

	1/0! + 1/1! + 1/2! + 1/3! + 1/4! + 1/5! + ...

which fits very naturally into our factorial format for the fractional part.
The first two integer parts have to be added together because the 1/!0  and
1/!1 registers are not used in our format. The result is:

	1*2! + 0*1! . + 1/!2 + 1/3! + 1/4! + 1/5! + ...

which in our notation can be written simply as:

!10.1111111110101010101010101010101010101010101010101 ...

Notice how the 1's from the 10th place on are 'right-justified, zero-padded'.
Finally, with APA(X) we allow you to convert a factorial format number back
to decimal by a '!' postfix. This use does not interfere with the original
factorial postfix usage because that one operates on fixed-radix numbers
only. So, let us see what APA(X) does for us when we enter the above number
and then turn it back to decimal for comparison to Sun Microsystem's value.
Remember that we cut short our input series at 31 factorial registers in
the fractional part of our number.

.MB
-------------------------------------------------------------------------
|									|
|(!10.11111111101010101010101010101010101010101010101010101!)		|
|2.718281828459045235360287471352662497638597041190020371406518038541420|
|74115966388	     ^							|
|(									|
-------------------------------------------------------------------------
.ME

We see that Sun Microsystems rounded up the value at their maximum precision
which we marked with a caret character ^ below the value '3'. APA(X)
seems more precise but not all the numbers are significant. Let us see how
many are significant by increasing the length of the input and see which is
the first register value to change:

.MB
-------------------------------------------------------------------------
|									|
|(!10.111111111010101010101010101010101010101010101010101010101010101!)	|
|2.718281828459045235360287471352662497757247091737715433136639032814646|
|8619609855702251317407551	      ^					|
|(									|
-------------------------------------------------------------------------
.ME

Exact values stopped at the caret mark above, which indicates that 31
fractional factorial registers gave us an exact decimal rendering of e
to 36 decimal points. We have increased the accuracy of our calculation
from from the computer's 20 significant digits to 36 with little effort.

This has got to be a step in the right direction! Or else, this is NOT your
Guide to Better Accuracy in Computing.()

.FS
Further Information may be obtained from:
Precis Computing Products
400 E Sibley Blvd, Harvey IL 60426. Phone No. (708)596-0938.
.FE
