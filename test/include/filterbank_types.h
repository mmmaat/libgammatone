#ifndef GAMMATONE_TEST_FILTERBANK_TYPES_H
#define GAMMATONE_TEST_FILTERBANK_TYPES_H

#include <gammatone/filterbank.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/gain.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/clipping.hpp>
#include <boost/mpl/list.hpp>


// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

// channels
template<class X1,template<class> class X2> using b1 = gammatone::policy::channels::fixed_size<X1,X2>;
template<class X1,template<class> class X2> using b2 = gammatone::policy::channels::fixed_overlap<X1,X2>;

// gains
using c1 = gammatone::policy::gain::forall_0dB;
using c2 = gammatone::policy::gain::peroctave_6dB;

// bandwidth
template<class X> using d = gammatone::policy::bandwidth::glasberg1990<X>;

// clipping
using e = gammatone::policy::clipping::off;


// enumeration of all possible filterbank types (core/policy combination)

using T = double;


using filterbank_types = boost::mpl::list
  <
  gammatone::filterbank<T,a1,b1,c1,d,e>,
  gammatone::filterbank<T,a1,b1,c2,d,e>,
  gammatone::filterbank<T,a1,b2,c1,d,e>,
  gammatone::filterbank<T,a1,b2,c2,d,e>,

  gammatone::filterbank<T,a2,b1,c1,d,e>,
  gammatone::filterbank<T,a2,b1,c2,d,e>,
  gammatone::filterbank<T,a2,b2,c1,d,e>,
  gammatone::filterbank<T,a2,b2,c2,d,e>,

  gammatone::filterbank<T,a3,b1,c1,d,e>,
  gammatone::filterbank<T,a3,b1,c2,d,e>,
  gammatone::filterbank<T,a3,b2,c1,d,e>,
  gammatone::filterbank<T,a3,b2,c2,d,e>
>;

#endif // GAMMATONE_TEST_FILTERBANK_TYPES_H
