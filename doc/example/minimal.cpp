#include <gammatone/filter.hpp>
#include <vector>

int main()
{
    // Init a gammatone filter sampled at 44.1 kHz, centered at 1 kHz
    gammatone::filter<double> filter(44100, 1000);

    // An input signal (here 1000 times zero, silly but minimal)
    std::vector<double> input(1000, 0.0);

    // Init an output buffer to store the filter response
    std::vector<double> output(1000);
    
    // Compute the output signal from input
    filter.compute_range(input.begin(), input.end(), output.begin());
    
    return 0;
}
