#ifndef GAMMATONE_TEST_FILTER_TYPES_H
#define GAMMATONE_TEST_FILTER_TYPES_H

#include <gammatone/gammatone.hpp>
#include <boost/mpl/list.hpp>

// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

// enumeration of all possible filter types (core/policy combination)

using T = double;


using filter_types = boost::mpl::list
  <
  gammatone::filter<T,a1>,
  gammatone::filter<T,a2>,
  gammatone::filter<T,a3>
  >;
  
#endif // GAMMATONE_TEST_FILTER_TYPES_H
