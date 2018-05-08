#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

using namespace std;

const double GHz = 1e9;
const double MHz = 1e6;
const double KHz = 1e3;
const double Hz = 1;
const double mks_ = 1e-6;

bool defined(const int& _var) {
	return (_var != -1);
}

bool defined(const double& _var) {
	return (_var != -1);
}

bool defined(const string& _var) {
	return (_var != "");
}

bool is(string& s, string expr) {
	bool eq = (s.substr(0, expr.size()) == expr);

	if (eq) {
		s = s.substr(expr.size());
	}

	return eq;
}

bool get_string(string& s, string str) {
	int l = str.length();

	if (s.substr(0, l) == str) {
		s = s.substr(l);

		return true;
	}

	return false;
}

int get_int(string& s) {
	int counter = 0;

	while (s[counter] >= '0' && s[counter] <= '9') {
		counter++;
	}

	Assert(counter != 0, s.c_str(), __FILE__, __LINE__);

	int val = stoi(s.substr(0, counter));

	s = s.substr(counter);

	return val;
}

double get_double(string& s) {
	int found = s.find(" * ");

	double val = stod(s.substr(0, found));
	// cout << "val = " << val << endl;
	Assert(val > 0, s.c_str(), __FILE__, __LINE__);

	s = s.substr(found);

	return val;
}

void skip(string& s, char c) {
	int cnt = 0;

	while (s[cnt] == c) {
		cnt++;
	}

	s = s.substr(cnt);
}

class Config {

public:
	Config() {
		// ------------
		capacity_ = -1;
		// ------------
		T_ = -1;
		dt_ = -1;
		nt_ = -1;
		// -------------
		precision_ = -1;
		// -------------
		H_csv_ = "";
		U_csv_ = "";

		x_csv_ = "";
		t_csv_ = "";
		z_csv_ = "";
		z_all_csv_ = "";
	}

	// ----------------------------------------------------------------------------------------------------------------
	void get_capacity(string& s) {
		if (!defined(capacity_)) {
			if (is(s, "capacity")) {
				skip(s, ' ');
				skip(s, '=');
				skip(s, ' ');

				capacity_ = stoi(s);
			}
		}
	}
	// ----------------------------------------------------------------------------------------------------------------
	void get_T(string& s) {
		if (!defined(T_))
			if (get_string(s, "T = ")) {
				T_ = get_double(s);

				skip(s, ' ');
				skip(s, '*');
				skip(s, ' ');

				if (get_string(s, "mks")) {
					cout << "mks found" << endl;
					T_ *= mks();
				}
			}
	}

	void get_dt(string& s) {
		if (!defined(dt_)) {
			if (is(s, "dt")) {
				skip(s, ' ');
				skip(s, '=');
				skip(s, ' ');

				if (is(s, "T")) {
					skip(s, ' ');
					skip(s, '/');
					skip(s, ' ');

					dt_ = T_ / get_int(s);
				}
			}

			nt_ = T_ / dt_;
		}
	}
	// ----------------------------------------------------------------------------------------------------------------
	void get_precision(string& s) {
		if (!defined(precision_)) {
			if (is(s, "precision")) {
				skip(s, ' ');
				skip(s, '=');
				skip(s, ' ');

				precision_ = get_int(s);
			}
		}
	}
	// ----------------------------------------------------------------------------------------------------------------
	void get_H_csv() {
		if (defined(path_) && !defined(H_csv_)) {
			H_csv_ = path_ + "H.csv";
		}
	}

	void get_U_csv() {
		if (defined(path_) && !defined(U_csv_)) {
			U_csv_ = path_ + "U.csv";
		}
	}
	// ----------------------------------------------------------------------------------------------------------------
	void get_x_csv() {
		if (defined(path_) && !defined(x_csv_)) {
			x_csv_ = path_ + "x.csv";
		}
	}

	void get_t_csv() {
		if (defined(path_) && !defined(t_csv_)) {
			t_csv_ = path_ + "t.csv";
		}
	}

	void get_z_csv() {
		if (defined(path_) && !defined(z_csv_)) {
			z_csv_ = path_ + "z.csv";
		}
	}

	void get_z_all_csv() {
		if (defined(path_) && !defined(z_all_csv_)) {
			z_all_csv_ = path_ + "z_all.csv";
		}
	}
	// ----------------------------------------------------------------------------------------------------------------
	void println() { cout << endl; }
	double* parse_list(string s, string header, int n) {
		if (s.substr(0, header.size()) == header && s[s.size() - 1] == ']') {
			double* wa = new double[n];
			int s_begin = header.size();

			string s_copy = s.substr(header.size());

			int shift = header.size();

			for (int i = 0; i < n; i++) {
				int s_end;

				if (i == n - 1) {
					s_end = s_copy.size() - 1;
				} else {
					s_end = s_copy.find(", ");
				}

				if (s_end != -1) {
					string s_part = s_copy.substr(0, s_end);

					int found = s_part.find(" * ");

					wa[i] = stod(s_part.substr(0, found));

					if (s_part.substr(found + 3) == "GHz") {
						wa[i] *= GHz;
					} else if (s_part.substr(found + 3) == "MHz") {
						wa[i] *= MHz;
					} else if (s_part.substr(found + 3) == "KHz") {
						wa[i] *= KHz;
					}

					if (i != n - 1) {
						shift += s_end + 2;
						s_copy = s.substr(shift);
					}
				}
			}

			return wa;
		}
	}
	// BEGIN---------------------------------------------- GETTERS ----------------------------------------------------
	int capacity()     const { return capacity_;   }
	// ---------------------------------------------
	double T()         const { return T_;          }

	double dt()        const { return dt_;         }

	int nt()           const { return nt_;         }
	// ---------------------------------------------
	int precision()    const { return precision_;  }

	string path()      const { return path_;       }

	string H_csv()     const { return H_csv_;      }
	string U_csv()     const { return U_csv_;      }

	string x_csv()     const { return x_csv_;      }
	string t_csv()     const { return t_csv_;      }
	string z_csv()     const { return z_csv_;      }
	string z_all_csv() const { return z_all_csv_;  }

	double mks()       const { return mks_;        }

	// END------------------------------------------------ GETTERS ----------------------------------------------------

	// BEGIN---------------------------------------------- PRINT ------------------------------------------------------
	void print_capacity() const {
		cout << "capacity = " << capacity_ << endl;
	}
	// --------------------------------------------
	void print_T() const {
		cout << "T = " << T_ << endl;
	}
	void print_dt() const {
		cout << "dt = " << dt_ << endl;
	}
	void print_nt() const {
		cout << "nt = " << nt_ << endl;
	}
	// ----------------------------------------------
	void print_precision() const {
		cout << "precision = " << precision_ << endl;
	}
	// ----------------------------------------------
	void print_path() const {
		cout << "path = " << path_ << endl;
	}

	void print_H_csv() const {
		cout << "H_csv = " << H_csv_ << endl;
	}

	void print_U_csv() const {
		cout << "U_csv = " << U_csv_ << endl;
	}

	void print_x_csv() const {
		cout << "x_csv = " << x_csv_ << endl;
	}
	void print_t_csv() const {
		cout << "t_csv = " << t_csv_ << endl;
	}
	void print_z_csv() const {
		cout << "z_csv = " << z_csv_ << endl;
	}
	void print_z_all_csv() const {
		cout << "z_all_csv = " << z_all_csv_ << endl;
	}
	// END------------------------------------------------ PRINT ------------------------------------------------------

	~Config() {}

protected:
	// -----------
	int capacity_;
	// -----------
	double T_;
	double dt_;
	int nt_;
	// ------------
	int precision_;
	// ------------
	string path_;

	string H_csv_;
	string U_csv_;

	string x_csv_;
	string t_csv_;
	string z_csv_;
	string z_all_csv_;
};
