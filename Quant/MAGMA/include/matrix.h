#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include "../Common/Types.h"
#include "../Common/File.h"

using namespace std;

class Matrix {

public:
	Matrix() : size_(0) {}

	Matrix(int M, int N) : M_(M), N_(N) {
		size_ = M_ * N_;

		cpu_alloc();
		gpu_alloc();
	}

	int M() const { return M_; }
	int N() const { return N_; }

	int size() const { return size_; }

	magmaDoubleComplex* cpu_data() { return cpu_data_; }
	magmaDoubleComplex* gpu_data() { return gpu_data_; }

	void cpu_alloc() {
		int err = magma_zmalloc_pinned(&cpu_data_, size_);
	}

	void gpu_alloc() {
		int err = magma_zmalloc(&gpu_data_, size_);
	}

	void init() {
		for (int i = 0; i < M_; i++) {
			for (int j = 0; j < N_; j++) {
				cpu_data_[ i * M_ + j ] = MAGMA_Z_MAKE(10 * i + j, 0.0);
				// cpu_data_[ i * M_ + j ] = MAGMA_Z_MAKE(10 * i + j, 0.0);
			}
		}
	}

	void print_diag() const {
		for (int i = 0; i < M_; i++) {
			printf("(%.3f, %.3f) ", MAGMA_Z_REAL(cpu_data_[ i * M_ + i ]), MAGMA_Z_IMAG(cpu_data_[ i * M_ + i ]));
		}

		cout << endl;
	}

	void print() const {
		for (int i = 0; i < M_; i++) {
			for (int j = 0; j < N_; j++) {
				printf("(%.3f, %.3f) ", MAGMA_Z_REAL(cpu_data_[ i * M_ + j ]), MAGMA_Z_IMAG(cpu_data_[ i * M_ + j ]));
				// cout << setw(10) << setprecision(3) << MAGMA_Z_REAL(cpu_data_[ i * M_ + j ]) << " ";
			}

			cout << endl;
		}

		cout << endl;
	}

	void print_re() const {
		for (int i = 0; i < M_; i++) {
			for (int j = 0; j < N_; j++) {
				printf("%.3f ", MAGMA_Z_REAL(cpu_data_[ i * M_ + j ]));
				// cout << setw(10) << setprecision(3) << MAGMA_Z_REAL(cpu_data_[ i * M_ + j ]) << " ";
			}

			cout << endl;
		}
	}

	vector<Complexd> diag() const {
		vector<Complexd> diag_;

		for (int i = 0; i < M_; i++) {
			diag_.push_back(get(i, i));
		}

		return diag_;
	}

	void set(int i, int j, Complexd val) {
		cpu_data_[ i * N_ + j ] = val.value();
	}

	void set(int i, int j, double val) {
		cpu_data_[ i * N_ + j ] = MAGMA_Z_MAKE(val, 0);
	}

	Complexd get(int i, int j) const {
		return Complexd(cpu_data_[ i * N_ + j ]);
	}

	void write_to_file(string filename, int precision = 3) {
		double eps = 1.0 / pow(10, precision);

		ofstream fout;
		cout << filename << endl;
		fout.open(filename, ios::out);

		for (int i = 0; i < M_; i++) {
			for (int j = 0; j < N_; j++) {
				Complexd v = get(i, j);

				double re = v.real();
				double im = v.imag();

				if (abs(re) < eps) {
					re = 0;
				}

				if (abs(im) < eps) {
					im = 0;
				}

				fout << fixed << setprecision(precision) << "(" << re << "," << im << ")";

				if (j != M_ - 1) {
					fout << ",";
				}
			}

			fout << endl;
		}

		fout.close();
	}

	// void friend GEMM(Matrix* A, Matrix* B, Matrix* C, magma_queue_t queue, Complexd* alpha, Complexd* beta, const char _a = 'N', const char _b = 'N') {
	// 	// -----------------------
	// 	magma_trans_t tr_a, tr_b;

	// 	switch (_a) {
	// 	case 'N':
	// 		tr_a = MagmaNoTrans;
	// 		break;
	// 	case 'T':
	// 		tr_a = MagmaTrans;
	// 		break;
	// 	case 'C':
	// 		tr_a = MagmaConjTrans;
	// 		break;
	// 	default:
	// 		break;
	// 	}

	// 	switch (_b) {
	// 	case 'N':
	// 		tr_b = MagmaNoTrans;
	// 		break;
	// 	case 'T':
	// 		tr_b = MagmaTrans;
	// 		break;
	// 	case 'C':
	// 		tr_b = MagmaConjTrans;
	// 		break;
	// 	default:
	// 		break;
	// 	}
	// 	// -----------------------------------------------------------------------------------------------------------
	// 	magma_zsetmatrix(A->M(), A->N(), A->cpu_data(), A->M(), A->gpu_data(), A->M(), queue);
	// 	magma_zsetmatrix(B->M(), B->N(), B->cpu_data(), B->M(), B->gpu_data(), B->M(), queue);
	// 	magma_zsetmatrix(C->M(), C->N(), C->cpu_data(), C->M(), C->gpu_data(), C->M(), queue);
	// 	// -----------------------------------------------------------------------------------------------------------
	// 	magma_zgemm(tr_a, tr_b, A->M(), C->N(), A->N(), alpha->value(), A->gpu_data(), A->M(), B->gpu_data(), A->N(), beta->value(), C->gpu_data(), A->M(), queue);
	// 	// -----------------------------------------------------------------------------------------------------------
	// 	magma_zgetmatrix(C->M(), C->N(), C->gpu_data(), C->M(), C->cpu_data(), C->M(), queue);
	// 	// -----------------------------------------------------------------------------------------------------------
	// }

	void friend GEMM(Matrix* A, Matrix* B, Matrix* C, magma_queue_t queue, Complexd* alpha, Complexd* beta, const char _a = 'N', const char _b = 'N') {
		// -----------------------
		magma_trans_t tr_a, tr_b;

		switch (_a) {
		case 'N':
			tr_a = MagmaNoTrans;
			break;
		case 'T':
			tr_a = MagmaTrans;
			break;
		case 'C':
			tr_a = MagmaConjTrans;
			break;
		default:
			break;
		}

		switch (_b) {
		case 'N':
			tr_b = MagmaNoTrans;
			break;
		case 'T':
			tr_b = MagmaTrans;
			break;
		case 'C':
			tr_b = MagmaConjTrans;
			break;
		default:
			break;
		}
		// -----------------------
		magma_zsetmatrix(C->M(), C->N(), C->cpu_data(), C->M(), C->gpu_data(), C->M(), queue);

		Matrix* A_tmp;
		Matrix* B_tmp;

		if (A == C) {
			A_tmp = new Matrix(A->M(), A->N());
			memcpy(A_tmp->cpu_data(), A->cpu_data(), A_tmp->M() * A_tmp->N() * sizeof(magmaDoubleComplex));
		} else {
			A_tmp = A;
		}

		if (B == C) {
			B_tmp = new Matrix(B->M(), B->N());
			memcpy(B_tmp->cpu_data(), B->cpu_data(), B->M() * B->N() * sizeof(magmaDoubleComplex));
		} else {
			B_tmp = B;
		}

		// -----------------------------------------------------------------------------------------------------------
		magma_zsetmatrix(A_tmp->M(), A_tmp->N(), A_tmp->cpu_data(), A_tmp->M(), A_tmp->gpu_data(), A_tmp->M(), queue);
		magma_zsetmatrix(B_tmp->M(), B_tmp->N(), B_tmp->cpu_data(), B_tmp->M(), B_tmp->gpu_data(), B_tmp->M(), queue);
		// -----------------------------------------------------------------------------------------------------------

		magma_zgemm(tr_a, tr_b, A->M(), C->N(), A->N(), alpha->value(), A_tmp->gpu_data(), A->M(), B_tmp->gpu_data(), A->N(), beta->value(), C->gpu_data(), A->M(), queue);

		magma_zgetmatrix(C->M(), C->N(), C->gpu_data(), C->M(), C->cpu_data(), C->M(), queue);

		if (A == C) {
			delete A_tmp;
		}

		if (B == C) {
			delete B_tmp;
		}
	}

	/*
		void friend GEMM(Matrix* A, Matrix* B, Matrix* C, magma_queue_t queue, Complexd* alpha, Complexd* beta, const char _a = 'N', const char _b = 'N') {
			// -----------------------
			magma_trans_t tr_a, tr_b;

			switch (_a) {
			case 'N':
				tr_a = MagmaNoTrans;
				break;
			case 'T':
				tr_a = MagmaTrans;
				break;
			case 'C':
				tr_a = MagmaConjTrans;
				break;
			default:
				break;
			}

			switch (_b) {
			case 'N':
				tr_b = MagmaNoTrans;
				break;
			case 'T':
				tr_b = MagmaTrans;
				break;
			case 'C':
				tr_b = MagmaConjTrans;
				break;
			default:
				break;
			}
			// -----------------------
			if (beta != 0) {
				magma_zsetmatrix(C->M(), C->N(), C->cpu_data(), C->M(), C->gpu_data(), C->M(), queue);
			}
			// Matrix* A_tmp;
			// Matrix* B_tmp;

			// if (A == C) {
			// 	A_tmp = new Matrix(A->M(), A->N());
			// 	memcpy(A_tmp->cpu_data(), A->cpu_data(), A_tmp->M() * A_tmp->N() * sizeof(magmaDoubleComplex));
			// } else {
			// 	A_tmp = A;
			// }

			// if (B == C) {
			// 	B_tmp = new Matrix(B->M(), B->N());
			// 	memcpy(B_tmp->cpu_data(), B->cpu_data(), B->M() * B->N() * sizeof(magmaDoubleComplex));
			// } else {
			// 	B_tmp = B;
			// }

			// -----------------------------------------------------------------------------------------------------------
			magma_zsetmatrix(A->M(), A->N(), A->cpu_data(), A->M(), A->gpu_data(), A->M(), queue);
			magma_zsetmatrix(B->M(), B->N(), B->cpu_data(), B->M(), B->gpu_data(), B->M(), queue);
			// 	magma_zsetmatrix(C->M(), C->N(), C->cpu_data(), C->M(), C->gpu_data(), C->M(), queue);
			// magma_zsetmatrix(A_tmp->M(), A_tmp->N(), A_tmp->cpu_data(), A_tmp->M(), A_tmp->gpu_data(), A_tmp->M(), queue);
			// magma_zsetmatrix(B_tmp->M(), B_tmp->N(), B_tmp->cpu_data(), B_tmp->M(), B_tmp->gpu_data(), B_tmp->M(), queue);
			// -----------------------------------------------------------------------------------------------------------
			magma_zgemm(tr_a, tr_b, A->M(), C->N(), A->N(), alpha->value(), A->gpu_data(), A->M(), B->gpu_data(), A->N(), beta->value(), C->gpu_data(), A->M(), queue);
			// magma_zgemm(tr_a, tr_b, A->M(), C->N(), A->N(), alpha->value(), A_tmp->gpu_data(), A->M(), B_tmp->gpu_data(), A->N(), beta->value(), C->gpu_data(), A->M(), queue);

			magma_zgetmatrix(C->M(), C->N(), C->gpu_data(), C->M(), C->cpu_data(), C->M(), queue);

			// if (A == C) {
			// 	delete A_tmp;
			// }

			// if (B == C) {
			// 	delete B_tmp;
			// }
		}
	*/
	void exp_diag(Complexd coeff) {
		// accept(grid_, !is_diag(), "Error: matrix is not diagonal");

		vector<Complexd> diag_ = diag();

		for (int i = 0; i < N_; i++) {
			Complexd b = diag_.at(i);

			b = Complexd(MAGMA_Z_MUL(b.value(), coeff.value()));

			double re = b.real();
			double im = b.imag();

			Complexd tmp = Complexd(exp(re) * cos(im), exp(re) * sin(im));

			set(i, i, tmp);
		}
	}

	Matrix get_sqrt() {
		magma_int_t n2 = M_ * N_;

		Matrix a(M_, N_);

		for (int i = 0; i < M_; i++) {
			for (int j = 0; j < N_; j++) {
				a.set(i, j, get(i, j));
			}
		}

		double *s1;
		magma_int_t info ;
		magma_int_t ione = 1;
		double error = 1.;
		double mone = -1.0;
		magma_int_t lwork;
		magma_int_t min_mn = min(M_, N_);

		double* rwork;

		magma_int_t* iwork = new magma_int_t[N_];
		magma_int_t liwork = 3 + 5 * N_;

		magma_dmalloc_cpu(&s1 , min_mn );


		magma_int_t nb = magma_get_zhetrd_nb(N_);

		lwork = max( N_ + N_ * nb, 2 * N_ + N_ * N_ );
		magma_int_t lrwork = 1 + 5 * N_ + 2 * N_ * N_;

		magma_dmalloc_pinned (& rwork , lrwork);
		complexd* work;

		magma_zmalloc_pinned(&work, lwork);

		magma_int_t aux_iwork[1], lda = N_;
		magmaDoubleComplex aux_work[1];
		double aux_rwork[1];

		magma_zheevd( MagmaVec, MagmaLower,
		              N_, NULL,
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

		magma_zmalloc_cpu(&h_A, N_ * lda);
		magma_zsetmatrix(M_, N_, cpu_data(), M_, h_R, M_, queue);

		magma_dmalloc_cpu(&w1, N_);
		magma_dmalloc_cpu(&w2, N_);
		magma_dmalloc_cpu(&rwork, lrwork);
		magma_imalloc_cpu(&iwork, liwork);

		magma_zmalloc_pinned(&h_R, N_ * lda);

		magma_zmalloc_pinned(&h_work, lwork);

		magma_zheevd( MagmaVec, MagmaLower,
		              N_, a.cpu_data(), lda, w1,
		              h_work, lwork,
		              rwork, lrwork,
		              iwork, liwork,
		              &info );


		Matrix V(M_, M_);
		Matrix D(M_, M_);

		Matrix VD(M_, M_);
		Matrix U(M_, M_);
		// Matrix D(M, M);

		// memcpy(V.cpu_data(), a.cpu_data(), M_ * N_ * sizeof(magmaDoubleComplex));
		// memcpy(D.cpu_data(), .cpu_data(), M_ * N_ * sizeof(magmaDoubleComplex));

		for (int i = 0; i < M_; i++) {
			for (int j = 0; j < N_; j++) {
				V.set(i, j, a.get(i, j));
			}

			D.set(i, i, w1[i]);
		}

		D.sqrt_diag();

		Complexd Alpha = cONE;
		Complexd Beta = cZERO;

		magmaDoubleComplex alpha = MAGMA_Z_MAKE(1, 0);
		magmaDoubleComplex beta = MAGMA_Z_MAKE(0, 0);

		int M = M_;
		int N = N_;

		// GEMM(&V, &D, &VD, queue, &Alpha, &Beta, 'N', 'N');
		// GEMM(&VD, &V, &U, queue, &Alpha, &Beta, 'N', 'C');

		magma_zsetmatrix(M, M, V.cpu_data(), M, V.gpu_data(), M, queue);
		magma_zsetmatrix(M, M, D.cpu_data(), M, D.gpu_data(), M, queue);
		magma_zsetmatrix(M, M, VD.cpu_data(), M, VD.gpu_data(), M, queue);
		magma_zsetmatrix(M, M, U.cpu_data(), M, U.gpu_data(), M, queue);

		magma_zgemm(MagmaNoTrans, MagmaNoTrans, M, M, M, alpha, V.gpu_data(), M, D.gpu_data(), M, beta, VD.gpu_data(), M, queue);
		magma_zgetmatrix(M, M, VD.gpu_data(), M, VD.cpu_data(), M, queue);

		magma_zsetmatrix(M, M, V.cpu_data(), M, V.gpu_data(), M, queue);

		magma_zgemm(MagmaNoTrans, MagmaTrans, M, M, M, alpha, VD.gpu_data(), M, V.gpu_data(), M, beta, U.gpu_data(), M, queue);
		magma_zgetmatrix(M, M, U.gpu_data(), M, U.cpu_data(), M, queue);

		U.print_diag();

		free(s1);
		free(w1);
		free(w2);
		free(rwork);
		free(iwork);

		magma_free_pinned(h_work);
		magma_free_pinned(h_R);

		return U;
	}
	void sqrt_diag() {
		// accept(grid_, !is_diag(), "Error: matrix is not diagonal");

		vector<Complexd> diag_ = diag();

		for (int i = 0; i < N_; i++) {
			Complexd b = diag_.at(i);

			set(i, i, b.sqrt());
		}
	}

	double trace() {
		double trace = 0;

		vector<Complexd> diag_ = diag();

		for (int i = 0; i < N_; i++) {
			trace += diag_.at(i).abs();
		}

		return trace;
	}

	~Matrix() {
		// if (size_ > 0) {
		magma_free_pinned(cpu_data_);
		magma_free(gpu_data_);

		size_ = 0;
		// }
	}

	magma_queue_t queue() {
		return *queue_;
	}

protected:
	long size_;
	int M_, N_;

	complexd* cpu_data_;
	complexd* gpu_data_;

	bool copy_alloced_;

	magma_queue_t* queue_;
};
