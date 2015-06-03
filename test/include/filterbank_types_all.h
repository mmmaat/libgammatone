#ifndef GAMMATONE_TEST_FILTERBANK_TYPES_ALL_H
#define GAMMATONE_TEST_FILTERBANK_TYPES_ALL_H

#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/core/slaney1993.hpp>
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

// channels
template<class X1,template<class> class X2,class X3> using c1 = gammatone::policy::channels::fixed_size<X1,X2,X3>;
template<class X1,template<class> class X2,class X3> using c2 = gammatone::policy::channels::fixed_overlap<X1,X2,X3>;

// orders
using d1 = gammatone::policy::order::increasing;
using d2 = gammatone::policy::order::decreasing;

// gains
using e1 = gammatone::policy::gain::forall_0dB;
using e2 = gammatone::policy::gain::peroctave_6dB;

// clipping
using f1 = gammatone::policy::clipping::on;
using f2 = gammatone::policy::clipping::off;

// postprocessing
template<class Arg> using g1 = gammatone::policy::postprocessing::off<Arg>;
template<class Arg> using g2 = gammatone::policy::postprocessing::hwr<Arg>;


// enumeration of all possible filterbank types (core/policy combination)

using T = double;


using types111 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c1,d2,e2,f2,g2>
  >;

using types112 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b1,c2,d2,e2,f2,g2>
  >;

using types121 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c1,d2,e2,f2,g2>
  >;

using types122 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b2,c2,d2,e2,f2,g2>
  >;


using types131 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c1,d2,e2,f2,g2>
  >;

using types132 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a1,b3,c2,d2,e2,f2,g2>
  >;


using types1 = boost::mpl::joint_view<boost::mpl::joint_view<
                                        boost::mpl::joint_view<types111,types112>::type,
                                        boost::mpl::joint_view<types121,types122>::type>::type,
                                      boost::mpl::joint_view<types131,types132>::type>::type;

//////////////////////////////////////////////////////////////////


using types211 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c1,d2,e2,f2,g2>
  >;

using types212 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b1,c2,d2,e2,f2,g2>
  >;

using types221 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c1,d2,e2,f2,g2>
  >;

using types222 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b2,c2,d2,e2,f2,g2>
  >;


using types231 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c1,d2,e2,f2,g2>
  >;

using types232 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a2,b3,c2,d2,e2,f2,g2>
  >;


using types2 = boost::mpl::joint_view<boost::mpl::joint_view<
                                        boost::mpl::joint_view<types211,types212>::type,
                                        boost::mpl::joint_view<types221,types222>::type>::type,
                                      boost::mpl::joint_view<types231,types232>::type>::type;

//////////////////////////////////////////////////////////////////


using types311 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c1,d2,e2,f2,g2>
  >;

using types312 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b1,c2,d2,e2,f2,g2>
  >;

using types321 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c1,d2,e2,f2,g2>
  >;

using types322 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b2,c2,d2,e2,f2,g2>
  >;


using types331 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c1,d2,e2,f2,g2>
  >;

using types332 = boost::mpl::list
  <
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d1,e2,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e1,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e1,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e1,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e1,f2,g2>,

  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e2,f1,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e2,f1,g2>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e2,f2,g1>,
  gammatone::filterbank::concrete<T,a3,b3,c2,d2,e2,f2,g2>
  >;


using types3 = boost::mpl::joint_view<boost::mpl::joint_view<
                                        boost::mpl::joint_view<types311,types312>::type,
                                        boost::mpl::joint_view<types321,types322>::type>::type,
                                      boost::mpl::joint_view<types331,types332>::type>::type;

//////////////////////////////////////////////////////////////////


using filterbank_types = boost::mpl::joint_view<boost::mpl::joint_view<types1,types2>::type,types3>::type;

#endif // GAMMATONE_TEST_FILTERBANK_TYPES_ALL_H
