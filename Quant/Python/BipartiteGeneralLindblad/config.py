MHz = 10 ** 6  # 1 МГц
GHz = 10 ** 9  # 1 ГГц
mks = 1e-6  # 1 мкс

resonance_frequency = 21.506 * GHz  # Rb 85
# МОДЕЛЬ ТАВИСА-КАММИНГСА С 2 ГРУППАМИ АТОМОВ ПО n ЭЛЕМЕНТОВ В КАЖДОЙ, ГДЕ ИЗУЧАЮТСЯ ОСЦИЛЛЯЦИИ МЕЖДУ ВОЗБУЖДЕННЫМИ СОСТОЯНИЯМИ ОДНОЙ ГРУППЫ И ДРУГОЙ
# ЛИНДБЛАД
# ВЫЧИСЛЕНИЕ И ПОСТРОЕНИЕ ГРАФИКОВ ПРОИЗВОДИТСЯ В ВЫЧИСЛИТЕЛЬНОМ БАЗИСЕ
# АТОМЫ НУМЕРУЮТСЯ, НАЧИНАЯ С 0; В БАЗИСНОМ СОСТОЯНИИ - СПРАВА НАЛЕВО
#
# capacity - полная энергия полости
# n - количество атомов в группе (общее число атомов - 2*n)
# wc - cavity frequency (Hz)
# wa - atomic transition frequency (Hz)
# g - vacuum Rabi frequency which characterizes to the photon-atom interaction strength (Hz)
#
# T - время (мкс)
# dt - временной шаг (мкс)
# nt - число шагов по времени
#
# init_state - произвольное начальное состояние
#
# Пример:
# 	capacity = 8
# 	init_state = [5, [0, 0, 0, 1, 1, 1]]
#   в полости 5 фотонов в фоковском состоянии |5>
# 	первая группа атомов |000> - в основном состоянии
# 	вторая группа атомов |111> - в возбужденном состоянии

# BEGIN--------------------------------------------------- CAVITY -----------------------------------------------------
capacity = 2
n = 4
wc = 21.506 * GHz
wa = [21.506 * GHz] * n
g = [1.0 * 1e-2 * 21.506 * GHz] * int(n / 2) + \
    [1.0 * 1e-2 * 21.506 * GHz] * int(n / 2)
# END----------------------------------------------------- CAVITY -----------------------------------------------------

# ---------------------------------------------------------------------------------------------------------------------
T = 0.05 * mks
# T = 1.0 * mks

dt = T / 1000
nt = int(T / dt)
# ---------------------------------------------------------------------------------------------------------------------

init_state = [capacity - int(n / 2), [0] * int(n / 2) + [1] * int(n / 2)]

precision = 10

# ---------------------------------------------------------------------------------------------------------------------

l = g[0] * 2
# l = 0.1 * MHz
# l = 0
# ---------------------------------------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------------------------------------

# Относительные пути файлов вывода для построения графика
path = "BipartiteGeneralLindblad/out/" + str(capacity) + "_" + str(n) + "/"

H_html = path + "H.html"
H_csv = path + "H.csv"

ro_0_csv = path + "ro_0.csv"
U_csv = path + "U.csv"

x_csv = path + "x.csv"
y_csv = path + "t.csv"
z_csv = path + "z.csv"

a_csv = path + "a.csv"
a_cross_a_csv = path + "a_cross_a.csv"

L1_csv = path + "L1.csv"
L_csv = path + "L.csv"
# ---------------------------------------------------------------------------------------------------------------------
