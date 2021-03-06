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

#include <gammatone/gammatone.hpp>
#include <gnuplot-iostream.h>
#include <fstream>
#include <string>
#include <sstream>
#include <test_setup.hpp>


typedef double T;
using namespace gammatone;

// core types
template<class... X> using a1 = core::convolution<X...>;
template<class... X> using a2 = core::cooke1993<X...>;
template<class... X> using a3 = core::slaney1993<X...>;

// channel policy
template<class X, template<class> class Y> using b = policy::channels::fixed_size<X,Y>;

// gain types
using g1 = policy::gain::forall_0dB;
using g2 = policy::gain::peroctave_6dB;


// filterbank types
using filterbank11 = filterbank<T,a1,b,g1>;
using filterbank21 = filterbank<T,a2,b,g1>;
using filterbank31 = filterbank<T,a3,b,g1>;

using filterbank12 = filterbank<T,a1,b,g2>;
using filterbank22 = filterbank<T,a2,b,g2>;
using filterbank32 = filterbank<T,a3,b,g2>;


// filterbank parameters
const T fs = 44100;           // Hz
const T lf = 1000, hf = 5000; // Hz

// for plotting
template<class DataType>
void plot(std::vector<T> xaxis,
          std::vector<DataType> data,
          const std::string ylabel,
          const std::string xlabel = "frequency (Hz)")
{
  Gnuplot gp;
  gp << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set xlabel '"<<xlabel<<"'" << std::endl
     << "set ylabel '"<<ylabel<<"'" << std::endl
     << "set key top right" << std::endl
     << "plot ";

  std::stringstream cmd;
  unsigned color = 0;
  for(const auto& d:data)
    cmd << "'-' u 1:2 w l ls "<<11+color++<<" t '"<<std::get<1>(d)<<"', ";
  gp << cmd.str().substr(0,cmd.str().size()-2) << std::endl; // rm the last 2 characters (= ' ,')

  for(const auto& d:data)
    gp.send1d(std::make_pair(xaxis,std::get<0>(d)));
}

template<class Filter>
void check(const T& sample_frequency,
	   const T& frequency,
	   const std::string& name)
{
  Filter f1(sample_frequency,   frequency);
  Filter f2(sample_frequency, 2*frequency);
  std::cout << name << ": " << f1.gain() << " " << f2.gain() << std::endl;
}

int main()
{
  const auto fc = filterbank11(fs,lf,hf).center_frequency();
  typedef std::tuple<std::vector<T>,const std::string> DataType;

  //////////////////////////
  //   Gain
  //////////////////////////

  // gain of each filterbank type
  std::vector<DataType> data_base;
  data_base.push_back(make_tuple(filterbank11(fs,lf,hf).gain(), "convolution 0dB"));
  data_base.push_back(make_tuple(filterbank12(fs,lf,hf).gain(), "convolution 6dB"));
  data_base.push_back(make_tuple(filterbank31(fs,lf,hf).gain(), "cooke 0dB"));
  data_base.push_back(make_tuple(filterbank32(fs,lf,hf).gain(), "cooke 6dB"));
  data_base.push_back(make_tuple(filterbank21(fs,lf,hf).gain(), "slaney 0dB"));
  data_base.push_back(make_tuple(filterbank22(fs,lf,hf).gain(), "slaney 6dB"));

  // direct gain
  plot(fc, data_base, "gain");

  // // normalized gain
  // for(auto& data:data_base)
  //   detail::decibel(std::get<0>(data).begin(),std::get<0>(data).end());
  // plot(fc, data_base, "normalized gain (dB)");


  //////////////////////////
  //   Impulse responses
  //////////////////////////

  // // 3000 Hz IR
  // using ir = detail::impulse_response;
  // const T f = 3000;
  // const auto t = ir::time(fs,0.01);
  // std::vector<DataType> ir_base;
  // ir_base.push_back(make_tuple(ir::implemented(filterbank11::filter(fs,f),t.begin(),t.end()), "convolution 0dB"));
  // ir_base.push_back(make_tuple(ir::implemented(filterbank12::filter(fs,f),t.begin(),t.end()), "convolution 6dB"));
  // ir_base.push_back(make_tuple(ir::implemented(filterbank31::filter(fs,f),t.begin(),t.end()), "cooke 0dB"));
  // ir_base.push_back(make_tuple(ir::implemented(filterbank32::filter(fs,f),t.begin(),t.end()), "cooke 6dB"));
  // ir_base.push_back(make_tuple(ir::implemented(filterbank21::filter(fs,f),t.begin(),t.end()), "slaney 0dB"));
  // ir_base.push_back(make_tuple(ir::implemented(filterbank22::filter(fs,f),t.begin(),t.end()), "slaney 6dB"));

  //  plot(t, ir_base, "amplitude","time (s)");

  // // normalized IR
  // for(auto& ir:ir_base)
  //   detail::normalize(std::get<0>(ir).begin(),std::get<0>(ir).end());
  // plot(t, ir_base, "normalized amplitude","time (s)");


  //////////////////////////
  //   Check octave
  //////////////////////////

  // std::vector<T> frequencies({500,1000,2000,3000,5000});
  // for(const auto& f : frequencies)
  //   {
  //     check<filter11>(fs,f,"convolution 0dB");
  //     check<filter12>(fs,f,"convolution 6dB");
  //     check<filter21>(fs,f,"slaney 0dB");
  //     check<filter22>(fs,f,"slaney 6dB");
  //     check<filter31>(fs,f,"cooke 0dB");
  //     check<filter32>(fs,f,"cooke 6dB");
  //   }

  return 0;
}
