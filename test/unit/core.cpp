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
#include <boost/mpl/list.hpp>
#include <boost/mpl/joint_view.hpp>

#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/policy/gain.hpp>
#include <gammatone/policy/clipping.hpp>

#include <test_utils.hpp>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

using namespace std;
using namespace gammatone;
using T = double;

// enumeration of all possible core types
using a1 = policy::gain::forall_0dB;
using a2 = policy::gain::peroctave_6dB;
using b1 = policy::clipping::off;
using b2 = policy::clipping::on;
template<class... X> using c1 = core::convolution<T,X...>;
template<class... X> using c2 = core::slaney1993<T,X...>;
template<class... X> using c3 = core::cooke1993<T,X...>;
using t1 = boost::mpl::list< c1<a1,b1>, c1<a1,b2>, c1<a2,b1>, c1<a2,b2> >;
using t2 = boost::mpl::list< c2<a1,b1>, c2<a1,b2>, c2<a2,b1>, c2<a2,b2> >;
using t3 = boost::mpl::list< c3<a1,b1>, c3<a1,b2>, c3<a2,b1>, c3<a2,b2>>;
using core_ref = c1<a1,b1>;
using core_types = boost::mpl::joint_view<boost::mpl::joint_view<t1,t2>::type,t3>::type;

const vector<double> in1 = utils::make_sinus(44100,1000,10000);
const vector<double> in2 = utils::make_sinus(44100,500,10000);
const vector<double> in3 = utils::random<double>(-1.0,1.0,10000);
const vector<double> coef({1,2,3,10,0.5,1.05,2.1,30015,1.02e-3});

// return the linear mean over a range
template<class I>
inline double mean(I first,I last)
{
  return accumulate(first,last,0.0)/distance(first,last);
}

BOOST_AUTO_TEST_SUITE(core_test)

//================================================
// Gammatone output is expected to be linear wrt the amplitude of the
// input signal. That's what we are looking to in this test.
// input duration = 10000 samples, but mean computation only on the last
// 1000 (we are sure filter state is stable).

BOOST_AUTO_TEST_CASE_TEMPLATE(linear, C, core_types)
{
  C c(44100,1000,100);

  for(const auto& in : {in1,in2,in3})
    {
      vector<double> out_ref(in.size());
      std::transform(in.begin(),in.end(),out_ref.begin(),
                     [&](double x){return c.compute(x);});
      double m_ref = mean(out_ref.rbegin(),out_ref.rbegin()+1000);
      vector<double> m;

      for(double k:coef)
        {
          c.reset();

          vector<double> ink(in.size());
          std::transform(in.begin(),in.end(),ink.begin(),[&](double x){return k*x;});

          vector<double> out(in.size());
          std::transform(ink.begin(),ink.end(),out.begin(),[&](double x){return c.compute(x);});
	  
          m.push_back(mean(out.rbegin(),out.rbegin()+1000));
        }

      // check mean ratio close to k... (clipping introduces little nonlinearities)
      for(size_t i=0;i<coef.size();i++)
        {
          BOOST_CHECK_CLOSE(coef[i],m[i]/m_ref, 1e-5);
        }
    }
}


// //================================================
// // Check that all cores have same response

// BOOST_AUTO_TEST_CASE_TEMPLATE(identical_cores, C, core_types)
// {
//   core_ref c1(44100,100,100);
//   C c2(44100,100,100);

//   for(const auto& in : {in1,in2,in3})
//     {
//       vector<T> out1(1000);
//       vector<T> out2(1000);
//       std::transform(in.end()-1000,in.end(),out1.begin(),[&](T x){return c1.compute(x);});
//       std::transform(in.end()-1000,in.end(),out2.begin(),[&](T x){return c2.compute(x);});
//       detail::normalize(out1.begin(),out1.end());
//       detail::normalize(out2.begin(),out2.end());
//       for(size_t i=0;i<1000;i++)	
// 	BOOST_CHECK_CLOSE(out1[i],out2[i],1);
//     }
// }

BOOST_AUTO_TEST_SUITE_END()
