/*
  Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>

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
#include <gammatone/detail/decibel.hpp>

#include <vector>
#include <algorithm>
using namespace std;
using namespace gammatone::detail;

const vector<double> levels = {10,0,-3,-10,-30,-60};

// TODO This test contains only silly tests. Go deeper.

BOOST_AUTO_TEST_SUITE(detail_decibel)

//================================================

BOOST_AUTO_TEST_CASE(decibel_test)
{
  vector<double> e;
  decibel(e.begin(),e.end());
  BOOST_CHECK_EQUAL( (size_t)0, e.size() );

  vector<double> o = {1};
  decibel(o.begin(),o.end());
  BOOST_CHECK_EQUAL( 0.0, o[0] );
  BOOST_CHECK_EQUAL( (size_t)1, o.size() );
}


//================================================

BOOST_AUTO_TEST_CASE( find_attenuation_test )
{
  const vector<double> empty;
  for(auto l:levels)
    BOOST_CHECK( find_attenuation(empty,l) == empty.end() );

  const vector<double> one = {1};
  for(auto l:levels)
    BOOST_CHECK( find_attenuation(one,l) == one.end() );
}


//================================================

BOOST_AUTO_TEST_CASE( shrink_to_attenuation_test )
{
  const vector<double> empty;
  for(auto l:levels)
    {
      auto e = empty;
      shrink_to_attenuation(e,l);
      BOOST_CHECK_EQUAL((size_t)0, e.size());
    }

  const vector<double> one = {1};
  for(auto l:levels)
    {
      auto e = one;
      shrink_to_attenuation(e,l);
      BOOST_CHECK_EQUAL((size_t)1, e.size());
    }
}

BOOST_AUTO_TEST_SUITE_END()
