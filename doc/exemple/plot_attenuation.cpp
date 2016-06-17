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

#include <gammatone/filter.hpp>
#include <gammatone/detail/impulse_response.hpp>

#include <test_setup.hpp>
#include <iostream>
#include <limits>

using ir = gammatone::detail::impulse_response;

template<class Filter, class Container, class IRBase>
void plot(const Filter& f, const Container& ref, const IRBase& base, const Container& db)
{
  Gnuplot gp;//(stdout);
  gp << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set xlabel 'time (s)'" << std::endl
     << "set ylabel 'amplitude'" << std::endl
     << "plot ";

  const double fs = f.sample_frequency();
  const double duration = (ref.size()-1)/fs;

  // plot reference IR
  gp << "'-' u 1:2 w l ls 11 t 'reference', ";

  // plot with attenuation
  for(std::size_t i = 0; i < base.size()-1; i++)
    gp << "'-' u 1:2 w l ls " << 12+i << " t '" << (int)db[i] << " dB', ";
  gp << "'-' u 1:2 w l ls " << 12+base.size() << " t '" << (int)*db.rbegin() << " dB'" << std::endl;

  // send data to gnuplot
  gp.send1d(std::make_pair(ir::time(fs,duration), ref));
  for(const auto& ir : base)
  gp.send1d(std::make_pair(ir::time(fs,(ir.size()-1)/fs), ir));
}


int main()
{
  // the gammatone filter
  const gammatone::filter<double> f(44100, 1000);

  // the reference impulse response (0.1 second long)
  const double max_duration = 0.1;
  const std::vector<double> ir_ref = ir::theorical(f,max_duration);

  // IR for different attenuation levels (in db)
  std::vector<double> db = {-200,-100,-60,-30,-20,-10,-6,-3};
  std::vector<std::vector<double> > ir_db;
  for(auto level : db) ir_db.push_back(ir::theorical_attenuate(f,level,max_duration));

  plot(f,ir_ref,ir_db,db);

  return 0;
}
