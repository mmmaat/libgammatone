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
#include <gammatone/filter/interface.hpp>
#include <gammatone/filterbank/interface.hpp>

#include <test_utils.hpp>
#include <vector>
typedef double T;

class fixture_filter : public gammatone::filter::interface<T>
{
protected:
  T sample_frequency() const {return 0;}
  T center_frequency() const {return 0;};
  T bandwidth() const {return 0;}
  T gain() const {return 0;}
  void reset(){}
private:
  T compute_internal(const T& in){return 2*in;}
};

class fixture_filterbank : public gammatone::filterbank::interface<T>
{
  typedef gammatone::filterbank::interface<T>::output_type U;
protected:
  T sample_frequency() const {return 0;}
  U center_frequency() const {return empty;};
  U bandwidth() const {return empty;}
  U gain() const {return empty;}
  void reset(){}
private:
  U compute_internal(const T& in){return {in, 10*in, 100*in};}
  const U empty;
};


BOOST_AUTO_TEST_SUITE(detail_interface)

//================================================

BOOST_FIXTURE_TEST_CASE(compute_works, fixture_filter)
{
  BOOST_CHECK_EQUAL(2, compute(1));

  std::vector<T> input(1,1);
  BOOST_CHECK_EQUAL(2, compute(input[0]));
  BOOST_CHECK_EQUAL(2, compute(input)[0]);

  std::vector<T> output(input.size());
  compute(input.begin(),input.end(),output.begin());
  BOOST_CHECK_EQUAL(2, output[0]);

  std::vector<T> in = random<T>(-1,1,100);
  // 1st version
  std::vector<T> out1(in.size());
  std::transform(in.begin(),in.end(),out1.begin(),
                 [&](const auto& x){return this->compute(x);});

  // 2nd version
  std::vector<T> out2(in.size());
  compute(in.begin(),in.end(),out2.begin());

  // 3rd version
  std::vector<T> out3 = compute(in);

  BOOST_CHECK(std::equal(out1.begin(),out1.end(),out2.begin()));
  BOOST_CHECK(std::equal(out1.begin(),out1.end(),out3.begin()));
}


//================================================

BOOST_FIXTURE_TEST_CASE(compute_works_on_bank, fixture_filterbank)
{
  std::vector<T> r = compute(1.0);
  BOOST_CHECK_EQUAL(3, r.size());

  std::vector<T> in = random<T>(-1,1,100);
  // 1st version
  std::vector<std::vector<T> > out1(in.size(),std::vector<T>(3));
  std::transform(in.begin(),in.end(),out1.begin(),
                 [&](const auto& x){return this->compute(x);});

  // 2nd version
  std::vector<std::vector<T> > out2(in.size(),std::vector<T>(3));
  compute(in.begin(),in.end(),out2.begin());

  // 3rd version
  auto out3 = compute(in);

  for(std::size_t i=0;i<in.size();i++)
    {
      BOOST_CHECK_EQUAL(3, out1[i].size());
      BOOST_CHECK(std::equal(out1[i].begin(),out1[i].end(),out2[i].begin()));
      BOOST_CHECK(std::equal(out1[i].begin(),out1[i].end(),out3[i].begin()));
    }
}

BOOST_AUTO_TEST_SUITE_END()
