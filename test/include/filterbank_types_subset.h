#ifndef GAMMATONE_TEST_FILTERBANK_TYPES_SUBSET_H
#define GAMMATONE_TEST_FILTERBANK_TYPES_SUBSET_H

#include <gammatone/filterbank.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <boost/mpl/list.hpp>


// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

template<class T>
using filterbank_types = boost::mpl::list
  <
  gammatone::filterbank<T,a1>,
  gammatone::filterbank<T,a2>,
  gammatone::filterbank<T,a3>
>;

#endif // GAMMATONE_TEST_FILTERBANK_TYPES_SUBSET_H
