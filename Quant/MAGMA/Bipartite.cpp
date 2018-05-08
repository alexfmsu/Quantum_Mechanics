// ------------------------------------------------------------------------------------------------
// system
#include <iostream>
#include <cstring>
#include <cmath>

#include "magma-2.3.0/include/magma_v2.h"
// ------------------------------------------------------------------------------------------------
// BipartiteGeneral
#include "Cavity.h"
#include "Hamiltonian.h"

#include "WaveFunction.h"
#include "DensityMatrix.cpp"

#include "Evolution.cpp"

#include "parser.cpp" // config
// ------------------------------------------------------------------------------------------------
// Common
#include "ext.cpp"
// ------------------------------------------------------------------------------------------------

using namespace std;

// ------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	// --------------------------------------------------------------------------------------------
	magma_init();

	magma_queue_t queue = NULL;
	magma_int_t dev = 0;

	magma_queue_create(dev, &queue);
	// --------------------------------------------------------------------------------------------
	const char* config_filename = (argc > 1) ? (argv[1]) : ("Bipartite/config");

	BGconfig config;
	config.read_from_file(config_filename);
	print("Parse config... done\n\n", "yellow");

	cout << '[' << config.init_state()->first << "," << config.init_state()->second << "]" << endl;
	// --------------------------------------------------------------------------------------------
	mkdir(config.path());

	string cmd = "cp " + string(config_filename) + " " + config.path() + "/config.py";

	if (system(cmd.c_str())) {
		Assert(false, "Error: system(cmd.c_str())", __FILE__, __LINE__);
	}
	// --------------------------------------------------------------------------------------------
	Cavity cavity(config.n(), config.wc(), config.wa(), config.g());

	cavity.print_n();
	cavity.print_wc();
	cavity.print_wa();
	cavity.print_g();
	// --------------------------------------------------------------------------------------------
	Hamiltonian H(config.capacity(), &cavity, &queue);
	print("Hamiltonian... done\n\n", "yellow");

	H.print_states();

	#if DEBUG
	// H.write_to_file(config.H_csv(), config.precision());
	// H.print_bin_states();
	// H.print_html();
	#endif
	// --------------------------------------------------------------------------------------------
	DensityMatrix ro_0(&H, H.states());

	cout << "config.T = " << config.T() << endl;
	cout << "config.nt = " << config.nt() << endl;
	cout << "config.dt = " << config.dt() << endl;

	ro_0.set_ampl(config.init_state(), MAGMA_Z_MAKE(1, 0));
	print("Density matrix... done\n\n", "yellow");

	#if DEBUG
	// // ro_0.write_to_file(config.ro0_csv, config.precision());
	#endif
	// --------------------------------------------------------------------------------------------

	// run(ro_0, H, config.dt(), config, true);
	// print("Evolution... done\n\n", "yellow");

	WaveFunction wf(H.states());
	wf.set_amplitude(config.init_state(), 1.0);

	wf.print();

	run_wf(wf, H, config.dt(), config, true);

	print("\nSUCCESS\n");
	// --------------------------------------------------------------------------------------------
	magma_queue_destroy(queue);

	magma_finalize();
	// --------------------------------------------------------------------------------------------
	return 0;
}
