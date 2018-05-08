# -------------------------------------------------------------------------------------------------
# system
import sys
# -------------------------------------------------------------------------------------------------
# Bipartite
from Bipartite_n.Cavity import *
from Bipartite_n.Hamiltonian import *

from Bipartite_n.WaveFunction import *

from Bipartite_n.Evolution import *

from Bipartite_n.config import *
# -------------------------------------------------------------------------------------------------
# Common
from Common.ext import mkdir
from Common.PyPlot import PyPlot3D
# -------------------------------------------------------------------------------------------------
# print(path)
mkdir(path)
# exit(1)
# -------------------------------------------------------------------------------------------------
cavity = Cavity(n=n, wc=wc, wa=wa, g=g)

cavity.print_n()
cavity.print_wc()
cavity.print_wa()
cavity.print_g()
# -------------------------------------------------------------------------------------------------
H = Hamiltonian(capacity=capacity, cavity=cavity)

# if __debug__:
# H.write_to_file(filename=H_csv)

# H.print_states()
# H.print_html(filename=H_html)
# -------------------------------------------------------------------------------------------------
w_0 = WaveFunction(states=H.states, init_state=init_state)

# if __debug__:
# w_0.print()
# -------------------------------------------------------------------------------------------------
ro_0 = DensityMatrix(w_0)

# if __debug__:
# ro_0.write_to_file(filename=ro_0_csv)
# -------------------------------------------------------------------------------------------------

run_ro(ro_0, H, dt=dt, nt=nt)

# -------------------------------------------------------------------------------------------------

if not __debug__:
    title = "<b>Bipartite oscillations</b>" + "<br>capacity = " + str(capacity) + ", n = " + str(n) + ", t = " + str(T / mks) + " мкс"
    title += "<br>wc = " + str(wc / GHz) + " GHz"
    title += "<br>wa = " + str(np.array(wa) / GHz) + " GHz"
    title += "<br>g = " + str(np.array(g) / MHz) + " MHz"

    PyPlot3D(
        title=title,
        z_csv=path + "/" + "z.csv",
        x_csv=path + "/" + "x.csv",
        y_csv=path + "/" + "t.csv",
        online=False,
        path=path,
        filename="Bipartite",
        xaxis="states",
        yaxis="time, мкс"
    )
    # -------------------------------------------------------------------------------------------------
