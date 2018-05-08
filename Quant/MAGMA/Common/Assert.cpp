#pragma once

using namespace std;

#include "Print.cpp"

void Assert(bool expr, const char* err, const char* file, const int line) {
	if (!expr) {
		print("Error: ", "red");

		print(err);
		print(" in ");
		print(file);
		print(" at line ");
		print(line);

		println();

		exit(1);
	}
}