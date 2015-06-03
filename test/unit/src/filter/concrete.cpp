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
#include <iostream>


#ifdef LIBGAMMATONE_TEST_ALL
#include <filter_types_all.h>
#else
#include <filter_types.h>
#endif


using namespace gammatone;

template<class Filter>
class fixture
{
protected:
  fixture() : signal(random<T>(-1.0,1.0,1000))
  {
    std::for_each(cf.begin(),cf.end(),[&](const auto& x){filters.push_back(Filter(fs,x));});
  };

  bool is_same_filter(Filter& f1, Filter& f2) const
  {
    f1.reset(); f2.reset();
    for(const auto& x:signal)
      {
	const auto o1 = f1.compute(x);
	const auto o2 = f2.compute(x);
	if(o1 != o2) return false;
      }
    return true;
  }

  const T fs = 44100;
  // cf below 20 don't pass reset_works()... sometimes, not all the times.
  const std::vector<T> cf = {50,100,451.215,2351.2,6842,12504,15478,fs/2};
  const std::vector<T> signal;
  std::vector<Filter> filters;

};



BOOST_AUTO_TEST_SUITE(filter_concrete_test)


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(copy_op_works, F, filter_types, fixture<F>)
{
  const T fc = 2000;
  F f1(44100,fc);
  F f2(44100*1.6572,2.054*fc);
  f2 = f1;
  BOOST_CHECK(this->is_same_filter(f1,f2));
}

//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(copy_ctor_works, F, filter_types, fixture<F>)
{
  const T fc = 2000;
  F f1(44100,fc);
  F f2(f1);
  BOOST_CHECK(this->is_same_filter(f1,f2));
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(move_op_works, F, filter_types, fixture<F>)
{
  const T fc = 2000;
  F f1(44100,fc);
  F f(44100,fc);
  F f2(44100*1.6572,2.054*fc);
  f2 = std::move(f);
  BOOST_CHECK(this->is_same_filter(f1,f2));
}

//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(move_ctor_works, F, filter_types, fixture<F>)
{
  const T fc = 2000;
  F f1(44100,fc);
  F f(44100,fc);
  F f2(std::move(f));
  BOOST_CHECK(this->is_same_filter(f1,f2));
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(compute_works, F, filter_types, fixture<F>)
{
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      f.reset();
      std::vector<T> c1 = f.compute(x);

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
          BOOST_CHECK_EQUAL(c1[i],c2[i]);
          BOOST_CHECK_EQUAL(c1[i],c3[i]);
	  BOOST_CHECK_EQUAL(c1[i],c4[i]);
        }
    }
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(reset_works, F, filter_types, fixture<F>)
{
  const auto& x = this->signal;
  for(auto& f:this->filters)
    {
      auto c = f.compute(x);

      auto c1 = f.compute(x);
      std::vector<T> d1(x.size());
      std::transform(c.begin(),c.end(),c1.begin(),d1.begin(),
                     [&](const T& x,const T& y){return x-y;});
      bool b1 = std::all_of(d1.begin(),d1.end(),[](const T& x){return x==0;});
      if(b1)
	std::cout << f.center_frequency() << std::endl;
      BOOST_CHECK_EQUAL(false,b1);

      f.reset();
      auto c2 = f.compute(x);
      std::vector<T> d2(x.size());
      std::transform(c.begin(),c.end(),c2.begin(),d2.begin(),
                     [&](const T& x,const T& y){return x-y;});
      bool b2 = std::all_of(d2.begin(),d2.end(),[](const T& x){return x==0;});
      BOOST_CHECK_EQUAL(true,b2);
    }
}


//================================================

BOOST_FIXTURE_TEST_CASE_TEMPLATE(accessors_works, F, filter_types, fixture<F>)
{
  size_t i=0;
  for(const auto& f:this->filters)
    {
      BOOST_CHECK_EQUAL( this->fs, f.sample_frequency() );
      BOOST_CHECK_EQUAL( this->cf[i++], f.center_frequency() );
      BOOST_CHECK_GT( f.bandwidth(), 0.0 );
      BOOST_CHECK_GT( f.gain(), 0.0 );
    }
}

BOOST_AUTO_TEST_SUITE_END()
