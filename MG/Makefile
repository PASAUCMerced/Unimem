SHELL=/bin/sh
BENCHMARK=mg
BENCHMARKU=MG

include ../config/make.def

OBJS = mg.o ${COMMON}/print_results.o  \
       ${COMMON}/${RAND}.o ${COMMON}/timers.o unimem.o

include ../sys/make.common

ERROR_FLAGS =  -Wall -Werror
CFLAGS = $(ERROR_FLAGS) -D_REENTRANT -DLinux -D_GNU_SOURCE -O0 -g -mcmodel=large

${PROGRAM}: config ${OBJS}
	${FLINK} ${FLINKFLAGS} -o ${PROGRAM} ${OBJS} ${FMPI_LIB} -L/home/kwu/mpi_project/2016winter/NPB3.3-MPI/Unimem -lnuma -lpthread -lnumap

unimem.o :	../Unimem/unimem.c
	gcc -c ../Unimem/unimem.c -I/home/kwu/mpi_project/2016winter/NPB3.3-MPI/Unimem -mcmodel=large  -lnuma -lpthread -lnumap

mg.o:		mg.f  globals.h mpinpb.h npbparams.h
	${FCOMPILE} mg.f

clean:
	- rm -f *.o *~ 
	- rm -f npbparams.h core



