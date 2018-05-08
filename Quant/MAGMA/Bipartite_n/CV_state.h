#pragma once

#include <vector>

using namespace std;

typedef vector<int> AtomState;


class CV_state {

public:
	CV_state() {}

	void set(int _ph, AtomState _at) {
		state_.first = _ph;
		state_.second = _at;
	}

	CV_state(int _capacity, int _n): capacity_(_capacity), n_(_n) {
		ph_ = 0;
		at_ = AtomState(n_ * 2);

		state_ = make_pair(ph_, at_);
	}

	int sum() {
		int sum_ = state_.first;

		for (int i = 0; i < n_ * 2; i++) {
			sum_ += state_.second[i];
		}

		return sum_;
	}

	int sum_1() {
		int sum_ = 0;

		for (int i = 0; i < n_; i++) {
			sum_ += state_.second[i];
		}

		return sum_;
	}

	int sum_2() {
		int sum_ = 0;

		for (int i = n_; i < n_ * 2; i++) {
			sum_ += state_.second[i];
		}

		return sum_;
	}

	int at_sum() {
		int sum_ = 0;

		for (int i = 0; i < n_ * 2; i++) {
			sum_ += state_.second[i];
		}

		return sum_;
	}

	int& ph() {
		return state_.first;
	}

	AtomState at() {
		return (state_.second);
	}

	int& at(int _n) {
		return (state_.second[_n]);
	}

	bool operator!=(CV_state& _cv_state) {
		return !(*this == _cv_state);
	}

	bool operator==(CV_state& _cv_state) {
		bool at_eq = true;
		for (int _ = 0; _ < n_ * 2; _++) {
			if (at()[_] != _cv_state.at()[_]) {
				at_eq = false;
				break;
			}
		}

		return ph() == _cv_state.ph() && at_eq;
	}

	void print() {
		cout << "[" << state_.first << ", [";

		for (int i = 0; i < (state_.second).size(); i++) {
			cout << (state_.second)[i];

			if (i != state_.second.size() - 1) {
				cout << ", ";
			}
		}

		cout << "]]" << endl;
	}

	bool inc() {
		bool inced = false;

		for (int i = n_ * 2 - 1; i >= 0; i--) {
			if (state_.second[i] == 1) {
				state_.second[i] = 0;
				continue;
			} else if (state_.second[i] == 0) {
				state_.second[i] = 1;
				inced = true;
				break;
			}
		}

		if (!inced) {
			if (sum() > capacity_) {
				return false;
			}

			state_.first++;
			inced = true;
		}

		return inced;
	}
	void zeros() {
		state_.first = 0;

		for (int i = 0; i < n_ * 2; i++) {
			state_.second[i] = 0;
		}
	}

	int ph_;
	AtomState at_;
	std::pair<int, AtomState>state_;

private:
	int capacity_;
	int n_;
};


typedef vector<CV_state> State;


class Full_state {

public:
	Full_state() {}

	Full_state(int _capacity, State& _cv_states) {
		capacity_ = _capacity;
		state_ = _cv_states;
	}

	State state() {
		return state_;
	}

	int sum() {
		int sum_ = 0;

		for (int i = 0; i < 2; i++) {
			sum_ += state_[i].sum();
		}

		return sum_;
	}

	int inc() {
		while (1) {
			bool inced = false;

			for (int i = 1; i >= 0; i--) {
				if (!state_[i].inc()) {
					state_[i].zeros();

					continue;
				} else {
					inced = true;

					break;
				}
			}

			if (!inced) {
				return -1;
			}

			if (sum() == capacity_) {
				return 0;
			}
		}

		return 0;
	}

	void print() {
		cout << "[";

		for (int i = 0; i < state_.size(); i++) {
			state_[i].print();

			if (i != state_.size() - 1) {
				cout << ", ";
			}
		}

		cout << "]" << endl;
	}

	void print_all() {
		cout << "[";

		for (int i = 0; i < state_.size(); i++) {
			state_[i].print();

			if (i != state_.size() - 1) {
				cout << ", ";
			}
		}

		cout << "]" << endl;

		cout << endl;
	}

private:
	int capacity_;

	State state_;
};
