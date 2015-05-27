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
#include <gammatone/policy/channels.hpp>
#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/detail/utils.hpp>
#include <vector>
#include <iterator>

#include <limits>
using namespace std;

typedef double T;
const T eps = numeric_limits<T>::epsilon();


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
  using increasing_fb = gammatone::filterbank::concrete<T,
                                                        gammatone::core::cooke1993,
                                                        gammatone::policy::bandwidth::glasberg1990,
                                                        gammatone::policy::channels::fixed_size,
                                                        gammatone::policy::order::increasing>;

  using decreasing_fb = gammatone::filterbank::concrete<T,
                                                        gammatone::core::cooke1993,
                                                        gammatone::policy::bandwidth::glasberg1990,
                                                        gammatone::policy::channels::fixed_size,
                                                        gammatone::policy::order::decreasing>;

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

BOOST_AUTO_TEST_SUITE_END()
