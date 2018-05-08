# -----------------------------------------------
import numpy as np
import scipy.linalg as lg

from Common.Matrix import *
# -----------------------------------------------


# -----------------------------------------------
class Unitary:

    def __init__(self, H, dt):
        self.size = H.size

        H = np.array(H.matrix.data, dtype=np.complex128)

        self.data = np.matrix(lg.expm(-1j * H * dt), dtype=np.complex128)
        self.matrix = Matrix(self.size, self.size, dtype=np.complex128)
        self.matrix.data = self.data

    def get_conj(self):
        return self.data.getH()

    def write_to_file(self, filename):
        U = Matrix(self.size, self.size)
        print(U.m)
        U.data = self.data
        U.write_to_file(filename)

    def print(self):
        print('U:')

        self.matrix.print()

        print()

        return
# -----------------------------------------------


# -----------------------------------------------
def get_ro(w):
    w = w.data

    ro = w.dot(w.getH())

    return np.matrix(ro)
# -----------------------------------------------


# -----------------------------------------------
def check_herm(ro):
    if np.any(ro != ro.getH()):
        exit(1)
# -----------------------------------------------


# -----------------------------------------------
def D(ro_1, ro_2, precision=1e-10):  # Следовая метрика (trace distance)
    # -------------------------------------------
    # D(ro, sigma) = 0.5 * tr(|ro - sigma|)
    # D(ro, sigma) = 0, когда ro = sigma
    # -------------------------------------------
    # d_ro = ro_1 - ro_2
    # d_ro_H = d_ro.getH()

    # d_ro_abs = lg.fractional_matrix_power(d_ro_H.dot(d_ro), 0.5)

    # if np.any(abs(d_ro_abs.dot(d_ro_abs) - d_ro_H.dot(d_ro)) > precision):
    #     print(d_ro_abs)
    #     print(d_ro_H)
    #     exit(0)

    # return 0.5 * abs(np.trace(d_ro_abs))
    # -------------------------------------------
    d_ro = ro_1 - ro_2
    d_ro_H = d_ro.getH()

    tr = np.trace(lg.fractional_matrix_power(d_ro_H.dot(d_ro), 0.5))

    return 0.5 * abs(tr)
# -----------------------------------------------


# -----------------------------------------------
def D_HS(ro_1, ro_2):  # Метрика Гильберта-Шмидта (Hilbert-Schmidt distance)
    # D(ro, sigma) = 0.5 * tr(|ro - sigma|)
    # D(ro, sigma) = 0, когда ro = sigma
    d_ro = ro_1 - ro_2

    # return sqrt(abs(np.trace(ro_1 - ro_2) ** 2))
    return sqrt(abs(np.trace(d_ro.dot(d_ro))))
# -----------------------------------------------


# -----------------------------------------------
def Fidelity(ro_sqrt, sigma):  # Ulmann
    # ro_sqrt = lg.sqrtm(ro)
    #sigma_sqrt = lg.fractional_matrix_power(sigma, 0.5)
    # sigma_sqrt = lg.sqrtm(sigma, disp=False)
    # print(sigma_sqrt)
    # exit(1)
    # ro_sqrt_sigma_sqrt = ro_sqrt.dot(sigma_sqrt)
    # fid_matrix = lg.fractional_matrix_power(
    #    sigma_sqrt.dot(ro).dot(sigma_sqrt), 0.5)

    fid_matrix = lg.fractional_matrix_power(
        ro_sqrt.dot(sigma).dot(ro_sqrt), 0.5)

    # return 0.5 *
    # np.trace(lg.sqrtm(ro_sqrt_sigma_sqrt.dot(ro_sqrt_sigma_sqrt)))
    return np.trace(np.abs(fid_matrix))
    # return np.abs(np.trace(fid_matrix))
# -----------------------------------------------
