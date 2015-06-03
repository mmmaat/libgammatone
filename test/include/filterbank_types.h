#ifndef GAMMATONE_TEST_FILTERBANK_TYPES_H
#define GAMMATONE_TEST_FILTERBANK_TYPES_H

#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <boost/mpl/list.hpp>


// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

// bandwidth
template<class X> using b = gammatone::policy::bandwidth::glasberg1990<X>;

// channels
template<class X1,template<class> class X2,class X3> using c1 = gammatone::policy::channels::fixed_size<X1,X2,X3>;
template<class X1,template<class> class X2,class X3> using c2 = gammatone::policy::channels::fixed_overlap<X1,X2,X3>;

// orders
using d = gammatone::policy::order::increasing;

// gains
using e1 = gammatone::policy::gain::forall_0dB;
using e2 = gammatone::policy::gain::peroctave_6dB;


// enumeration of all possible filterbank types (core/policy combination)

using T = double;


using filterbank_types = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b,c1,d,e1>,
  gammatone::filterbank::concrete<T,a1,b,c1,d,e2>,

  gammatone::filterbank::concrete<T,a1,b,c2,d,e1>,
  gammatone::filterbank::concrete<T,a1,b,c2,d,e2>,

  gammatone::filterbank::concrete<T,a2,b,c1,d,e1>,
  gammatone::filterbank::concrete<T,a2,b,c1,d,e2>,

  gammatone::filterbank::concrete<T,a2,b,c2,d,e1>,
  gammatone::filterbank::concrete<T,a2,b,c2,d,e2>,

  gammatone::filterbank::concrete<T,a3,b,c1,d,e1>,
  gammatone::filterbank::concrete<T,a3,b,c1,d,e2>,

  gammatone::filterbank::concrete<T,a3,b,c2,d,e1>,
  gammatone::filterbank::concrete<T,a3,b,c2,d,e2>
  >;

#endif // GAMMATONE_TEST_FILTERBANK_TYPES_H
