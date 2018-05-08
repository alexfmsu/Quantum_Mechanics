import numpy as np

A = np.zeros([3, 3])
B = np.zeros([3, 3])

for i in range(0, 3):
    for j in range(0, 3):
        A[i, j] = 10 * i + j

for i in range(0, 3):
    for j in range(0, 3):
        B[i, j] = 10 * i + j

C = A.dot(B)

# Matrix S = C.sqrt()

print(C)
