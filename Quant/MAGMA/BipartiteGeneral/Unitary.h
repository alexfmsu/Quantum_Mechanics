#pragma once

#include <iostream>

#include "matrix.h"
#include "Types.h"

Matrix getU(Matrix* H, double dt) {
    int M = H->M();
    int N = H->N();

    magma_int_t n2 = M * N;

    Matrix a(M, N);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            a.set(i, j, H->get(i, j));
        }
    }

    double *s1;
    magma_int_t info ;
    magma_int_t ione = 1;
    double error = 1.;
    double mone = -1.0;
    magma_int_t lwork;
    magma_int_t min_mn = min(M, N);

    double* rwork;

    magma_int_t* iwork = new magma_int_t[N];
    magma_int_t liwork = 3 + 5 * N;

    magma_dmalloc_cpu(&s1 , min_mn );


    magma_int_t nb = magma_get_zhetrd_nb(N);

    lwork = max( N + N * nb, 2 * N + N * N );
    magma_int_t lrwork = 1 + 5 * N + 2 * N * N;

    magma_dmalloc_pinned (& rwork , lrwork);
    complexd* work;

    magma_zmalloc_pinned(&work, lwork);

    magma_int_t aux_iwork[1], lda = N;
    magmaDoubleComplex aux_work[1];
    double aux_rwork[1];

    magma_zheevd( MagmaVec, MagmaLower,
                  N, NULL,
                  lda, NULL,
                  aux_work,  -1,
                  aux_rwork, -1,
                  aux_iwork, -1,
                  &info );

    lwork = (magma_int_t) MAGMA_Z_REAL( aux_work[0] );
    lrwork = (magma_int_t) aux_rwork[0];
    liwork = aux_iwork[0];

    magmaDoubleComplex *h_A, *h_R, *h_work;
    double *w1, *w2, result[3], eps;

    magma_queue_t queue = NULL ;
    magma_device_t device;
    magma_queue_create(device, &queue);

    magma_zmalloc_cpu(&h_A, N * lda);
    magma_zsetmatrix(H->M(), H->N(), H->cpu_data(), H->M(), h_R, H->M(), queue);

    magma_dmalloc_cpu(&w1, N);
    magma_dmalloc_cpu(&w2, N);
    magma_dmalloc_cpu(&rwork, lrwork);
    magma_imalloc_cpu(&iwork, liwork);

    magma_zmalloc_pinned(&h_R, N * lda);

    magma_zmalloc_pinned(&h_work, lwork);

    magma_zheevd( MagmaVec, MagmaLower,
                  N, a.cpu_data(), lda, w1,
                  h_work, lwork,
                  rwork, lrwork,
                  iwork, liwork,
                  &info );


    Matrix V(M, M);
    Matrix D(M, M);

    Matrix VD(M, M);
    Matrix U(M, M);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            V.set(i, j, a.get(i, j));
        }

        D.set(i, i, w1[i]);
    }

    D.exp_diag(Complexd(0, -dt));

    magmaDoubleComplex alpha = MAGMA_Z_MAKE(1, 0);
    magmaDoubleComplex beta = MAGMA_Z_MAKE(0, 0);

    magma_zsetmatrix(M, M, V.cpu_data(), M, V.gpu_data(), M, queue);
    magma_zsetmatrix(M, M, D.cpu_data(), M, D.gpu_data(), M, queue);
    magma_zsetmatrix(M, M, VD.cpu_data(), M, VD.gpu_data(), M, queue);
    magma_zsetmatrix(M, M, U.cpu_data(), M, U.gpu_data(), M, queue);

    magma_zgemm(MagmaNoTrans, MagmaNoTrans, M, M, M, alpha, V.gpu_data(), M, D.gpu_data(), M, beta, VD.gpu_data(), M, queue);
    magma_zgetmatrix(M, M, VD.gpu_data(), M, VD.cpu_data(), M, queue);

    magma_zsetmatrix(M, M, V.cpu_data(), M, V.gpu_data(), M, queue);

    magma_zgemm(MagmaNoTrans, MagmaTrans, M, M, M, alpha, VD.gpu_data(), M, V.gpu_data(), M, beta, U.gpu_data(), M, queue);
    magma_zgetmatrix(M, M, U.gpu_data(), M, U.cpu_data(), M, queue);

    free(s1);
    free(w1);
    free(w2);
    free(rwork);
    free(iwork);

    magma_free_pinned(h_work);
    magma_free_pinned(h_R);

    return U;
}
