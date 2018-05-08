# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
# -------------------------------------------------------------------------------------------------
# system
from math import sqrt
from Common.html import *
# -------------------------------------------------------------------------------------------------
# Common
from Common.Matrix import *
from Common.Assert import *
from Common.Print import *
from Common.C_n_k import *
from Common.ext import cf, print_error
# -------------------------------------------------------------------------------------------------
import html
import pandas as pd
import webbrowser


def a_plus(ph):
    return sqrt(ph + 1)


def a_minus(ph):
    return sqrt(ph)


class Hamiltonian:

    # ---------------------------------------------------------------------------------------------
    def __init__(self, capacity, cavity_1, cavity_2, mu):
        self.cavity_1 = cavity_1

        self.capacity = capacity

        self.cavity_1 = cavity_1
        self.cavity_2 = cavity_2

        self.n_1 = n_1 = cavity_1.n
        self.n_2 = n_2 = cavity_2.n

        wc_1 = cavity_1.wc
        wc_2 = cavity_2.wc

        wa_1 = cavity_1.wa
        wa_2 = cavity_2.wa

        g_1 = cavity_1.g
        g_2 = cavity_2.g

        self.init_states()
        self.init_bin_states()
        # ---------------------------------------
        # ---------------------------------------
        self.matrix = Matrix(self.size, self.size, dtype=np.complex128)

        cnk = Cnk(capacity)

        for i in range(self.size):
            st_i = self.states[i]

            ph_i = [st_i[0][0], st_i[1][0]]
            at_i = np.array([st_i[0][1], st_i[1][1]])

            for j in range(0, self.size):
                st_j = self.states[j]

                ph_j = [st_j[0][0], st_j[1][0]]
                at_j = np.array([st_j[0][1], st_j[1][1]])

                if i == j:
                    self.matrix.data[i, j] = wc_1 * \
                        ph_i[0] + wc_2 * ph_i[1] + wa_1 * \
                        at_i[0][0] + wa_2 * at_i[1][0]

                    # print(self.states[i], '->',
                    # self.states[j], self.matrix.data[i, j])
                elif abs(ph_j[0] - ph_i[0]) == 1 and (ph_j[0] - ph_i[0]) == -(ph_j[1] - ph_i[1]) and np.all(at_i == at_j):
                    d_ph_0 = ph_j[0] - ph_i[0]
                    d_ph_1 = ph_j[1] - ph_i[1]

                    if abs(d_ph_0) == 1 and (d_ph_1 + d_ph_0 == 0):
                        if at_i[0] == at_j[0] and at_i[1] == at_j[1]:
                            a_1 = min(at_i[0][0], at_j[0][0])
                            a_2 = min(at_i[1][0], at_j[1][0])

                            if ph_j[0] > ph_i[0]:
                                self.matrix.data[i, j] += mu * \
                                    a_plus(ph_i[0]) * a_minus(ph_i[1]
                                                              )  # * cnk.get(n_1, a_1+1)
                                # * C_n_k(n_2, at_j[0])
                            else:
                                self.matrix.data[i, j] += mu * \
                                    a_plus(ph_i[1]) * a_minus(ph_i[0]
                                                              )  # * cnk.get(n_2, a_2+1)
                                # * C_n_k(n_2, at_j[0])

                            # self.matrix.data[i, j] *= cnk.get(n_1, a_1) * cnk.get(n_2, a_2)
                    # self.matrix.data[i, j] = mu * sqrt(max(ph_j[0], ph_j[1]))

                    # print(self.states[i], '->',
                          # self.states[j], self.matrix.data[i, j])
                else:
                    if np.all(st_i[1] == st_j[1]):
                        if abs(at_i[0][0] - at_j[0][0]) == 1:
                            p = min(ph_i[0], ph_j[0])
                            a = min(at_i[0][0], at_j[0][0])

                            self.matrix.data[i, j] = g_1 * (n_1-a) * sqrt((p+1)
                                                                          * cnk.get(n_1, a)
                                                                          / cnk.get(n_1, a+1)
                                                                          )

                            # print(
                            # self.states[i], '->', self.states[j], self.matrix.data[i, j])
                    elif np.all(st_i[0] == st_j[0]):
                        if abs(at_i[1][0] - at_j[1][0]) == 1:
                            p = min(ph_i[1], ph_j[1])
                            a = min(at_i[1][0], at_j[1][0])

                            self.matrix.data[i, j] = g_2 * (n_2-a) * sqrt((p+1)
                                                                          * cnk.get(n_2, a)
                                                                          / cnk.get(n_2, a+1)
                                                                          )

                            # print(
                            # self.states[i], '->', self.states[j], self.matrix.data[i, j])
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def get_index(self, state):
        for k, v in self.states.items():
            if v == state:
                return k
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def write_to_file(self, filename):
        self.matrix.write_to_file(filename)
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def get_states(self):
        return self.states
    # ---------------------------------------------------------------------------------------------

    def print_html(self, filename):
        pass
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def init_states(self):
        self.states = dict()

        count = 0

        for ph_1 in range(self.capacity+1):
            for at_1 in range(self.cavity_1.n+1):
                for ph_2 in range(self.capacity+1):
                    for at_2 in range(self.cavity_2.n+1):
                        if ph_1 + at_1 + ph_2 + at_2 != self.capacity:
                            continue

                        self.states[count] = [[ph_1, [at_1]], [ph_2, [at_2]]]

                        count += 1

        self.size = len(self.states)

        return
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def init_bin_states(self):
        self.bin_states = dict()

        count = 0

        for at_1 in range(self.cavity_1.n+1):
            for at_2 in range(self.cavity_2.n+1):
                if at_1 + at_2 > self.capacity:
                    continue

                self.bin_states[count] = [at_1, at_2]

                count += 1

        return
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print_states(self):
        print("States:", color="green")

        print()

        for k, v in self.states.items():
            print(v)

        print()
    # ---------------------------------------------------------------------------------------------


# ---------------------------------------------------------------------------------------------------------------------
# -------------------------------------------------------- STUFF ------------------------------------------------------
# ---------------------------------------------------------------------------------------------------------------------
# def print_html(self, filename):
        # f = open(filename, "w")

        # html = """            <!DOCTYPE html>
        #     <html>
        #         <head>
        #             <title>
        #                 States
        #             </title>
        #         </head>

        #         <body>
        #             <table border=1>
        #     """
        # html += "<tr>"        html += "<td>"        html += "</td>"
        # # for i in range(0, len(self.states)):
        # #     html += "<td>"        #     html += "[" + str(self.states[i].n1) + "," + str(self.states[i].n2) + "]"        #     html += "</td>"
        # # html += "</tr>"
        # # for i in range(0, len(self.states)):
        # #     html += "<tr>"        #     html += "<td>"        #     html += "[" + str(self.states[i].n1) + "," + str(self.states[i].n2) + "]"        #     html += "</td>"
        # #     for j in range(0, len(self.states)):
        # #         html += "<td>"
        # #         if sqrt:
        # #             html += "&radic;" + "<span style="text-decoration:overline;">" + str(abs(self.matrix.data[i, j]) / self.g) + "</span>"        #         else:
        # #             html += "&radic;" + "<span style="text-decoration:overline;">" + str(abs(self.matrix.data[i, j])) + "</span>"
        # #         html += "</td>"
        # #     html += "</tr>"
        # html += """                    </table>
        #         </body>
        #     </html>
        #     """
        # f.write(html)
        # f.close()

        # webbrowser.open(filename)
