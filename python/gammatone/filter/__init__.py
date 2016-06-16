from _filter import *

def _compute(self, input_array):
    output_array = np.zeros(input_array.shape)
    for n, i in enumerate(input_array):
        output_array[n] = self._compute(i)
    return output_array

Filter.compute = _compute
