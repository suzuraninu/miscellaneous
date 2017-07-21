import numpy as np

# フィボナッチ数列．

def gf(A, B, N):
    # O(2^n)
    if N == 0:
        return A
    elif N == 1:
        return B
    else:
        return gf(A, B, N - 1) + gf(A, B, N - 2)


def gf2(A, B, N):
    # O(n)
    for i in range(N - 1):
        tmp = A + B
        A = B
        B = tmp
    return tmp


def gf3(A, B, N):
    # O(log(n))
    M = np.array([[1, 1], [1, 0]])
    F = np.array([[B], [A]])
    c = False
    while (N > 0):
        bN = bin(N)
        if bN[-1] == '1':
            if not c:
                Ms = M @ F
                c = True
            else:
                Ms = M @ Ms
        M = M @ M.T
        N = int(bin(N >> 1), 2)

    return Ms[1][0]


A = 1
B = 1
N = 99
print(gf3(A, B, N))
