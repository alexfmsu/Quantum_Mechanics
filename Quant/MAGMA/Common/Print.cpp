#pragma once

#include <vector>

void print(int _x) {
	cout << _x;
}

void print(double _x) {
	cout << _x;
}

void print(string _s = "", string color = "") {
	if (color == "green") {
		cout << "\033[1;36;1m" << _s << "\033[1;32;0m";
	} else if (color == "red") {
		cout << "\033[1;31;1m" << _s << "\033[1;32;0m";
	} else if (color == "yellow") {
		cout << "\033[1;33;1m" << _s << "\033[1;32;0m";
	} else if (color == "blue") {
		cout << "\033[1;34;1m" << _s << "\033[1;32;0m";
	} else {
		cout << _s;
	}
}

void println(int count) {
	for (int i = 0; i < count; i++) {
		cout << endl;
	}
}

void println(string _s = "", string color = "") {
	print(_s, color);

	cout << endl;
}


void print(vector<int> _v) {
	for (int i = 0; i < _v.size(); i++) {
		cout << _v[i];
	}

	cout << endl;
}
