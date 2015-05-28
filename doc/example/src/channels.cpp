// channels.cpp
// Compile with: g++ -std=c++14 -o channels channels.cpp -I/path/to/libgammatone/include
#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/order.hpp>
#include <iostream>
using namespace std;
int main()
{
  using namespace gammatone;
  using T = double;
  
  // sample frequency, lowest and highest center frequencies
  const T fs = 44100, fl = 1000, fh = 5000;
  
  // Creates a filterbank with 20 channels
  const std::size_t nb_channels = 20;
  filterbank::concrete<T, core::cooke1993,
                       policy::bandwidth::glasberg1990,
                       policy::channels::fixed_size>
    fb1(fs,fl,fh,nb_channels);

  // Creates a filterbank with an overlap factor of 0.3
  const T overlap = 0.3;
  filterbank::concrete<T, core::cooke1993,
                       policy::bandwidth::glasberg1990,
                       policy::channels::fixed_overlap>
    fb2(fs,fl,fh,overlap);

  // assert(fb1.nb_channels() == 20);
  // assert(fb2.overlap() == 0.3);
  cout << fb1.nb_channels() << " " << fb1.overlap() << endl;
  cout << fb2.nb_channels() << " " << fb2.overlap() << endl;
  return 0;
}
