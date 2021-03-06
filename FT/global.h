      include 'npbparams.h'

c 2D processor array -> 2D grid decomposition (by pencils)
c If processor array is 1xN or -> 1D grid decomposition (by planes)
c If processor array is 1x1 -> 0D grid decomposition
c For simplicity, do not treat Nx1 (np2 = 1) specially
      integer np1, np2, np
c       pointer(ptr_np, np)
c basic decomposition strategy
      integer layout_type
      integer layout_0D, layout_1D, layout_2D
c		       pointer(ptr_layout_type, layout_type)
      parameter (layout_0D = 0, layout_1D = 1, layout_2D = 2)
                       pointer(ptr_layout_0D, layout_0D)
                       pointer(ptr_layout_1D, layout_1D)
                       pointer(ptr_layout_2D, layout_2D)
      common /procgrid/ np1, np2, layout_type, np


c Cache blocking params. These values are good for most
c RISC processors.  
c FFT parameters:
c  fftblock controls how many ffts are done at a time. 
c  The default is appropriate for most cache-based machines
c  On vector machines, the FFT can be vectorized with vector
c  length equal to the block size, so the block size should
c  be as large as possible. This is the size of the smallest
c  dimension of the problem: 128 for class A, 256 for class B and
c  512 for class C.
c Transpose parameters:
c  transblock is the blocking factor for the transposes when there
c  is a 1-D layout. On vector machines it should probably be
c  large (largest dimension of the problem).


      integer fftblock_default, fftblockpad_default
      pointer(ptr_fftblock_default, fftblock_default)
      pointer(ptr_fftblockpad_default, fftblockpad_default)

      parameter (fftblock_default=16, fftblockpad_default=18)
      integer transblock, transblockpad
      pointer(ptr_transblock, transblock)
      pointer(ptr_transblockpad, transblockpad)

      parameter(transblock=32, transblockpad=34)
      
      integer fftblock, fftblockpad
      pointer(ptr_fftblock, fftblock)
     
      common /blockinfo/ ptr_fftblock, fftblockpad

c we need a bunch of logic to keep track of how
c arrays are laid out. 
c coords of this processor
      integer me, me1, me2
      pointer(ptr_me, me)
      pointer(ptr_me1, me1)
      pointer(ptr_me2, me2)

      common /coords/ ptr_me, ptr_me1, ptr_me2
c need a communicator for row/col in processor grid
      integer commslice1, commslice2
      pointer(ptr_commslice1, commslice1)
      pointer(ptr_commslice2, commslice2)

      common /comms/ ptr_commslice1, ptr_commslice2



c There are basically three stages
c 1: x-y-z layout
c 2: after x-transform (before y)
c 3: after y-transform (before z)
c The computation proceeds logically as

c set up initial conditions
c fftx(1)
c transpose (1->2)
c ffty(2)
c transpose (2->3)
c fftz(3)
c time evolution
c fftz(3)
c transpose (3->2)
c ffty(2)
c transpose (2->1)
c fftx(1)
c compute residual(1)

c for the 0D, 1D, 2D strategies, the layouts look like xxx
c        
c            0D        1D        2D
c 1:        xyz       xyz       xyz
c 2:        xyz       xyz       yxz
c 3:        xyz       zyx       zxy

c the array dimensions are stored in dims(coord, phase)
      integer dims(3, 3)
      integer xstart(3), ystart(3), zstart(3)
      integer xend(3), yend(3), zend(3)
      pointer(ptr_dims, dims)
      pointer(ptr_xstart, xstart)
      pointer(ptr_ystart, ystart)
      pointer(ptr_zstart, zstart)
      pointer(ptr_xend, xend)
      pointer(ptr_yend, yend)
      pointer(ptr_zend, zend)
 
      common /layout/ ptr_dims,
     >                ptr_xstart, ptr_ystart, ptr_zstart, 
     >                ptr_xend, ptr_yend, ptr_zend

      integer T_total, T_setup, T_fft, T_evolve, T_checksum, 
     >        T_fftlow, T_fftcopy, T_transpose, 
     >        T_transxzloc, T_transxzglo, T_transxzfin, 
     >        T_transxyloc, T_transxyglo, T_transxyfin, 
     >        T_synch, T_init, T_max

      pointer(ptr_T_toal, T_total)
      pointer(ptr_T_setup, T_setup)
      pointer(ptr_T_fft, T_fft)
      pointer(ptr_T_evolve, T_evolve)
      pointer(ptr_T_checksum, T_checksum)
      pointer(ptr_T_fftlow, T_fftlow)
      pointer(ptr_T_fftcopy, T_fftcopy)
      pointer(ptr_T_transpose, T_transpose)
      pointer(ptr_T_transxzloc, T_transxzloc)
      pointer(ptr_T_transxzglo, T_transxzglo)
      pointer(ptr_T_transxzfin, T_transxzfin)
      pointer(ptr_T_transxyloc, T_transxyloc)
      pointer(ptr_T_transxyglo, T_transxyglo)
      pointer(ptr_T_transxyfin, T_transxyfin)
      pointer(ptr_T_synch, T_synch)
      pointer(ptr_T_init, T_init)
      pointer(ptr_T_max, T_max)

      parameter (T_total = 1, T_setup = 2, T_fft = 3, 
     >           T_evolve = 4, T_checksum = 5, 
     >           T_fftlow = 6, T_fftcopy = 7, T_transpose = 8,
     >           T_transxzloc = 9, T_transxzglo = 10, T_transxzfin = 11, 
     >           T_transxyloc = 12, T_transxyglo = 13, 
     >           T_transxyfin = 14,  T_synch = 15, T_init = 16,
     >           T_max = 16)



      logical timers_enabled
      pointer(ptr_timers_enabled, timers_enabled)

      external timer_read
      double precision timer_read
      external ilog2
      integer ilog2

      external randlc
      double precision randlc


c other stuff
      logical debug, debugsynch
      pointer(ptr_debug, debug)
      pointer(ptr_debugsynch, debugsynch)

      common /dbg/ ptr_debug, ptr_debugsynch, ptr_timers_enabled

      double precision seed, a, pi, alpha
       pointer(ptr_seed, seed)
       pointer(ptr_a, a)
       pointer(ptr_pi, pi)
       pointer(ptr_alpha, alpha)

      parameter (seed = 314159265.d0, a = 1220703125.d0, 
     >  pi = 3.141592653589793238d0, alpha=1.0d-6)

c roots of unity array
c relies on x being largest dimension?
      double complex u(nx)
      pointer(ptr_u, u)

      common /ucomm/ ptr_u


c for checksum data
      double complex sums(0:niter_default)
      pointer(ptr_sums, sums)
      common /sumcomm/ ptr_sums

c number of iterations
      integer niter
      pointer(ptr_niter, niter)

      common /iter/ ptr_niter
