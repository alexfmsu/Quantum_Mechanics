#pragma once

#include <complex>

class Complexd {

public:
    Complexd(double re, double im = 0.0) {
        value_ = MAGMA_Z_MAKE(re, im);
    }

    Complexd(magmaDoubleComplex v) {
        value_ = v;
    }

    double real() const {
        return MAGMA_Z_REAL(value_);
    }

    double imag() const {
        return MAGMA_Z_IMAG(value_);
    }

    double abs() const {
        return MAGMA_Z_ABS(value_);
    }

    Complexd sqrt() const {
        return Complexd(magma_zsqrt(value_));
    }

    Complexd conj() const {
        return Complexd(MAGMA_Z_CONJ(value_));
    }

    Complexd operator+(double tmp) {
        magmaDoubleComplex v1 = value_;
        magmaDoubleComplex v2 = MAGMA_Z_MAKE(tmp, 0);

        return Complexd(MAGMA_Z_ADD(v1, v2));
    }

    Complexd operator+(Complexd tmp) {
        magmaDoubleComplex v1 = value_;
        magmaDoubleComplex v2 = tmp.value_;

        return Complexd(MAGMA_Z_ADD(v1, v2));
    }

    Complexd& operator+=(Complexd tmp) {
        *this = *this + tmp;

        return *this;
    }



    Complexd operator*(Complexd tmp) {
        magmaDoubleComplex v1 = value_;
        magmaDoubleComplex v2 = tmp.value_;

        return Complexd(MAGMA_Z_MUL(v1, v2));
    }

    magmaDoubleComplex value() const {
        return value_;
    }

private:
    magmaDoubleComplex value_;

};

const Complexd cONE(1.0);
const Complexd cZERO(0.0);


/*-----------------PC----------------*/
// typedef int INT;
// typedef complex<double> complexd;
typedef magmaDoubleComplex complexd;

/*-----------------PC----------------*/

/*-----------------SC----------------*/
// typedef MKL_INT INT;
// typedef MKL_Complex16 complexd;
/*-----------------SC----------------*/

// int iZERO = 0;
// complexd cZERO(0, 0);

// int iONE = 1;
// complexd cONE(1, 0);

// void print(vector<complexd> tmp) {
//     for (int i = 0; i < tmp.size(); i++) {
//         cout << tmp.at(i) << " ";
//     }
// }

// void open_file(const char* filename, bool mpiroot) {
//     if (mpiroot) {
//         ofstream fout;

//         fout.open(filename, std::ios_base::out);
//     }
// }

// void write_to_file(vector<complexd> tmp, const char* filename, bool mpiroot) {
//     if (mpiroot) {
//         ofstream fout;

//         fout.open(filename, std::ios_base::app);

//         for (int i = 0; i < tmp.size(); i++) {
//             fout << tmp.at(i) << " ";
//         }

//         fout << "\n";

//         fout.close();
//     }
// }

// void accept(Grid* grid, bool expr, const char* msg) {
//     if (expr) {
//         if (grid->myid() == 0) {
//             cout << msg << endl;
//         }
//         grid->abort();
//         exit(1);
//     }

// }

// const complexd alpha = cONE;
// const complexd beta = cZERO;
