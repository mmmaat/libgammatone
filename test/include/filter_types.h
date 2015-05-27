#ifndef GAMMATONE_TEST_FILTER_TYPES_H
#define GAMMATONE_TEST_FILTER_TYPES_H

#include <gammatone/gammatone.hpp>
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

// gains
using c1 = gammatone::policy::gain::forall_0dB;
using c2 = gammatone::policy::gain::peroctave_6dB;

// clipping
using d1 = gammatone::policy::clipping::on;
using d2 = gammatone::policy::clipping::off;

// postprocessing
template<class Arg> using e1 = gammatone::policy::postprocessing::off<Arg>;
template<class Arg> using e2 = gammatone::policy::postprocessing::hwr<Arg>;


// enumeration of all possible filter types (core/policy combination)

using T = double;

using types11 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a1,b1,c1,d1,e1>,
  gammatone::filter::concrete<T,a1,b1,c1,d1,e2>,
  gammatone::filter::concrete<T,a1,b1,c1,d2,e1>,
  gammatone::filter::concrete<T,a1,b1,c1,d2,e2>,

  gammatone::filter::concrete<T,a1,b1,c2,d1,e1>,
  gammatone::filter::concrete<T,a1,b1,c2,d1,e2>,
  gammatone::filter::concrete<T,a1,b1,c2,d2,e1>,
  gammatone::filter::concrete<T,a1,b1,c2,d2,e2>
  >;

//

using types12 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a1,b2,c1,d1,e1>,
  gammatone::filter::concrete<T,a1,b2,c1,d1,e2>,
  gammatone::filter::concrete<T,a1,b2,c1,d2,e1>,
  gammatone::filter::concrete<T,a1,b2,c1,d2,e2>,

  gammatone::filter::concrete<T,a1,b2,c2,d1,e1>,
  gammatone::filter::concrete<T,a1,b2,c2,d1,e2>,
  gammatone::filter::concrete<T,a1,b2,c2,d2,e1>,
  gammatone::filter::concrete<T,a1,b2,c2,d2,e2>
  >;

//

using types13 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a1,b3,c1,d1,e1>,
  gammatone::filter::concrete<T,a1,b3,c1,d1,e2>,
  gammatone::filter::concrete<T,a1,b3,c1,d2,e1>,
  gammatone::filter::concrete<T,a1,b3,c1,d2,e2>,

  gammatone::filter::concrete<T,a1,b3,c2,d1,e1>,
  gammatone::filter::concrete<T,a1,b3,c2,d1,e2>,
  gammatone::filter::concrete<T,a1,b3,c2,d2,e1>,
  gammatone::filter::concrete<T,a1,b3,c2,d2,e2>
  >;

using cooke_types = boost::mpl::joint_view<boost::mpl::joint_view<types11,types12>::type,types13>::type;

//////////

using types21 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a2,b1,c1,d1,e1>,
  gammatone::filter::concrete<T,a2,b1,c1,d1,e2>,
  gammatone::filter::concrete<T,a2,b1,c1,d2,e1>,
  gammatone::filter::concrete<T,a2,b1,c1,d2,e2>,

  gammatone::filter::concrete<T,a2,b1,c2,d1,e1>,
  gammatone::filter::concrete<T,a2,b1,c2,d1,e2>,
  gammatone::filter::concrete<T,a2,b1,c2,d2,e1>,
  gammatone::filter::concrete<T,a2,b1,c2,d2,e2>
  >;

//

using types22 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a2,b2,c1,d1,e1>,
  gammatone::filter::concrete<T,a2,b2,c1,d1,e2>,
  gammatone::filter::concrete<T,a2,b2,c1,d2,e1>,
  gammatone::filter::concrete<T,a2,b2,c1,d2,e2>,

  gammatone::filter::concrete<T,a2,b2,c2,d1,e1>,
  gammatone::filter::concrete<T,a2,b2,c2,d1,e2>,
  gammatone::filter::concrete<T,a2,b2,c2,d2,e1>,
  gammatone::filter::concrete<T,a2,b2,c2,d2,e2>
  >;

//

using types23 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a2,b3,c1,d1,e1>,
  gammatone::filter::concrete<T,a2,b3,c1,d1,e2>,
  gammatone::filter::concrete<T,a2,b3,c1,d2,e1>,
  gammatone::filter::concrete<T,a2,b3,c1,d2,e2>,

  gammatone::filter::concrete<T,a2,b3,c2,d1,e1>,
  gammatone::filter::concrete<T,a2,b3,c2,d1,e2>,
  gammatone::filter::concrete<T,a2,b3,c2,d2,e1>,
  gammatone::filter::concrete<T,a2,b3,c2,d2,e2>
  >;

using slaney_types = boost::mpl::joint_view<boost::mpl::joint_view<types21,types22>::type,types23>::type;

////////

using types31 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a3,b1,c1,d1,e1>,
  gammatone::filter::concrete<T,a3,b1,c1,d1,e2>,
  gammatone::filter::concrete<T,a3,b1,c1,d2,e1>,
  gammatone::filter::concrete<T,a3,b1,c1,d2,e2>,

  gammatone::filter::concrete<T,a3,b1,c2,d1,e1>,
  gammatone::filter::concrete<T,a3,b1,c2,d1,e2>,
  gammatone::filter::concrete<T,a3,b1,c2,d2,e1>,
  gammatone::filter::concrete<T,a3,b1,c2,d2,e2>
  >;

//

using types32 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a3,b2,c1,d1,e1>,
  gammatone::filter::concrete<T,a3,b2,c1,d1,e2>,
  gammatone::filter::concrete<T,a3,b2,c1,d2,e1>,
  gammatone::filter::concrete<T,a3,b2,c1,d2,e2>,

  gammatone::filter::concrete<T,a3,b2,c2,d1,e1>,
  gammatone::filter::concrete<T,a3,b2,c2,d1,e2>,
  gammatone::filter::concrete<T,a3,b2,c2,d2,e1>,
  gammatone::filter::concrete<T,a3,b2,c2,d2,e2>
  >;

//

using types33 = boost::mpl::list
  <
  gammatone::filter::concrete<T,a3,b3,c1,d1,e1>,
  gammatone::filter::concrete<T,a3,b3,c1,d1,e2>,
  gammatone::filter::concrete<T,a3,b3,c1,d2,e1>,
  gammatone::filter::concrete<T,a3,b3,c1,d2,e2>,

  gammatone::filter::concrete<T,a3,b3,c2,d1,e1>,
  gammatone::filter::concrete<T,a3,b3,c2,d1,e2>,
  gammatone::filter::concrete<T,a3,b3,c2,d2,e1>,
  gammatone::filter::concrete<T,a3,b3,c2,d2,e2>
  >;

using convolution_types = boost::mpl::joint_view<boost::mpl::joint_view<types31,types32>::type,types33>::type;


using filter_types =
  boost::mpl::joint_view<boost::mpl::joint_view<cooke_types,slaney_types>::type,convolution_types>::type;

#endif // GAMMATONE_TEST_FILTER_TYPES_H
