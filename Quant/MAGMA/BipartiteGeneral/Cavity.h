// ------------------------------------------------------------------------------------------------
#pragma once
// ------------------------------------------------------------------------------------------------
// Common
#include "Assert.cpp"
#include "Print.cpp"
#include "STR.cpp"
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
class Cavity {

public:
    // BEGIN------------------------------------ CONSTRUCTOR --------------------------------------
    Cavity(const int _n, const double _wc, const double* _wa, const double* _g) {
        Assert(_n > 0, "n <= 0", __FILE__, __LINE__);
        n_ = _n;

        Assert(_wc > 0, "wc <= 0", __FILE__, __LINE__);
        wc_ = _wc;

        wa_ = new double[n_];
        g_ = new double[n_];

        for (int i = 0; i < n_; i++) {
            Assert(_wa[i] > 0, "wa <= 0", __FILE__, __LINE__);
            wa_[i] = _wa[i];

            Assert(_g[i] > 0, "g <= 0", __FILE__, __LINE__);
            g_[i] = _g[i];
        }
    }
    // END-------------------------------------- CONSTRUCTOR --------------------------------------

    Cavity operator=(const Cavity& _cv) {
        return Cavity(_cv.n_, _cv.wc_, _cv.wa_, _cv.g_);
    }

    // BEGIN------------------------------------ GETTERS ------------------------------------------
    int     n()             const { return      n_; }
    double wc()             const { return     wc_; }
    double wa(const int _i) const { return wa_[_i]; }
    double  g(const int _i) const { return  g_[_i]; }
    // END-------------------------------------- GETTERS ------------------------------------------

    // BEGIN------------------------------------ PRINT --------------------------------------------
    void print_n() const {
        print(" n: ", "green");

        print(n_);

        println(2);
    }

    void print_wc() const {
        print("wc: ", "green");

        print(wc_str(wc_));

        println(2);
    }

    void print_wa() const {
        print("wa: ", "green");

        print("[");

        for (int i = 0; i < n_; i++) {
            print(wa_str(wa_[i]));

            if (i != n_ - 1) {
                cout << ", ";
            }
        }

        print("]");

        println(2);
    }

    void print_g() const {
        print(" g: ", "green");

        print("[");

        for (int i = 0; i < n_; i++) {
            print(g_str(g_[i]));

            if (i != n_ - 1) {
                cout << ", ";
            }
        }

        print("]");

        println(2);
    }
    // END-------------------------------------- PRINT --------------------------------------------

    // BEGIN------------------------------------ DESTRUCTOR ---------------------------------------
    ~Cavity() {
        delete[] wa_;
        delete[]  g_;
    }
    // END-------------------------------------- DESTRUCTOR ---------------------------------------

private:
    int n_;

    double wc_;
    double* wa_;

    double* g_;
};
// ------------------------------------------------------------------------------------------------
