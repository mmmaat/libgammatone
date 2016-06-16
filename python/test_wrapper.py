#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import os
import sys

sys.path.append(os.path.abspath('../build/python'))
import gammatone

def pulse(size):
    p = np.zeros((size,))
    p[0] = 1.0
    return p

fs = 44100
cf = 1000

f = gammatone.Filter(fs, cf)
out = f.compute(pulse(1000))
plt.plot(out)

fb = gammatone.Filterbank(fs, 300, 8000, 10)
out = fb.compute(pulse(1000))
plt.plot(out)

plt.show()
