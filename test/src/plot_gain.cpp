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
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/detail/utils.hpp>

#include <fstream>
#include <test_setup.hpp>

// filterbank types
typedef double T;
typedef gammatone::filterbank::concrete<T,gammatone::core::cooke1993<T> >   filterbank1;
typedef gammatone::filterbank::concrete<T,gammatone::core::slaney1993<T> >  filterbank2;
typedef gammatone::filterbank::concrete<T,gammatone::core::convolution<T> > filterbank3;


// filterbank parameters
const T fs = 44100;           // Hz
const T lf = 100, hf = 20000; // Hz
const T duration = 0.01;      // s


template<class T> void plot(const T& t1, const T& t2, const T& t3)
{
  Gnuplot gp;
  gp << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set xlabel 'frequency (Hz)'" << std::endl
     << "set ylabel 'normalized gain (dB)'" << std::endl
     << "set key bottom right" << std::endl
     << "plot '-' u 1:2 w l ls 11 t '"<<std::get<2>(t1)<<"', "
     << "     '-' u 1:2 w l ls 12 t '"<<std::get<2>(t2)<<"', "
     << "     '-' u 1:2 w l ls 13 t '"<<std::get<2>(t3)<<"', "<< std::endl;
  gp.send1d(std::make_pair(std::get<0>(t1),std::get<1>(t1)));
  gp.send1d(std::make_pair(std::get<0>(t2),std::get<1>(t2)));
  gp.send1d(std::make_pair(std::get<0>(t3),std::get<1>(t3)));
}

int main()
{
  // gain of each filterbank type
  const auto fb1 = filterbank1(fs,lf,hf);
  const auto f1 = fb1.center_frequency();
  auto g1 = fb1.gain();
  gammatone::detail::decibel(g1.begin(), g1.end());
  
  const auto fb2 = filterbank2(fs,lf,hf);
  const auto f2 = fb2.center_frequency();
  auto g2 = fb2.gain();
  gammatone::detail::decibel(g2.begin(), g2.end());
  
  const auto fb3 = filterbank3(fs,lf,hf);
  const auto f3 = fb3.center_frequency();
  auto g3 = fb3.gain();
  gammatone::detail::decibel(g3.begin(), g3.end());
  
  plot(make_tuple(f1,g1, "cooke"),
       make_tuple(f2,g2, "slaney"),
       make_tuple(f3,g3, "convolution"));

  return 0;
}
