from Common.PyPlot3d import PyPlot3D
from Common.LoadPackage import *
from Common.STR import *
from Common.ext import *
import pandas as pd
import glob
import os
import re

import numpy as np

# limit = 37
limit = None

# out_dir = "../C++/Bipartite_n_g/out_2"
out_dir = "../C++/Bipartite_n_g_copy/out"
# out_dir = "Bipartite/out/3d"

g = set()
n = set()

for i in glob.glob(out_dir + "/*"):
    path = os.path.basename(i)

    res = re.match(r'([^\_]+)\_([^\_]+)', path)

    if res:
        print(i, res.group(1), res.group(2))
        n.add(int(res.group(1)))
        # g.add(float(res.group(2)))
        g.add(res.group(2))

n = list(sorted(n))
g = list(sorted(g))

print(n)
print(g)

h = np.zeros((len(n), len(g)))

# print(h)

for i, n_ in enumerate(n):
    for j, g_ in enumerate(g):
        z_0 = out_dir + "/" + str(n_) + '_' + str(g_) + "/z_0.csv"

        if not os.path.isfile(z_0):
            break

        hp0_data = pd.read_csv(z_0)['fidelity']
        hp0_data = list(hp0_data)

        p_data = pd.read_csv(out_dir + '/' + str(n_) +
                             '_' + str(g_) + "/z_max.csv")['fidelity']
        p_data = list(p_data)
        # h[i, j] = hp0_data[-1] / p_data[-1]
        # h[i, j] = np.max(hp0_data[1:])
        # h[i, j] = np.max(p_data[1:])
        # h[i, j] = hp0_data[-1] / p_data[-1]
        # h[i, j] = np.mean(np.array(hp0_data[:]))
        # h[i, j] = np.max(np.array(p_data[1:]))
        # h[i, j] = np.min(np.array(p_data[1:]))
        # h[i, j] = np.mean(np.array(hp0_data[:]))
        # h[i, j] = np.mean(np.array(p_data[:]))
        # h[i, j] = np.mean(np.array(p_data[:]))
        # h[i, j] = np.mean(np.array(hp0_data[:]) / np.array(p_data[:]))
        # h[i, j] = np.max(np.array(hp0_data[1:]) / np.array(p_data[1:]))
        # h[i, j] = np.max(hp0_data[1:] / p_data[1:])
        h[i, j] = np.max(hp0_data[1:]) / np.max(p_data[1:])
        # print(hp0_data)
        # h[i, j] = hp0_data[-1]
# exit(1)
if limit is not None:
    h = h[:limit]

N = pd.DataFrame(n, columns=['vals'])
N.index.name = 'y'
N.to_csv('_n.csv')

G = pd.DataFrame(g, columns=['vals'])
G.index.name = 'x'
G.to_csv('_g.csv')

if limit:
    h = h[:limit, :]

H = pd.DataFrame(h, index=None)
print(H)
H.to_csv('_h.csv', index=False, header=None)
# ------------------------------------------
title = ''

PyPlot3D(
    title=title,
    z_csv="_h.csv",
    x_csv="_g.csv",
    y_csv="_n.csv",
    online=False,
    path='.',
    filename="Bipartite",
    xaxis="g/hw<sub>c</sub>        .",
    yaxis="n",
    y_scale=1
    # y_scale=y_scale
)
# ------------------------------------------
