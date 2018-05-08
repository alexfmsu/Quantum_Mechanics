# -------------------------------------------------------------------------------------------------
# system
import sys
# -------------------------------------------------------------------------------------------------
# Bipartite2General
from Bipartite2General.Cavity import *
from Bipartite2General.Hamiltonian import *

from Bipartite2General.WaveFunction import *
from Bipartite2General.DensityMatrix import *

from Bipartite2General.Evolution import *
# -------------------------------------------------------------------------------------------------
# Common
from Common.LoadPackage import *
from Common.Print import *

from Common.ext import mkdir
from Common.PyPlot import PyPlot3D
# -------------------------------------------------------------------------------------------------
config = load_pkg("config", "Bipartite2General/config.py")

mkdir(config.path)
# -------------------------------------------------------------------------------------------------
cavity_1 = Cavity(n=config.n_1, wc=config.wc_1, wa=config.wa_1, g=config.g_1)
cavity_2 = Cavity(n=config.n_2, wc=config.wc_2, wa=config.wa_2, g=config.g_2)

print("Cavity 1:\n", color="yellow")
cavity_1.print_n()
cavity_1.print_wc()
cavity_1.print_wa()
cavity_1.print_g()

print()

print("Cavity 2:\n", color="yellow")
cavity_2.print_n()
cavity_2.print_wc()
cavity_2.print_wa()
cavity_2.print_g()
# -------------------------------------------------------------------------------------------------
H = Hamiltonian(capacity=config.capacity, cavity=[
                cavity_1, cavity_2], mu=config.mu)
# H.matrix.set_header(H.states)
# print("L2:\n", color="yellow")
# H.matrix.print_pd()

if __debug__:
    H.write_to_file(filename=config.H_csv)
    # H.print_html()

    print("Hamiltonian states:\n", color="yellow")
    H.print_states()
    H.print_bin_states()
# -------------------------------------------------------------------------------------------------
w_0 = WaveFunction(states=H.states, init_state=config.init_state)

if __debug__:
    print("wf\n", color="yellow")
    w_0.print()
# -------------------------------------------------------------------------------------------------
ro_0 = DensityMatrix(w_0)

if __debug__:
    ro_0.write_to_file(filename=config.ro_0_csv)
# -------------------------------------------------------------------------------------------------

run(ro_0=ro_0, H=H, dt=config.dt, nt=config.nt, config=config)

# -------------------------------------------------------------------------------------------------
from Common.STR import *

if not __debug__:
    title = "<b>"
    # title += "<br>wc = " + str(wc / GHz) + " GHz"    # title += "<br>wa = "
    # + str(np.array(wa) / GHz) + " GHz"    # title += "<br>g = " +
    # str(np.array(g) / MHz) + " MHz"
    title = "<b>Bipartite oscillations</b>"
    title += "<br>capacity = " + \
        str(config.capacity) + ", n_1 = " + \
        str(config.n_1) + ", n_2 = " + str(config.n_2)
    title += "<br>t = " + T_str(config.T)
    title += "</b>"

    PyPlot3D(
        title=title,
        z_csv=config.path + "/" + "z.csv",
        x_csv=config.path + "/" + "x.csv",
        y_csv=config.path + "/" + "t.csv",
        online=False,
        path=config.path,
        filename="Bipartite2",
        xaxis="states",
        yaxis="time, мкс",
        y_scale=5,
    )
# -------------------------------------------------------------------------------------------------
