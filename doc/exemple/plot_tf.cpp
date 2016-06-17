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
#include <gammatone/detail/utils.hpp>
#include <test_setup.hpp>
#include <fstream>

/*!
  Compute the theorical transfer function of a gammatone filter.
  \see Holdsworth1988 eq. 2
  \tparam F Type of the input gammatone filter
  \param filter The gammatone filter to be computed
  \param freq   The frequency for which to compute the result (Hz)
  \return The transfer function value at the given frequency
*/
template<class F, class T>
static inline std::complex<T> formula_tf
(const F& filter, const T& freq)
{
  return std::pow(std::complex<T>(1,(freq-filter.center_frequency())/filter.bandwidth()), -4)
    + std::pow(std::complex<T>(1,(freq+filter.center_frequency())/filter.bandwidth()), -4);
}

template<class F, class T>
static inline std::complex<T> formula_tf_approx
(const F& filter, const T& freq)
{
  return std::pow(std::complex<T>(1,(freq-filter.center_frequency())/filter.bandwidth()), -4);
}


typedef double T;
typedef gammatone::filter<T> filter;


int main()
{
  const T fs = 44100;
  const T fc = 1000;
  const std::size_t size = 5000;

  filter f(fs, fc);
  const auto freq = gammatone::detail::linspace<T>(0.0,2.0*fc,size);

  std::vector<std::pair<T,T> > tf(size), tf_aprox(size);
  std::transform(freq.begin(),freq.end(),tf.begin(),
                 [&](const T& x){std::complex<T> tmp = formula_tf(f,x);
		   return std::make_pair(std::abs(tmp),std::arg(tmp));});

  std::transform(freq.begin(),freq.end(),tf_aprox.begin(),
                 [&](const T& x){auto tmp = formula_tf_approx(f,x);
		   return std::make_pair(std::abs(tmp),std::arg(tmp));});

  Gnuplot gp_gain;
  gp_gain << std::ifstream(gpsetup).rdbuf() << std::endl
          << "set xlabel 'frequency (Hz)'" << std::endl
          << "set ylabel 'gain'" << std::endl
          << "plot '-' u 1:2 w l ls 11 lw 2, '-' u 1:2 w l ls 12" << std::endl;
  gp_gain.send1d(std::make_pair(freq,tf));
  gp_gain.send1d(std::make_pair(freq,tf_aprox));

  Gnuplot gp_phase;
  gp_phase << std::ifstream(gpsetup).rdbuf() << std::endl
           << "set xlabel 'frequency (Hz)'" << std::endl
           << "set ylabel 'phase'" << std::endl
           << "plot '-' u 1:3 w l ls 11 lw 2, '-' u 1:3 w l ls 12" << std::endl;
  gp_phase.send1d(std::make_pair(freq,tf));
  gp_phase.send1d(std::make_pair(freq,tf_aprox));

  std::cout << freq.size() << " " << tf.size() << std::endl;

  return 0;
}
