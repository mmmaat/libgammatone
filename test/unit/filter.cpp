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

#include <filter_types.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <test_utils.hpp>
#include <iostream>
using namespace gammatone;

template<class Filter>
class fixture
{
protected:
  fixture() : signal(random<double>(-1.0,1.0,1000))
  {
    std::for_each(cf.begin(),cf.end(),[&](const auto& x){filters.push_back(Filter(fs,x));});
  };

  using T = typename Filter::scalar_type;
  
  const T fs = 44100;
  // FIXME cf below 20 don't pass reset_works()... sometimes, not all the times.
  const std::vector<T> cf = {50,100,451.215,2351.2,6842,12504,15478,fs/2};
  std::vector<Filter> filters;
  std::vector<T> signal;
};


BOOST_AUTO_TEST_SUITE(filter_test)


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(fixture_works, F, filter_types<double>, fixture<F>)
{
  BOOST_REQUIRE(this->signal.size() != 0);
}

//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(copy_works, F, filter_types<double>, fixture<F>)
{
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      F f1(f);
      BOOST_CHECK_EQUAL( f1.sample_frequency(), f.sample_frequency() );
      BOOST_CHECK_EQUAL( f1.center_frequency(), f.center_frequency() );
      BOOST_CHECK_EQUAL( f1.bandwidth(), f.bandwidth() );
      BOOST_CHECK_EQUAL( f1.gain(), f.gain() );

      F f2 = f;
      BOOST_CHECK_EQUAL( f2.sample_frequency(), f.sample_frequency() );
      BOOST_CHECK_EQUAL( f2.center_frequency(), f.center_frequency() );
      BOOST_CHECK_EQUAL( f2.bandwidth(), f.bandwidth() );
      BOOST_CHECK_EQUAL( f2.gain(), f.gain() );

      std::vector<typename F::scalar_type> out(x.size());
      auto out1 = out;
      auto out2 = out;
      f.compute(x.begin(),x.end(),out.begin());
      f1.compute(x.begin(),x.end(),out1.begin());
      f2.compute(x.begin(),x.end(),out2.begin());
      
      for(size_t i=0;i<x.size();i++)
        {
          BOOST_CHECK_EQUAL(out[i],out1[i]);
          BOOST_CHECK_EQUAL(out[i],out2[i]);
        }
    }
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(compute_works, F, filter_types<double>, fixture<F>)
{
  using T = typename F::scalar_type;
  
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      // f.reset();
      // std::vector<T> c1 = f.compute(x);

      f.reset();
      std::vector<T> c2(x.size());
      f.compute(x.begin(),x.end(),c2.begin());

      f.reset();
      std::vector<T> c3(x.size());
      std::transform(x.begin(),x.end(),c3.begin(),[&](const T& xx){return f.compute(xx);});

      f.reset();
      std::vector<T> c4(x.size());
      f.compute(x.size(),x.data(),c4.data());
      
      for(std::size_t i=0;i<x.size();i++)
        {
	  //BOOST_CHECK_EQUAL(c1[i],c2[i]);
          BOOST_CHECK_EQUAL(c2[i],c3[i]);
	  BOOST_CHECK_EQUAL(c2[i],c4[i]);
        }
    }
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(reset_works, F, filter_types<double>, fixture<F>)
{
  using T = typename F::scalar_type;
  const auto& x = this->signal;
  std::vector<T> c(x.size());
  std::vector<T> c1(x.size());
  std::vector<T> c2(x.size());
  
  for(auto& f:this->filters)
    {
      f.compute(x.begin(),x.end(),c.begin());
      f.compute(x.begin(),x.end(),c1.begin());

      std::vector<T> d1(x.size());
      std::transform(c.begin(),c.end(),c1.begin(),d1.begin(),
                     [&](const T& x,const T& y){return x-y;});
      bool b1 = std::all_of(d1.begin(),d1.end(),[](const T& x){return x==0;});
      if(b1)
	std::cout << f.center_frequency() << std::endl;
      BOOST_CHECK_EQUAL(false,b1);

      f.reset();
      f.compute(x.begin(),x.end(),c2.begin());
      std::vector<T> d2(x.size());
      std::transform(c.begin(),c.end(),c2.begin(),d2.begin(),
                     [&](const T& x,const T& y){return x-y;});
      bool b2 = std::all_of(d2.begin(),d2.end(),[](const T& x){return x==0;});
      BOOST_CHECK_EQUAL(true,b2);
    }
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(accessors_works, F, filter_types<double>, fixture<F>)
{
  size_t i=0;
  for(auto& f:this->filters)
    {
      BOOST_CHECK_EQUAL( this->fs, f.sample_frequency() );
      BOOST_CHECK_EQUAL( this->cf[i++], f.center_frequency() );
      BOOST_CHECK_GT( f.bandwidth(), 0.0 );
      BOOST_CHECK_GT( f.gain(), 0.0 );
    }
}

BOOST_AUTO_TEST_SUITE_END()
