# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
# -------------------------------------------------------------------------------------------------
# system
import csv
# -------------------------------------------------------------------------------------------------
# Bipartite2General
from Bipartite2General.Unitary import *
# -------------------------------------------------------------------------------------------------
# Import
# from Import.ext import *
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
def run(ro_0, H, dt, nt, config):
    # ----------------
    U = Unitary(H, dt)

    if __debug__:
        U.write_to_file(config.U_csv)

    U_conj = U.conj()
    # -------------------
    ro_t = ro_0.data
    # --------------------------------------
    p_bin = dict.fromkeys(H.states_bin_keys)
    print(H.states_bin_keys)
    # --------------------------------------
    with open(config.z_csv, "w") as csv_file:
        writer = csv.writer(
            csv_file, quoting=csv.QUOTE_NONE, lineterminator="\n")

        with open(config.z_all_csv, "w") as csv_all_file:
            writer_all = csv.writer(
                csv_all_file, quoting=csv.QUOTE_NONE, lineterminator="\n")

            for t in range(0, nt+1):
                diag = np.diag(ro_t)

                p = np.abs(diag)
                p = np.asarray(p).reshape(-1)
                # p = np.around(p, precision)

                norm = np.sum(np.abs(diag))

                if abs(norm - 1) > 0.01:
                    print("ro not normed")
                    exit(0)

                for k, v in p_bin.items():
                    p_bin[k] = 0

                for k, v in H.states_bin.items():
                    for ind in v:
                        p_bin[k] += p[ind]

                v_bin = [p_bin[k] for k in H.states_bin_keys]
                # --------------------------------------------------
                writer.writerow(["{:.5f}".format(x) for x in v_bin])

                if __debug__:
                    writer_all.writerow(["{:.5f}".format(x) for x in p])
                # --------------------------------------------------
                ro_t = U.data.dot(ro_t).dot(U_conj)
    # ---------------------------------------------
    states_bin = {}

    cnt = 0

    for k in H.states_bin_keys:
        if k == "[" + str(0) + "," + str(config.n_2) + "]" or k == "[" + str(config.n_2) + "," + str(0) + "]":
            states_bin[cnt] = str(k)
        else:
            states_bin[cnt] = ""
        cnt += 1
    # ---------------------------------------------
    states = {}

    cnt = 0

    for v in H.states_bin_keys:
        states[cnt] = v

        cnt += 1
    # -------------------------
    write_x(states, config.x_csv, ind=[])
    write_t(config.T / config.mks, config.nt, config.y_csv)
    # -------------------------
    return
