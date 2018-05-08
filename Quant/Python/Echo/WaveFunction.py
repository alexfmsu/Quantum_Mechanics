# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
# -------------------------------------------------------------------------------------------------
# Common
from Common.Assert import *
from Common.Matrix import *
from Common.C_n_k import *

from math import exp
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
class WaveFunction(Matrix):

    # ---------------------------------------------------------------------------------------------
    def __init__(self, states, init_state=None, amplitude=1):
        Assert(isinstance(states, dict), "states is not dict", cf())
        Assert(len(states) > 1, "w_0 is not set", cf())

        self.states = states

        super(WaveFunction, self).__init__(
            m=len(states), n=1, dtype=np.complex128)

        if init_state is None:
            return

        Assert(isinstance(init_state, list), "init_state is not list", cf())

        k_found = None

        for k, v in states.items():
            if init_state == v:
                k_found = k

                break

        Assert(k_found, "w_0 is not set", cf())

        super(WaveFunction, self).__init__(
            m=len(states), n=1, dtype=np.complex128)

        self.data[k_found] = amplitude
    # ---------------------------------------------------------------------------------------------

    def set_ampl(self, state, amplitude=1):
        k_found = None

        for k, v in self.states.items():
            if state == v:
                k_found = k
                break

        Assert(k_found is not None, "w_0 is not set", cf())
        self.data[k_found] = amplitude

    # ---------------------------------------------------------------------------------------------
    def glauber(self, capacity, cv_num):
        n = capacity
        print(self.states)

        alpha = sqrt(capacity)
        # alpha = 1

        # for a in range(n+1):
        for i in range(len(self.states)):
                # for _, n_ in self.states.items():
                    # n_ = self.states(i)
                    # n_ = [[i], [0]]

                    # print(n_)
            n_ = self.states[i][cv_num][0]

            drop = False

            for _cv in range(len(self.states[i])):
                # print([0] + [0]*(len(self.states[i][_cv])-1))
                print(self.states[i][_cv])
                if _cv != cv_num and self.states[i][_cv] != [0] + [0]*(len(self.states[i][_cv])-1):
                    drop = True
                    break

            if drop:
                continue

            self.data[i] = exp(-abs(alpha**2/2)) * \
                (alpha**n_) / sqrt(fact(n_))
            print(alpha, n_, "exp(", -abs(alpha**2/2), ") * ", alpha**n_,
                  " / ", sqrt(fact(n_)), "=", self.data[i])
            # print(15, self.data[n_])
        # self.print()
        # exit(0)
        # sum += a**n / sqrt(fact(n))
        # self.data[state] = exp(-abs(a)/2) *

    def normalize(self):
        norm = np.linalg.norm(self.data)

        Assert(norm > 0, "norm <= 0", cf())

        self.data /= norm
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print(self):
        for k, v in self.states.items():
            print(v, np.asarray(self.data[k]).reshape(-1)[0])
    # ---------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------
