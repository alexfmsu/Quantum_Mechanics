// ------------------------------------------------------------------------------------------------
#pragma once
// ------------------------------------------------------------------------------------------------
// Common
#include "ext.cpp"
#include "Fidelity.cpp"
// ------------------------------------------------------------------------------------------------
#include "parser.cpp"
#include "Unitary.h"
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
void run(DensityMatrix& ro_0, Hamiltonian& H, double dt, BGconfig& config) {
	Matrix U = getU(&H, dt);

	#if DEBUG
	U.write_to_file(config.U_csv(), config.precision());
	#endif

	#if DEBUG
	File f_all(config.z_all_csv());
	#endif

	File f(config.z_csv());

	f.erase();
	f.open("w");

	#if DEBUG
	f_all.erase();
	f_all.open("w");
	#endif

	vector<Complexd> diag_;

	map<std::string, double> p_bin;
	vector<double> v_bin;

	Complexd alpha = cONE;
	Complexd beta = cZERO;

	Matrix ro_0_sqrt = ro_0.get_sqrt();

	vector<double> FIDELITY;

	DensityMatrix ro_t(ro_0);

	for (int i = 0; i < config.nt(); i++) {
		double fidelity_t = fidelity(&ro_0_sqrt, &ro_0, H.queue());
		FIDELITY.push_back(fidelity_t);

		diag_ = ro_t.diag();

		#if DEBUG
		f_all.write(diag_, config.precision());
		#endif

		double diag_norm = ro_t.norm(diag_);

		if (abs(1 - diag_norm) > 1) {
			cout << "Not normed" << endl;
			cout << "i = " << i << ", " << diag_norm << endl;

			// string cmd = "if [ -d '" + config.path() + "' ]; then rm -r " + config.path() + "; fi";

			// int _sys = system(cmd.c_str());

			// exit(1);
		}

		for (auto k : H.bin_states_keys) {
			p_bin[k] = 0;
		}

		v_bin.resize(0);

		for (auto k : H.bin_states_) {
			for (int ind : k.second) {
				p_bin[k.first] += diag_[ind].abs();
			}
		}

		for (int k = 0; k < H.bin_states_keys.size(); k++) {
			v_bin.push_back(p_bin[H.bin_states_keys[k]]);
		}

		f.write(v_bin, config.precision());

		GEMM(&U, &ro_t, &ro_t, H.queue(), &alpha, &beta, 'N', 'N');
		GEMM(&ro_t, &U, &ro_t, H.queue(), &alpha, &beta, 'N', 'C');
	}

	#if DEBUG
	f_all.close();
	#endif

	f.close();
	// --------------------------------------------------------------------------------------------
	write_states(config.x_csv(), H.bin_states_keys);

	write_t(config.t_csv(), config.T(), config.nt());
	// --------------------------------------------------------------------------------------------
	File f_fid_csv(config.fid_csv());
	f_fid_csv.open("w");

	for (size_t i = 0; i < FIDELITY.size(); i++) {
		f_fid_csv.writeln(FIDELITY[i], 5);
	}

	f_fid_csv.close();
	// --------------------------------------------------------------------------------------------
}
// ------------------------------------------------------------------------------------------------

void run_wf(WaveFunction& wf_0, Hamiltonian& H, double dt, BGconfig& config) {
	Matrix U = getU(&H, dt);

	#if DEBUG
	U.write_to_file(config.U_csv(), config.precision());
	#endif

	#if DEBUG
	File f_all(config.z_all_csv());
	#endif

	File f(config.z_csv());

	f.erase();
	f.open("w");

	#if DEBUG
	f_all.erase();
	f_all.open("w");
	#endif

	map<std::string, double> p_bin;
	vector<double> v_bin;

	Complexd alpha = cONE;
	Complexd beta = cZERO;

	vector<double> FIDELITY;

	vector<double> diag_;

	WaveFunction wf_t(wf_0);

	for (int i = 0; i < config.nt(); i++) {
		double fidelity_t = fidelity_wf(&wf_0, &wf_t);
		FIDELITY.push_back(fidelity_t);

		double diag_norm = wf_t.norm();
		diag_ = wf_t.diag();

		if (abs(1 - diag_norm) > 1) {
			cout << "Not normed" << endl;
			cout << "i = " << i << ", " << diag_norm << endl;

			// string cmd = "if [ -d '" + config.path() + "' ]; then rm -r " + config.path() + "; fi";

			// int _sys = system(cmd.c_str());

			// exit(1);
		}

		for (auto k : H.bin_states_keys) {
			p_bin[k] = 0;
		}

		v_bin.resize(0);

		for (auto k : H.bin_states_) {
			for (int ind : k.second) {
				// p_bin[k.first] += diag_[ind].abs();
				p_bin[k.first] += diag_[ind];
			}
		}

		for (int k = 0; k < H.bin_states_keys.size(); k++) {
			v_bin.push_back(p_bin[H.bin_states_keys[k]]);
		}

		f.write(v_bin, config.precision());

		#if DEBUG
		f_all.write(diag_, config.precision());
		#endif

		GEMV(&U, &wf_t, &wf_t, H.queue(), &alpha, &beta, 'N', 'N');
	}

	#if DEBUG
	f_all.close();
	#endif

	f.close();
	// --------------------------------------------------------------------------------------------
	write_states(config.x_csv(), H.bin_states_keys);

	write_t(config.t_csv(), config.T(), config.nt());
	// --------------------------------------------------------------------------------------------
	File f_fid_csv(config.fid_csv());
	f_fid_csv.open("w");

	for (size_t i = 0; i < FIDELITY.size(); i++) {
		f_fid_csv.writeln(FIDELITY[i], 5);
	}

	f_fid_csv.close();
	// --------------------------------------------------------------------------------------------
}
// ------------------------------------------------------------------------------------------------

