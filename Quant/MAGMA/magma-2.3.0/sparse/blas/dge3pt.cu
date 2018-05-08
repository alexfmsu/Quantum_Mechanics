/*
    -- MAGMA (version 2.3.0) --
       Univ. of Tennessee, Knoxville
       Univ. of California, Berkeley
       Univ. of Colorado, Denver
       @date November 2017

       @generated from sparse/blas/zge3pt.cu, normal z -> d, Wed Nov 15 00:34:24 2017

*/
#include "magmasparse_internal.h"

#define BLOCK_SIZE 512


// 3-pt stencil kernel
__global__ void 
dge3pt_kernel( 
    int num_rows, 
    int num_cols,
    double alpha,
    double beta,
    double * dx,
    double * dy)
{
    int row = blockDim.x * blockIdx.x + threadIdx.x;
    
    if( row >= num_rows ){
        return;
    } else {
        
        for( int i=0; i<num_cols; i++ ){
            if (row == num_rows-1) {
                dy[ row+i*num_rows ] = alpha * (- 2 * dx[ row+i*num_rows ] 
                                        + dx[ row+i*num_rows-1 ])
                                        + beta * dy[ row+i*num_rows ] ;
            } else if(row == 0) {
                dy[ row+i*num_rows ] = alpha * (- 2 * dx[ row+i*num_rows ] 
                                        + dx[ row+i*num_rows+1 ])
                                        + beta * dy[ row+i*num_rows ] ;
            } else {
                dy[ row+i*num_rows ] = alpha * (- 2 * dx[ row+i*num_rows ] 
                                        + dx[ row+i*num_rows-1 ] + dx[ row+i*num_rows+1 ])
                                        + beta * dy[ row+i*num_rows ] ;
            }
        }
    }
}

/**
    Purpose
    -------
    
    This routine is a 3-pt-stencil operator derived from a FD-scheme in 2D
    with Dirichlet boundary.
    It computes y_i = -2 x_i + x_{i-1} + x_{i+1}
    
    Arguments
    ---------
    
    @param[in]
    m           magma_int_t
                number of rows in x and y

    @param[in]
    n           magma_int_t
                number of columns in x and y
                
    @param[in]
    alpha       double
                scalar multiplier
                
    @param[in]
    beta        double
                scalar multiplier
                
    @param[in]
    dx          magmaDouble_ptr
                input vector x

    @param[out]
    dy          magmaDouble_ptr
                output vector y

    @param[in]
    queue       magma_queue_t
                Queue to execute in.

    @ingroup magmasparse_dblas
    ********************************************************************/

extern "C" magma_int_t
magma_dge3pt(
    magma_int_t m, 
    magma_int_t n,
    double alpha,
    double beta,
    magmaDouble_ptr dx,
    magmaDouble_ptr dy,
    magma_queue_t queue )
{
    dim3 grid( magma_ceildiv( m, BLOCK_SIZE ) );
    magma_int_t threads = BLOCK_SIZE;
    dge3pt_kernel<<< grid, threads, 0, queue->cuda_stream() >>>
                  ( m, n, alpha, beta, dx, dy );
    return MAGMA_SUCCESS;
}
