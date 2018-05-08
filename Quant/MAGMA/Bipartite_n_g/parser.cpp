#pragma once

#include "CV_state.h"

#include "Parser.cpp"

typedef pair<int, int> point;

class BGconfig : public Config {

public:
	BGconfig() : Config() {
		n_ = -1;

		wc_ = -1;
		wa_ = -1;

		g_ = -1;
	}

	void read_from_file(const char* _filename) {
		std::ifstream file(_filename);

		std::string str;

		while (std::getline(file, str)) {
			get_capacity(str);
			get_n(str);

			get_wc(str);
			get_wa(str);
			get_g(str);

			get_T(str);
			get_dt(str);

			// get_init_state(str);

			get_precision(str);

			get_path(str);

			get_H_csv();
			get_U_csv();

			get_x_csv();
			get_t_csv();
			get_z_csv();
			get_z_all_csv();

			get_fid_csv();
		}

		print();
	}

	void get_n(string& s) {
		if (!defined(n_)) {
			if (s.substr(0, 4) == "n = " ) {
				n_ = stoi(s.substr(4));

				init_state_ = new point(0, n_);
			}
		}
	}

	void get_wc(string& s) {
		if (!defined(wc_)) {
			if (s.substr(0, 5) == "wc = " ) {
				int found = s.find(" * ");

				if (found != -1) {
					wc_ = stod(s.substr(5, found));

					if (s.substr(found + 3) == "GHz") {
						wc_ *= GHz;
					} else if (s.substr(found + 3) == "MHz") {
						wc_ *= MHz;
					} else if (s.substr(found + 3) == "KHz") {
						wc_ *= KHz;
					}
				}
			}
		}
	}

	void get_wa(string& s) {
		if (!defined(wa_)) {
			if (s.substr(0, 5) == "wa = " ) {
				int found = s.find(" * ");

				if (found != -1) {
					wa_ = stod(s.substr(5, found));

					if (s.substr(found + 3) == "GHz") {
						wa_ *= GHz;
					} else if (s.substr(found + 3) == "MHz") {
						wa_ *= MHz;
					} else if (s.substr(found + 3) == "KHz") {
						wa_ *= KHz;
					}
				}
			}
		}
	}

	// BEGIN---------------------------------------------- GETTERS ----------------------------------------------------
	int n()          const { return n_;  }

	double wc()      const { return wc_; }

	double wa()      const { return wa_; }
	double g()      const { return g_;  }
	// -------------------------------------------
	point* init_state() { return init_state_; }
	// END------------------------------------------------ GETTERS ----------------------------------------------------

	void get_g(string& s) {
		if (!defined(g_)) {
			if (s.substr(0, 5) == "g  = " ) {
				int found = s.find(" * ");

				if (found != -1) {
					g_ = stod(s.substr(5, found));

					if (s.substr(found + 3) == "GHz") {
						g_ *= GHz;
					} else if (s.substr(found + 3) == "MHz") {
						g_ *= MHz;
					} else if (s.substr(found + 3) == "KHz") {
						g_ *= KHz;
					}
				}
			}
		}
	}

	// void get_init_state(string& s) {
	// 	if (s.substr(0, 14) == "init_state = [" && s[s.size() - 1] == ']') {
	// 		int s_begin = 14;

	// 		string s_copy = s.substr(14);

	// 		int shift = 14;


	// 		for (int i = 0; i < n_ * 2 + 1; i++) {
	// 			int s_end;

	// 			if (i == n_ * 2 - 1) {
	// 				s_end = s_copy.size() - 1;
	// 			} else {
	// 				s_end = s_copy.find(", ");
	// 			}

	// 			if (s_end != -1) {
	// 				string s_part = s_copy.substr(0, s_end);

	// 				if (i == 0) {
	// 					init_state_->ph() = stoi(s_part);
	// 				} else {
	// 					init_state_->at(i - 1) = stoi(s_part);
	// 				}

	// 				if (i != n_ * 2 - 1) {
	// 					shift += s_end + 2;
	// 					s_copy = s.substr(shift);
	// 				}
	// 			}
	// 		}

	// 	}
	// }

	void get_path(string& s) {
		if (!defined(path_) && s.substr(0, 8) == "path = \"") {
			path_ = s.substr(8, s.length() - 8 - 1);
		}
	}

	void get_fid_csv() {
		if (defined(path_) && !defined(fid_csv_)) {
			fid_csv_ = path_ + "fid.csv";
		}
	}

	string fid_csv()     const { return fid_csv_;      }


	void print_wa() {
		cout << "wa = ";

		if (wa_ >= GHz) {
			cout << wa_ / GHz << " GHz";
		} else if (wa_ >= MHz) {
			cout << wa_ / MHz << " MHz";
		} else if (wa_ >= KHz) {
			cout << wa_ / KHz << " KHz";
		} else {
			cout << wa_ << " Hz";
		}

		cout << endl;
	}

	void print_g() {
		cout << "g = ";

		if (g_ >= GHz) {
			cout << g_ / GHz << " GHz";
		} else if (g_ >= MHz) {
			cout << g_ / MHz << " MHz";
		} else if (g_ >= KHz) {
			cout << g_ / KHz << " KHz";
		} else {
			cout << g_ << " Hz";
		}

		cout << endl;
	}

	void print() {
		print_capacity();
		cout << "n = " << n_ << endl;
		println();
		cout << "wc = " << wc_ << endl;
		print_wa();
		print_g();
		// init_state_->print();
		println();
		print_T();
		print_dt();
		print_nt();
		println();
		print_precision();
		println();
		print_path();
		println();
		print_H_csv();
		print_U_csv();
		println();
		print_x_csv();
		print_t_csv();
		print_z_csv();
		print_z_all_csv();
		// print_fid_csv();
		println();
	}

	~BGconfig() {}

private:
	// ----
	int n_;
	// --------
	double wc_;
	double wa_;
	double g_;
	// -------------
	string fid_csv_;
	// -------------
	point* init_state_;
	// -------------------
};
