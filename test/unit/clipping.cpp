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
#include <gammatone/policy/clipping.hpp>
#include <vector>
#include <algorithm>

typedef double T;
using namespace gammatone::policy::clipping;


BOOST_AUTO_TEST_SUITE(policy_channels)


//================================================

BOOST_AUTO_TEST_CASE(complex_test)
{
  using C = std::complex<T>;

  std::vector<C> r = {-1e-201,1e-200-1, 0, 1e-250,1e-100,1,1e100};
  std::vector<C> a = {      0,1e-200-1, 0,      0,1e-100,1,1e100};
  std::vector<C> c1(r.size()), c2(r.size());

  std::transform(r.begin(),r.end(),a.begin(),c1.begin(),
                 [&](C& x,C& y){return on::clip(x);});

  std::transform(r.begin(),r.end(),c2.begin(),
                 [&](C& x){return off::clip(x);});

  for(std::size_t i=0;i<r.size();i++)
    {
      BOOST_CHECK_EQUAL( c1[i].real(), a[i].real() );
      BOOST_CHECK_EQUAL( c1[i].imag(), a[i].imag() );

      BOOST_CHECK_EQUAL( c2[i].real(), r[i].real() );
      BOOST_CHECK_EQUAL( c2[i].imag(), r[i].imag() );
    }
}


BOOST_AUTO_TEST_SUITE_END()
