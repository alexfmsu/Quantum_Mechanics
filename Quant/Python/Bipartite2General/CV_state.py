import numpy as np
import time


class Full_state:

    def __init__(self, capacity, cv_states):
        self.capacity = capacity
        self.state_ = cv_states

        return

    def sum(self):
        self.sum_ = 0

        for i in self.state_:
            self.sum_ += i.sum()

        return self.sum_

    def state(self):
        return [self.state_[0].state, self.state_[1].state]

    def inc(self):
        while(1):
            inced = False

            for i in self.state_[::-1]:
                if not i.inc():
                    i.zeros()

                    continue
                else:
                    inced = True
                    break

            if not inced:
                return -1
            if self.sum() == self.capacity:
                return

    def print(self):
        for i in self.state_:
            i.print(end=" ")

        print()


class CV_state:

    def __init__(self, capacity, n):
        self.capacity = capacity
        self.n = n

        self.state = [0, [0] * self.n]

    def inc(self):
        inced = False

        for n_ in range(self.n - 1, -1, -1):
            if self.state[1][n_] == 1:
                self.state[1][n_] = 0
                continue
            elif self.state[1][n_] == 0:
                self.state[1][n_] = 1
                inced = True
                break

        if not inced:
            if self.sum() > self.capacity:
                return False

            self.state[0] += 1
            inced = True

        return inced

    def zeros(self):
        self.state[0] = 0
        self.state[1] = [0] * self.n

    def print(self, end=""):
        print(self.state, end=end)

    def sum(self):
        return self.state[0] + np.sum(self.state[1])
