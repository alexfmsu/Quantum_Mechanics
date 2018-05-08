# -------------------------------------------------------------------------------------------------
# scientific
# -------------------------------------------------------------------------------------------------
# system
from math import sqrt
# -------------------------------------------------------------------------------------------------
# quant
# from Echo.Cavity import *

from Echo.config import *

from Echo.Hamiltonian import *
from Echo.WaveFunction import *
from Echo.Evolution import *

from Echo.DensityMatrix import *
from Common.LoadPackage import *

from shutil import copyfile

from Common.STR import *
# -------------------------------------------------------------------------------------------------
config = load_pkg("config", "Echo/config.py")

mkdir(config.path)
copyfile("Bipartite/config.py", config.path + '/config.py')
# -------------------------------------------------------------------------------------------------


def hr():
    print('-' * 100)


# -------------------------------------------------------------------------------------------------
# 0 atoms
cv1 = Cavity(n=n_1, wc=config.wc_1, wa=config.wa_1, g=config.g_1)
cv2 = Cavity(n=n_2, wc=config.wc_2, wa=config.wa_2, g=config.g_2)

H = Hamiltonian(capacity=config.capacity, cavities=[
                cv1, cv2], links={"0 <-> 1": config.l})
H.print_states()
H.print()
# print(dt)
# exit(0)
# print(np.round(get_U(H, dt), 4))
w0 = WaveFunction(states=H.states)
# w0.set_ampl([[0, 0], [0, 0]], 1 / sqrt(2))
# w0.set_ampl([[2, 0], [0, 0]], 1 / sqrt(2))
# w0.set_ampl([[0, 0], [0, 0]], 1 / sqrt(2))
# w0.set_ampl([[0, 2], [0, 0]], 1)
# w0.set_ampl([[0, 2], [0, 0]], 1 / sqrt(2))


# cv1 = Cavity(n=0, wc=wc_1, wa=wa_1, g=config.g_1)
# cv2 = Cavity(n=0, wc=wc_2, wa=wa_2, g=config.g_2)
# cv3 = Cavity(n=0, wc=wc_2, wa=wa_2, g=config.g_3)

# H = Hamiltonian(capacity=config.capacity, cavities=[cv1, cv2, cv3],
#                 links={"0 <-> 1": config.l_1_2,
#                        "1 <-> 2": config.l_2_3,
#                        # "0 <-> 2": 1
#                        }
#                 )
# H.print()
# H.print_states()

# w0 = WaveFunction(states=H.states)
# # w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# # w0.set_ampl('[[1, 0], [0, 0]]', 1 / sqrt(2))
# # w0.set_ampl([[0], [0], [0]], 1 / sqrt(2))
# # w0.set_ampl([[0], [1], [0]], 1 / sqrt(2))
# w0.set_ampl([[0], [1], [0]], 1)


# w0.set_ampl([[1, 0], [0, 0]], 1/sqrt(2))
# w0.set_ampl([[1], [0]], 1)
w0.glauber(capacity=config.capacity, cv_num=0)
w0.normalize()

w0.print()
# exit(0)
# w0.set_ampl([[0], [0]], 1 / sqrt(2))
# w0.set_ampl('[[0], [0]]', 1 / sqrt(2))
# w0.set_ampl('[[1], [0]]', 1 / sqrt(2))

ro_0 = get_ro(w0)
run_ro(ro_0=ro_0, H=H, config=config)
# plot_fidelity()
# exit(0)
# -------------------------------------------------------------------------------------------------
# 1 atom
# cv1 = Cavity(n=1, wc=wc_1, wa=wa_1, g=1)
# cv2 = Cavity(n=1, wc=wc_2, wa=wa_2, g=1)

# H = Hamiltonian(capacity=1, cavities=[cv1, cv2], links={"0 <-> 1": 1})
# H.print()
# H.print_states()

# w0 = WaveFunction(H=H)
# w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# w0.set_ampl('[[1, 0], [0, 0]]', 1 / sqrt(2))
# w0.print()

# ro_0 = get_ro(w0)
# run_ro(ro_0=ro_0, H=H, dt=dt, nt=nt, outfile='out/1.csv', w0=w0.data)
# plot_fidelity()
# -------------------------------------------------------------------------------------------------
# 2 atoms
# cv1 = Cavity(n=2, wc=wc_1, wa=wa_1, g=1)
# cv2 = Cavity(n=2, wc=wc_2, wa=wa_2, g=1)

# H = Hamiltonian(capacity=1, cavities=[cv1, cv2], links={"0 <-> 1": 1})
# H.print()
# H.print_states()

# w0 = WaveFunction(H=H)
# w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# w0.set_ampl('[[1, 0], [0, 0]]', 1 / sqrt(2))
# # w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# # w0.set_ampl('[[0, 1], [0, 0]]', 1 / sqrt(2))
# w0.print()

# ro_0 = get_ro(w0)
# run_ro(ro_0=ro_0, H=H, dt=dt, nt=nt, outfile='out/1.csv', w0=w0.data)
# plot_fidelity()
# -------------------------------------------------------------------------------------------------
# 3 atoms
# cv1 = Cavity(n=3, wc=wc_1, wa=wa_1, g=1)
# cv2 = Cavity(n=3, wc=wc_2, wa=wa_2, g=1)

# H = Hamiltonian(capacity=1, cavities=[cv1, cv2], links={"0 <-> 1": 1})
# H.print()
# H.print_states()

# w0 = WaveFunction(H=H)
# # w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# # w0.set_ampl('[[1, 0], [0, 0]]', 1 / sqrt(2))
# w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# w0.set_ampl('[[0, 1], [0, 0]]', 1 / sqrt(2))
# w0.print()

# ro_0 = get_ro(w0)
# run_ro(ro_0=ro_0, H=H, dt=dt, nt=nt, outfile='out/1.csv', w0=w0.data)
# plot_fidelity()
# -------------------------------------------------------------------------------------------------
# 3 cavities
# cv1 = Cavity(n=0, wc=wc_1, wa=wa_1, g=1)
# cv2 = Cavity(n=0, wc=wc_2, wa=wa_2, g=1)
# cv3 = Cavity(n=0, wc=wc_2, wa=wa_2, g=1)

# H = Hamiltonian(capacity=1, cavities=[cv1, cv2, cv3],
#                 links={"0 <-> 1": 1,
#                        "1 <-> 2": 1,
#                        # "0 <-> 2": 1
#                        }
#                 )
# H.print()
# H.print_states()

# w0 = WaveFunction(H=H)
# # w0.set_ampl('[[0, 0], [0, 0]]', 1 / sqrt(2))
# # w0.set_ampl('[[1, 0], [0, 0]]', 1 / sqrt(2))
# w0.set_ampl('[[0], [0], [0]]', 1 / sqrt(2))
# w0.set_ampl('[[0], [1], [0]]', 1 / sqrt(2))
# w0.print()

# ro_0 = get_ro(w0)
# run_ro(ro_0=ro_0, H=H, dt=dt, nt=nt, outfile='out/1.csv', w0=w0.data)

fid_plot = True
# fid_plot = False

if fid_plot:
    from py import *

    def plot_fidelity(filename=config.fid_csv):
        z_data = pd.read_csv(filename)

        t = np.around(np.linspace(0, config.nt, config.nt), 3)

        title = ""
        # title += "<span style='font-size:18'>"
        # title += "<b>Fidelity</b>"
        # title += "</span>"
        # title += "<br>"
        # title += "<span style='font-size:11'>"
        # title += "n = " + str(config.n)
        # title += "<br>init. state: " + str(config.init_state)
        # # title += "<br>t = " + T_str(config.T)
        # title += "<br>"
        # title += "<br>w<sub>c</sub> = " + wc_str(config.wc)
        # title += "<br>w<sub>a</sub> = " + wa_str(config.wa)
        # title += "<br>g = " + g_str(config.g)
        # title += "</span>"

        PYPLOT2D(
            data_0={
                "title": title,
                # "title": "<b>w<sub>c</sub> = w<sub>a</sub> = 2 PI x 0.5 MHz;\tg / (hw<sub>c</sub>) = 0.001<br>" +
                # "m = g<b>",
                "x": {
                    "title": "Time, " + T_str_mark(config.T),
                    "data": t,

                    "ticktext": np.around(np.linspace(0, T_str_v(config.T), 6), 3),
                    # "ticktext": np.around(np.linspace(0, T_str_v(config.T), 6), 3),
                    "tickvals": np.around(np.linspace(0, config.nt, 6), 3)
                },
                "y":
                {
                    "title": "Fidelity",
                    "data": [
                        z_data["fidelity"],
                    ]
                },
            },
            online=False,
            filename=config.path + "/" + "Fidelity"
        )

        return
        # --------------------------------

    def plot_fidelity_small(filename=config.fid_csv):
        z_data = pd.read_csv(filename)

        t = np.around(np.linspace(0, config.nt, config.nt), 3)

        title = ""
        title += "<span style='font-size:18'>"
        title += "<b>Fidelity</b>"
        title += "</span>"
        # title += "<br>"
        # title += "<span style='font-size:11'>"
        # title += "n = " + str(config.n)
        # title += "<br>init. state: " + str(config.init_state)
        # # title += "<br>t = " + T_str(config.T)
        # title += "<br>"
        # title += "<br>w<sub>c</sub> = " + wc_str(config.wc)
        # title += "<br>w<sub>a</sub> = " + wa_str(config.wa)
        # title += "<br>g = " + g_str(config.g)
        # title += "</span>"

        PYPLOT2D(
            data_0={
                "title": title,
                # "title": "<b>w<sub>c</sub> = w<sub>a</sub> = 2 PI x 0.5 MHz;\tg / (hw<sub>c</sub>) = 0.001<br>" +
                # "m = g<b>",
                "x": {
                    "title": "Time, " + T_str_mark(config.T),
                    "data": t,

                    "ticktext": np.around(np.linspace(0, T_str_v(config.T), 11), 3),
                    "tickvals": np.around(np.linspace(0, config.nt, 11), 3)
                },
                "y":
                {
                    "title": "Fidelity",
                    "data": [
                        z_data["fidelity"],
                    ]
                },
            },
            online=False,
            filename=config.path + "/" + "Fidelity_small"
        )

        return
        # --------------------------------
    plot_fidelity(config.fid_csv)
    # plot_fidelity(config.fid_small_csv)
    # plot_fidelity_small(config.fid_small_csv)
