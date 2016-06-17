#!/usr/bin/env python
#
# Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>
#
# This file is part of libgammatone
#
# libgammatone is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libgammatone. If not, see <http://www.gnu.org/licenses/>.

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
