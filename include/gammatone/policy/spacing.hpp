/*!
  \file   spacing.hpp
  \brief  Channel spacing for gammatone filterbank generation

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

#ifndef GAMMATONE_POLICY_SPACING_HPP
#define GAMMATONE_POLICY_SPACING_HPP

#include <gammatone/policy/bandwidth.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

namespace gammatone
{
  namespace policy
  {
    /*!
      \namespace gammatone::policy::spacing

      Compute the center frequncies of filters in a filterbank
      according a bandwidth polcy and lowest/hightest frequencies in
      the bank.
     */
    namespace spacing
    {
      template<class Scalar,
	       class BandwidthPolicy = gammatone::policy::bandwidth::glasberg1990<Scalar> >
      class base
      {
      protected:
        static inline const Scalar alpha();

        static inline const Scalar beta(const Scalar& low_frequency, const Scalar& high_frequency);

        static std::vector<Scalar> compute(const Scalar& fl, const Scalar& fh, const Scalar& b, const std::size_t& nbc);
      };

      template<class Scalar,
	       class BandwidthPolicy = gammatone::policy::bandwidth::glasberg1990<Scalar> >
      class fixed_size : public base<Scalar,BandwidthPolicy>
      {
      public:
        static std::vector<Scalar> center_frequency(const Scalar& low_frequency,
                                               const Scalar& high_frequency,
                                               const std::size_t& nb_channels);
      };

      template<class Scalar,
	       class BandwidthPolicy = gammatone::policy::bandwidth::glasberg1990<Scalar> >
      class fixed_overlap : public base<Scalar,BandwidthPolicy>
      {
      public:
        static std::vector<Scalar> center_frequency(const Scalar& low_frequency,
                                               const Scalar& high_frequency,
                                               const Scalar& overlap_factor);
      };
    }
  }
}

template<class Scalar, class BandwidthPolicy> const Scalar gammatone::policy::spacing::base<Scalar,BandwidthPolicy>::
alpha()
{
  //! \todo Where is order ?
  return BandwidthPolicy::earq * BandwidthPolicy::minbw;
}

template<class Scalar, class BandwidthPolicy> const Scalar gammatone::policy::spacing::base<Scalar,BandwidthPolicy>::
beta(const Scalar& fl, const Scalar& fh)
{
  const auto a = alpha();
  return std::log(fl+a) - std::log(fh+a);
}

template<class Scalar, class BandwidthPolicy> std::vector<Scalar> gammatone::policy::spacing::base<Scalar,BandwidthPolicy>::
compute(const Scalar& fl,
        const Scalar& fh,
        const Scalar& b,
        const std::size_t& nbc)
{
  const auto a = base<Scalar>::alpha();

  std::vector<Scalar> frequencies(nbc);
  std::size_t i = 0;
  std::for_each(frequencies.begin(),frequencies.end(),
                [&](auto& x){x = -a + (fh+a)*std::exp(b*i++);});

  return std::move(frequencies);
}

template<class Scalar, class BandwidthPolicy>
std::vector<Scalar> gammatone::policy::spacing::fixed_size<Scalar,BandwidthPolicy>::
center_frequency(const Scalar& fl, const Scalar& fh, const std::size_t& nbc)
{
  typedef fixed_size<Scalar,BandwidthPolicy> T;
  
  const auto b = T::beta(fl,fh) / static_cast<Scalar>(nbc);
  return std::move(T::compute(fl,fh,b,nbc));
}


template<class Scalar, class BandwidthPolicy>
std::vector<Scalar> gammatone::policy::spacing::fixed_overlap<Scalar,BandwidthPolicy>::
center_frequency(const Scalar& fl, const Scalar& fh, const Scalar& overlap_factor)
{
  typedef fixed_overlap<Scalar,BandwidthPolicy> T;
  
  const auto b = -overlap_factor / BandwidthPolicy::earq;
  const std::size_t nbc = T::beta(fl,fh) / b;
  return std::move(T::compute(fl,fh,b,nbc));
}

#endif // GAMMATONE_POLICY_SPACING_HPP
