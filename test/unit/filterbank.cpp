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
#include <boost/test/test_case_template.hpp>
#include <filterbank_types.h>
#include <test_utils.hpp>
using namespace gammatone;

template<class Filterbank>
class fixture
{
  using T = typename Filterbank::scalar_type;
protected:
  const T m_sample_frequency = 44100;
  const T m_low = 500, m_high = 8000;
};


BOOST_AUTO_TEST_SUITE(filterbank_concrete_test)


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(accessors_works, F, filterbank_types<double>, fixture<F>)
{
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

BOOST_AUTO_TEST_CASE(center_frequencies_works)
{
  using T = double;
  using namespace gammatone;
  using namespace gammatone::policy;

  const T m_sample_frequency = 44100;
  const T m_low = 500, m_high = 8000;

  filterbank<T, gammatone::core::cooke1993, channels::fixed_size> fi(
      m_sample_frequency, m_low, m_high);

  BOOST_CHECK_LE( m_low, fi.begin()->center_frequency() );
  BOOST_CHECK_EQUAL( m_high, fi.rbegin()->center_frequency() );
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(compute_works, F, filterbank_types<double>, fixture<F>)
{
    // this is double
    using T = typename F::scalar_type;

    // random input signal
    const auto x = utils::random<T>(-1.0, 1.0, 1000);

    // initialize filterbank
    F f(this->m_sample_frequency, this->m_low, this->m_high);

    const auto xsize = x.size();
    const auto ysize = f.nb_channels();

    f.reset();
    std::vector<std::vector<T> > c1;
    std::for_each(x.begin(), x.end(),
                  [&](const T& xx){c1.push_back(f.compute_allocate(xx));});

    f.reset();
    std::vector<std::vector<T> > c2(xsize,std::vector<double>(ysize));
    f.compute_range(x.begin(), x.end(), c2.begin());

    f.reset();
    std::vector<std::vector<T> > c3(xsize,std::vector<double>(ysize));
    auto out = c3.begin();
    std::for_each(x.begin(), x.end(),
                  [&](const T& xx){f.compute(xx, *out++);});

    f.reset();
    std::vector<T> c4(xsize*ysize);
    f.compute_ptr(xsize, x.data(), c4.data());

    for(std::size_t i=0; i < xsize; i++)
        for(std::size_t j=0; j < ysize; j++)
        {
            BOOST_CHECK_EQUAL(c1[i][j],c2[i][j]);
            BOOST_CHECK_EQUAL(c2[i][j], c3[i][j]);
            BOOST_CHECK_EQUAL(c2[i][j], c4[i*ysize+j]);
        }
}

BOOST_AUTO_TEST_SUITE_END()
