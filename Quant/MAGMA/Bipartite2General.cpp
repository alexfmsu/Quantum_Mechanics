// ------------------------------------------------------------------------------------------------
// system
#include <iostream>
#include <cstring>
#include <cmath>

#include "magma-2.3.0/include/magma_v2.h"
// ------------------------------------------------------------------------------------------------
// BipartiteGeneral
#include "Bipartite2General/Cavity.h"
#include "Bipartite2General/Hamiltonian.h"

// #include "BipartiteGeneral/WaveFunction.h"
#include "Bipartite2General/DensityMatrix.cpp"

#include "Bipartite2General/Evolution.cpp"

#include "Bipartite2General/parser.cpp" // config
// ------------------------------------------------------------------------------------------------
// Import
#include "Common/ext.cpp"
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
	const char* config_filename = (argc > 1) ? (argv[1]) : ("Bipartite2General/config");

	BG2config config;
	config.read_from_file(config_filename);
	print("Parse config... done\n\n", "yellow");
	// --------------------------------------------------------------------------------------------
	mkdir(config.path());

	string cmd = "cp " + string(config_filename) + " " + config.path() + "/config.py";

	if (system(cmd.c_str())) {
		Assert(false, "Error: system(cmd.c_str())", __FILE__, __LINE__);
	}
	// --------------------------------------------------------------------------------------------
	Cavity cv_1(config.n_1(), config.wc_1(), config.wa_1(), config.g_1());
	Cavity cv_2(config.n_2(), config.wc_2(), config.wa_2(), config.g_2());

	cv_1.print_n();
	cv_1.print_wc();
	cv_1.print_wa();
	cv_1.print_g();

	cv_2.print_n();
	cv_2.print_wc();
	cv_2.print_wa();
	cv_2.print_g();
	// --------------------------------------------------------------------------------------------
	Hamiltonian H(config.capacity(), cv_1, cv_2, config.mu(), &queue);
	print("Hamiltonian... done\n\n", "yellow");

	#if DEBUG
	// H.write_to_file(H_csv, config.precision());

	// H.print_states();
	// H.print_bin_states();
	// H.print_html();
	#endif
	// --------------------------------------------------------------------------------------------
	DensityMatrix ro_0(&H, H.states());

	ro_0.set_ampl(config.init_state(), MAGMA_Z_MAKE(1, 0));
	print("Density matrix... done\n\n", "yellow");

	#if DEBUG
	// ro_0.write_to_file(config.ro0_csv, config.precision());
	#endif
	// --------------------------------------------------------------------------------------------
	run(ro_0, H, config.dt(), config);
	print("Evolution... done\n\n", "yellow");

	print("\nSUCCESS\n");
	// --------------------------------------------------------------------------------------------
	magma_queue_destroy(queue);

	magma_finalize();
	// --------------------------------------------------------------------------------------------
	return 0;
}
