#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "File.h"
#include "Assert.cpp"

using namespace std;

void mkdir(std::string path) {
	string cmd;

	cmd = "if [ -d '" + path + "' ]; then rm -r " + path + "; fi";

	if (system(cmd.c_str())) {
		Assert(false, "Error: system(cmd.c_str())", __FILE__, __LINE__);
	}

	cmd = "mkdir -p " + path;

	if (system(cmd.c_str())) {
		Assert(false, "Error: system(cmd.c_str())", __FILE__, __LINE__);
	}
}

void write_states(string x_csv, vector<std::string> bin_states_keys) {
	File X_csv(x_csv);

	X_csv.erase();
	X_csv.open("w");

	X_csv.write("x,vals");
	X_csv.writeln();

	int cnt = 0;

	for (int i = 0; i < bin_states_keys.size(); i++) {
		std::ostringstream ostream;
		ostream << "\"" << bin_states_keys[i] << "\"," << cnt;

		X_csv.write(ostream.str());
		X_csv.writeln();

		cnt++;
	}

	X_csv.close();
}

void write_t(string t_csv, double T, int nt) {
	// T *= 1e6;

	File T_csv(t_csv);

	T_csv.erase();
	T_csv.open("w");

	T_csv.write("y,vals");
	T_csv.writeln();

	int count = 10;

	double _dt = T / count;
	double _nt = (nt + .0) / count;

	double cnt = 0;

	for (double t = 0; t <= T; t += _dt, cnt += _nt) {
		std::ostringstream ostream;
		ostream << std::fixed << std::setprecision(1) << t << "," << std::setprecision(1) << cnt;

		T_csv.write(ostream.str());
		T_csv.writeln();
	}

	T_csv.close();
}
