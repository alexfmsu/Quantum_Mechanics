// ------------------------------------------------------------------------------------------------
// system
#include <iostream>
#include <cstring>
#include <cmath>

#include "magma-2.3.0/include/magma_v2.h"
// ------------------------------------------------------------------------------------------------
// BipartiteGeneral
// #include "Common/Cavity.h"
// #include "Bipartite_n/Hamiltonian.h"

// #include "Bipartite/WaveFunction.h"
// #include "Bipartite_n/DensityMatrix.cpp"

// #include "Bipartite_n/Evolution.cpp"

// #include "Bipartite_n/parser.cpp" // config
// ------------------------------------------------------------------------------------------------
// Common
// #include "Common/ext.cpp"
#include "include/matrix.h"
// ------------------------------------------------------------------------------------------------

using namespace std;

// ------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {

	// --------------------------------------------------------------------------------------------
	magma_init();

	magma_device_t *  	devices;
	magma_int_t  	size;
	magma_int_t *  	num_dev;

	magma_getdevices(devices,
	                 size,
	                 num_dev
	                );

	// // err = magma_zmalloc_gpu(&data, sizeof(magmaDoubleComplex) * 1e8);

	// magma_queue_t queue = NULL;
	// magma_int_t dev = 0;

	// magma_queue_create(dev, &queue);
	/*
		// --------------------------------------------------------------------------------------------
		Matrix A(3, 3);
		Matrix B(3, 3);
		Matrix C(3, 3);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				A.set(i, j, 10 * i + j);
				B.set(i, j, 10 * i + j);
			}
		}

		Complexd alpha = cONE;
		Complexd beta = cZERO;

		GEMM(&A, &B, &C, queue, &alpha, &beta, 'N', 'N');

		cout << "C" << endl;
		C.print();

		// A.print();

		Matrix S = C.get_sqrt();

		// Matrix C_res(3, 3);
		// Matrix S_cpy(S);

		// GEMM(&S, &S_cpy, &C_res, queue, &alpha, &beta, 'N', 'N');

		cout << "S:" << endl;
		S.print();
		GEMM(&S, &S, &C, queue, &alpha, &beta, 'N', 'N');

		C.print();

		// A.print();

		// B.print();








		// const char* config_filename = (argc > 1) ? (argv[1]) : ("Bipartite_n/config");
		/*
			BGconfig config;
			config.read_from_file(config_filename);

			int capacity = config.capacity();

			int n = config.n();
			double wc = config.wc();
			double wa = config.wa();
			double g = config.g();

			double T = config.T();
			double dt = config.dt();

			int nt = config.nt();

			int precision = config.precision();

			std::string path = config.path();
			std::string H_csv = config.H_csv();
			// --------------------------------------------------------------------------------------------
			mkdir(path);

			string cmd = "cp " + string(config_filename) + " " + path + "/config.py";

			if (system(cmd.c_str())) {
				Assert(false, "Error: system(cmd.c_str())", __FILE__, __LINE__);
			}
			// --------------------------------------------------------------------------------------------
			Cavity cavity(n, wc, wa, g);

			cavity.print_n();
			cavity.print_wc();
			cavity.print_wa();
			cavity.print_g();
			// --------------------------------------------------------------------------------------------
			Hamiltonian H(capacity, &cavity, &queue);

			#if DEBUG
			// H.write_to_file(H_csv, precision);

			// H.print_states();
			// H.print_bin_states();
			// H.print_html();
			#endif
			// --------------------------------------------------------------------------------------------
			DensityMatrix ro_0(&H, H.states());
			cout << '[' << config.init_state()->first << "," << config.init_state()->second << "]" << endl;
			ro_0.set_ampl(config.init_state(), MAGMA_Z_MAKE(1, 0));

			// #if DEBUG
			// // ro_0.write_to_file(config.ro0_csv, config.precision());
			// #endif
			// // --------------------------------------------------------------------------------------------
			run(ro_0, H, dt, config);

		*/

	magma_queue_t queues[MagmaMaxGPUs];
	for ( int dev = 0; dev < size; ++dev ) {
		magma_queue_create( dev, &queues[dev] );
	}

	magmaDoubleComplex* data;
	magma_print_environment();


	// magma_int_t err;
	// // err = magma_zmalloc_cpu(&data, sizeof(magmaDoubleComplex) * 1e8);
	// err = magma_zmalloc(&data, sizeof(magmaDoubleComplex) * 1e8);
	// // // err = magma_zmalloc_cpu(&data, sizeof(magmaDoubleComplex) * 1e9);
	// cout << "err = " << err << endl;
	// magma_free(data);
	cout << "\nSUCCESS\n"  << endl;
	// print("\nSUCCESS\n");
	// --------------------------------------------------------------------------------------------
	// magma_queue_destroy(queue);

	magma_finalize();
	// --------------------------------------------------------------------------------------------
	return 0;
}
