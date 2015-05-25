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
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/detail/utils.hpp>
#include <limits>
using namespace std;

typedef double T;
const T eps = numeric_limits<T>::epsilon();


BOOST_AUTO_TEST_SUITE(policy_bandwidth)


//================================================

BOOST_AUTO_TEST_CASE(silly_test)
{
  using namespace gammatone::policy::bandwidth;
  BOOST_CHECK_EQUAL(glasberg1990<T>::order, greenwood1990<T>::order);
  BOOST_CHECK_EQUAL(2, (int)slaney1988<T>::order);

  typedef glasberg1990<T> g;
  BOOST_CHECK_EQUAL(g::minbw*g::bw_correction, g::bandwidth(0));
}


//================================================

BOOST_AUTO_TEST_CASE(glasberg1990_works)
{
  using namespace gammatone::policy::bandwidth;

  // test on a lot of frequencies from 1Hz to 100kHz...  
  auto f = gammatone::detail::linspace<T>(1,100000,10000);
  typedef glasberg1990<T> g;
  
  for_each(f.begin(),f.end(), [&](const auto& x)
           {BOOST_CHECK_CLOSE(g::bandwidth(x),g::bw_correction*24.7*(4.37*x/1000 + 1),1e-2);});
  for_each(f.begin(),f.end(), [&](const auto& x)
           {BOOST_CHECK_CLOSE(g::bandwidth(x),g::bw_correction*24.7*(4.3700009346995*x/1000 + 1),5e-14);});
}

BOOST_AUTO_TEST_SUITE_END()
