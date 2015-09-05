#include <gammatone/filter.hpp>
#include <vector>

int main()
{
  // An input signal (here 1000 times zero, silly but minimal)
  std::vector<double> input(1000, 0.0);

  // A gammatone filter sampled at 44.1 kHz, centered at 1 kHz
  gammatone::filter<double> filter(44100, 1000);

  // TODO: no allocation !
  // Compute the output signal from input
  std::vector<double> output = filter.compute(input);

  return 0;
}
