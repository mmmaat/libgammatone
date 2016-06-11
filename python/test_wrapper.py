#!/usr/bin/env python

import sys
import os
sys.path.append(os.path.abspath('../build/python'))
import gammatone

print
print 'Testing gammatone python wrapper'
print

fs = 44100
cf = 1000

f = gammatone.Filter(fs, cf)
assert cf == f.center_frequency
assert fs == f.sample_frequency
print f.center_frequency, f.sample_frequency

inp = 0.1
out = f.compute(inp)
print out
