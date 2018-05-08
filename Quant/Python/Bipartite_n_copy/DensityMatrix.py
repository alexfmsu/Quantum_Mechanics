# -------------------------------------------------------------------------------------------------
from .Hamiltonian import Hamiltonian
from .WaveFunction import WaveFunction
# -------------------------------------------------------------------------------------------------
# scientific
import numpy as np
import scipy.linalg as lg
# -------------------------------------------------------------------------------------------------
# Common
from Common.Matrix import *
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
class Unitary(Matrix):

    # ---------------------------------------------------------------------------------------------
    def __init__(self, H, dt):
        Assert(isinstance(H, Hamiltonian), "H is not Hamiltonian", cf())
        Assert(isinstance(dt, (int, float)), "dt is not numeric", cf())

        Assert(dt > 0, "dt <= 0", cf())

        super(Unitary, self).__init__(m=H.size, n=H.size, dtype=np.complex128)

        H = np.array(H.matrix.data, dtype=np.complex128)

        self.data = np.matrix(lg.expm(-1j * H * dt), dtype=np.complex128)
    # ---------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
class DensityMatrix(Matrix):

    # ---------------------------------------------------------------------------------------------
    def __init__(self, w):
        Assert(isinstance(w, WaveFunction), "W is not WaveFunction", cf())

        super(DensityMatrix, self).__init__(m=w.m, n=w.m, dtype=np.complex128)

        w_data = w.data

        ro_data = w_data.dot(w_data.getH())

        Assert(np.shape(ro_data) == (self.m, self.n), "size mismatch", cf())

        self.data = np.matrix(ro_data)
    # ---------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------
