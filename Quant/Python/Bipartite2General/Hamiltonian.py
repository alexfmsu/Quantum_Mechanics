# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
# -------------------------------------------------------------------------------------------------
# system
from math import sqrt
import webbrowser
import copy
import time
# -------------------------------------------------------------------------------------------------
# Common
from Common.Assert import *
from Common.Matrix import *
from Common.Print import *
# -------------------------------------------------------------------------------------------------
from .Cavity import Cavity

from Bipartite2General.CV_state import *

# -------------------------------------------------------------------------------------------------


def a_plus(ph):
    return sqrt(ph + 1)


def a_minus(ph):
    return sqrt(ph)


class Hamiltonian:

    def __init__(self, capacity, cavity, mu):
        Assert(isinstance(capacity, int), "capacity is not integer", cf())
        Assert(capacity > 0, "capacity <= 0", cf())

        Assert(isinstance(cavity, list), "capacity is not list", cf())

        for _ in range(len(cavity)):
            Assert(isinstance(cavity[_], Cavity), "is not cavity", cf())

        self.capacity = capacity

        self.cavity = cavity

        self.n = []

        self.wc = []
        self.wa = []

        self.g = []

        for i in range(len(cavity)):
            self.n.append(cavity[i].n)

            self.wc.append(cavity[i].wc)
            self.wa.append(cavity[i].wa)

            self.g.append(cavity[i].g)

        self.mu = mu

        self.states = self.get_states()
        self.get_states_bin()
        # print(self.states)
        # exit(1)

        self.size = len(self.states)

        self.matrix = Matrix(self.size, self.size, dtype=np.complex128)

        for i in range(self.size):
            i_state = self.states[i]

            i_ph = []
            i_at = []

            i_ph.append(i_state[0][0])
            i_at.append(i_state[0][1])

            i_ph.append(i_state[1][0])
            i_at.append(i_state[1][1])

            for j in range(self.size):
                j_state = self.states[j]

                j_ph = []
                j_at = []

                j_ph.append(j_state[0][0])
                j_at.append(j_state[0][1])

                j_ph.append(j_state[1][0])
                j_at.append(j_state[1][1])

                if i_state == j_state:
                    for _ in range(len(cavity)):
                        self.matrix.data[i, j] += self.wc[_] * i_ph[_]

                        for n_ in range(self.n[_]):
                            self.matrix.data[i,
                                             j] += self.wa[_][n_] * i_at[_][n_]
                else:
                    diff_cv = 0

                    for cv_ in range(2):
                        if i_state[cv_] != j_state[cv_]:
                            diff_cv += 1
                            cv = cv_

                    if diff_cv == 1:
                        d_ph = j_ph[cv] - i_ph[cv]

                        if abs(d_ph) == 1:
                            diff_at_cnt = 0

                            for n_ in range(len(i_at[cv])):
                                d_at = j_at[cv][n_] - i_at[cv][n_]

                                if d_ph + d_at == 0:
                                    diff_at_cnt += 1
                                    diff_at_num = n_
                                elif d_at != 0:
                                    diff_at_cnt = 0
                                    break

                                if diff_at_cnt > 1:
                                    break

                            if diff_at_cnt == 1:
                                self.matrix.data[i, j] = self.g[cv][diff_at_num] * \
                                    sqrt(max(i_ph[cv], j_ph[cv]))

                                # print(self.states[i], '->', self.states[j], self.matrix.data[i, j])

                    else:
                        d_ph_0 = j_ph[0] - i_ph[0]
                        d_ph_1 = j_ph[1] - i_ph[1]

                        if abs(d_ph_0) == 1 and (d_ph_1 + d_ph_0 == 0):
                            if i_at[0] == j_at[0] and i_at[1] == j_at[1]:
                                if j_ph[0] > i_ph[0]:
                                    self.matrix.data[i, j] += mu * \
                                        a_plus(i_ph[0]) * a_minus(i_ph[1])
                                else:
                                    self.matrix.data[i, j] += mu * \
                                        a_plus(i_ph[1]) * a_minus(i_ph[0])

                                # print(self.states[i], '->', self.states[j], self.matrix.data[i, j])

        # for i in range(self.size):
        #     for j in range(self.size):
        #         if self.matrix.data[i, j] != self.matrix.data[j, i]:
        #             print(self.states[i], self.states[j], self.matrix.data[i, j], self.matrix.data[j, i])

        self.matrix.check_hermiticity()

        return
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print_html(self):
        states = self.states

        st = list(states.values())

        for i in range(0, len(st)):
            st[i] = str(st[i])

        df = pd.DataFrame(self.matrix_n.data, columns=st,
                          index=st, dtype=str)

        f = open("Hamiltonian.html", "w")

        style = """        <style>
            .dataframe th, td {
                text-align:center;
                # min-width:200px;
                white-space: nowrap;
                word-break:keep-all;
            }
        </style>"""
        f.write(style + df.to_html())
        webbrowser.open("Hamiltonian.html")

        f.close()

        return
    # -------------------------------------------------------------------------------------------------

    # -------------------------------------------------------------------------------------------------
    def write_to_file(self, filename):
        self.matrix.write_to_file(filename)

        return
    # -------------------------------------------------------------------------------------------------

    # -------------------------------------------------------------------------------------------------
    def get_states(self):
        state_0 = CV_state(self.capacity, self.n[0])
        state_1 = CV_state(self.capacity, self.n[1])

        self.states = {}

        cnt = 0

        full_state = Full_state(self.capacity, [state_0, state_1])

        self.states[cnt] = copy.deepcopy(full_state.state())

        while(full_state.inc() != -1):
            self.states[cnt] = copy.deepcopy(full_state.state())
            cnt += 1
            print(cnt)
        for k, v in self.states.items():
            print(k, v)

        # self.check_states()

        return self.states
    # -------------------------------------------------------------------------------------------------

    def get_states_bin(self):
        states_bin = {}

        for k, v in self.states.items():
            en1 = np.sum(v[0][1])
            en2 = np.sum(v[1][1])

            st = "[" + str(en1) + "," + str(en2) + "]"
            if not st in states_bin.keys():
                states_bin[st] = []

            states_bin[st].append(k)

        self.states_bin = {}
        self.states_bin_keys = []

        for k1 in range(self.n[0] + 1):
            for k2 in range(self.n[1] + 1):
                if k1 + k2 > self.capacity:
                    break

                k = "[" + str(k1) + "," + str(k2) + "]"
                self.states_bin_keys.append(k)
                self.states_bin[k] = states_bin[k]

        return self.states_bin

    # -------------------------------------------------------------------------------------------------
    def check_states(self):
        try:
            Assert(len(self.states) > 0, "len(states) <= 0", cf())

            for state in self.states.values():
                ph = state[0]
                at = state[1]

                Assert(0 <= ph <= self.capacity,
                       "incorrect state " + str(state), cf())
                Assert(ph + np.sum(at) == self.capacity,
                       "incorrect state " + str(state), cf())
                for n_ in range(len(at)):
                    Assert(0 <= at[n_] <= 1,
                           "incorrect state " + str(state), cf())
        except:
            print_error("incorrect states generation", cf())
            exit(1)

        return
    # -------------------------------------------------------------------------------------------------

    # -------------------------------------------------------------------------------------------------
    def print_states(self):
        print("Hamiltonian states:", color="yellow")

        print()

        try:
            for v in self.states.values():
                print(v)

            print()
        except:
            print_error("states not set", cf())
            exit(1)

        return
    # -------------------------------------------------------------------------------------------------

    def print_bin_states(self):
        print("Hamiltonian bin states:", color="yellow")

        print()

        try:
            for k in self.states_bin.keys():
                print(k)

            print()
        except:
            print_error("states not set", cf())
            exit(1)

        return
