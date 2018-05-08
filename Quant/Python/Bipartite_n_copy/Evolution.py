# -------------------------------------------------------------------------------------------------
# system
import csv
# -------------------------------------------------------------------------------------------------
# Bipartite
from Bipartite_n.DensityMatrix import *

from Bipartite_n.config import *
# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
import scipy.linalg as lg
# -------------------------------------------------------------------------------------------------
# Common
from Common.ext import *
from Common.STR import *
# -------------------------------------------------------------------------------------------------
from Common.Fidelity import *


def run_ro(ro_0, H, dt, nt):
    # ---------------------------------------------------------------------------------------------
    U = Unitary(H, dt)

    if __debug__:
        U.write_to_file(U_csv)

    U_conj = U.conj()
    # ----------------------------------------------------
    ro_t = ro_0.data
    ro_0_sqrt = lg.fractional_matrix_power(ro_0.data, 0.5)
    # ----------------------------------------------------
    FIDELITY = []
    # ----------------------------------------------------
    with open(z_csv, "w") as csv_file:
        writer = csv.writer(csv_file, quoting=csv.QUOTE_NONE, lineterminator="\n")

        for t in range(0, nt):
            p = np.abs(np.diag(ro_t))

            trace = np.sum(p)

            Assert(abs(1 - trace) <= 0.1, "ro is not normed", cf())

            writer.writerow(["{:.5f}".format(x) for x in p])
            # --------------------------------------------------
            fidelity_t = Fidelity(ro_0_sqrt, ro_t)
            FIDELITY.append(fidelity_t)
            # --------------------------------------------------
            ro_t = U.data.dot(ro_t).dot(U_conj)
    # ---------------------------------------------------------------------------------------------
    states = H.states

    write_x(states, x_csv, ind_1=[0, H.n], ind_2=[H.n, 0])
    write_t(T_str_v(T), nt, y_csv)
    # write_t(T / mks / 1e-2, nt, y_csv)
    # # -------------------------
    list_to_csv(fid_csv, FIDELITY, header=["fidelity"])
    # # -------------------------------------------------
