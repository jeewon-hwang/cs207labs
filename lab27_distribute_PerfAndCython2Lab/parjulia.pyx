# distutils: extra_compile_args = -fopenmp
# distutils: extra_link_args = -fopenmp
#your code here
cimport cython
import numpy as np
cimport numpy as np
from cython.parallel cimport prange

cdef inline double norm2(double complex z) nogil:
    return z.real*z.real + z.imag*z.imag

cdef int escape(int maxiter, double complex z, double complex c) nogil:
    cdef int n=0
    while n < maxiter and norm2(z) < 4:
        z=z*z+c
        n+=1
    return n

@cython.boundscheck(False)
@cython.wraparound(False)
cpdef run_cython_amalgamated2(int gridsize, box, double complex c, int maxiter):
    #your code here
    cdef double xstep = (box.x2 - box.x1)/(gridsize - 1.0)
    cdef double ystep = (box.y2 - box.y1)/(gridsize - 1.0)
    
    cdef double xstart = box.x1
    cdef double ystart = box.y1
    #zs = []
    #out = []
    
    cdef double complex[:] zs = np.empty(gridsize*gridsize, dtype=complex)
    cdef double complex[:] output = np.empty(gridsize*gridsize, dtype=complex)
    
    cdef unsigned int i, j
    cdef double x,y
    cdef double complex z
    
    for i in prange(gridsize, nogil=True,
                    schedule='static', chunksize=1):
        x = xstart + i*xstep
        for j in range(gridsize):
            y = ystart + i*ystep

            z.real = x
            z.imag = y
            zs[i*gridsize+j] = z
            output[i*gridsize+j] = escape(maxiter, z, c)