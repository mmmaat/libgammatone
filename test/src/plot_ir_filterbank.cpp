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

#include <gammatone/filterbank/concrete.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/detail/impulse_response.hpp>

#include <test_setup.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

using T = double;
template<class... X> using a = gammatone::core::convolution<X...>;
template<class X>    using b = gammatone::policy::bandwidth::glasberg1990<X>;
template<class... X> using c = gammatone::policy::channels::fixed_size<X...>;
using d = gammatone::policy::order::increasing;
using e = gammatone::policy::gain::forall_0dB;
using filterbank = gammatone::filterbank::concrete<T,a,b,gammatone::policy::channels::fixed_size,d,e>;

const T duration = 0.02;
const T sample_frequency = 44100;
const size_t nb_channels = 5;
const T low_cf = 500, high_cf = 8000;

int main(int argc, char** argv)
{
  filterbank bank(sample_frequency, low_cf, high_cf, nb_channels);
  
  auto t = gammatone::impulse_response::time(bank.begin()->sample_frequency(), duration);

  vector<vector<T> > ir(bank.nb_channels());
  transform(bank.begin(),bank.end(),ir.begin(),
   	    [&](auto& x){return gammatone::impulse_response::implemented(x, t.begin(),t.end());});
 
  // initialize gnuplot
  Gnuplot gp;
  gp << ifstream(gpsetup).rdbuf() << endl
       << "set xlabel 'time (s)'" << endl
       << "set ylabel 'amplitude'" << endl;

  // generate gnuplot command
  stringstream s; s << "plot ";
  for(size_t i=0;i<bank.nb_channels();i++)
    s << "'-' u 1:2 w l ls "<< 11+i <<" t '" << static_cast<int>(bank.center_frequency()[i]) << " Hz', ";
  string cmd = s.str();
  cmd = cmd.substr(0,cmd.size()-2);

  // sending command and data to gnuplot
  gp << cmd << endl;
  for(size_t i=0;i<bank.nb_channels();i++)
    gp.send1d(make_pair(t,ir[i]));

   //  copy(t.begin(),t.end(),ostream_iterator<float>(cout, "\n" ));
  
  return 0;
}
