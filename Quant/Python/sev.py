import numpy as np
import pandas as pd

import plotly
# from plotly import tools
import plotly.plotly as py
import plotly.graph_objs as go

import glob
import os
from Common.LoadPackage import *
from Common.STR import *

# plotly.tools.set_credentials_file(username="alexfmsu", api_key="g8ocp0PgQCY1a2WqBpyr")
plotly.tools.set_credentials_file(
    username="alexf-msu", api_key="VSOCzkhAhdKQDuV7eiYq")

n_list = []

for i in glob.glob("Bipartite/out/*"):
    filename = os.path.basename(i)
    print(i)
    n_list.append(int(filename))

n_list = sorted(n_list)

print(n_list)

# T = 1 * 1e-6
# nt = 1000

config_dir = 'Bipartite/out/1'
config_path = config_dir + '/' + 'config.py'
config = load_pkg('config', config_path)

t = np.around(np.linspace(0, config.nt+1, config.nt+1)/20, 1)
print(t)
trace = {}

for n in n_list:
    fid_path = "Bipartite/out/" + str(n) + "/fid.csv"
    if not os.path.exists(fid_path):
        continue

    z_data = pd.read_csv(fid_path)

    data_0 = {
        "title": "<b>Fidelity</b>",
        # "title": "<b>w<sub>c</sub> = w<sub>a</sub> = 2 PI x 0.5 MHz;\tg / (hw<sub>c</sub>) = 0.001<br>" +
        # "m = g<b>",
        "x": {
            "title": "Time, mks",
            "data": t,

            "ticktext": np.around(np.linspace(0, T_str_v(config.T), 6), 3),
            "tickvals": np.around(np.linspace(0, config.nt+1, 6), 3)
        },
        "y":
        {
            "title": "Fidelity",
            "data": [
                z_data["fidelity"],
            ]
        },
    }

    for i in data_0["y"]["data"]:
        plot = go.Scatter(
            x=data_0["x"]["data"],
            y=i,
            name="n = " + str(n),

            # showlegend=False,
            # font=dict(
            #     color="#000000",
            #     # color="#222",
            #     size=40,
            # ),
            # line = dict(
            #     color = ("rgb(205, 12, 24)"),
            #     width = 4
            # )
        )

    trace[n] = plot


fig = plotly.tools.make_subplots(
    rows=1,
    cols=1,
    specs=[[{}]],
    shared_xaxes=True, shared_yaxes=True,
)

for k, v in trace.items():
    fig.append_trace(v, 1, 1)

fig['layout']['font'] = dict(
    color="#222",
    size=24,
)

fig["layout"].update(
    height=800,
    width=1600,
    # title="<b>Fidelity</b>",

    margin=dict(l=180, t=-100, b=150),

    titlefont=dict(
        color="#222",
        size=30,
    ),
    xaxis=dict(
        # title=data_0["x"]["title"],
        title="Time, " + T_str_mark(config.T),

        titlefont=dict(
            color="#222",
            size=35,
        ),

        zeroline=True,

        rangemode='tozero',

        tickprefix=" "*50,
        ticks='outside',
        tickfont=dict(
            color="#000000",

            # color="#222",
            size=30,
        ),
        ticklen=10,

        linewidth=2,

        ticktext=np.around(np.linspace(0, T_str_v(config.T), 6), 3),
        # tickvals=np.around(np.linspace(0, T_str_v(config.T), 11), 3),
        tickvals=np.around(np.linspace(0, (config.nt)/20, 6), 3)
        # ticktext=np.around(np.linspace(0, T * 1e6, 11), 3),
        # tickvals=np.around(np.linspace(0, nt, 11), 3)
    ),
    yaxis=dict(
        title="Fidelity",
        titlefont=dict(
            color="#000000",
            # color="#222",
            size=35,
        ),
        zeroline=True,
        rangemode='tozero',

        ticks='outside',
        tickfont=dict(
            color="#000000",
            # color="#222",
            size=30,
        ),
        ticklen=10,
        tickprefix=" "*50,
        linewidth=2,
        range=[0, 1],
    ),
)
# py.iplot(fig, filename="stacked-subplots-shared-xaxes")
# py.plot(fig, filename="stacked-subplots-shared-xaxes")
plotly.offline.plot(fig, filename="Fidelity" + ".html")
