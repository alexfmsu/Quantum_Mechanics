#pragma once

#include <cmath>

#include "CV_state.h"
#include "matrix.h"


class DensityMatrix: public Matrix {

public:
    DensityMatrix(Matrix* _H, map<int, CV_state>* _states) : Matrix(_H->M(), _H->N()) {
        states_ = _states;
    }

    DensityMatrix(DensityMatrix& tmp) : Matrix(tmp.M_, tmp.N_) {
        states_ = tmp.states_;

        memcpy(cpu_data(), tmp.cpu_data(), M_ * N_ * sizeof(magmaDoubleComplex));
    }

    void set_ampl(CV_state* _state, Complexd _val) {
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

    ~DensityMatrix() {}

private:
    map<int, CV_state>* states_;
};
