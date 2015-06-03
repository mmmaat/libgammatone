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
#include <test_utils.hpp>


#ifdef LIBGAMMATONE_TEST_ALL
#include <filterbank_types_all.h>
#else
#include <filterbank_types.h>
#endif


template<class F>
class fixture_fb
{
protected:
  fixture_fb()
    : signal(random<T>(-1.0,1.0,5000)),
      fb(fs,fl,fh)
  {}
  
  const T fs = 44100, fl = 500, fh = 8000;
  
  std::vector<T> signal;
  F fb;
  
  bool is_same_filterbank(F& f1, F& f2) const
  {
    f1.reset(); f2.reset();
    for(const auto& x:signal)
      {
	const auto o1 = f1.compute(x);
	const auto o2 = f2.compute(x);
	if(!std::equal(o1.begin(),o1.end(),o2.begin())) return false;
      }
    return true;
  }
};


BOOST_AUTO_TEST_SUITE(filterbank_concrete_test)


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(copy_op_works, F, filterbank_types, fixture_fb<F>)
{
  F fb2(44100*1.6572,54,8120);
  fb2 = this->fb;
  BOOST_CHECK(this->is_same_filterbank(this->fb,fb2));
}

//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(copy_ctor_works, F, filterbank_types, fixture_fb<F>)
{
  F fb2(this->fb);
  BOOST_CHECK(this->is_same_filterbank(this->fb,fb2));
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(move_op_works, F, filterbank_types, fixture_fb<F>)
{
  F tmp(this->fb);
  F f2(44100*1.6572,2.054*1000,6847.12);
  f2 = std::move(tmp);
  BOOST_CHECK(this->is_same_filterbank(this->fb,f2));
}

//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(move_ctor_works, F, filterbank_types, fixture_fb<F>)
{  
  F tmp(this->fb);
  F fb2(std::move(tmp));
  BOOST_CHECK(this->is_same_filterbank(this->fb,fb2));
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(accessors_works, F, filterbank_types, fixture_fb<F>)
{
  BOOST_CHECK_EQUAL( this->fs, this->fb.sample_frequency() );

  size_t i=0;
  for(const auto& f:this->fb)
    {
      BOOST_CHECK_GT( f.bandwidth(), 0.0 );
      BOOST_CHECK_GT( f.gain(), 0.0 );

      BOOST_CHECK_EQUAL( this->fb.center_frequency()[i], f.center_frequency() );
      BOOST_CHECK_EQUAL( this->fb.bandwidth()[i], f.bandwidth() );
      BOOST_CHECK_EQUAL( this->fb.gain()[i], f.gain() );
      i++;
    }
}

//================================================

BOOST_AUTO_TEST_CASE(center_frequencies_works)
{
  const T m_sample_frequency = 44100;
  //  const T m_nb_channels = 20;
  const T m_low = 500, m_high = 8000;

  using namespace gammatone::filterbank;
  using namespace gammatone::policy;
  using decreasing = concrete<T, gammatone::core::cooke1993,
                              bandwidth::glasberg1990,
                              channels::fixed_size,
                              order::decreasing>;

  using increasing = concrete<T, gammatone::core::cooke1993,
                              bandwidth::glasberg1990,
                              channels::fixed_size,
                              order::increasing>;

  increasing fi(m_sample_frequency,m_low,m_high);
  BOOST_CHECK_LE( m_low, fi.begin()->center_frequency() );
  BOOST_CHECK_EQUAL( m_high, fi.rbegin()->center_frequency() );

  decreasing fd(m_sample_frequency,m_low,m_high);
  BOOST_CHECK_EQUAL( m_high, fd.begin()->center_frequency() );
  BOOST_CHECK_LE( m_low, fd.rbegin()->center_frequency() );

  auto it=fi.begin(); auto it2=fd.rbegin();
  while(it!=fi.end())
    BOOST_CHECK_EQUAL(it++->center_frequency(), it2++->center_frequency());
}



//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(compute_works, F, filterbank_types, fixture_fb<F>)
{
  const auto& x = this->signal;
  auto& f = this->fb;
  
  const auto xsize = x.size();
  const auto ysize = f.nb_channels();
  
  f.reset();
  std::vector<std::vector<T> > c1 = f.compute(x);
  
  f.reset();
  std::vector<std::vector<T> > c2(xsize,std::vector<double>(ysize));
  f.compute(x.begin(),x.end(),c2.begin());
  
  f.reset();
  std::vector<std::vector<T> > c3(xsize,std::vector<double>(ysize));
  std::transform(x.begin(),x.end(),c3.begin(),[&](const T& xx){return f.compute(xx);});
  
  f.reset();
  std::vector<T> c4(xsize*ysize);
  f.compute(xsize,ysize,x.data(),c4.data());
  
  for(std::size_t i=0;i<xsize;i++)
    for(std::size_t j=0;j<ysize;j++)
    {
      BOOST_CHECK_EQUAL(c1[i][j],c2[i][j]);
      BOOST_CHECK_EQUAL(c1[i][j],c3[i][j]);
      BOOST_CHECK_EQUAL(c1[i][j],c4[i*ysize+j]);
    }
}


BOOST_AUTO_TEST_SUITE_END()
