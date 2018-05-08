// ------------------------------------------------------------------------------------------------
#pragma once
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
double fidelity(Matrix* ro_0_sqrt, Matrix* ro_t, magma_queue_t queue) {
	Complexd alpha = cONE;
	Complexd beta = cZERO;

	Matrix* mul = new Matrix(ro_t->N(), ro_t->N());

	GEMM(ro_0_sqrt, ro_t, mul, queue, &alpha, &beta);
	GEMM(mul, ro_0_sqrt, mul, queue, &alpha, &beta);

	double tr = 0;

	Matrix mul_sqrt = mul->get_sqrt();
	tr = mul_sqrt.trace();
	// cout << "tr = " << tr << endl;
	delete mul;

	return tr;
}

double fidelity_wf(Matrix* wf_0, Matrix* wf_t) {
	Complexd fid(0, 0);

	for (int i = 0; i < wf_0->M(); i++) {
		fid += (wf_0->get(i, 0).conj() * wf_t->get(i, 0));
	}

	return fid.abs();
}
// ------------------------------------------------------------------------------------------------
