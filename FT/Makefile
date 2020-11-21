SHELL=/bin/sh
BENCHMARK=ft
BENCHMARKU=FT

include ../config/make.def

include ../sys/make.common

OBJS = ft.o ${COMMON}/${RAND}.o ${COMMON}/print_results.o ${COMMON}/timers.o unimem.o

ERROR_FLAGS =  -Wall -Werror
CFLAGS = $(ERROR_FLAGS) -D_REENTRANT -DLinux -D_GNU_SOURCE -O0 -g -mcmodel=large

${PROGRAM}: config ${OBJS}
	${FLINK} ${FLINKFLAGS} -o ${PROGRAM} ${OBJS} ${FMPI_LIB} -L/home/kwu/mpi_project/2016winter/NPB3.3-MPI/Unimem -lnuma -lpthread -lnumap

unimem.o : ../Unimem/unimem.c
	gcc -c ../Unimem/unimem.c  -I/home/kwu/mpi_project/2016winter/NPB3.3-MPI/Unimem -mcmodel=large  -lnuma -lpthread -lnumap

.f.o:
	${FCOMPILE} $<

ft.o:             ft.f  global.h mpinpb.h npbparams.h

clean:
	- rm -f *.o *~ mputil*
	- rm -f ft npbparams.h core
