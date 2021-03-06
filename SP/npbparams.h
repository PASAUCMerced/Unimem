c NPROCS = 16 CLASS = D
c  
c  
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the class of the NPB
c  in this directory. Do not modify it by hand.
c  
        integer maxcells, problem_size, niter_default
        parameter (maxcells=4, problem_size=408, niter_default=500)
        double precision dt_default
        parameter (dt_default = 0.00030d0)
        logical  convertdouble
        parameter (convertdouble = .false.)
        character*11 compiletime
        parameter (compiletime='05 Jan 2017')
        character*5 npbversion
        parameter (npbversion='3.3.1')
        character*6 cs1
        parameter (cs1='mpif77')
        character*9 cs2
        parameter (cs2='$(MPIF77)')
        character*40 cs3
        parameter (cs3='-L/opt/pasa_lib/openmpi-1.10.0/lib -lmpi')
        character*40 cs4
        parameter (cs4='-I/opt/pasa_lib/openmpi-1.10.0/include  ')
        character*46 cs5
        parameter (cs5='-O  -fcray-pointer -mcmodel=large  -fno-ran...')
        character*2 cs6
        parameter (cs6='-O')
        character*6 cs7
        parameter (cs7='randi8')
