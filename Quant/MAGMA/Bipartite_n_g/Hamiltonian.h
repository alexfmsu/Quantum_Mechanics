#include <iostream>
#include <string>
#include <map>

#include "matrix.h"

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

        // init_states();
        // -------------
        int M = capacity_;
        int n = cv_->n();

        int _min = std::min(M, n);

        int count = 0;
        // -------------
        for (int i1 = 0; i1 <= _min; i1++) {
            for (int i2 = 0; i2 <= min(n, M - i1); i2++) {
                states_[count] = make_pair(i1, i2);

                count++;
            }
        }

        int dime = states_.size();

        M_ = N_ = dime;
        size_ = M_ * N_;

        cpu_alloc();
        gpu_alloc();

        double wc = cv_->wc();
        double wa = cv_->wa();
        double g = cv_->g();

        int i = 1, j;
        count = 0;

        std::pair<int, int> p;

        // FROM PYTHON
        for (int i1 = 0; i1 <= _min; i1++) {
            for (int i2 = 0; i2 <= min(n, M - i1); i2++) {
                j = 1;

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
                            set(i - 1, j - 1, g * sqrt(max(M - i1 - i2, M - j1 - j2)) * kappa);
                        } else if (abs(i1 - j1) + abs(i2 - j2) == 0) {
                            set(i - 1, j - 1, (M - (i1 + i2)) * wc + (i1 + i2) * wa);
                        } else {
                            set(i - 1, j - 1, 0);
                        }

                        j++;
                    }
                }

                i++;
            }
        }

        cout << states_.size() << endl;
    }

    // BEGIN------------------------------------ GETTERS ------------------------------------------
    double capacity() const { return capacity_; }

    Cavity* cavity() { return cv_; }
    // END-------------------------------------- GETTERS ------------------------------------------
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
        cout << "STATES:" << endl;

        for (auto k : states_) {
            cout << "[" << k.second.first << " " << k.second.second << "]" << endl;
            // k.second.print();
        }
    }
    // --------------------------------------------------------------------------------------------

private:
    int capacity_;

    Cavity* cv_;

    map<int, point> states_;
};
