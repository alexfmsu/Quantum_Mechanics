# -------------------------------------------------------------------------------------------------
# system
import sys
# -------------------------------------------------------------------------------------------------
# BipartiteGeneralLindblad
from BipartiteGeneralLindblad.Cavity import *
from BipartiteGeneralLindblad.Hamiltonian import *

from BipartiteGeneralLindblad.WaveFunction import *
from BipartiteGeneralLindblad.DensityMatrix import *

from BipartiteGeneralLindblad.Evolution import *
# -------------------------------------------------------------------------------------------------
# Common
from Common.LoadPackage import *

from Common.ext import mkdir
from Common.PyPlot import PyPlot3D
# -------------------------------------------------------------------------------------------------
config = load_pkg("config", "BipartiteGeneralLindblad/config.py")
# -------------------------------------------------------------------------------------------------
mkdir(config.path)
# -------------------------------------------------------------------------------------------------
cavity = Cavity(n=config.n, wc=config.wc, wa=config.wa, g=config.g)

cavity.print_n()
cavity.print_wc()
cavity.print_wa()
cavity.print_g()
# -------------------------------------------------------------------------------------------------
H = Hamiltonian(capacity=config.capacity, cavity=cavity)

if __debug__:
    H.write_to_file(filename=config.H_csv)

    H.print_bin_states()
    H.print_html(filename=H_html)
# -------------------------------------------------------------------------------------------------
w_0 = WaveFunction(states=H.states, init_state=config.init_state)

if __debug__:
    w_0.print()
# -------------------------------------------------------------------------------------------------
ro_0 = DensityMatrix(w_0)

# if __debug__:
# ro_0.write_to_file(filename=ro_0_csv)
# -------------------------------------------------------------------------------------------------

run(ro_0=ro_0, H=H, dt=config.dt, nt=config.nt, l=config.l, config=config)

# -------------------------------------------------------------------------------------------------
from Common.STR import *

y_scale = 1

if config.T == 0.05 * config.mks:
    y_scale = 0.1
elif config.T == 0.5 * config.mks:
    y_scale = 0.01
elif config.T == 1 * config.mks:
    y_scale = 7.5
    # y_scale = 10
elif config.T == 5 * config.mks:
    y_scale = 1

if not __debug__:
    title = "<b>"
    title += "capacity = " + str(config.capacity) + ", n = " + str(config.n)
    title += "<br>w<sub>c</sub> = " + wc_str(config.wc)
    title += "<br>w<sub>a</sub> = " + \
        "[" + ", ".join([wa_str(i) for i in config.wa]) + "]"
    title += "<br>g = " + "[" + ", ".join([g_str(i) for i in config.g]) + "]"
    title += "<br>t = " + T_str(config.T)
    title += "<br>l = " + wc_str(config.l)
    title += "</b>"

    PyPlot3D(
        title=title,
        z_csv=config.path + "/" + "z.csv",
        x_csv=config.path + "/" + "x.csv",
        y_csv=config.path + "/" + "t.csv",
        online=False,
        path=config.path,
        filename="BipartiteGeneralLindblad",
        xaxis="states",
        yaxis="time, " + T_str_mark(config.T),
        y_scale=y_scale
    )
# -------------------------------------------------------------------------------------------------
