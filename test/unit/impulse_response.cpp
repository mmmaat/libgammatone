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
#include <boost/test/test_case_template.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <gammatone/detail/impulse_response.hpp>
#include <filter_types.h>
#include <iostream>

template<class Filter>
class fixture2
{
protected:
  fixture2():filter(fs,fc){}
  ~fixture2(){}

  using T = typename Filter::scalar_type;
  
  const T fs = 44100;
  const T fc = 3000;
  const T duration = 0.01;
  const T attenuation = -60;
  Filter filter;
};

using ir = gammatone::detail::impulse_response;
using gammatone::detail::absmax;


BOOST_AUTO_TEST_SUITE(detail_impulse_response)


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(time_works, F, filter_types<double>, fixture2<F>)
{
  const auto time = ir::time(this->filter.sample_frequency(), this->duration);
  const auto mm = std::minmax_element(time.begin(),time.end());

  // increasing order
  BOOST_CHECK( std::adjacent_find(time.begin(), time.end(), std::greater<int>()) == time.end());

  // size, min and max
  BOOST_CHECK_EQUAL( time.size(), this->fs*this->duration+1);
  BOOST_CHECK_EQUAL( *mm.first, 0.0);
  BOOST_CHECK_EQUAL( *time.begin(), 0.0);
  BOOST_CHECK_EQUAL( *mm.second, this->duration);
  BOOST_CHECK_EQUAL( *time.rbegin(), this->duration);
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(theorical_works, F, filter_types<double>, fixture2<F>)
{
  const auto ir = ir::theorical(this->filter,this->duration);

  BOOST_CHECK_NE( ir.size() , 0);
  BOOST_CHECK_NE( absmax(ir.cbegin(),ir.cend()), 0.0);
  BOOST_CHECK_LE( *std::min_element(ir.cbegin(),ir.cend()), 0.0);
  BOOST_CHECK_GE( *std::max_element(ir.cbegin(),ir.cend()), 0.0);
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(attenuation_works, F, filter_types<double>, fixture2<F>)
{
  const auto ir = ir::theorical_attenuate(this->filter.center_frequency(),
                                          this->filter.bandwidth(),
                                          this->filter.sample_frequency(),
                                          this->attenuation);

  BOOST_CHECK_NE( ir.size() , 0);
  BOOST_CHECK_NE( absmax(ir.cbegin(),ir.cend()), 0.0);
  BOOST_CHECK_LE( *std::min_element(ir.cbegin(),ir.cend()), 0.0);
  BOOST_CHECK_GE( *std::max_element(ir.cbegin(),ir.cend()), 0.0);
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(max_duration_works, F, filter_types<double>, fixture2<F>)
{
  // test 1s
  std::size_t size = 44100+1;
  auto ir1 = ir::theorical_attenuate<typename F::scalar_type>(5000,200,44100,1);
  BOOST_CHECK_EQUAL(size, ir1.size());

  // test 2s
  size = 2*44100+1;
  ir1 = ir::theorical_attenuate<typename F::scalar_type>(5000,200,44100,1,2);
  BOOST_CHECK_EQUAL(size, ir1.size());
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(implemented_works, F, filter_types<double>, fixture2<F>)
{
  const auto ir1 = ir::implemented(this->filter,this->duration);

  BOOST_CHECK_NE( (int)ir1.size() , 0);
  BOOST_CHECK_NE( absmax(ir1.cbegin(),ir1.cend()), 0.0);
  BOOST_CHECK_LE( *std::min_element(ir1.cbegin(),ir1.cend()), 0.0);
  BOOST_CHECK_GE( *std::max_element(ir1.cbegin(),ir1.cend()), 0.0);
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(polymorphism_works, F, filter_types<double>, fixture2<F>)
{
  const auto t  = ir::time(this->filter.sample_frequency(), this->duration);
  const auto it = ir::theorical(this->filter, t.begin(), t.end());
  const auto pt = ir::theorical(this->filter,this->duration);
  const auto et = ir::theorical(this->fc,
                                this->filter.bandwidth(),
                                this->fs, this->duration);

  const auto pi = ir::implemented(this->filter,this->duration);
  const auto ii = ir::implemented(this->filter, t.begin(), t.end());

  BOOST_CHECK_EQUAL(t.size(), ii.size());
  BOOST_CHECK_EQUAL(t.size(), pi.size());

  BOOST_CHECK_EQUAL(t.size(), it.size());
  BOOST_CHECK_EQUAL(t.size(), pt.size());
  BOOST_CHECK_EQUAL(t.size(), et.size());

  BOOST_CHECK_NE(0, t.size());

  for(std::size_t n=0; n<t.size();n++)
  {
    BOOST_CHECK_EQUAL(it[n], pt[n]);
    BOOST_CHECK_EQUAL(it[n], et[n]);
    BOOST_CHECK_EQUAL(ii[n], pi[n]);
  }
}

BOOST_AUTO_TEST_SUITE_END()
