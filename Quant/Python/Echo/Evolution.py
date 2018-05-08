# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
from scipy.linalg import expm
import pandas as pd
# -------------------------------------------------------------------------------------------------
# system
import csv
# -------------------------------------------------------------------------------------------------
# user
from Common.ext import list_to_csv
# -------------------------------------------------------------------------------------------------
# quant
from Echo.config import *
from Echo.DensityMatrix import *

from Common.Fidelity import *
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
def get_U(H, dt):
    H = np.array(H.H)

    U = expm(-1j * H * dt)

    return np.matrix(U)
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
def run_ro(ro_0, H, config):
    # ---------------------------------------------------------------------------------------------
    U = Unitary(H, config.dt)
    # U.write_to_file(U_csv)

    U_conj = U.get_conj()
    # ---------------------------------------------------------------------------------------------
    ro_t = ro_0

    ro_0_sqrt = lg.fractional_matrix_power(ro_0, 0.5)

    fidelity = []

    with open(config.fid_csv, 'w') as csv_file:
        writer = csv.writer(
            csv_file, lineterminator='\n')

        st = []

        for i in H.states.values():
            st.append(str(i))

        writer.writerow(st)

        for t in range(0, config.nt + 1):
            fidelity_t = Fidelity(ro_sqrt=ro_0_sqrt, sigma=ro_t)
            # print(fidelity_t)
            # exit(0)
            ro_t = U.data.dot(ro_t).dot(U_conj)
            # print(np.around(ro_t, 3))
            # if t > 3:
            # exit(1)
            # fidelity_t = D(ro_0, ro_t)
            # fidelity_t = D_HS(ro_0, ro_t)

            # fidelity_t = round(fidelity_t, 5)

            # print(np.abs(np.diag(ro_t.data)))
            # exit(0)

            fidelity.append(fidelity_t)
    # exit(0)
    print('Min fidelity:', np.min(fidelity))
    print('Max fidelity:', np.max(fidelity))

    list_to_csv(config.fid_csv, fidelity, header=['fidelity'])
    # list_to_csv(config.fid_csv, fidelity, header=['fidelity'])
