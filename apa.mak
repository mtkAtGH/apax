# "@(#) Makefile 1.5 Makefile";
CFLAGS =  -c -I.
LDFLAGS =  -Bstatic
CCSHARE_OPTS = $(CC) $(CFLAGS) -fPIC -rdynamic -shared
CC = cc -pg
AR = ar
#CC = cc -g -DDBG_OBJT # to turn on object inspection on an object reference
all:
	make sob
	make apa
	make lib
	
sob: $(OBJS)
	$(CCSHARE_OPTS) $(OBJS:.o=.c)
	$(CC) $(SHARE_OPTS) $(OBJS) -o libapa.so 

lib: apax.o cplx.o fctr.o fixr.o fnct.o help.o objx.o util.o
	$(AR) cuv libapa.a  apax.o cplx.o fctr.o fixr.o fnct.o help.o \
                     objx.o util.o

apa: apax.o calc.o cplx.o fctr.o fixr.o fnct.o help.o objx.o util.o
	$(CC) $(LDFLAGS) -o apa apax.o calc.o cplx.o fctr.o fixr.o fnct.o \
			help.o objx.o util.o

apax.o: apax.c defn.h
	$(CC) $(CFLAGS) apax.c

calc.o: calc.c defn.h
	$(CC) $(CFLAGS) calc.c

cplx.o: cplx.c defn.h
	$(CC) $(CFLAGS) cplx.c

fctr.o: fctr.c defn.h
	$(CC) $(CFLAGS) fctr.c

fixr.o: fixr.c defn.h
	$(CC) $(CFLAGS) fixr.c

fnct.o: fnct.c defn.h
	$(CC) $(CFLAGS) fnct.c

help.o: help.c defn.h
	$(CC) $(CFLAGS) help.c

objx.o: objx.c defn.h
	$(CC) $(CFLAGS) objx.c

util.o: util.c defn.h
	$(CC) $(CFLAGS) util.c
