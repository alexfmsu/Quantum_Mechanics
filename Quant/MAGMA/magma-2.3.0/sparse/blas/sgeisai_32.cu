/*
    -- MAGMA (version 2.3.0) --
       Univ. of Tennessee, Knoxville
       Univ. of California, Berkeley
       Univ. of Colorado, Denver
       @date November 2017

       @generated from sparse/blas/zgeisai_32.cu, normal z -> s, Wed Nov 15 00:34:24 2017

*/
#include "magmasparse_internal.h"
#include "shuffle.cuh"

#define PRECISION_s
#define REAL
#define BLOCKSIZE 32
#define WARP_SIZE 32
#define WRP 32
#define WRQ 4


#include <cuda.h>  // for CUDA_VERSION

#if (CUDA_VERSION >= 7000)

__device__
void strsv_lower_32kernel_general(float *dA, float *dB, int *sizes)
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;

    float rB[ 2 ];
    float rA[ 2 ];

    int n;
    int k;
    int N = sizes[j];

    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;


    // Read B to regs.
    #pragma unroll
    for (n = 0; n < 2; n++)
        rB[n] = dB[n*WARP_SIZE+idn];


    // Triangular solve in regs.
    #pragma unroll
    for (k = 0; k < N; k++)
    {
        #pragma unroll
        for (n = 0; n < 2; n++)
            rA[n] = dA[k*WARP_SIZE+n*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB[k/WARP_SIZE] /= rA[k/WARP_SIZE];

        float top = magmablas_sshfl(rB[k/WARP_SIZE], k%WARP_SIZE);

        #pragma unroll
        for (n = 0; n < 2; n++)
            if (n*WARP_SIZE+idn > k)
                rB[n] -= (top*rA[n]);
    }
    // Drop B to dev mem.
    #pragma unroll
    for (n = 0; n < 2; n++)
        if (n*WARP_SIZE+idn < N)
            dB[n*WARP_SIZE+idn] = rB[n];

#endif
}


__device__
void strsv_upper_32kernel_general(float *dA, float *dB, int *sizes)
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;

    float rB[ 2 ];
    float rA[ 2 ];

    int n;
    int N = sizes[j];

    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;


    // Read B to regs.
    #pragma unroll
    for (n = 0; n < 2; n++)
        rB[n] = dB[n*WARP_SIZE+idn];


    // Triangular solve in regs.
    #pragma unroll
    for (int k = N-1; k > -1; k--)
    {
        #pragma unroll
        for (n = 0; n < 2; n++)
            rA[n] = dA[k*WARP_SIZE+n*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB[k/WARP_SIZE] /= rA[k/WARP_SIZE];

        float top = magmablas_sshfl(rB[k/WARP_SIZE], k%WARP_SIZE);

        #pragma unroll
        for (n = 0; n < 2; n++)
            if (n*WARP_SIZE+idn < k)
                rB[n] -= (top*rA[n]);
    }
    // Drop B to dev mem.
    #pragma unroll
    for (n = 0; n < 2; n++)
        if (n*WARP_SIZE+idn < N)
            dB[n*WARP_SIZE+idn] = rB[n];

#endif
}



__device__
void strsv_lower_32kernel_1(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 1; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_2(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 2; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_3(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 3; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_4(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 4; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_5(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 5; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_6(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 6; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_7(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 7; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_8(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 8; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_9(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 9; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_10(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 10; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_11(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 11; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_12(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 12; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_13(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 13; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_14(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 14; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_15(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 15; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_16(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 16; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_17(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 17; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_18(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 18; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_19(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 19; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_20(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 20; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_21(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 21; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_22(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 22; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_23(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 23; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_24(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 24; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_25(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 25; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_26(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 26; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_27(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 27; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_28(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 28; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_29(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 29; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_30(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 30; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_31(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 31; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_lower_32kernel_32(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 32; k++)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn > k)
            rB -= (top*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}


__global__
void strsv_lower_32kernel_switch(float *dA, float *dB, int *sizes, int num_rows )
{
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    if (j < num_rows) {
        int N = sizes[j];
        switch( N ) {
            case  1:
                strsv_lower_32kernel_1( dA, dB ); break;
            case  2:
                strsv_lower_32kernel_2( dA, dB ); break;
            case  3:
                strsv_lower_32kernel_3( dA, dB ); break;
            case  4:
                strsv_lower_32kernel_4( dA, dB ); break;
            case  5:
                strsv_lower_32kernel_5( dA, dB ); break;
            case  6:
                strsv_lower_32kernel_6( dA, dB ); break;
            case  7:
                strsv_lower_32kernel_7( dA, dB ); break;
            case  8:
                strsv_lower_32kernel_8( dA, dB ); break;
            case  9:
                strsv_lower_32kernel_9( dA, dB ); break;
            case  10:
                strsv_lower_32kernel_10( dA, dB ); break;
            case  11:
                strsv_lower_32kernel_11( dA, dB ); break;
            case  12:
                strsv_lower_32kernel_12( dA, dB ); break;
            case  13:
                strsv_lower_32kernel_13( dA, dB ); break;
            case  14:
                strsv_lower_32kernel_14( dA, dB ); break;
            case  15:
                strsv_lower_32kernel_15( dA, dB ); break;
            case  16:
                strsv_lower_32kernel_16( dA, dB ); break;
            case  17:
                strsv_lower_32kernel_17( dA, dB ); break;
            case  18:
                strsv_lower_32kernel_18( dA, dB ); break;
            case  19:
                strsv_lower_32kernel_19( dA, dB ); break;
            case  20:
                strsv_lower_32kernel_20( dA, dB ); break;
            case  21:
                strsv_lower_32kernel_21( dA, dB ); break;
            case  22:
                strsv_lower_32kernel_22( dA, dB ); break;
            case  23:
                strsv_lower_32kernel_23( dA, dB ); break;
            case  24:
                strsv_lower_32kernel_24( dA, dB ); break;
            case  25:
                strsv_lower_32kernel_25( dA, dB ); break;
            case  26:
                strsv_lower_32kernel_26( dA, dB ); break;
            case  27:
                strsv_lower_32kernel_27( dA, dB ); break;
            case  28:
                strsv_lower_32kernel_28( dA, dB ); break;
            case  29:
                strsv_lower_32kernel_29( dA, dB ); break;
            case  30:
                strsv_lower_32kernel_30( dA, dB ); break;
            case  31:
                strsv_lower_32kernel_31( dA, dB ); break;
            case  32:
                strsv_lower_32kernel_32( dA, dB ); break;
            default:
                strsv_lower_32kernel_general( dA, dB, sizes ); break;
        }
    }
}
__device__
void strsv_upper_32kernel_1(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 1-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_2(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 2-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_3(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 3-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_4(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 4-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_5(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 5-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_6(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 6-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_7(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 7-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_8(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 8-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_9(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 9-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_10(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 10-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_11(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 11-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_12(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 12-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_13(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 13-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_14(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 14-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_15(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 15-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_16(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 16-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_17(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 17-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_18(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 18-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_19(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 19-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_20(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 20-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_21(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 21-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_22(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 22-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_23(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 23-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_24(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 24-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_25(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 25-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_26(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 26-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_27(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 27-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_28(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 28-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_29(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 29-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_30(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 30-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_31(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 31-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}



__device__
void strsv_upper_32kernel_32(float *dA, float *dB )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int idn = threadIdx.x;
    float rB;
    float rA;
    dA += (j)*WARP_SIZE*WARP_SIZE;
    dB += (j)*WARP_SIZE;

    // Read B to regs.
    rB = dB[idn];
    // Triangular solve in regs.
    #pragma unroll
    for (int k = 32-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+idn];
        if (k%WARP_SIZE == idn)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( idn < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev mem.
    dB[idn] = rB;
#endif
}


__global__
void strsv_upper_32kernel_switch(float *dA, float *dB, int *sizes, int num_rows )
{
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    if (j < num_rows) {
        int N = sizes[j];
        switch( N ) {
            case  1:
                strsv_upper_32kernel_1( dA, dB ); break;
            case  2:
                strsv_upper_32kernel_2( dA, dB ); break;
            case  3:
                strsv_upper_32kernel_3( dA, dB ); break;
            case  4:
                strsv_upper_32kernel_4( dA, dB ); break;
            case  5:
                strsv_upper_32kernel_5( dA, dB ); break;
            case  6:
                strsv_upper_32kernel_6( dA, dB ); break;
            case  7:
                strsv_upper_32kernel_7( dA, dB ); break;
            case  8:
                strsv_upper_32kernel_8( dA, dB ); break;
            case  9:
                strsv_upper_32kernel_9( dA, dB ); break;
            case  10:
                strsv_upper_32kernel_10( dA, dB ); break;
            case  11:
                strsv_upper_32kernel_11( dA, dB ); break;
            case  12:
                strsv_upper_32kernel_12( dA, dB ); break;
            case  13:
                strsv_upper_32kernel_13( dA, dB ); break;
            case  14:
                strsv_upper_32kernel_14( dA, dB ); break;
            case  15:
                strsv_upper_32kernel_15( dA, dB ); break;
            case  16:
                strsv_upper_32kernel_16( dA, dB ); break;
            case  17:
                strsv_upper_32kernel_17( dA, dB ); break;
            case  18:
                strsv_upper_32kernel_18( dA, dB ); break;
            case  19:
                strsv_upper_32kernel_19( dA, dB ); break;
            case  20:
                strsv_upper_32kernel_20( dA, dB ); break;
            case  21:
                strsv_upper_32kernel_21( dA, dB ); break;
            case  22:
                strsv_upper_32kernel_22( dA, dB ); break;
            case  23:
                strsv_upper_32kernel_23( dA, dB ); break;
            case  24:
                strsv_upper_32kernel_24( dA, dB ); break;
            case  25:
                strsv_upper_32kernel_25( dA, dB ); break;
            case  26:
                strsv_upper_32kernel_26( dA, dB ); break;
            case  27:
                strsv_upper_32kernel_27( dA, dB ); break;
            case  28:
                strsv_upper_32kernel_28( dA, dB ); break;
            case  29:
                strsv_upper_32kernel_29( dA, dB ); break;
            case  30:
                strsv_upper_32kernel_30( dA, dB ); break;
            case  31:
                strsv_upper_32kernel_31( dA, dB ); break;
            case  32:
                strsv_upper_32kernel_32( dA, dB ); break;
            default:
                strsv_upper_32kernel_general( dA, dB, sizes ); break;
        }
    }
}


// initialize arrays with zero
__global__ void
magma_sgpumemzero_32kernel(
    float * d,
    int n,
    int dim_x,
    int dim_y )
{
    int i = blockIdx.y * gridDim.x + blockIdx.x;
    int idx = threadIdx.x;

    if( i >= n ){
       return;
    }
    if( idx >= dim_x ){
       return;
    }

    for( int j=0; j<dim_y; j++)
        d[ i*dim_x*dim_y + j*dim_y + idx ] = MAGMA_S_MAKE( 0.0, 0.0 );
}

__global__ void
magma_slocations_lower_32kernel(
    magma_int_t n,
    magma_index_t *row,
    magma_index_t *col,
    float *val,
    magma_index_t *sizes,
    magma_index_t *locations,
    float *trisystems,
    float *rhs )
{
    int i = threadIdx.x;
    int j = blockIdx.y * gridDim.x + blockIdx.x;

    if( j >= n ){
        return;
    }
    int start = row[j];
    int end = row[j+1];
    int count = end-start;
    if( i == 0 ){
        sizes[j] = count;
        rhs[ j*WARP_SIZE ] = MAGMA_S_ONE;
    }

    if ( i<count ){
        locations[ j*WARP_SIZE + i ] = col[ row[j]+i ];
    }
}// kernel


__global__ void
magma_slocations_trunc_lower_32kernel(
    magma_int_t n,
    magma_index_t *row,
    magma_index_t *col,
    float *val,
    magma_index_t *sizes,
    magma_index_t *locations,
    float *trisystems,
    float *rhs )
{
    int i = threadIdx.x;
    int j = blockIdx.y * gridDim.x + blockIdx.x;

    if( j >= n ){
        return;
    }
    int start = row[j];
    int end = row[j+1];
    int count = end-start;

    // normal case
    if( count <= BLOCKSIZE ){ // normal case
        if( i == 0 ){
            sizes[j] = count;
            rhs[ j*WARP_SIZE ] = MAGMA_S_ONE;
        }
        if ( i<count ){
            locations[ j*WARP_SIZE + i ] = col[ row[j]+i ];
        }
    }
    else {
        // truncate in this row to the blocksize,
        // take only the 32 elements close to the main diagonal into account
        count = BLOCKSIZE;
        if (i == 0) {
            sizes[j] = count;
            rhs[ j*WARP_SIZE ] = MAGMA_S_ONE;
        }

        locations[ j*WARP_SIZE + i ] = col[ row[j+1]-BLOCKSIZE+i ];
    }
}// kernel



__global__ void
magma_slocations_upper_32kernel(
    magma_int_t n,
    magma_index_t *row,
    magma_index_t *col,
    float *val,
    magma_index_t *sizes,
    magma_index_t *locations,
    float *trisystems,
    float *rhs )
{
    int i = threadIdx.x;
    int j = blockIdx.y * gridDim.x + blockIdx.x;

    if( j >= n ){
        return;
    }
    int start = row[j];
    int end = row[j+1];
    int count = end-start;
    if( i == 0 ){
        sizes[j] = count;
        rhs[ j*WARP_SIZE+count-1 ] = MAGMA_S_ONE;
    }

    if ( i<count ){
        locations[ j*WARP_SIZE + i ] = col[ row[j]+i ];
    }
}// kernel

__global__ void
magma_slocations_trunc_upper_32kernel(
    magma_int_t n,
    magma_index_t *row,
    magma_index_t *col,
    float *val,
    magma_index_t *sizes,
    magma_index_t *locations,
    float *trisystems,
    float *rhs )
{
    int i = threadIdx.x;
    int j = blockIdx.y * gridDim.x + blockIdx.x;

    if( j >= n ){
        return;
    }
    int start = row[j];
    int end = row[j+1];
    int count = end-start;

    // normal case
    if( count <= BLOCKSIZE ){ // normal case
        if( i == 0 ){
            sizes[j] = count;
            rhs[ j*WARP_SIZE+count-1 ] = MAGMA_S_ONE;
        }
        if ( i<count ){
            locations[ j*WARP_SIZE + i ] = col[ row[j]+i ];
        }
    }
    else {
        // truncate in this row to the blocksize,
        // take only the 32 elements close to the main diagonal into account
        count = BLOCKSIZE;
        if (i == 0) {
            sizes[j] = count;
            rhs[ j*WARP_SIZE+count-1 ] = MAGMA_S_ONE;
        }

        locations[ j*WARP_SIZE + i ] = col[ row[j]+i ];
    }
}// kernel

__global__ void
magma_sfilltrisystems_32kernel(
    magma_int_t offset,
    magma_int_t limit,
    magma_index_t *row,
    magma_index_t *col,
    float *val,
    magma_index_t *sizes,
    magma_index_t *locations,
    float *trisystems,
    float *rhs )
{
    int i = (blockDim.x * blockIdx.x + threadIdx.x)+offset;
    int ii = (blockDim.x * blockIdx.x + threadIdx.x);

    if ( ii>=limit ){
        return;
    }
    //if ( i<offset ){
    //    return;
    //}

    for( int j=0; j<sizes[ i ]; j++ ){// no need for first
        int k = row[ locations[ j+i*WARP_SIZE ] ];
        int l = i*WARP_SIZE;
        int idx = 0;
        while( k < row[ locations[ j+i*WARP_SIZE ]+1 ] && l < (i+1)*WARP_SIZE ){ // stop once this column is done
            if( locations[ l ] == col[k] ){ //match
                // int loc = i*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx;
                trisystems[ ii*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx ]
                                                        = val[ k ];
                k++;
                l++;
                idx++;
            } else if( col[k] < locations[ l ] ){// need to check next element
                k++;
            } else { // element does not exist, i.e. l < LC.col[k]
                // printf("increment l\n");
                l++; // check next elment in the sparsity pattern
                idx++; // leave this element equal zero
            }
        }
    }
}// kernel


__global__ void
magma_sbackinsert_32kernel(
    magma_int_t n,
    magma_index_t *row,
    magma_index_t *col,
    float *val,
    magma_index_t *sizes,
    float *rhs )
{
    int i = threadIdx.x;
    int j = blockIdx.y * gridDim.x + blockIdx.x;
    int end = sizes[j];
    if( j >= n ){
        return;
    }

    if ( i>=end ){
        return;
    }

    val[row[j]+i] = rhs[j*WARP_SIZE+i];
}// kernel


// try to do everything in shared memory and registers!
//one thread block per row of A

__global__ void
magma_slowertrisystems_32kernel_s(
    magma_int_t n,
    magma_index_t *Arow,
    magma_index_t *Acol,
    float *Aval,
    magma_index_t *Mrow,
    magma_index_t *Mcol,
    float *Mval,
    magma_index_t *sizes,
    magma_index_t *locations )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int row = blockIdx.y * gridDim.x + blockIdx.x;
    int tid = threadIdx.x;
    float rB;    // registers for trsv
    float rA;

    __shared__ float dA[32*32];

    // only if within this chunk
    if ( row>=n ){
        return;
    }

    // only if within the size
    int size = sizes[ row ];
    if( tid >= size ){
        return;
    }

    // set dA to 0
    for( int j=0; j<32; j++ ){
        dA[ j*32 + tid ] = MAGMA_S_ZERO;
    }
    /*
    // for debuggging: let thred 0 do everything
    if (tid == 0) {
        // first: generate the triangular systems
        for (int j=0; j<size; j++) { // no need for first
            int k = Arow[ locations[ j+row*WARP_SIZE ] ];
            int l = row*WARP_SIZE;
            int idx = 0;
            while (k < Arow[ locations[ j+row*WARP_SIZE ]+1 ] && l < (row+1)*WARP_SIZE) { // stop once this column is done
                if (locations[ l ] == Acol[k]) { // match
                    // int loc = i*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx;
                    dA[ j*32 + idx ] = Aval[ k ];
                    k++;
                    l++;
                    idx++;
                }
                else if (Acol[k] < locations[ l ]) { // need to check next element
                    k++;
                }
                else { // element does not exist, i.e. l < LC.col[k]
                    l++; // check next elment in the sparsity pattern
                    idx++; // leave this element equal zero
                }
            }
        }
    }
    __syncthreads();
    */
    int k = Arow[ locations[ tid+row*WARP_SIZE ] ];
    int l = row*WARP_SIZE;
    int idx = 0;
    while( k < Arow[ locations[ tid+row*WARP_SIZE ]+1 ] && l < (row+1)*WARP_SIZE ){ // stop once this column is done
        if( locations[ l ] == Acol[k] ){ //match
            // int loc = i*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx;
            dA[ tid*32 + idx ] = Aval[ k ];
            k++;
            l++;
            idx++;
        } else if( Acol[k] < locations[ l ] ){// need to check next element
            k++;
        } else { // element does not exist, i.e. l < LC.col[k]
            l++; // check next elment in the sparsity pattern
            idx++; // leave this element equal zero
        }
    }


    // second: solve the triangular systems - in registers

    // Read B to regs.
    rB = (tid == 0) ? MAGMA_S_ONE : MAGMA_S_ZERO;

        // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 32; k++)
    {
        rA = dA[k*WARP_SIZE+tid];
        if (k%WARP_SIZE == tid)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( tid > k)
            rB -= (top*rA);
    }
    // Drop B to dev memory - in ISAI preconditioner M
    Mval[ Mrow[row] + tid ] = rB;

#endif
}// kernel


__global__ void
magma_suppertrisystems_32kernel_s(
    magma_int_t n,
    magma_index_t *Arow,
    magma_index_t *Acol,
    float *Aval,
    magma_index_t *Mrow,
    magma_index_t *Mcol,
    float *Mval,
    magma_index_t *sizes,
    magma_index_t *locations )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int row = blockIdx.y * gridDim.x + blockIdx.x;
    int tid = threadIdx.x;
    float rB;    // registers for trsv
    float rA;

    __shared__ float dA[32*32];

    // only if within this chunk
    if ( row>=n ){
        return;
    }

    // only if within the size
    int size = sizes[ row ];
    if( tid >= size ){
        return;
    }

    // set dA to 0
    for( int j=0; j<32; j++ ){
        dA[ j*32 + tid ] = MAGMA_S_ZERO;
    }
    /*
    // for debuggging: let thred 0 do everything
    if (tid == 0) {
        // first: generate the triangular systems
        for (int j=0; j < size; j++) { // no need for first
            int k = Arow[ locations[ j+row*WARP_SIZE ] ];
            int l = row*WARP_SIZE;
            int idx = 0;
            while (k < Arow[ locations[ j+row*WARP_SIZE ]+1 ] && l < (row+1)*WARP_SIZE) { // stop once this column is done
                if (locations[ l ] == Acol[k]) { // match
                    // int loc = i*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx;
                    dA[ j*32 + idx ] = Aval[ k ];
                    k++;
                    l++;
                    idx++;
                }
                else if (Acol[k] < locations[ l ]) { // need to check next element
                    k++;
                }
                else { // element does not exist, i.e. l < LC.col[k]
                    l++; // check next elment in the sparsity pattern
                    idx++; // leave this element equal zero
                }
            }
        }
    }
    __syncthreads();
    */
    int k = Arow[ locations[ tid+row*WARP_SIZE ] ];
    int l = row*WARP_SIZE;
    int idx = 0;
    while( k < Arow[ locations[ tid+row*WARP_SIZE ]+1 ] && l < (row+1)*WARP_SIZE ){ // stop once this column is done
        if( locations[ l ] == Acol[k] ){ //match
            // int loc = i*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx;
            dA[ tid*32 + idx ] = Aval[ k ];
            k++;
            l++;
            idx++;
        } else if( Acol[k] < locations[ l ] ){// need to check next element
            k++;
        } else { // element does not exist, i.e. l < LC.col[k]
            l++; // check next elment in the sparsity pattern
            idx++; // leave this element equal zero
        }
    }


    // second: solve the triangular systems - in registers

    // Read B to regs.
    rB = (tid == size-1) ? MAGMA_S_ONE : MAGMA_S_ZERO;

    // Triangular solve in regs.
    #pragma unroll
    for (int k = 32-1; k >-1; k--)
    {
        rA = dA[k*WARP_SIZE+tid];
        if (k%WARP_SIZE == tid)
            rB /= rA;
        float bottom = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( tid < k)
            rB -= (bottom*rA);
    }
    // Drop B to dev memory - in ISAI preconditioner M
    Mval[ Mrow[row] + tid ] = rB;

#endif
}// kernel


__global__ void
magma_slowertrisystems_32kernel(
    magma_int_t n,
    magma_index_t *Arow,
    magma_index_t *Acol,
    float *Aval,
    magma_index_t *Mrow,
    magma_index_t *Mcol,
    float *Mval,
    magma_index_t *sizes,
    magma_index_t *locations )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int row = blockIdx.y * gridDim.x + blockIdx.x;
    int tid = threadIdx.x;
    float rB;    // registers for trsv
    float rA;

    float dA[32];

    // only if within this chunk
    if ( row>=n ){
        return;
    }

    // only if within the size
    int size = sizes[ row ];
    if( tid >= size ){
        return;
    }

    // set dA to 0
    for( int j=0; j<32; j++ ){
        dA[ j ] = MAGMA_S_ZERO;
    }

    // for debuggging: let thred 0 do everything
    //if(tid==0){
    {
    // first: generate the triangular systems
    #pragma unroll
    for( int j=0; j<size; j++ ){// no need for first
        int k = Arow[ locations[ j+row*WARP_SIZE ] ];
        int l = row*WARP_SIZE;
        int idx = 0;
        while( k < Arow[ locations[ j+row*WARP_SIZE ]+1 ] && l < (row+1)*WARP_SIZE ){ // stop once this column is done
            if( locations[ l ] == Acol[k] ){ //match
                if( tid == idx ){
                    dA[ j ] = Aval[ k ];
                }
                //__syncthreads();
                // int loc = i*WARP_SIZE*WARP_SIZE + j*WARP_SIZE + idx;
                k++;
                l++;
                idx++;
            } else if( Acol[k] < locations[ l ] ){// need to check next element
                k++;
            } else { // element does not exist, i.e. l < LC.col[k]
                l++; // check next elment in the sparsity pattern
                idx++; // leave this element equal zero
            }
        }
    }
    }
    // not sure whether we need this here....
    //__syncthreads();


    // second: solve the triangular systems - in registers

    // Read B to regs.
    rB = (tid == 0) ? MAGMA_S_ONE : MAGMA_S_ZERO;

    // Triangular solve in regs.
    #pragma unroll
    for (int k = 0; k < 32; k++)
    {
        rA = dA[ k ];
        if (k%WARP_SIZE == tid)
            rB /= rA;
        float top = magmablas_sshfl(rB, k%WARP_SIZE);
        if ( tid > k)
            rB -= (top*rA);
    }
    // Drop B to dev memory - in ISAI preconditioner M
    Mval[ Mrow[row] + tid ] = rB;

#endif
}// kernel


__global__ void
magma_suppertrisystems_32kernel(
    magma_int_t n,
    const magma_index_t * __restrict__ Arow,
    const magma_index_t * __restrict__ Acol,
    const float * __restrict__ Aval,
    magma_index_t *Mrow,
    magma_index_t *Mcol,
    float *Mval )
{
#if (defined( REAL ) && ( __CUDA_ARCH__ >= 300 ))
    int row = blockIdx.y * gridDim.x + blockIdx.x;
    int tid = threadIdx.x;

    float rB;    // registers for trsv
    float rA[32];

    // only if within this chunk
    if ( row>=n ){
        return;
    }

    // only if within the size
    int mstart = Mrow[ row ];
    int mlim = Mrow[ row+1 ];
    int size = mlim - mstart;
    if( tid >= size ){
        return;
    }

    // set rA to 0
    for( int j=0; j<32; j++ ){
        rA[ j ] = MAGMA_S_ZERO;
    }

    // generate the triangular systems
    #pragma unroll
    for( int j=0; j<size; j++ ){// no need for first
        int t = Mcol[ mstart + j ];
        int k = Arow[ t ];
        int l = mstart;
        int idx = 0;
        while( k < Arow[ t+1 ] && l < mlim ){ // stop once this column is done
            int mcol =  Mcol[ l ];
            int acol = Acol[k];
            if( mcol == acol ){ //match
                if( tid == idx ){
                    rA[ j ] = Aval[ k ];
                }
                k++;
                l++;
                idx++;
            } else if( acol < mcol ){// need to check next element
                k++;
            } else { // element does not exist, i.e. l < LC.col[k]
                l++; // check next elment in the sparsity pattern
                idx++; // leave this element equal zero
            }
        }
    }

    // second: solve the triangular systems - in registers
    // we know how RHS looks like
    rB = (tid == size-1) ? MAGMA_S_ONE : MAGMA_S_ZERO;

    // Triangular solve in regs.
    #pragma unroll
    for (int k = 32-1; k >-1; k--)
    {
        if (k%32 == tid)
            rB /= rA[k];
        float bottom = magmablas_sshfl(rB, k%32);
        if ( tid < k)
            rB -= (bottom*rA[k]);
    }
    // Drop B to dev memory - in ISAI preconditioner M
    Mval[ mstart + tid ] = rB;

#endif
}// kernel

#endif  // CUDA >= 7000


/**
    Purpose
    -------
    This routine is designet to combine all kernels into one.

    Arguments
    ---------


    @param[in]
    uplotype    magma_uplo_t
                lower or upper triangular

    @param[in]
    transtype   magma_trans_t
                possibility for transposed matrix

    @param[in]
    diagtype    magma_diag_t
                unit diagonal or not

    @param[in]
    L           magma_s_matrix
                triangular factor for which the ISAI matrix is computed.
                Col-Major CSR storage.

    @param[in,out]
    M           magma_s_matrix*
                SPAI preconditioner CSR col-major

    @param[out]
    sizes       magma_int_t*
                Number of Elements that are replaced.

    @param[out]
    locations   magma_int_t*
                Array indicating the locations.

    @param[out]
    trisystems  float*
                trisystems

    @param[out]
    rhs         float*
                right-hand sides

    @param[in]
    queue       magma_queue_t
                Queue to execute in.

    @ingroup magmasparse_saux
    ********************************************************************/

extern "C" magma_int_t
magma_sisaigenerator_32_gpu(
    magma_uplo_t uplotype,
    magma_trans_t transtype,
    magma_diag_t diagtype,
    magma_s_matrix L,
    magma_s_matrix *M,
    magma_index_t *sizes,
    magma_index_t *locations,
    float *trisystems,
    float *rhs,
    magma_queue_t queue )
{
    magma_int_t info = 0;

#if (CUDA_VERSION >= 7000)
    magma_int_t arch = magma_getdevice_arch();

    cudaDeviceSetCacheConfig( cudaFuncCachePreferL1 );

    // routine 1
    int r1bs1 = WARP_SIZE;
    int r1bs2 = 1;
    int r1dg1 = min( int( sqrt( float( M->num_rows ))), 65535 );
    int r1dg2 = min(magma_ceildiv( M->num_rows, r1dg1 ), 65535);
    int r1dg3 = magma_ceildiv( M->num_rows, r1dg1*r1dg2 );

    dim3 r1block( r1bs1, r1bs2, 1 );
    dim3 r1grid( r1dg1, r1dg2, r1dg3 );

    int r2bs1 = WARP_SIZE;
    int r2bs2 = 1;
    int r2dg1 = magma_ceildiv( L.num_rows, r2bs1 );
    int r2dg2 = 1;
    int r2dg3 = 1;
    dim3 r2block( r2bs1, r2bs2, 1 );
    dim3 r2grid( r2dg1, r2dg2, r2dg3 );

    int r3bs1 = WARP_SIZE;
    int r3bs2 = 1;
    int r3dg1 = magma_ceildiv( 32000, r2bs1 );
    int r3dg2 = 1;
    int r3dg3 = 1;
    dim3 r3block( r3bs1, r3bs2, 1 );
    dim3 r3grid( r3dg1, r3dg2, r3dg3 );

    int recursive = magma_ceildiv( M->num_rows, 32000 );

    if (arch >= 300) {
        magma_sgpumemzero_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                rhs, L.num_rows, WARP_SIZE, 1);

        if (uplotype == MagmaLower) {
            magma_slocations_lower_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                            M->num_rows,
                            M->drow,
                            M->dcol,
                            M->dval,
                            sizes,
                            locations,
                            trisystems,
                            rhs );
        }
        else {
            magma_slocations_upper_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                            M->num_rows,
                            M->drow,
                            M->dcol,
                            M->dval,
                            sizes,
                            locations,
                            trisystems,
                            rhs );
        }
        /*
        if (uplotype == MagmaLower) {
            printf("in here lower\n");
            magma_slowertrisystems_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                L.num_rows,
                L.drow,
                L.dcol,
                L.dval,
                M->drow,
                M->dcol,
                M->dval,
                sizes,
                locations );
        }
        else {
            printf("in here upper\n");
            magma_suppertrisystems_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                L.num_rows,
                L.drow,
                L.dcol,
                L.dval,
                M->drow,
                M->dcol,
                M->dval );
        }
        */

        // chunk it recursively into batches of 3200
        for (int z=0; z < recursive; z++) {
            int limit = min(32000, L.num_rows-32000*z);

            magma_sgpumemzero_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                trisystems, limit, WARP_SIZE, WARP_SIZE );

            magma_sfilltrisystems_32kernel<<< r3grid, r3block, 0, queue->cuda_stream() >>>(
                                32000*z,
                                limit,
                                L.drow,
                                L.dcol,
                                L.dval,
                                sizes,
                                locations,
                                trisystems,
                                rhs );

            // routine 2
            if (uplotype == MagmaLower) {
                strsv_lower_32kernel_switch<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                        trisystems,
                        rhs+32000*32*z,
                        sizes+32000*z,
                        limit );
            }
            else {
                strsv_upper_32kernel_switch<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                        trisystems,
                        rhs+32000*32*z,
                        sizes+32000*z,
                        limit );
            }
        }

        // routine 3
        magma_sbackinsert_32kernel<<< r1grid, r1block, 0, queue->cuda_stream() >>>(
                M->num_rows,
                M->drow,
                M->dcol,
                M->dval,
                sizes,
                rhs );
    }
    else {
        info = MAGMA_ERR_NOT_SUPPORTED;
    }
#else
    // CUDA < 7000
    printf( "%% error: ISAI preconditioner requires CUDA > 7.0.\n" );
    info = MAGMA_ERR_NOT_SUPPORTED;
#endif

    return info;
}
