#pragma once

#include <omp.h>

typedef pair<int, int> point;

class WaveFunction : public Matrix {

public:
	WaveFunction(map<int, point>* states) : Matrix() {
		size_ = M_ = states->size();
		N_ = 1;

		cpu_alloc();
		gpu_alloc();

		states_ = states;
	}

	WaveFunction(WaveFunction& wf) {
		size_ = M_ = wf.M_;
		N_ = 1;

		cpu_alloc();
		gpu_alloc();

		memcpy(cpu_data_, wf.cpu_data(), size_ * sizeof(magmaDoubleComplex));

		states_ = wf.states_;
	}

	void set_amplitude(point* state, double val) {
		for (auto k : *states_) {
			if (k.second == *state) {
				set(k.first, 0, Complexd(1, 0));

				return;
			}
		}

		cout << "ERROR\n";
	}

	void print(int precision = 3) const {
		double eps = 1.0 / pow(10, precision);

		for (int i = 0; i < M_; i++) {
			Complexd v = get(i, 0);

			double re = v.real();
			double im = v.imag();

			if (abs(re) < eps) {
				re = 0;
			}

			if (abs(im) < eps) {
				im = 0;
			}

			cout << fixed << setprecision(precision) << "(" << re << "," << im << ")" << endl;
		}

		cout << endl;
	}

	void friend GEMV(Matrix* A, Matrix* B, Matrix* C, magma_queue_t queue, Complexd* alpha, Complexd* beta, const char _a = 'N', const char _b = 'N') {
		magmaDoubleComplex Alpha = MAGMA_Z_MAKE(1, 0);
		magmaDoubleComplex Beta = MAGMA_Z_MAKE(0, 0);

		Matrix* B_tmp;

		if (B == C) {
			B_tmp = new Matrix(B->M(), B->N());
			memcpy(B_tmp->cpu_data(), B->cpu_data(), B_tmp->M() * B_tmp->N() * sizeof(magmaDoubleComplex));
		} else {
			B_tmp = B;
		}

		magma_zsetmatrix (A->M(), A->N(), A->cpu_data(), A->M() , A->gpu_data(), A->M(), queue);

		magma_zsetvector (A->N() , B_tmp->cpu_data(), 1 , B_tmp->gpu_data() , 1 , queue);
		magma_zsetvector (A->M() , C->cpu_data() , 1 , C->gpu_data() , 1 , queue);

		magma_zgemv( MagmaNoTrans, A->M(), A->N(), Alpha, A->gpu_data(), A->M(), B_tmp->gpu_data(), 1, Beta, C->gpu_data(), 1, queue);

		magma_zgetvector (A->M() , C->gpu_data(), 1 , C->cpu_data() , 1 , queue);

		if (B == C) {
			delete B_tmp;
		}
	}

	double norm() {
		// magmaDoubleComplex norm;
		// Complexd norm(0, 0);
		double norm_d = 0;

		int i;
		// #pragma omp parallel private(i) reduction(+:norm_d)
		for (i = 0; i < M_; i++) {
			norm_d += (get(i, 0).conj() * get(i, 0)).abs();
		}

		return norm_d;
		// return MAGMA_Z_ABS(norm);
	}

	vector<double> diag() {
		vector<double> diag_;

		Complexd val(0, 0);

		for (int i = 0; i < M_; i++) {
			val = get(i, 0);

			diag_.push_back((val.conj() * val).abs());
		}

		return diag_;
	}

	void print_diag() {
		vector<double> diag_ = diag();

		for (int i = 0; i < M_; i++) {
			cout << diag_[i] << " ";
		}

		cout << endl;
	}

	~WaveFunction() {
		magma_free_pinned(cpu_data_);
		magma_free(gpu_data_);
	}

private:
	map<int, point>* states_;
};