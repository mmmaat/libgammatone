// minimal.cpp
// Compile with: g++ -o minimal minimal.cpp -I/path/to/libgammatone/include -std=c++14 
#include <gammatone/filter.hpp>
#include <vector>

//! Minimal example: a default filterbank to compute a cochleogram
int main()
{
  // An input signal (here 1000 times zero, silly but minimal)
  std::vector<double> input(1000, 0.0);

  // A gammatone filter sampled at 44.1 kHz, centered at 1 kHz
  gammatone::filter<double> fb(44100, 1000);

  // Compute the output signal from input
  std::vector<double> output = fb.compute(input);

  return 0;
}
