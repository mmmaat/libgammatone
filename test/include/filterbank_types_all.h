#ifndef GAMMATONE_TEST_FILTERBANK_TYPES_ALL_H
#define GAMMATONE_TEST_FILTERBANK_TYPES_ALL_H

#include <gammatone/filterbank.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/gain.hpp>
#include <gammatone/policy/clipping.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/joint_view.hpp>


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

// bandwidths
template<class X> using d1 = gammatone::policy::bandwidth::glasberg1990<X>;
template<class X> using d2 = gammatone::policy::bandwidth::slaney1988<X>;
template<class X> using d3 = gammatone::policy::bandwidth::greenwood1990<X>;

// clipping
using e1 = gammatone::policy::clipping::on;
using e2 = gammatone::policy::clipping::off;


// enumeration of all possible filterbank types (core/policy combination)

template<class T>
using t11 = boost::mpl::list
  <
  gammatone::filterbank<T,a1,b1,c1,d1,e1>,
  gammatone::filterbank<T,a1,b1,c1,d1,e2>,

  gammatone::filterbank<T,a1,b1,c1,d2,e1>,
  gammatone::filterbank<T,a1,b1,c1,d2,e2>,

  gammatone::filterbank<T,a1,b1,c1,d3,e1>,
  gammatone::filterbank<T,a1,b1,c1,d3,e2>,

  gammatone::filterbank<T,a1,b1,c2,d1,e1>,
  gammatone::filterbank<T,a1,b1,c2,d1,e2>,

  gammatone::filterbank<T,a1,b1,c2,d2,e1>,
  gammatone::filterbank<T,a1,b1,c2,d2,e2>,

  gammatone::filterbank<T,a1,b1,c2,d3,e1>,
  gammatone::filterbank<T,a1,b1,c2,d3,e2>
  >;

template<class T>
using t12 = boost::mpl::list
  <
  gammatone::filterbank<T,a1,b2,c1,d1,e1>,
  gammatone::filterbank<T,a1,b2,c1,d1,e2>,

  gammatone::filterbank<T,a1,b2,c1,d2,e1>,
  gammatone::filterbank<T,a1,b2,c1,d2,e2>,

  gammatone::filterbank<T,a1,b2,c1,d3,e1>,
  gammatone::filterbank<T,a1,b2,c1,d3,e2>,

  gammatone::filterbank<T,a1,b2,c2,d1,e1>,
  gammatone::filterbank<T,a1,b2,c2,d1,e2>,

  gammatone::filterbank<T,a1,b2,c2,d2,e1>,
  gammatone::filterbank<T,a1,b2,c2,d2,e2>,

  gammatone::filterbank<T,a1,b2,c2,d3,e1>,
  gammatone::filterbank<T,a1,b2,c2,d3,e2>
  >;

template<class T>
using t1 = typename boost::mpl::joint_view<t11<T>, t12<T> >::type;


//////////////////////////////////////////////////////////////////


template<class T>
using t21 = boost::mpl::list
  <
  gammatone::filterbank<T,a2,b1,c1,d1,e1>,
  gammatone::filterbank<T,a2,b1,c1,d1,e2>,

  gammatone::filterbank<T,a2,b1,c1,d2,e1>,
  gammatone::filterbank<T,a2,b1,c1,d2,e2>,

  gammatone::filterbank<T,a2,b1,c1,d3,e1>,
  gammatone::filterbank<T,a2,b1,c1,d3,e2>,

  gammatone::filterbank<T,a2,b1,c2,d1,e1>,
  gammatone::filterbank<T,a2,b1,c2,d1,e2>,

  gammatone::filterbank<T,a2,b1,c2,d2,e1>,
  gammatone::filterbank<T,a2,b1,c2,d2,e2>,

  gammatone::filterbank<T,a2,b1,c2,d3,e1>,
  gammatone::filterbank<T,a2,b1,c2,d3,e2>
  >;

template<class T>
using t22 = boost::mpl::list
  <
  gammatone::filterbank<T,a2,b2,c1,d1,e1>,
  gammatone::filterbank<T,a2,b2,c1,d1,e2>,

  gammatone::filterbank<T,a2,b2,c1,d2,e1>,
  gammatone::filterbank<T,a2,b2,c1,d2,e2>,

  gammatone::filterbank<T,a2,b2,c1,d3,e1>,
  gammatone::filterbank<T,a2,b2,c1,d3,e2>,

  gammatone::filterbank<T,a2,b2,c2,d1,e1>,
  gammatone::filterbank<T,a2,b2,c2,d1,e2>,

  gammatone::filterbank<T,a2,b2,c2,d2,e1>,
  gammatone::filterbank<T,a2,b2,c2,d2,e2>,

  gammatone::filterbank<T,a2,b2,c2,d3,e1>,
  gammatone::filterbank<T,a2,b2,c2,d3,e2>
  >;

template<class T>
using t2 = typename boost::mpl::joint_view<t21<T>, t22<T> >::type;


/////////////////////////////////////////////////////////////



template<class T>
using t31 = boost::mpl::list
  <
  gammatone::filterbank<T,a3,b1,c1,d1,e1>,
  gammatone::filterbank<T,a3,b1,c1,d1,e2>,

  gammatone::filterbank<T,a3,b1,c1,d2,e1>,
  gammatone::filterbank<T,a3,b1,c1,d2,e2>,

  gammatone::filterbank<T,a3,b1,c1,d3,e1>,
  gammatone::filterbank<T,a3,b1,c1,d3,e2>,

  gammatone::filterbank<T,a3,b1,c2,d1,e1>,
  gammatone::filterbank<T,a3,b1,c2,d1,e2>,

  gammatone::filterbank<T,a3,b1,c2,d2,e1>,
  gammatone::filterbank<T,a3,b1,c2,d2,e2>,

  gammatone::filterbank<T,a3,b1,c2,d3,e1>,
  gammatone::filterbank<T,a3,b1,c2,d3,e2>
  >;

template<class T>
using t32 = boost::mpl::list
  <
  gammatone::filterbank<T,a3,b2,c1,d1,e1>,
  gammatone::filterbank<T,a3,b2,c1,d1,e2>,

  gammatone::filterbank<T,a3,b2,c1,d2,e1>,
  gammatone::filterbank<T,a3,b2,c1,d2,e2>,

  gammatone::filterbank<T,a3,b2,c1,d3,e1>,
  gammatone::filterbank<T,a3,b2,c1,d3,e2>,

  gammatone::filterbank<T,a3,b2,c2,d1,e1>,
  gammatone::filterbank<T,a3,b2,c2,d1,e2>,

  gammatone::filterbank<T,a3,b2,c2,d2,e1>,
  gammatone::filterbank<T,a3,b2,c2,d2,e2>,

  gammatone::filterbank<T,a3,b2,c2,d3,e1>,
  gammatone::filterbank<T,a3,b2,c2,d3,e2>
  >;

template<class T>
using t3 = typename boost::mpl::joint_view<t31<T>, t32<T> >::type;



template<class T>
using filterbank_types = typename boost::mpl::joint_view<
  typename boost::mpl::joint_view<t1<T>, t2<T> >::type,
  t3<T> >::type;

#endif // GAMMATONE_TEST_FILTERBANK_TYPES_ALL_H
