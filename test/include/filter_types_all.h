#ifndef GAMMATONE_TEST_FILTER_TYPES_ALL_H
#define GAMMATONE_TEST_FILTER_TYPES_ALL_H

#include <gammatone/filter.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/joint_view.hpp>

// cores
template<class... X> using a1 = gammatone::core::cooke1993<X...>;
template<class... X> using a2 = gammatone::core::slaney1993<X...>;
template<class... X> using a3 = gammatone::core::convolution<X...>;

// bandwidths
template<class X> using b1 = gammatone::policy::bandwidth::glasberg1990<X>;
template<class X> using b2 = gammatone::policy::bandwidth::slaney1988<X>;
template<class X> using b3 = gammatone::policy::bandwidth::greenwood1990<X>;

// clipping
using c1 = gammatone::policy::clipping::on;
using c2 = gammatone::policy::clipping::off;


// enumeration of all possible filter types (core/policy combination)

template<class T>
using t1 = boost::mpl::list
  <
  gammatone::filter<T,a1,b1,c1>,
  gammatone::filter<T,a1,b1,c2>,

  gammatone::filter<T,a1,b2,c1>,
  gammatone::filter<T,a1,b2,c2>,

  gammatone::filter<T,a1,b3,c1>,
  gammatone::filter<T,a1,b3,c2>
  >;

template<class T>
using t2 = boost::mpl::list
  <
  gammatone::filter<T,a2,b1,c1>,
  gammatone::filter<T,a2,b1,c2>,

  gammatone::filter<T,a2,b2,c1>,
  gammatone::filter<T,a2,b2,c2>,

  gammatone::filter<T,a2,b3,c1>,
  gammatone::filter<T,a2,b3,c2>
  >;

template<class T>
using t3 = boost::mpl::list
  <
  gammatone::filter<T,a3,b1,c1>,
  gammatone::filter<T,a3,b1,c2>,

  gammatone::filter<T,a3,b2,c1>,
  gammatone::filter<T,a3,b2,c2>,

  gammatone::filter<T,a3,b3,c1>,
  gammatone::filter<T,a3,b3,c2>
  >;

template<class T>
using filter_types = typename boost::mpl::joint_view<
  typename boost::mpl::joint_view<t1<T>,t2<T> >::type,
  t3<T> >::type;

#endif // GAMMATONE_TEST_FILTER_TYPES_ALL_H
