#pragma once

#include <iostream>
#include <fstream>
#include <fstream>

#include "Types.h"

using namespace std;

class File {

public:
    File(string _filename) {
        filename = _filename;

        ofstream fout;
    }

    void erase() {
        fout.open(filename, ios::out);
        fout.close();
    }

    void open(const char* mode) {
        if (mode == "w") {
            fout.open(filename, ios_base::app);
        }
    }

    void write(double tmp, int precision = 3) {
        if (precision != -1) {
            fout << fixed << setprecision(precision) << tmp;
        } else {
            fout << fixed << tmp;
        }
    }

    void write(const char* tmp) {
        fout << tmp;
    }

    void writeln(const char* tmp) {
        fout << tmp << endl;
    }


    void write(string tmp) {
        fout << tmp;
    }


    void write(Complexd tmp, int precision = 3) {
        double re = tmp.real();
        double im = tmp.imag();

        double eps = 1e-4;

        if (abs(re) < eps && re < 0) {
            re = +0.0;
        }

        if (abs(im) < eps && im < 0) {
            im = +0.0;
        }

        if (precision != -1) {
            fout << fixed << setprecision(precision) << tmp.abs();
        } else {
            fout << fixed << tmp.abs();
        }
        // fout << "(" << fixed << std::setprecision(precision) << re << "," << im << ")";
    }

    void writeln(Complexd tmp, int precision = 3) {
        write(tmp, precision);

        fout << "\n";
    }

    void write(vector<Complexd> tmp, int precision = 3) {
        for (int i = 0; i < tmp.size(); i++) {
            write(tmp.at(i), precision);

            if (i !=  tmp.size() - 1) {
                fout << ",";
            }
        }

        fout << endl;
    }

    void write(vector<double> tmp, int precision = 3) {
        for (int i = 0; i < tmp.size(); i++) {
            write(tmp.at(i), precision);

            if (i !=  tmp.size() - 1) {
                fout << ",";
            }
        }

        fout << endl;
    }

    // template<class T>
    // void write(vector<T> tmp, int precision = 3) {
    //     for (int i = 0; i < tmp.size(); i++) {
    //         write(tmp.at(i), precision);

    //         if (i !=  tmp.size() - 1) {
    //             fout << ",";
    //         }
    //     }

    //     fout << endl;
    // }

    void writeln() {
        fout << endl;
    }

    void close() {
        fout.close();
    }


private:
    string filename;

    ofstream fout;
};
