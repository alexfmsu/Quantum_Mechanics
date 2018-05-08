// ------------------------------------------------------------------------------------------------
#pragma once
// ------------------------------------------------------------------------------------------------
// Common
#include "ext.cpp"
#include "Fidelity.cpp"
// ------------------------------------------------------------------------------------------------
#include "parser.cpp"
#include "Unitary.h"
#include <sstream>
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
void run(DensityMatrix& ro_0, Hamiltonian& H, double dt, BGconfig& config, bool fidelity_mode = false) {
	cout << "dt = " << dt << endl;
	Matrix U = getU(&H, dt);

	#if DEBUG
	U.write_to_file(config.U_csv(), config.precision());
	#endif

	File f(config.z_csv());

	f.erase();
	f.open("w");

	vector<Complexd> diag_;

	map<std::string, double> p_bin;
	vector<double> v_bin;

	Complexd alpha = cONE;
	Complexd beta = cZERO;

	vector<double> FIDELITY;

	Matrix ro_0_sqrt = ro_0.get_sqrt();

	DensityMatrix ro_t(ro_0);

	// Matrix* A_tmp;
	// A_tmp = new Matrix(ro_t.M(), ro_t.N());

	// Matrix* B_tmp;
	// B_tmp = new Matrix(ro_t.M(), ro_t.N());

	for (int i = 0; i < config.nt(); i++) {
		if (fidelity_mode) {
			double fidelity_t = fidelity(&ro_0_sqrt, &ro_t, H.queue());
			FIDELITY.push_back(fidelity_t);
		}

		diag_ = ro_t.diag();

		double diag_norm = ro_t.norm(diag_);

		if (abs(1 - diag_norm) > 1) {
			cout << "Not normed" << endl;
			cout << "i = " << i << ", " << diag_norm << endl;

			// string cmd = "if [ -d '" + config.path() + "' ]; then rm -r " + config.path() + "; fi";

			// int _sys = system(cmd.c_str());

			exit(1);
		}

		f.write(diag_, config.precision());

		GEMM(&U, &ro_t, &ro_t, H.queue(), &alpha, &beta, 'N', 'N');
		GEMM(&ro_t, &U, &ro_t, H.queue(), &alpha, &beta, 'N', 'C');

		// memcpy(B_tmp->cpu_data(), ro_t.cpu_data(), B_tmp->M() * B_tmp->N() * sizeof(magmaDoubleComplex));
		// GEMM(&U, B_tmp, &ro_t, H.queue(), &alpha, &beta);
		// GEMM(&U, &ro_t, &ro_t, H.queue(), &alpha, &beta);

		// memcpy(A_tmp->cpu_data(), ro_t.cpu_data(), A_tmp->M() * A_tmp->N() * sizeof(magmaDoubleComplex));
		// GEMM(A_tmp, &U, &ro_t, H.queue(), &alpha, &beta, 'N', 'C');
		// GEMM(&ro_t, &U, &ro_t, H.queue(), &alpha, &beta, 'N', 'C');

	}

	// delete A_tmp;
	// delete B_tmp;

	f.close();
	// --------------------------------------------------------------------------------------------
	vector<std::string> st;

	for (auto k : *H.states()) {
		std::stringstream ss;

		if (k.second == make_pair(0, H.cavity()->n()) || k.second == make_pair(H.cavity()->n(), 0)) {
			ss << "[" << k.second.first << ", " << k.second.second << "]";
		} else {
			ss << "";
		}

		st.push_back(ss.str());
	}

	write_states(config.x_csv(), st);

	write_t(config.t_csv(), T_str_v(config.T()), config.nt());
	// --------------------------------------------------------------------------------------------
	if (fidelity_mode) {
		File f_fid_csv(config.fid_csv());
		f_fid_csv.open("w");

		f_fid_csv.writeln("fidelity");

		for (size_t i = 0; i < FIDELITY.size(); i++) {
			f_fid_csv.writeln(FIDELITY[i], config.precision());
		}

		f_fid_csv.close();
	}
	// --------------------------------------------------------------------------------------------
}
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
void run_wf(WaveFunction& wf_0, Hamiltonian& H, double dt, BGconfig& config, bool fidelity_mode = false) {
	Matrix U = getU(&H, dt);

	// #if DEBUG
	// U.write_to_file(config.U_csv(), config.precision());
	// #endif

	File f(config.z_csv());

	f.erase();
	f.open("w");

	vector<double> diag_;

	map<std::string, double> p_bin;
	vector<double> v_bin;

	Complexd alpha = cONE;
	Complexd beta = cZERO;

	vector<double> z_0;
	vector<double> z_1;
	vector<double> z_max;
	vector<double> FIDELITY;
	vector<double> FIDELITY_SMALL;

	WaveFunction wf_t(wf_0);

	double dt_ = config.nt() / (config.T() / 1e-6) / 20000 * 1000;
	int nt_ = int(config.nt() / dt_);

	// H.states();
	int ind_0 = -1, ind_1 = -1;

	for (auto k : *H.states()) {
		if (k.second == make_pair(0, (H.cavity())->n())) {
			ind_0 = k.first;
		} else if (k.second == make_pair((H.cavity())->n(), 0)) {
			ind_1 = k.first;
		}
	}

	for (int i = 0; i <= config.nt(); i++) {
		if (fidelity_mode) {
			double fidelity_t = fidelity_wf(&wf_0, &wf_t);

			FIDELITY.push_back(fidelity_t);

			if (i % nt_ == 0) {
				FIDELITY_SMALL.push_back(fidelity_t);
			}
		}

		double diag_norm = wf_t.norm();
		diag_ = wf_t.diag();

		if (abs(1 - diag_norm) > 0.1) {
			cout << "Not normed" << endl;
			cout << "i = " << i << ", " << diag_norm << endl;

			string cmd = "if [ -d '" + config.path() + "' ]; then rm -r " + config.path() + "; fi";

			int _sys = system(cmd.c_str());

			exit(1);
		}

		double zmax = 0;

		for (int i_ = 0; i_ < diag_.size(); i_++) {
			if (i_ != ind_0 && i_ != ind_1 && diag_[i_] > zmax) {
				zmax = diag_[i_];
			}
		}


		// if (config.T() > 0.5 * config.mks()) {
		// 	if (i % nt_ == 0) {
		// 		f.write(diag_, config.precision());
		// 	}
		// } else {
		// 	f.write(diag_, config.precision());
		// }

		z_max.push_back(zmax);
		z_0.push_back(diag_[ind_0]);
		z_1.push_back(diag_[ind_1]);


		GEMV(&U, &wf_t, &wf_t, H.queue(), &alpha, &beta, 'N', 'N');
	}

	f.close();
	// --------------------------------------------------------------------------------------------
	vector<std::string> st;

	for (auto k : *H.states()) {
		std::stringstream ss;

		if (k.second == make_pair(0, H.cavity()->n()) || k.second == make_pair(H.cavity()->n(), 0)) {
			ss << "[" << k.second.first << ", " << k.second.second << "]";
		} else {
			ss << "";
		}

		st.push_back(ss.str());
	}

	write_states(config.x_csv(), st);

	write_t(config.t_csv(), T_str_v(config.T()), config.nt());
	// --------------------------------------------------------------------------------------------
	File z_0_csv(config.path() + "/z_0.csv");
	z_0_csv.open("w");

	z_0_csv.writeln("fidelity");

	for (size_t i = 0; i < z_0.size(); i++) {
		z_0_csv.writeln(z_0[i], config.precision());
	}

	z_0_csv.close();
	// --------------------------------------------------------------------------------------------
	File z_1_csv(config.path() + "/z_1.csv");
	z_1_csv.open("w");

	z_1_csv.writeln("fidelity");

	for (size_t i = 0; i < z_1.size(); i++) {
		z_1_csv.writeln(z_1[i], config.precision());
	}

	z_1_csv.close();
	// --------------------------------------------------------------------------------------------
	File z_max_csv(config.path() + "/z_max.csv");
	z_max_csv.open("w");

	z_max_csv.writeln("fidelity");

	for (size_t i = 0; i < z_max.size(); i++) {
		z_max_csv.writeln(z_max[i], config.precision());
	}

	z_max_csv.close();
	// --------------------------------------------------------------------------------------------
	if (fidelity_mode) {
		File f_fid_csv(config.fid_csv());
		f_fid_csv.open("w");

		f_fid_csv.writeln("fidelity");

		for (size_t i = 0; i < FIDELITY.size(); i++) {
			f_fid_csv.writeln(FIDELITY[i], config.precision());
		}

		f_fid_csv.close();
		// ------------------------------------------ -
		File f_fid_small_csv(config.path() + "fid_small.csv");
		f_fid_small_csv.open("w");

		f_fid_small_csv.writeln("fidelity");

		for (size_t i = 0; i < FIDELITY_SMALL.size(); i++) {
			f_fid_small_csv.writeln(FIDELITY_SMALL[i], config.precision());
		}

		f_fid_small_csv.close();
	}
	// --------------------------------------------------------------------------------------------
}
// ------------------------------------------------------------------------------------------------
