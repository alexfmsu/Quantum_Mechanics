GHz = 1e9
MHz = 1e6
KHz = 1e3

mks = 1e-6

# BEGIN--------------------------------------------------- CAVITY -----------------------------------------------------
capacity = 10
n = 5

wc = 21.506 * GHz
wa = 21.506 * GHz
g  = 0.1076471508442 * GHz
# END----------------------------------------------------- CAVITY -----------------------------------------------------

# ---------------------------------------------------------------------------------------------------------------------
T = 0.05 * mks
dt = T / 500
nt = int(T / dt)
# ---------------------------------------------------------------------------------------------------------------------

init_state = [0, 5]

precision = 5

# ---------------------------------------------------------------------------------------------------------------------
path = "Bipartite_n/out/5/"
H_csv = path + "H.csv"
U_csv = path + "U.csv"

x_csv = path + "x.csv"
y_csv = path + "t.csv"
z_csv = path + "z.csv"

fid_csv = path + "fid.csv"
# ---------------------------------------------------------------------------------------------------------------------
