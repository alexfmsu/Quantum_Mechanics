# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
import pandas as pd
# -------------------------------------------------------------------------------------------------
# system
import webbrowser
import copy
from math import sqrt
# -------------------------------------------------------------------------------------------------
# user
from Common.C_n_k import Cnk
from Common.ext import *
from Common.Matrix import Matrix
from Common.Assert import *
# -------------------------------------------------------------------------------------------------
# quant
from Echo.Cavity import Cavity
# -------------------------------------------------------------------------------------------------


def a_plus(ph):
    return sqrt(ph + 1)


def a_minus(ph):
    return sqrt(ph)


# -------------------------------------------------------------------------------------------------
class FullState:

    # -------------------------------------------
    def __init__(self, dims):
        self.dims = dims
        self.size = len(dims)
        self.x = []
        self.data = []

        for i in dims:
            if type(i) == type([]):
                n = FullState(i)

                self.x.append(n)
                self.data.append(n.data)
            else:
                self.x.append(0)
                self.data.append(0)

        return
    # -------------------------------------------

    # -------------------------------------------
    def inc(self, depth=0):
        for i in range(self.size - 1, -1, -1):
            if type(self.x[i]) == FullState:
                err = self.x[i].inc(depth)

                if err:
                    return err
                else:
                    continue

            if self.data[i] < self.dims[i]:
                print(self.dims)
                exit(1)
                self.data[i] += 1

                return True
            else:
                self.data[i] = 0

                continue

        return False
    # -------------------------------------------

    # -------------------------------------------
    def print(self):
        print(self.data)

        return
    # -------------------------------------------

# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
class FullState:

    # -------------------------------------------
    def __init__(self, dims):
        self.dims = dims
        self.size = len(dims)
        self.x = []
        self.data = []

        for i in dims:
            if type(i) == type([]):
                n = FullState(i)

                self.x.append(n)
                self.data.append(n.data)
            else:
                self.x.append(0)
                self.data.append(0)

        return
    # -------------------------------------------

    # -------------------------------------------
    def inc(self, depth=0):
        for i in range(self.size - 1, -1, -1):
            if type(self.x[i]) == FullState:
                err = self.x[i].inc(depth)

                if err:
                    return err
                else:
                    continue

            if self.data[i] < self.dims[i] - 1:
                self.data[i] += 1

                return True
            else:
                self.data[i] = 0

                continue

        return False
    # -------------------------------------------

    # -------------------------------------------
    def print(self):
        print(self.data)

        return
    # -------------------------------------------

# -------------------------------------------------------------------------------------------------


class Hamiltonian:

    def __init__(self, capacity, cavities, links, RWA=True):
        # -------------------------------------------------
        # < AssertION
        Assert(isinstance(capacity, int), 'capacity is not int', cf())
        Assert(capacity > 0, 'capacity <= 0', cf())

        # Assert(isinstance(cv1, Cavity), 'cv1 is not Cavity')
        # Assert(isinstance(cv2, Cavity), 'cv2 is not Cavity')
        # Assert(isinstance(m, (int, float)), 'm is not real')

        # > AssertION
        # -------------------------------------------------
        self.capacity = capacity
        cv = self.cavities = cavities
        ln = self.links = links

        self.states = self.get_statesFull()
        # -------------------------------------------------
        self.size = len(self.states)
        self.matrix = Matrix(self.size, self.size, dtype=np.complex128)
        # ---------------------------------------
        max_n = 0

        for i in cv:
            max_n = max(i.n, max_n)
        # ---------------------------------------
        cnk = Cnk(max_n)
        # ---------------------------------------
        cv_cnt = len(cv)
        # ---------------------------------------
        i_p = np.zeros(cv_cnt, dtype=int)
        i_a = np.zeros(cv_cnt, dtype=int)

        j_p = np.zeros(cv_cnt, dtype=int)
        j_a = np.zeros(cv_cnt, dtype=int)

        p = np.zeros(cv_cnt, dtype=int)
        a = np.zeros(cv_cnt, dtype=int)

        d_p = np.zeros(cv_cnt, dtype=int)
        d_a = np.zeros(cv_cnt, dtype=int)
        # ---------------------------------------
        for i in range(self.size):
            # -----------------------------------
            i_p.fill(0)
            i_a.fill(0)

            st_i = self.states[i]

            for c in range(cv_cnt):
                i_p[c] = st_i[c][0]

                if len(st_i[c]) == 2:
                    i_a[c] = st_i[c][1]
            # -----------------------------------
            for j in range(self.size):
                # -------------------------------
                j_p.fill(0)
                j_a.fill(0)

                st_j = self.states[j]

                for c in range(cv_cnt):
                    j_p[c] = st_j[c][0]

                    if len(st_j[c]) == 2:
                        j_a[c] = st_j[c][1]
                # -------------------------------
                p = np.minimum(i_p, j_p)
                a = np.minimum(i_a, j_a)

                d_p = j_p - i_p
                d_a = j_a - i_a

                ne_cv = []

                ne = 0

                for c in range(cv_cnt):
                    if st_i[c] != st_j[c]:
                        ne += 1
                        ne_cv.append(c)

                if ne == 0:
                    # print(st_i, p, a)
                    for c in range(cv_cnt):
                        self.matrix.data[i, j] += cv[c].wc * \
                            p[c] + cv[c].wa * a[c]
                elif ne == 1:
                    cv_num = ne_cv[0]

                    if abs(d_p[cv_num]) == 1 and d_p[cv_num] == -d_a[cv_num]:
                        _g = cv[cv_num].g
                        _n = cv[cv_num].n

                        _p = p[cv_num]
                        _a = a[cv_num]
                        self.matrix.data[
                            i, j] = _g * (_n - _a) * sqrt((_p + 1) / (cnk.get(_n, _a) * cnk.get(_n, _a + 1)))
                elif ne == 2:
                    _from = ne_cv[0]
                    _to = ne_cv[1]

                    if d_a[_from] == d_a[_to] == 0:
                        if d_p[_from] == 1 and d_p[_to] == -1 or d_p[_from] == -1 and d_p[_to] == 1:
                            _k = str(_from) + " <-> " + str(_to)
                            __k = str(_to) + " <-> " + str(_from)

                            for k, l in links.items():
                                if k == _k or k == __k:
                                    if d_p[_to] > d_p[_from]:
                                        self.matrix.data[i, j] = l * \
                                            a_plus(i_p[_to]) * \
                                            a_minus(i_p[_from])
                                    else:
                                        self.matrix.data[i, j] = l * \
                                            a_plus(i_p[_from]) * \
                                            a_minus(i_p[_to])

                                # if k == _k or k == __k:
                                #     self.matrix.data[i, j] = v

                                    # break

        if not self.is_hermitian():
            print("Hamiltonian is not hermitian")
            exit(1)

        self.H = self.matrix.data
        #---------------------------------------------------------------------
        return

    def is_hermitian(self):
        return np.all(self.matrix.data == self.matrix.data.getH())
    # -------------------------------------------

    def get_index(self, state):
        index = None

        for k, v in self.states.items():
            if v == state or str(v) == state:
                index = k

                break

        if index == None:
            print("Invalid value in map")
            exit(1)

        return index
    # -------------------------------------------

    # -------------------------------------------
    def print(self, precision=3):
        for i in range(0, self.size):
            for j in range(0, self.size):
                # re = np.real(self.matrix.data[i, j])
                # im = np.imag(self.matrix.data[i, j])

                # print("({0:5f}, {1:5f})".format(re, im), '\t', end='')

                print("{0:.1f}".format(self.matrix.data[i, j]), '\t', end='')

            print()

        # self.matrix.print()

        return
    # -------------------------------------------

    def get_statesFull(self):
        states = {}

        _st = []

        for i in self.cavities:
            if i.n > 0:
                _st.append([self.capacity + 1, i.n + 1])
            else:
                _st.append([self.capacity + 1])

        state = FullState(_st)
        print(_st)
        cnt = 0
        states[cnt] = copy.deepcopy(state.data)
        # print(states[cnt])
        # exit()
        while state.inc():
            if (np.sum(state.data) > self.capacity):
                continue

            cnt += 1
            states[cnt] = copy.deepcopy(state.data)

        return states

    def print_states(self):
        for i in self.states:
            print(i, self.states[i])

        return

    def print_html(self):
        states = self.states

        st = list(states.values())

        for i in range(0, len(st)):
            st[i] = str(st[i])

        df = pd.DataFrame(self.Hm, columns=st, index=st, dtype=str)

        f = open('report.html', 'w')
        style = '''
        <style>
            .dataframe th, td {
                text-align:right;
                min-width:200px;
                nowrap:nowrap;
                word-break:keep-all;
            }
        </style>'''

        f.write(style + df.to_html())
        webbrowser.open('report.html')

        f.close()

        return

# -------------------------------------------------------------------------------------------------
