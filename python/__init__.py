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

from _gammatone import *
import numpy

def _check(input_array):
    assert len(input_array.shape) == 1, 'input array must be 1D'


def _filter_compute(self, input_array):
    _check(input_array)

    # allocate the output array
    output_array = numpy.zeros(input_array.shape)

    # compute result sample per sample
    for n, i in enumerate(input_array):
        output_array[n] = self._compute(i)

    return output_array


def _filterbank_compute(self, input_array):
    _check(input_array)

    # allocate the output array
    output_array = numpy.zeros((input_array.shape[0], self.nb_channels))

    # compute result sample per sample
    for n, i in enumerate(input_array):
        for m, j in enumerate(self._compute(i)):
            output_array[n][m] = j

    return output_array


Filter.compute = _filter_compute
Filterbank.compute = _filterbank_compute
