#pragma once

#include <iomanip>
#include <sstream>

string wc_str(const double _wc) {
	std::stringstream ss;

	if (_wc >= 1e9) {
		ss << setprecision(3) << (_wc / 1e9) << " ";
		ss << "GHz";
	} else if (_wc >= 1e6) {
		ss << setprecision(3) << (_wc / 1e6) << " ";
		ss << "MHz";
	} else if (_wc >= 1e3) {
		ss << setprecision(3) << (_wc / 1e3) << " ";
		ss << "KHz";
	} else {
		ss << setprecision(3) << _wc / 1e3 << " ";
		ss << "Hz";
	}

	return ss.str();
}

string wa_str(const double _wa) {
	std::stringstream ss;

	if (_wa >= 1e9) {
		ss << setprecision(3) << (_wa / 1e9) << " ";
		ss << "GHz";
	} else if (_wa >= 1e6) {
		ss << setprecision(3) << (_wa / 1e6) << " ";
		ss << "MHz";
	} else if (_wa >= 1e3) {
		ss << setprecision(3) << (_wa / 1e3) << " ";
		ss << "KHz";
	} else {
		ss << setprecision(3) << _wa / 1e3 << " ";
		ss << "Hz";
	}

	return ss.str();
}

string g_str(const double _g) {
	std::stringstream ss;

	if (_g >= 1e9) {
		ss << setprecision(3) << (_g / 1e9) << " ";
		ss << "GHz";
	} else if (_g >= 1e6) {
		ss << setprecision(3) << (_g / 1e6) << " ";
		ss << "MHz";
	} else if (_g >= 1e3) {
		ss << setprecision(3) << (_g / 1e3) << " ";
		ss << "KHz";
	} else {
		ss << setprecision(3) << _g / 1e3 << " ";
		ss << "Hz";
	}

	return ss.str();
}

string T_str(const double _T) {
	std::stringstream ss;

	if (_T >= 1e-3) {
		ss << setprecision(3) << (_T * 1e3) << " ";
		ss << "ms";
	} else if (_T >= 1e-6) {
		ss << setprecision(3) << (_T * 1e6) << " ";
		ss << "mks";
	} else if (_T >= 1e-9) {
		ss << setprecision(3) << (_T * 1e9) << " ";
		ss << "ns";
	} else {
		ss << setprecision(3) << _T << " ";
		ss << "s";
	}

	return ss.str();
}

double T_str_v(const double _T) {
	if (_T >= 1e-3) {
		return (_T * 1e3);
	} else if (_T >= 1e-6) {
		return (_T * 1e6);
	} else if (_T >= 1e-9) {
		return (_T * 1e9);
	} else {
		return _T;
	}
}
