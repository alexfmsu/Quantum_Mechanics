# -------------------------------------------------------------------------------------------------
# system
import csv
# -------------------------------------------------------------------------------------------------
# Bipartite
from Bipartite2.Unitary import *
# -------------------------------------------------------------------------------------------------
# Common
from Common.ext import *
from Common.STR import *
from Common.Fidelity import *
# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
import scipy.linalg as lg
# -------------------------------------------------------------------------------------------------
import copy


# -------------------------------------------------------------------------------------------------
def run(ro_0, H, dt, nt, config, fidelity_mode=False):
    # --------------------------------------------------------
    U = Unitary(H, dt)

    if __debug__:
        U.write_to_file(config.U_csv)

    U_conj = U.conj()
    # --------------------------------------------------------
    if fidelity_mode:
        ro_0_sqrt = lg.fractional_matrix_power(ro_0.data, 0.5)

        fidelity = []

    ro_t = ro_0.data
    # ----------------------------------------------------------------------------
    with open(config.z_csv, "w") as csv_file:
        writer = csv.writer(
            csv_file, quoting=csv.QUOTE_NONE, lineterminator="\n")

        for t in range(0, nt):
            diag_abs = np.abs(np.diag(ro_t))

            trace_abs = np.sum(diag_abs)

            Assert(abs(1 - trace_abs) <= 0.1, "ro is not normed", cf())

            writer.writerow(["{:.5f}".format(x) for x in diag_abs])
            # --------------------------------------------------------------------
            if fidelity_mode:
                fidelity_t = round(Fidelity(ro_0_sqrt, ro_t), 5)
                fidelity_t = "{:.5f}".format(fidelity_t)

                fidelity.append(fidelity_t)
            # --------------------------------------------------------------------
            ro_t = U.data.dot(ro_t).dot(U_conj)
    # ----------------------------------------------------------------------------
    states = H.states

    write_x(states, config.x_csv, ind_1=[0, H.n], ind_2=[H.n, 0])
    write_t(T_str_v(config.T), config.nt, config.y_csv)
    # ----------------------------------------------------------
    if fidelity_mode:
        list_to_csv(config.fid_csv, fidelity, header=["fidelity"])
    # ----------------------------------------------------------

# -------------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------


def write_x2(states, x_csv, ind):
    _k = [int(k) for k in states.keys()]
    # _v = [str(v) for v in states.values()]
    _v = [v for v in states.values()]

    # if ind_1 != None and ind_2 != None:
    #     lKey = [key for key, value in states.items() if value == ind_1][0]
    #     rKey = [key for key, value in states.items() if value == ind_2][0]
    # print(ind)
    # print(states)
    # print(_k)
    # print(_v)
    # for i in range(len(ind)):
    # ind[i] = str(ind[i])
    # print(ind)
    for k in _k:
        if states[k] in ind:
            _v[k] = str(states[k])
        else:
            _v[k] = ''

    # exit(1)
    _x = np.matrix([
        _v,
        _k
    ]).getT()

    list_to_csv(x_csv, _x, ["x", "vals"])


def run_w(w_0, H, dt, nt, config, fidelity_mode=False):
    # --------------------------------------------------------
    U = Unitary(H, dt)

    if __debug__:
        U.write_to_file(config.U_csv)

    U_conj = U.conj()
    # --------------------------------------------------------
    if fidelity_mode:
        fidelity = []

    w_0 = np.matrix(w_0.data)
    w_t = np.array(w_0.data)
    # ----------------------------------------------------------------------------
    dt_ = nt / (config.T/config.mks) / 20000 * 1000
    nt_ = int(nt/dt_)

    z_0 = []
    z_1 = []

    ind_0 = None
    ind_1 = None

    for k, v in H.states.items():
        if v == [[config.capacity-config.n_2, [0]], [0, [config.n_2]]]:
            ind_0 = k
        elif v == [[config.capacity-config.n_2, [config.n_2]], [0, [0]]]:
            ind_1 = k

    bin_keys = list(H.bin_states.values())
    bin_keys_str = copy.copy(bin_keys)

    for i in range(len(bin_keys)):
        bin_keys_str[i] = str(bin_keys[i])

    p_bin = dict.fromkeys(bin_keys_str)

    for i in p_bin.keys():
        p_bin[i] = []

    for i in range(len(H.states)):
        at_1_sum = H.states[i][0][1][0]
        at_2_sum = H.states[i][1][1][0]

        p_bin[str([at_1_sum, at_2_sum])].append(i)

    with open(config.z_csv, "w") as csv_file:
        writer = csv.writer(
            csv_file, quoting=csv.QUOTE_NONE, lineterminator="\n")

        for t in range(0, nt+1):
            w_t_arr = w_t.reshape(1, -1)[0]

            diag_abs = np.abs(w_t_arr)**2
            trace_abs = np.sum(diag_abs)

            Assert(abs(1 - trace_abs) <= 0.01, "ro is not normed", cf())
            # --------------------------------------------------------------------
            if fidelity_mode:
                w_t = np.matrix(w_t)

                D = w_0.getH().dot(w_t).reshape(-1)[0, 0]

                fidelity_t = round(abs(D), 3)

                fidelity_t = "{:.5f}".format(fidelity_t)

                fidelity.append(float(fidelity_t))

            z_bin = np.zeros(len(p_bin))

            k = 0
            for v in p_bin.values():
                for _ in v:
                    z_bin[k] += diag_abs[_]
                k += 1

            z_0.append(diag_abs[ind_0])
            z_1.append(diag_abs[ind_1])

            writer.writerow(["{:.5f}".format(x)
                             for x in z_bin])
            # --------------------------------------------------------------------
            w_t = np.array(U.data.dot(w_t))
    # ----------------------------------------------------------------------------
    states = H.bin_states
    # print(states)
    # print([
    #     [0, H.cavity_2.n],
    #     [H.cavity_2.n, 0],
    # ])
    # exit(1)
    write_x2(states, config.x_csv, ind=[
        [0, H.cavity_2.n],
        [H.cavity_2.n, 0],
    ])

    write_t(T_str_v(config.T), config.nt, config.y_csv)
    # ----------------------------------------------------------
    if fidelity_mode:
        list_to_csv(config.fid_csv, fidelity, header=["fidelity"])

        fidelity = fidelity[::nt_]
        list_to_csv(config.fid_small_csv, fidelity, header=["fidelity"])

    # list_to_csv(config.path + 'z_0.csv', z_0, header=["fidelity"])
    # list_to_csv(config.path + 'z_1.csv', z_1, header=["fidelity"])
    # ----------------------------------------------------------

# -------------------------------------------------------------------------------------------------
