// ------------------------------------------------------------------------------------------------
// system
#include <iostream>
#include <cstring>
#include <cmath>

#include "magma-2.3.0/include/magma_v2.h"
// ------------------------------------------------------------------------------------------------
// BipartiteGeneral
#include "BipartiteGeneral/Cavity.h"
#include "BipartiteGeneral/Hamiltonian.h"

#include "BipartiteGeneral/WaveFunction.h"
#include "BipartiteGeneral/DensityMatrix.cpp"

#include "BipartiteGeneral/Evolution.cpp"

#include "BipartiteGeneral/parser.cpp" // config
// ------------------------------------------------------------------------------------------------
// Common
#include "Common/ext.cpp"
// ------------------------------------------------------------------------------------------------

using namespace std;


int main(int argc, char** argv) {
	// --------------------------------------------------------------------------------------------
	magma_init();

	magma_queue_t queue = NULL;
	magma_int_t dev = 0;

	magma_queue_create(dev, &queue);
	// --------------------------------------------------------------------------------------------
	const char* config_filename = (argc > 1) ? (argv[1]) : ("BipartiteGeneral/config");

	BGconfig config;
	config.read_from_file(config_filename);
	print("Parse config... done\n\n", "yellow");
	// --------------------------------------------------------------------------------------------
	mkdir(config.path());

	string cmd = "cp " + string(config_filename) + " " + config.path() + "/config.py";

	if (system(cmd.c_str())) {
		Assert(false, "Error: system(cmd.c_str())", __FILE__, __LINE__);
	}
	// --------------------------------------------------------------------------------------------
	cout << "capacity = " << config.capacity() << endl;
	Cavity cavity(config.n(), config.wc(), config.wa(), config.g());

	cavity.print_n();
	cavity.print_wc();
	cavity.print_wa();
	cavity.print_g();

	// --------------------------------------------------------------------------------------------
	Hamiltonian H(config.capacity(), &cavity, &queue);
	print("Hamiltonian... done\n\n", "yellow");

	#if DEBUG
	H.write_to_file(config.H_csv(), config.precision());

	H.print_states();
	H.print_bin_states();
	// H.print_html();
	#endif
	// --------------------------------------------------------------------------------------------
	DensityMatrix ro_0(&H, H.states());
	ro_0.set_ampl(config.init_state(), MAGMA_Z_MAKE(1, 0));
	// run(ro_0, H, config.dt(), config);

	WaveFunction wf(H.states());
	wf.set_amplitude(config.init_state(), 1.0);

	// CV_state st1(capacity, n/2);
	// CV_state st2(capacity, n/2);
	// // for (int i = n / 2; i < n; i++) {
	// // 	st1.at[i] = 1;
	// // }

	// st1.print();
	config.init_state()->print();
	// exit(1);

	#if DEBUG
	// ro_0.write_to_file(config.ro0_csv, config.precision());
	#endif
	// --------------------------------------------------------------------------------------------
	run_wf(wf, H, config.dt(), config);
	print("Evolution... done\n\n", "yellow");

	print("\nSUCCESS\n");
	// --------------------------------------------------------------------------------------------
	magma_queue_destroy(queue);

	magma_finalize();
	// --------------------------------------------------------------------------------------------
	return 0;
}
