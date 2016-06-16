#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import os
import sys

sys.path.append(os.path.abspath('../build/python'))
import _filter

def _compute(self, input_array):
    output_array = np.zeros(input_array.shape)
    for n, i in enumerate(input_array):
        output_array[n] = self._compute(i)
    return output_array

_filter.Filter.compute = _compute



def pulse(size):
    p = np.zeros((size,))
    p[0] = 1.0
    return p

print
print 'Testing gammatone python wrapper'
print

fs = 8000
cf = 1000

f = _filter.Filter(fs, cf)
out = f.compute(pulse(100))

plt.plot(out)
plt.show()
