// ------------------------------------------------------------------------------------------------
#pragma once
// ------------------------------------------------------------------------------------------------
// system
#include <iostream>
#include <string>
#include <map>
// ------------------------------------------------------------------------------------------------
#include "matrix.h"
// ------------------------------------------------------------------------------------------------
#include "Cavity.h"
#include "CV_state.h"
// ------------------------------------------------------------------------------------------------

using namespace std;

// ------------------------------------------------------------------------------------------------
typedef pair<int, int> BinState;
// ---------------------------
double a_cross(const int ph) {
    return sqrt(ph + 1);
}

double a(const int ph) {
    return sqrt(ph);
}
// ---------------------------


class Hamiltonian: public Matrix {

private:
    Cavity* cv_;

    int capacity_;

public:
    vector<std::string> bin_states_keys;
    map<int, CV_state> states_;
    map<std::string, vector<int> > bin_states_;

    Hamiltonian(int _capacity, Cavity* _cv, magma_queue_t* queue) : Matrix() {
        // -------------------
        Assert(_capacity > 0, "capacity <= 0", __FILE__, __LINE__);
        capacity_ = _capacity;

        cv_ = _cv;

        queue_ = queue;

        init_states();
        // -------------
        M_ = N_ = states_.size();
        size_ = M_ * N_;

        cpu_alloc();
        gpu_alloc();

        get_bin_states();

        for (int i = 0; i < M(); i++) {
            CV_state* state_i = &(states_[i]);

            int i_ph = states_[i].ph();
            AtomState i_at = states_[i].at();


            for (int j = 0; j < N(); j++) {
                CV_state* state_j = &(states_[j]);

                int j_ph = states_[j].ph();
                AtomState j_at = states_[j].at();

                if (i == j) {
                    double value = cv_->wc() * i_ph;

                    for (int _ = 0; _ < cv_->n(); _++) {
                        value += cv_->wa(_) * i_at[_];
                    }

                    set(i, j, value);
                } else {
                    int d_ph = j_ph - i_ph;

                    int diff_at_cnt;

                    if (abs(d_ph) == 1) {
                        diff_at_cnt = 0;

                        int diff_at_num;

                        for (int n_ = 0; n_ < i_at.size(); n_++) {
                            int d_at = j_at[n_] - i_at[n_];

                            if (d_ph + d_at == 0) {
                                diff_at_cnt++;
                                diff_at_num = n_;
                            } else if (d_at != 0) {
                                diff_at_cnt = 0;
                                break;
                            }

                            if (diff_at_cnt > 1) {
                                break;
                            }
                        }

                        if (diff_at_cnt == 1) {
                            double k;

                            if (d_ph > 0) {
                                k = a_cross(i_ph) * i_at[diff_at_num];
                            } else {
                                k = a_cross(j_ph) * j_at[diff_at_num];
                            }

                            set(i, j, cv_->g(diff_at_num) * sqrt(max(i_ph, j_ph)));
                        }
                    }
                }
            }
        }
    }


    double capacity() const { return capacity_; }

    void print_indices() {}

    // Cavity* cavity() {
    //     return cv_;
    // }


    void init_states() {
        CV_state state(capacity_, cv_->n());
        state.ph() = capacity_;

        int cnt = 0;

        states_[cnt] = state;
        cnt++;

        state.ph()--;

        while (1) {
            bool inced = false;

            for (int n_ = cv_->n() - 1; n_ >= 0; n_--) {
                if (state.at(n_) == 1) {
                    state.at(n_) = 0;

                    continue;
                }

                if (state.ph() + state.at_sum() > capacity_) {
                    continue;
                }

                state.at(n_)++;

                inced = true;

                state.ph() = capacity_ - state.at_sum();

                if (state.ph() >= 0) {
                    states_[cnt] = state;
                    cnt++;
                }

                break;
            }

            if (!inced) {
                break;
            }
        }
    }

    map<std::string, vector<int> >* get_bin_states() {
        map<std::string, vector<int> > states_bin;

        for (auto k : states_) {
            int en1 = k.second.sum_1();
            int en2 = k.second.sum_2();

            // if (en1 == 0 && en2 != cv_->n())
            // continue;

            std::string st = "[" + to_string((long long)en1) + "," + to_string((long long)en2) + "]";

            if (states_bin.find(st) == states_bin.end()) {
                states_bin[st] = vector<int>(0);
            }

            states_bin[st].push_back(k.first);
        }

        for (int k1 = 0; k1 <= cv_->n() / 2; k1++) {
            for (int k2 = 0; k2 <= cv_->n() / 2; k2++) {
                if (k1 + k2 > capacity_) {
                    // if (k1 + k2 > capacity_ || (k1 == 0 && k2 != cv_->n())) {
                    break;
                }

                std::string k = "[" + to_string((long long)k1) + "," + to_string((long long)k2) + "]";

                bin_states_keys.push_back(k);

                if (states_bin.find(k) != states_bin.end()) {
                    bin_states_[k] = states_bin[k];
                }
            }
        }

        return &bin_states_;
    }

    map<int, CV_state>* states() {
        return &states_;
    }
    // --------------------------------------------------------------------------------------------
    void print_bin_states() {
        for (auto k : bin_states_) {
            cout << k.first << " ";

            for (int i = 0; i < k.second.size(); i++) {
                cout << k.second[i];

                if (i != k.second.size() - 1) {
                    cout << ", ";
                }
            }

            cout << endl;
        }
    }

    void print_states() {
        cout << "STATES:" << endl;

        for (auto k : states_) {
            cout << k.first << " ";
            k.second.print();
        }
    }
    // --------------------------------------------------------------------------------------------
};
