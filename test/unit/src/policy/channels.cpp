/*
  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>

  This file is part of libgammatone

  libgammatone is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with libgammatone. If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/order.hpp>
#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/detail/utils.hpp>
#include <vector>
#include <iterator>

#include <limits>
using namespace std;

typedef double T;
const T eps = numeric_limits<T>::epsilon();

using namespace gammatone;

using fb_fixed_size =
  filterbank::concrete<T, core::cooke1993,
                       policy::bandwidth::glasberg1990,
                       policy::channels::fixed_size>;

using fb_fixed_overlap =
  filterbank::concrete<T, core::cooke1993,
                       policy::bandwidth::glasberg1990,
                       policy::channels::fixed_overlap>;


BOOST_AUTO_TEST_SUITE(policy_channels)


//================================================

BOOST_AUTO_TEST_CASE(silly_test)
{
  vector<T> v;
  using namespace gammatone::policy::order;
  BOOST_CHECK(increasing::begin(v) == increasing::end(v) );
  BOOST_CHECK(decreasing::begin(v) == decreasing::end(v) );
}


//================================================

BOOST_AUTO_TEST_CASE(iteration_test)
{
  const size_t n = 10;
  const auto v = gammatone::detail::linspace<T>(1.0,2.0,n);

  using namespace gammatone::policy::order;
  auto incr_begin = increasing::begin(v);
  auto incr_end   = increasing::end(v);
  auto decr_begin = decreasing::begin(v);
  auto decr_end   = decreasing::end(v);

  BOOST_CHECK_EQUAL( (int)n, distance(incr_begin,incr_end) );
  BOOST_CHECK_EQUAL( (int)n, distance(decr_begin,decr_end) );

  for(size_t i=0;i<n;i++)
    {
      BOOST_CHECK_EQUAL( *incr_begin++, *--decr_end );
      BOOST_CHECK_EQUAL( *decr_begin++, *--incr_end );
    }
}


//================================================

BOOST_AUTO_TEST_CASE(filterbank_test)
{
  using namespace gammatone;
  using increasing_fb = filterbank::concrete<T,
                                             core::cooke1993,
                                             policy::bandwidth::glasberg1990,
                                             policy::channels::fixed_size>;//,
  //                                             policy::order::increasing>;

  using decreasing_fb = filterbank::concrete<T,
                                             core::cooke1993,
                                             policy::bandwidth::glasberg1990,
                                             policy::channels::fixed_size,
                                             policy::order::decreasing>;

  const T fs = 44100, lf = 500, hf = 8000, nbc = 10;
  increasing_fb fi(fs,lf,hf,nbc);
  decreasing_fb fd(fs,lf,hf,nbc);

  const auto cfi = fi.center_frequency();
  const auto cfd = fd.center_frequency();

  auto i = fi.begin();
  auto d = fd.end(); d--;
  for(; i!=fi.end(); i++, d--)
    BOOST_CHECK_CLOSE(i->center_frequency(),d->center_frequency(), eps);
}


//================================================

BOOST_AUTO_TEST_CASE(ctor_works)
{
  using namespace gammatone::policy::channels;

  using c = fixed_size<T,policy::bandwidth::glasberg1990,policy::order::increasing>;
  fb_fixed_size fb1(44100,1000,5000);
  BOOST_CHECK_EQUAL(c::default_parameter(), fb1.nb_channels());

  using d = fixed_overlap<T,policy::bandwidth::glasberg1990,policy::order::increasing>;
  fb_fixed_overlap fb2(44100,1000,5000);
  BOOST_CHECK_EQUAL(d::default_parameter(), fb2.overlap());

  for(std::size_t n:{1,2,5,10,60,100,1000})
    {
      fb_fixed_size fb(44110,100,5000,n);
      BOOST_CHECK_EQUAL(n,fb.nb_channels());
    }

  for(T o:{0.01,0.1,0.3,0.5,0.7,0.9,0.99})
    {
      fb_fixed_overlap fb(44110,100,5000,o);
      BOOST_CHECK_EQUAL(o,fb.overlap());
    }
}


//================================================

BOOST_AUTO_TEST_CASE(nbc_overlap_works)
{
  for(std::size_t n:{1,2,5,10,60,100,1000})
    {
      fb_fixed_size fb(44100,1000,5000,n);
      fb_fixed_overlap fb2(44100,1000,5000,fb.overlap());
      BOOST_CHECK_EQUAL(fb.nb_channels(),fb2.nb_channels());
      BOOST_CHECK_EQUAL(fb.overlap(),fb2.overlap());      
    }

  for(T o:{0.01,0.1,0.3,0.5,0.7,0.9,0.99})
    {
      fb_fixed_overlap fb(44100,1000,5000,o);
      fb_fixed_size fb2(44100,1000,5000,fb.nb_channels());
      BOOST_CHECK_EQUAL(fb.nb_channels(),fb2.nb_channels());
      // +- 10%. We can't retrieve an exact overlap because nb_channels is interger.
      // To have BOOST_CHECK_EQUAL here require floating nb_channels...
      BOOST_CHECK_CLOSE(fb.overlap(),fb2.overlap(), 10);
    }
}

BOOST_AUTO_TEST_SUITE_END()
