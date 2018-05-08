#include <iostream>
#include <string>
#include <map>

#include "../include/matrix.h"

#include "Cavity.h"
#include "CV_state.h"

using namespace std;

typedef pair<int, int> BinState;
// ---------------------------
double a_cross(const int ph) {
    return sqrt(ph + 1);
}

double a(const int ph) {
    return sqrt(ph);
}
// ---------------------------

typedef pair<int, int> point;


class Hamiltonian: public Matrix {

public:
    vector<std::string> bin_states_keys;
    map<std::string, vector<int> > bin_states_;

    Hamiltonian(int _capacity, Cavity* _cv, magma_queue_t* queue) : Matrix() {
        // -------------------
        Assert(_capacity > 0, "capacity <= 0", __FILE__, __LINE__);
        capacity_ = _capacity;

        cv_ = _cv;

        queue_ = queue;

        init_states();
        // -------------
        int M = capacity_;
        int n = cv_->n();

        int _min = std::min(M, n);
        int dime = (_min + 1) * (_min + 1);

        // M_ = N_ = states_.size();
        M_ = N_ = dime;
        size_ = M_ * N_;

        cpu_alloc();
        gpu_alloc();


        //     for (int i = 0; i < M(); i++) {
        //         int i_n1 = states_[i].first;
        //         int i_n2 = states_[i].second;

        //         for (int j = 0; j < N(); j++) {
        //             if (i == j) {
        //                 int ph = capacity_ - states_[i].first - states_[i].second;
        //                 int at = states_[i].first + states_[i].second;

        //                 double val = ph * cv_->wc() + at * cv_->wa();

        //                 set(i, j, val);

        //             } else {
        //                 int j_n1 = states_[j].first;
        //                 int j_n2 = states_[j].second;

        //                 if (abs(i_n1 - j_n1) + abs(i_n2 - j_n2) == 1) {
        //                     int m = 1;

        //                     if (i_n1 != j_n1) {
        //                         m = min(i_n1, j_n1);
        //                     } else if (i_n2 != j_n2) {
        //                         m = min(i_n2, j_n2);
        //                     }

        //                     int kappa = (n - m) * (m + 1);

        //                     int _max = std::max(capacity_ - i_n1 - i_n2, capacity_ - j_n1 - j_n2);

        //                     double val = cv_->g() * sqrt(max(_max, 0) * kappa);
        //                     cout << i << " " << j << " -> " << val << endl;
        //                     set(i, j, val);
        //                 }
        //             }
        //         }
        //     }
        // }

        double wc = cv_->wc();
        double wa = cv_->wa();
        double g = cv_->g();

        int i = 1, j;
        int count = 0;

        std::pair<int, int> p;

        // FROM PYTHON
        for (int i1 = 0; i1 <= _min; i1++) {
            for (int i2 = 0; i2 <= min(n, M - i1); i2++) {
                j = 1;

                states_[count] = {i1, i2};

                count++;

                for (int j1 = 0; j1 <= _min; j1++) {
                    for (int j2 = 0; j2 <= min(n, M - j1); j2++) {
                        if (i1 != j1) {
                            p = make_pair(i1, j1);
                        } else if (i2 != j2) {
                            p = make_pair(i2, j2);
                        } else {
                            p = make_pair(1, 2);
                        }

                        int mi = min(p.first, p.second);

                        double kappa = sqrt((n - mi) * (mi + 1));

                        if (abs(i1 - j1) + abs(i2 - j2) == 1) {
                            // _max = max(M - i1 - i2, M - j1 - j2);
                            set(i - 1, j - 1, g * sqrt(max(M - i1 - i2, M - j1 - j2)) * kappa);
                        } else if (abs(i1 - j1) + abs(i2 - j2) == 0) {
                            set(i - 1, j - 1, (M - (i1 + i2)) * wc + (i1 + i2) * wa);

                            // cout << i << " " << j << " -> " << 0 << endl;

                        } else {
                            set(i - 1, j - 1, 0);
                            // cout << i << " " << j << " -> " << 0 << endl;
                        }

                        j++;
                    }
                }

                i++;
            }
        }

        // M = N =
    }

    double capacity() const { return capacity_; }

    // Cavity* cavity() {
    //     return cv_;
    // }

    void init_states() {
        int n = cv_->n();

        int k = 0;

        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= capacity_ - i; j++) {
                states_[k++] = make_pair(i, j);
            }
        }
    }

    map<int, point>* states() {
        return &states_;
    }

    // --------------------------------------------------------------------------------------------

    void print_states() {
        // cout << "STATES:" << endl;

        // for (auto k : states_) {
        //     cout << k.first << " ";
        //     k.second.print();
        // }
    }
    // --------------------------------------------------------------------------------------------

private:
    map<int, point> states_;
    Cavity* cv_;

    int capacity_;
};
