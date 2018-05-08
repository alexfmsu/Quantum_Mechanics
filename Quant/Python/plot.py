from py import *

import sys
import os
import re

if(len(sys.argv) < 2):
    print("Usage: python3 plot.py <path>")
    exit(1)

path = sys.argv[1]

PYPLOT3D(
    z_csv=path + "/" + "z.csv",
    x_csv=path + "/" + "x.csv",
    y_csv=path + "/" + "t.csv",
    online=False,
    filename="Bipartite")
