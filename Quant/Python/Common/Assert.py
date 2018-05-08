from .ext import *
from inspect import currentframe, getframeinfo

frameinfo = getframeinfo(currentframe())


def Assert(condition, error, cf):
    if not condition:
        # print(error, cf)
        print_error(error, cf)
        exit(1)
# -------------------------------------------------------------------------------------------------
