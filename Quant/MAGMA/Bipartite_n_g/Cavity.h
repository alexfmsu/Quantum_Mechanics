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
    Cavity(const int _n, const double _wc, const double _wa, const double _g) {
        Assert(_n > 0, "n <= 0", __FILE__, __LINE__);
        n_ = _n;

        Assert(_wc > 0, "wc <= 0", __FILE__, __LINE__);
        wc_ = _wc;

        Assert(_wa > 0, "wa <= 0", __FILE__, __LINE__);
        wa_ = _wa;

        Assert(_g > 0, "g <= 0", __FILE__, __LINE__);
        g_ = _g;
    }
    // END-------------------------------------- CONSTRUCTOR --------------------------------------

    Cavity operator=(const Cavity& _cv) {
        return Cavity(_cv.n_, _cv.wc_, _cv.wa_, _cv.g_);
    }

    // BEGIN------------------------------------ GETTERS ------------------------------------------
    int     n() const { return  n_; }
    double wc() const { return wc_; }
    double wa() const { return wa_; }
    double  g() const { return  g_; }
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

        print(wa_str(wa_));

        println(2);
    }

    void print_g() const {
        print(" g: ", "green");

        print(g_str(g_));

        println(2);
    }
    // END-------------------------------------- PRINT --------------------------------------------

    // BEGIN------------------------------------ DESTRUCTOR ---------------------------------------
    ~Cavity() {}
    // END-------------------------------------- DESTRUCTOR ---------------------------------------

private:
    int n_;

    double wc_;
    double wa_;

    double g_;
};
// ------------------------------------------------------------------------------------------------
