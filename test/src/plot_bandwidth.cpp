/*!
  \file   plot_bandwidth.hpp

  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <gammatone/policy/bandwidth.hpp>
#include <utils.hpp>
#include <gnuplot-iostream.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>

typedef double Scalar;

const std::string gpsetup = "/home/mathieu/dev/gammatone/test/data/setup.gp";

template<class T>
void plot(const std::string& setup,const T& f, const T& d1, const T& d2, const T& d3);

// Plot bandwidth in function of center frequency for the 3 available
// policies. Reproduces as precisely as possible the figures p.3 and
// p.4 in Slaney1993.
int main()
{
  // frequency axis
  const Scalar fl = 20, fh = 8000;
  const size_t n = 10000;
  const auto freq = utils::linspace<Scalar>(fl,fh,n);

  // Generation of bandwidths in function of frequency (first plot)
  using namespace gammatone::policy::bandwidth;
  std::vector<Scalar> b1(n), b2(n), b3(n);
  std::transform(freq.begin(),freq.end(),b1.begin(),[&](const auto& x){return glasberg1990<Scalar>::bandwidth(x);});
  std::transform(freq.begin(),freq.end(),b2.begin(),[&](const auto& x){return slaney1988<Scalar>::bandwidth(x);});
  std::transform(freq.begin(),freq.end(),b3.begin(),[&](const auto& x){return greenwood1990<Scalar>::bandwidth(x);});

  std::stringstream s1;
  s1 << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set logscale" << std::endl
     << "set key bottom right" << std::endl
     << "set xlabel 'frequency (Hz)'" << std::endl
     << "set xrange ["<<fl<<":"<<fh<<"]" << std::endl
     << "set ylabel 'bandwidth (Hz)'" << std::endl
     << "set yrange [20:1000]" << std::endl;

  plot(s1.str(),freq,b1,b2,b3);


  // Generation of quality factors (second plot)
  std::vector<Scalar> q1(n), q2(n), q3(n);
  std::transform(freq.begin(),freq.end(),b1.begin(),q1.begin(),std::divides<Scalar>());
  std::transform(freq.begin(),freq.end(),b2.begin(),q2.begin(),std::divides<Scalar>());
  std::transform(freq.begin(),freq.end(),b3.begin(),q3.begin(),std::divides<Scalar>());

  std::stringstream s2;
  s2 << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set key bottom right" << std::endl
     << "set xlabel 'frequency (Hz)'" << std::endl
     << "set ylabel 'filter quality'" << std::endl;

  plot(s2.str(),freq,q1,q2,q3);

  return 0;
}

template<class T>
void plot(const std::string& setup,const T& freq, const T& d1, const T& d2, const T& d3)
{
  Gnuplot gp;
  gp << setup
     << "plot '-' u 1:2 w l ls 11 t'glasberg1990', "
     << "     '-' u 1:2 w l ls 12 t'slaney1988', "
     << "     '-' u 1:2 w l ls 13 t'greenwood1990'"
     << std::endl;

  gp.send1d(make_pair(freq,d1));
  gp.send1d(make_pair(freq,d2));
  gp.send1d(make_pair(freq,d3));
}
