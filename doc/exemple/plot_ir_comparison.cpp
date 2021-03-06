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

// Comparison of the different impulse response avalaible in
// libgammatone. Compare both theorical and implemented cores IRs.

#include <gammatone/detail/impulse_response.hpp>
#include <gammatone/filter.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/detail/utils.hpp>

#include <test_setup.hpp>
#include <iostream>
#include <vector>
#include <array>

using namespace std;
typedef double T;
typedef gammatone::filter<T,gammatone::core::cooke1993 >   filter1;
typedef gammatone::filter<T,gammatone::core::slaney1993 >  filter2;
typedef gammatone::filter<T,gammatone::core::convolution > filter3;

const T sample_frequency = 44100; // Hz
const T center_frequency = 1000;  // Hz
const T duration = 0.01;           // s


template<class F, class C>
inline string tostring(const F& f, const C& ir)
{
  stringstream s;
  s << " cf = " << f.center_frequency()
    << " bw = " << f.bandwidth()
    << " g = " << f.gain()
    << " max = " << gammatone::detail::absmax(ir.begin(), ir.end());
  return s.str();
}

template<class T>
inline vector<pair<T,string> > compute_mse( const vector<pair<vector<T>,string> >& ir_data, bool cout_flag = false)
{
  vector<pair<T,string> > out;
  for(auto it=ir_data.begin();it!=ir_data.end();it++)
    for(auto it2=it+1;it2!=ir_data.end();it2++)
      {
        T mse = mean_squared_error(it->first.begin(),it->first.end(),it2->first.begin());
        string name(it->second+" vs. "+it2->second);
        out.push_back(make_pair(mse, name));
      }

  if(cout_flag)
  for_each(out.begin(),out.end(),[&](const typename decltype(out)::value_type& p)
	   {cout << p.second << " : " << p.first << endl;});

  return out;
}


int main(int argc, char** argv)
{
  // creation of Ma and Flax gammatone filters
  filter1 f1(sample_frequency,center_frequency);
  filter2 f2(sample_frequency,center_frequency);
  filter3 f3(sample_frequency,center_frequency);

  // impulse responses computation
  using ir = gammatone::detail::impulse_response;

  const auto t = ir::time(f1.sample_frequency(),duration);

  vector<pair<vector<T>,string> > ir_data;
  ir_data.push_back(make_pair(ir::theorical(  f1,t.begin(),t.end()), "theorical"));
  ir_data.push_back(make_pair(ir::implemented(f1,t.begin(),t.end()), "ma"));
  ir_data.push_back(make_pair(ir::implemented(f2,t.begin(),t.end()), "flax"));
  ir_data.push_back(make_pair(ir::implemented(f3,t.begin(),t.end()), "convolution"));

  cout << "  ma : " << tostring(f1, ir_data[1].first) << endl;
  cout << "flax : " << tostring(f2, ir_data[2].first) << endl;
  cout << "conv : " << tostring(f3, ir_data[3].first) << endl;
  cout << "                             "
       << "Theorical max = " << gammatone::detail::absmax(ir_data[0].first.begin(),ir_data[0].first.end()) << endl;


  // IR normalization
  bool norm = true;
  if(norm) for(auto& d:ir_data) gammatone::detail::normalize(d.first.begin(),d.first.end());

  // Compute MSE on normalized data
  //const auto mse = compute_mse(ir_data, true);

  // generate gnuplot command
  stringstream cmd;
  cmd << std::ifstream(gpsetup).rdbuf() << std::endl
      << "set xlabel 'time (s)'" << std::endl
      << "set ylabel 'amplitude'" << std::endl
      << "set xrange [0:"<<duration<<"]" << std::endl
      << "plot ";

  size_t i = 0;
  for(const auto& d:ir_data)
    {
      cmd << "'-' u 1:2 with line ls "<< 11+i << " lw " << 3-i << " title '" << d.second << "', ";
      i++;
    }

  // send gnuplot data
  Gnuplot gp;
  gp << cmd.str().substr(0,cmd.str().size()-2) << std::endl; // rm the last 2 characters (= ' ,')
  for(const auto& d:ir_data) gp.send1d(make_pair(t,d.first));

  return 0;
}
