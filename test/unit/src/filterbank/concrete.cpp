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
#include <filterbank_types.h>
#include <test_utils.hpp>
using namespace gammatone;

class fixture
{
protected:
  const T m_sample_frequency = 44100;
  const T m_low = 500, m_high = 8000;
};


BOOST_AUTO_TEST_SUITE(filterbank_concrete_test)


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(accessors_works, F, filterbank_types, fixture)
{
  using namespace gammatone;
  F m_filterbank( this->m_sample_frequency,this->m_low,this->m_high );

  BOOST_CHECK_EQUAL( this->m_sample_frequency, m_filterbank.sample_frequency() );

  size_t i=0;
  for(const auto& f:m_filterbank)
    {
      BOOST_CHECK_GT( f.bandwidth(), 0.0 );
      BOOST_CHECK_GT( f.gain(), 0.0 );

      BOOST_CHECK_EQUAL( m_filterbank.center_frequency()[i], f.center_frequency() );
      BOOST_CHECK_EQUAL( m_filterbank.bandwidth()[i], f.bandwidth() );
      BOOST_CHECK_EQUAL( m_filterbank.gain()[i], f.gain() );
      i++;
    }
}


//================================================

// BOOST_FIXTURE_TEST_CASE_TEMPLATE(center_frequencies_works, F, filterbank_types, fixture)
// {
//   const T m_sample_frequency = 44100;
//   //  const T m_nb_channels = 20;
//   const T m_low = 500, m_high = 8000;

//   using namespace gammatone::filterbank;
//   using namespace gammatone::policy;
//   typedef gammatone::core::cooke1993<T> core;
//   typedef bandwidth::glasberg1990<T> bandwidth;
//   typedef concrete<T,core,bandwidth,channels::fixed_size<T,order::increasing> > increasing;
//   typedef concrete<T,core,bandwidth,channels::fixed_size<T,order::decreasing> > decreasing;

//   increasing fi(m_sample_frequency,m_low,m_high);
//   EXPECT_LE( m_low, fi.begin()->center_frequency() );
//   EXPECT_EQ( m_high, fi.rbegin()->center_frequency() );

//   decreasing fd(m_sample_frequency,m_low,m_high);
//   EXPECT_EQ( m_high, fd.begin()->center_frequency() );
//   EXPECT_LE( m_low, fd.rbegin()->center_frequency() );

//   auto it=fi.begin(); auto it2=fd.rbegin();
//   while(it!=fi.end())
//     EXPECT_EQ(it++->center_frequency(), it2++->center_frequency());
// }

BOOST_AUTO_TEST_SUITE_END()
