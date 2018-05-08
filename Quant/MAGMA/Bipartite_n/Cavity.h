#pragma once

// ------------------------------------------------------------------------------------------------
// Common
#include "../Common/Assert.cpp"
#include "../Common/Print.cpp"
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
class Cavity {

public:
    // BEGIN------------------------------------ CONSTRUCTOR --------------------------------------
    Cavity(const int _n, const double _wc, const double _wa, const double _g) {
        Assert(_n > 0, "n <= 0", __FILE__, __LINE__);
        Assert(_wc > 0, "wc <= 0", __FILE__, __LINE__);
        Assert(_wa > 0, "wa <= 0", __FILE__, __LINE__);
        Assert(_g > 0, "g <= 0", __FILE__, __LINE__);

        n_ = _n;

        wc_ = _wc;
        wa_ = _wa;

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

        if (wc_ >= 1e9) {
            print(wc_ / 1e9);
            print(" GHz");
        } else if (wc_ >= 1e6) {
            print(wc_ / 1e6);
            print(" MHz");
        } else if (wc_ >= 1e3) {
            print(wc_ / 1e3);
            print(" KHz");
        } else {
            print(wc_);
            print(" Hz");
        }

        println(2);
    }

    void print_wa() const {
        print("wa: ", "green");

        if (wa_ >= 1e9) {
            print(wa_ / 1e9);
            print(" GHz");
        } else if (wa_ >= 1e6) {
            print(wa_ / 1e6);
            print(" MHz");
        } else if (wa_ >= 1e3) {
            print(wa_ / 1e3);
            print(" KHz");
        } else {
            print(wa_);
            print(" Hz");
        }

        println(2);
    }

    void print_g() const {
        print(" g: ", "green");

        if (g_ >= 1e9) {
            print(g_ / 1e9);
            print(" GHz");
        } else if (g_ >= 1e6) {
            print(g_ / 1e6);
            print(" MHz");
        } else if (g_ >= 1e3) {
            print(g_ / 1e3);
            print(" KHz");
        } else {
            print(g_);
            print(" Hz");
        }

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
