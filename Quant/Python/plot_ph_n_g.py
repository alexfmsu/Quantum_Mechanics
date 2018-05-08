from Common.PyPlot3dng import PyPlot3D
from Common.LoadPackage import *
from Common.STR import *
from Common.ext import *
import pandas as pd
import glob
import os
import re

import numpy as np
from Bipartite_peakng import *

# limit = 37
limit = None

out_dir = "/home/alexfmsu/Quant/C++/Bipartite_ph_n_g/out"

h_csv = '_h.csv'
g_csv = '_g.csv'
n_csv = '_n.csv'


capacity = set()
g = set()
n = set()
# print(out_dir)

n_ = 10
n.add(n_)

for i in glob.glob(out_dir + "/" + str(n_) + "/*"):
    path = os.path.basename(i)
    print(path)
    res = re.match(r'([^\_]+)\_([^\_]+)', path)

    if res:
        if float(res.group(2)) < 0.1:
            continue

        print(i, res.group(1), res.group(2))
        capacity.add(int(res.group(1))-n_)
        # g.add(float(res.group(2)))
        g.add(res.group(2))


for _ in (capacity, n, g):
    _ = list(sorted(_))

# capacity = list(sorted(capacity))
# n = list(sorted(n))
# g = list(sorted(g))

print(capacity)
print(n)
print(g)

h = np.zeros((len(capacity), len(g)))

for k, c_ in enumerate(capacity):
    # if c_ > 60:
        # break
    for i, n_ in enumerate(n):
        for j, g_ in enumerate(g):
            dir_ = out_dir + "/" + str(n_) + '/' + str(c_+n_) + '_' + str(g_)
            # print(dir_)
            z_0 = dir_ + "/z_0.csv"

            if not os.path.isfile(z_0):
                # print(z_0)
                # exit(1)
                break

            h[k, j] = get_period(filename=z_0, make_plot=False,
                                 thres_up=0.4, thres_down=0.8, npoints=10000)

            if not(h[k, j] > 0):
                print(h[k, j])
                exit(1)

if limit is not None:
    h = h[:limit]

print(h)
# exit(0)

N = pd.DataFrame(capacity, columns=['vals'])
N.index.name = 'y'
N.to_csv(n_csv)
print(N)

G = pd.DataFrame(g, columns=['vals'])
G.index.name = 'x'
G.to_csv(g_csv)

if limit:
    h = h[:limit, :]

H = pd.DataFrame(h, index=None)
print(H)
H.to_csv(h_csv, index=False, header=None)
# ------------------------------------------
title = ''

PyPlot3D(
    title=title,
    z_csv=h_csv,
    x_csv=g_csv,
    y_csv="_n.csv",
    # online=True,
    online=False,
    path='.',
    filename="Bipartite",
    xaxis=".                        g/hw<sub>c</sub>",
    yaxis="excess photons                        .",
    y_scale=1
)
# ------------------------------------------

# ___END___

# print(k, j, h[k, j])
# exit(0)
# print(h[k, j])
# if(float(g_) == 1 and (c_) == 20):
# h[k, j] = get_period(z_0, True, 0.2)
# exit(0)
# h[k, i, j] = get_period(z_0, True)

# hp0_data = pd.read_csv(z_0)['fidelity']
# hp0_data = list(hp0_data)

# p_data = pd.read_csv(dir_ + "/z_max.csv")['fidelity']
# p_data = list(p_data)
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
# h[k, i, j] = np.max(hp0_data[1:]) / np.max(p_data[1:])
# np.max(hp0_data[1:]) / np.max(p_data[1:])
# print(hp0_data)
# h[i, j] = hp0_data[-1]
# exit(1)
