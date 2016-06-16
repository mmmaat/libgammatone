from _gammatone import *
import numpy

def _filter_compute(self, input_array):
    assert len(input_array.shape) == 1, 'input array must be 1D'

    # allocate the output array
    output_array = numpy.zeros(input_array.shape)

    # compute result sample per sample
    for n, i in enumerate(input_array):
        output_array[n] = self._compute(i)

    return output_array

Filter.compute = _filter_compute


def _filterbank_compute(self, input_array):
    assert len(input_array.shape) == 1, 'input array must be 1D'

    # allocate the output array
    output_array = numpy.zeros((input_array.shape[0], self.nb_channels))

    # compute result sample per sample
    for n, i in enumerate(input_array):
        for m, j in enumerate(self._compute(i)):
            output_array[n][m] = j

    return output_array

Filterbank.compute = _filterbank_compute
