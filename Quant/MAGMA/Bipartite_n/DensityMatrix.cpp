#pragma once

#include <iostream>
#include <cmath>

#include "CV_state.h"
#include "../include/matrix.h"

using namespace std;

class DensityMatrix: public Matrix {

public:
    DensityMatrix(Matrix* _H, map<int, point>* _states) : Matrix(_H->M(), _H->N()) {
        states_ = _states;
    }

    void set_ampl(point* _state, Complexd _val) {
        bool found = false;

        for (auto k : *states_) {
            bool eq = (k.second == *_state);

            if (eq) {
                found = true;
                set(k.first, k.first, _val);

                break;
            }
        }

        if (!found) {
            cout << "Not found" << endl;
            exit(1);
        }
    }

    double norm(vector<Complexd>& diag_) {
        double norm_ = 0;

        for (int i = 0; i < diag_.size(); i++) {
            norm_ += diag_[i].abs();
        }

        return norm_;
    }

    double norm() {
        double norm_ = 0;

        vector<Complexd> diag_ = diag();

        for (int i = 0; i < diag_.size(); i++) {
            norm_ += diag_[i].abs();
        }

        return norm_;
    }

    ~DensityMatrix() {

    }

private:
    map<int, point>* states_;
};
