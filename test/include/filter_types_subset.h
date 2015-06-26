#ifndef GAMMATONE_TEST_FILTER_TYPES_SUBSET_H
#define GAMMATONE_TEST_FILTER_TYPES_SUBSET_H

#include <gammatone/filter.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <boost/mpl/list.hpp>

// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

//using T = double;

template<class T> using filter_types = boost::mpl::list
  <
  gammatone::filter<T,a1>,
  gammatone::filter<T,a2>,
  gammatone::filter<T,a3>
  >;

#endif // GAMMATONE_TEST_FILTER_TYPES_SUBSET_H
