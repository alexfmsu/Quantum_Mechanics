import glob
from Common.PyPlot import *
from Common.ext import mkdir

import sys
import os
import re

section = "BipartiteGeneral"
plot_dir = "./plots/" + section

mkdir(plot_dir)

for i in glob.glob(section + "/out/*"):
    path = i

    PyPlot3D(
        title="",
        z_csv=path + "/" + "z.csv",
        x_csv=path + "/" + "x.csv",
        y_csv=path + "/" + "t.csv",
        online=False,
        filename="Bipartite",
        to_file=plot_dir + "/" + os.path.basename(i) + ".png"    )
