#include <gammatone/policy/bandwidth.hpp>
#include <utils.hpp>
#include <gnuplot-iostream.h>

//! Plot bandwidth in function of center frequency for the 3 available
//! policies. Reproduces the figure p.3 in Slaney1993.
int main()
{
  //! 100 frequency tics in log scale from 1 Hz to 10 kHz.
  const auto frequency = utils::logspace(0,4,100);

  using namespace gammatone::policy::bandwidth;
  
  
  return 0;
}
