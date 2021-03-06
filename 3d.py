from random import random
from math import exp
import numpy as np

mcs = 230000
mcs0 = 30000
mag = 0.0
mag_sum = 0.0
T = 0.1
p = 0.5
L = 6
i_n = [0] * L
i_p = [0] * L
S = [[[0 for i in range(L)] for j in range(L)] for k in range(L)]
U = 0
var_mag = []

for l in range(L):
    i_n[l] = l + 1
    i_n[L - 1] = 0
    i_p[0] = L - 1
    if l > 0:
        i_p[l] = l - 1

for i in range(L):
    for j in range(L):
        for k in range(L):
            if random() < p:
                S[i][j][k] = -1
            else:
                S[i][j][k] = 1

plik = open('dane.txt', 'w')
plik.writelines('Temperatura\tSrednie namagnesowanie\tPodatnosc magnetyczna\n')

while (T <= 10):
    for m in range(mcs):
        for i in range(L):
            for j in range(L):
                for k in range(L):
                    U = 2 * S[i][j][k] * (S[i_n[i]][j][k] + S[i_p[i]][j][k]
                                        + S[i][i_n[j]][k] + S[i][i_p[j]][k]
                                        + S[i][j][i_n[k]] + S[i][j][i_p[k]])

                    if U < 0:
                        S[i][j][k] = -S[i][j][k]
                    else:
                        if random() < exp(-U / T):
                            S[i][j][k] = -S[i][j][k]

        if m > mcs0 and m % 100 == 0:
            mag = mag + np.sum(S)
            mag = mag / L ** 3
            mag_sum = mag_sum + abs(mag)
            var_mag.append(mag)
    mag_sum = mag_sum / 100.0
    chi = (L ** 3 / T) * np.var(var_mag)
    plik.writelines(f'{T}\t{mag_sum}\t{chi}\n')
    var_mag = []
    T += 0.05

plik.close()
