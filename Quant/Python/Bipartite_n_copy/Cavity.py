# -------------------------------------------------------------------------------------------------
# Common
from Common.Assert import *
from Common.Print import *
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
class Cavity:

    # ---------------------------------------------------------------------------------------------
    def __init__(self, n, wc, wa, g):
        Assert(isinstance(n, int), "n is not integer", cf())
        Assert(isinstance(wc, (int, float)), "wc is not numeric", cf())
        Assert(isinstance(wa, (int, float)), "wa is not numeric", cf())
        Assert(isinstance(g, (int, float)), "g is not numeric", cf())

        Assert(n > 0, "n <= 0", cf())
        Assert(wc > 0, "wc <= 0", cf())
        Assert(wa > 0, "wa <= 0", cf())
        Assert(g > 0, "g <= 0", cf())

        self.n = n

        self.wc = wc
        self.wa = wa

        self.g = g
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print_n(self):
        print(" n: ", color="green")

        print(self.n)

        print()
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print_wc(self):
        print("wc: ", color="green")

        if self.wc >= 1e9:
            print(self.wc / 1e9, "GHz")
        elif self.wc >= 1e6:
            print(self.wc / 1e6, "MHz")
        elif self.wc >= 1e3:
            print(self.wc / 1e3, "KHz")
        else:
            print(self.wc, "Hz")

        print()
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print_wa(self):
        print("wa: ", color="green")

        if self.wa >= 1e9:
            print(self.wa / 1e9, "GHz")
        elif self.wa >= 1e6:
            print(self.wa / 1e6, "MHz")
        elif self.wa >= 1e3:
            print(self.wa / 1e3, "KHz")
        else:
            print(self.wa, "Hz")

        print()
    # ---------------------------------------------------------------------------------------------

    # ---------------------------------------------------------------------------------------------
    def print_g(self):
        print(" g: ", color="green")

        if self.g >= 1e9:
            print(self.g / 1e9, "GHz")
        elif self.g >= 1e6:
            print(self.g / 1e6, "MHz")
        elif self.g >= 1e3:
            print(self.g / 1e3, "KHz")
        else:
            print(self.g, "Hz")

        print()
    # ---------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------
