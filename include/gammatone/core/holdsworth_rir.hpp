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

#ifndef GAMMATONE_CORE_HOLDSWORTH_RIR_HPP
#define GAMMATONE_CORE_HOLDSWORTH_RIR_HPP

#include <complex>
#include <queue>

#include <iostream>

namespace gammatone
{
  namespace core
  {
    template<class T>
    class holdsworth_rir
    {
    public:
      typedef T scalar_type;

      void init(const T& sample_frequency, const T& center_frequency, const T& bandwidth)
      {
        alpha = find_alpha(sample_frequency,bandwidth);
        omega = find_omega(sample_frequency,center_frequency);
	period = 1.0 / sample_frequency;
        reset();
      }

      virtual ~holdsworth_rir(){}

      void reset()
      {
        t = 0;
	while(!z.empty()) z.pop(); z.push(0); z.push(0);
	while(!w.empty()) w.pop(); w.push(0); w.push(0);
      }

      virtual inline T compute(const T& x)
      {
	std::cout << std::exp(-omega*t) << "  " << x << "  " << std::exp(-omega*t) * x << std::endl;

        z.push( std::exp(-omega*t) * x ); z.pop();
        w.push( w.front() + alpha*w.front()*z.front()); w.pop();
	auto y = std::real( std::exp(omega*t) * w.back() );
	t += period;
	return y;
      }

    protected:
      T period, t;
      T alpha;
      std::complex<T> omega;
      std::queue<std::complex<T> > z, w;

      inline T find_alpha(const T& fs, const T& bw)
      {
        return 1 - std::exp(-2*M_PI*bw/fs);
      }

      inline std::complex<T> find_omega(const T& fs, const T& fc)
      {
	const auto theta = 2*M_PI*fc/fs;
        return std::complex<T>(std::cos(theta),std::sin(theta));
      }
    };
  }
}

#endif // GAMMATONE_CORE_HOLDSWORTH_RIR_HPP
