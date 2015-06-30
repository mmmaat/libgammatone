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
#include <gammatone/detail/utils.hpp>

#include <test_utils.hpp>
#include <boost/math/constants/constants.hpp>
#include <limits>

typedef double T;
const T pi = boost::math::constants::pi<T>();
const T eps = std::numeric_limits<T>::epsilon();

using namespace gammatone::detail;

BOOST_AUTO_TEST_SUITE(detail_utils)


//================================================

BOOST_AUTO_TEST_CASE(linspace_test)
{
  // 0 - 0
  auto r = linspace<T>(0,0);
  BOOST_CHECK_EQUAL(100,r.size());
  r = linspace<T>(0,0,1);
  BOOST_CHECK_EQUAL(1,r.size());

  // 0 - 1
  r = linspace<T>(0,1);
  BOOST_CHECK_EQUAL(100,r.size());
  BOOST_CHECK_EQUAL(0,*r.begin());
  BOOST_CHECK_CLOSE(1,*r.rbegin(),eps);

  // 1 - 0
  r = linspace<T>(1,0);
  BOOST_CHECK_EQUAL(100,r.size());
  BOOST_CHECK_EQUAL(1,*r.begin());
  BOOST_CHECK_CLOSE(0,*r.rbegin(),eps);

  // 0 - 2pi - 5
  r = linspace<T>(0,2*pi,5);
  for(std::size_t i=0; i<5; i++)
    BOOST_CHECK_EQUAL(i*pi/2, r[i]);

  // 0 - 2pi - 10
  r = linspace<T>(0,2*pi,10);
  BOOST_CHECK_EQUAL(10,r.size());
  BOOST_CHECK_EQUAL(0,*r.begin());
  BOOST_CHECK_EQUAL(2*pi,*r.rbegin());

  // -9.1 - -3.215 - 7
  r = linspace<T>(-9.1,-3.215,7);
  BOOST_CHECK_EQUAL(7,r.size());
  BOOST_CHECK_EQUAL(-9.1,*r.begin());
  BOOST_CHECK_EQUAL(-3.215,*r.rbegin());
}


//================================================

BOOST_AUTO_TEST_CASE(absmax_test)
{
  // empty range return 0
  std::vector<double> v;
  BOOST_CHECK_EQUAL(0.0, absmax(v.begin(), v.end()));

  // x range return |x|
  for(auto x:std::vector<double>({-2,-1,0,5.12,4.01,0.0001}))
    {
      v.assign(10,x);
      BOOST_CHECK_EQUAL(std::abs(x), absmax(v.begin(), v.end()));
    }

  // sinus works
  auto sinus = utils::make_sinus(44100,1000);
  auto max = absmax(sinus.begin(),sinus.end());
  BOOST_CHECK_LT(0.999, max);
  BOOST_CHECK_GE(1, max);
}


BOOST_AUTO_TEST_SUITE_END()
