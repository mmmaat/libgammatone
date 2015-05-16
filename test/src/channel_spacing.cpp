#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/bandwidth.hpp>
//#include <utils/utils.hpp>
#include <utils/range.hpp>
#include <gnuplot-iostream.h>
#include <iostream>
using namespace std;

typedef double T;

const T fh = 8000;
const T fl = 100;
const T step_factor = 0.25;

int main()
{
  using namespace gammatone::policy;
  typedef bandwidth::glasberg1990<T> B;
  
  const auto cf1 = channels::fixed_overlap<T>::template center_frequency<B>(fl,fh,step_factor);
  const auto cf2 = channels::fixed_size<T>::center_frequency<B>(fl,fh,cf1.size());
  const auto cf3 = channels::fixed_size<T>::center_frequency<B>(fl,fh,30);

  Gnuplot gp;
  gp << "plot '-' u 1:2 w l lw 2 t 'cf1', "
     << "     '-' u 1:2 w l lw 1.5 t 'cf2', "
     << "     '-' u 1:2 w l lw 1 t 'cf3'  "
     << endl;
  gp.send1d(make_pair(cf1,utils::range<int>(1,cf1.size())));
  gp.send1d(make_pair(cf2,utils::range<int>(1,cf2.size())));
  gp.send1d(make_pair(cf3,utils::range<int>(1,cf3.size())));

  return 0;
}
