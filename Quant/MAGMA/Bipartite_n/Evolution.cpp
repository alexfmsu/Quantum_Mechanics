#include "parser.cpp"
#include "../Common/ext.cpp"
#include "Unitary.h"

double fidelity(Matrix* ro_0_sqrt, Matrix* ro_t, magma_queue_t queue) {
	Complexd alpha = cONE;
	Complexd beta = cZERO;

	Matrix* mul = new Matrix(ro_t->N(), ro_t->N());

	GEMM(ro_0_sqrt, ro_t, mul, queue, &alpha, &beta);
	GEMM(mul, ro_0_sqrt, mul, queue, &alpha, &beta);

	double tr = 0;

	Matrix mul_sqrt = mul->get_sqrt();
	tr = mul_sqrt.trace();
	delete mul;

	return tr;
}

void run(DensityMatrix& ro_0, Hamiltonian& H, double dt, BGconfig& config) {
	// ----------------------------------------
	double T = config.T();

	int nt = config.nt();

	std::string U_csv = config.U_csv();

	std::string x_csv = config.x_csv();
	std::string t_csv = config.t_csv();
	std::string z_csv = config.z_csv();
	std::string z_all_csv = config.z_all_csv();

	std::string fid_csv = config.fid_csv();

	int precision = config.precision();
	// ----------------------------------------
	Matrix U = getU(&H, dt);

	#if DEBUG
	// U.write_to_file(U_csv, precision);
	#endif

	File f(z_csv);

	f.erase();
	f.open("w");

	vector<Complexd> diag_;

	map<std::string, double> p_bin;
	vector<double> v_bin;

	Complexd alpha = cONE;
	Complexd beta = cZERO;

	Matrix ro_0_sqrt = ro_0.get_sqrt();

	vector<double> FIDELITY;
	for (int i = 0; i < nt; i++) {
		double fidelity_t = fidelity(&ro_0_sqrt, &ro_0, H.queue());
		FIDELITY.push_back(fidelity_t);

		diag_ = ro_0.diag();

		double diag_norm = ro_0.norm(diag_);

		if (abs(1 - diag_norm) > 1) {
			cout << "Not normed" << endl;
			cout << "i = " << i << ", " << diag_norm << endl;

			// string cmd = "if [ -d '" + config.path() + "' ]; then rm -r " + config.path() + "; fi";

			// int _sys = system(cmd.c_str());

			// exit(1);
		}

		f.write(diag_, precision);

		GEMM(&U, &ro_0, &ro_0, H.queue(), &alpha, &beta);

		// #if DEBUG
		// cout << "i = " << i << endl;
		// #endif

		GEMM(&ro_0, &U, &ro_0, H.queue(), &alpha, &beta, 'N', 'C');
	}

	f.close();
	// --------------------------------------------------------------------------------------------
	write_states(x_csv, H.bin_states_keys);

	write_t(t_csv, T, nt);
	// --------------------------------------------------------------------------------------------
	File f_fid_csv(fid_csv);
	f_fid_csv.open("w");

	for (size_t i = 0; i < FIDELITY.size(); i++) {
		f_fid_csv.writeln(FIDELITY[i], 5);
	}

	f_fid_csv.close();
	// --------------------------------------------------------------------------------------------
}
