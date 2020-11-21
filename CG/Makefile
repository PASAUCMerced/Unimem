SHELL=/bin/sh
BENCHMARK=cg
BENCHMARKU=CG

include ../config/make.def

OBJS = cg.o unimem.o ${COMMON}/print_results.o  \
       ${COMMON}/${RAND}.o ${COMMON}/timers.o

include ../sys/make.common

ERROR_FLAGS =  -Wall -Werror
CFLAGS = $(ERROR_FLAGS) -D_REENTRANT -DLinux -D_GNU_SOURCE -O0 -g -mcmodel=large



${PROGRAM}: config ${OBJS}
	${FLINK} ${FLINKFLAGS} -IUnimem -o ${PROGRAM} ${OBJS} ${FMPI_LIB} -lnuma -lnumap -lpthread -LUnimem 

unimem.o: ../Unimem/unimem.c  
	gcc -c ../Unimem/unimem.c -mcmodel=large  -lnuma -lnumap -lpthread

cg.o:		cg.f  mpinpb.h npbparams.h timing.h
	${FCOMPILE} cg.f

clean:
	- rm -f *.o *~ 
	- rm -f npbparams.h core



