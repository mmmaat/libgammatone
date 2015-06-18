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

#ifndef GAMMATONE_POLICY_CHANNELS_BASE_HPP
#define GAMMATONE_POLICY_CHANNELS_BASE_HPP

#include <gammatone/policy/policy.hpp>
#include <algorithm>
#include <utility> // for std::pair
#include <vector>
#include <cmath>

namespace gammatone
{
  namespace policy
  {
    namespace channels
    {
      //! Base class of channel spacing policies
      /*!
        The base class of channels::fixed_size and
        channels::fixed_overlap. This class contains only protected
        static methods and should not be called directly.

        \tparam Scalar           Type of scalar values.
        \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.
      */
      template
      <
        class Scalar,
        template<class> class BandwidthPolicy
        >
      class base : public gammatone::policy::policy
      {
      protected:
        //! Usefull constant
        /*!
          Given a bandwidth policy with it's related ear quality
          \f$Q\f$ and minimal bandwidth \f$b_m\f$, computes the
          following constant:
          \f[\alpha = Q b_m\f]

          \return The constant \f$\alpha = Q b_m\f$
        */
        static const Scalar alpha();

        //! Usefull constant
        /*!
          Given a bandwidth policy and \f$\alpha\f$ as previously,
          computes the following constant in function of lowest and
          hightest frequencies \f$f_l \f$ and  \f$f_h \f$ respectively:

          \f[\beta = log(\frac{f_l+\alpha}{f_h+\alpha})\f]

          \param low_frequency   The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency  The highest desired center frequency in the filterbank (Hz)
        */
        static const Scalar beta(const Scalar& low_frequency,
                                 const Scalar& high_frequency);

        //! Return type of setup()
        using setup_type = std::pair<std::vector<Scalar>,Scalar>;


        //! Computes center frequencies and overlap factor of a filterbank
        /*!

          - General form for setting up filterbank channels, called
          from derived classes.

          - Given lowest and hightest frequencies, \f$f_l \f$ and
          \f$f_h \f$ respectively, a bandwidth related coefficient
          \f$b\f$ and a number of channels \f$n\f$, this method
          computes an array of center frequencies
          \f$(f_k)_{k\in[1,n]}\f$ such as: \f[f_k =
          -\alpha+(f_h+\alpha)e^{kb}\f]

          \param low_frequency      The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency     The highest desired center frequency in the filterbank (Hz)
          \param bandwidth          A bandwidth related parameter. See derived classes.
          \param nb_channels        The number of frequency channels in the filterbank.

          \return A pair p, where p.first is the vector of center
          frequencies (Hz) and p.second the overlap factor. The number
          of channels is thus p.first.size()
        */
        static typename setup_type::first_type setup(const Scalar& low_frequency,
                                                     const Scalar& high_frequency,
                                                     const Scalar& bandwidth,
                                                     const std::size_t& nb_channels);
      };
    }
  }
}

template
<
  class Scalar,
  template<class> class BandwidthPolicy
  >
const Scalar
gammatone::policy::channels::base<Scalar,BandwidthPolicy>::
alpha()
{
  return BandwidthPolicy<Scalar>::earq * BandwidthPolicy<Scalar>::minbw;
}

template
<
  class Scalar,
  template<class> class BandwidthPolicy
  >
const Scalar
gammatone::policy::channels::base<Scalar,BandwidthPolicy>::
beta(const Scalar& fl, const Scalar& fh)
{
  const Scalar a = alpha();
  return std::log((fl+a) / (fh+a));
}

template
<
  class Scalar,
  template<class> class BandwidthPolicy
  >
typename gammatone::policy::channels::base<Scalar,BandwidthPolicy>::setup_type::first_type
gammatone::policy::channels::base<Scalar,BandwidthPolicy>::
setup(const Scalar& fl, const Scalar& fh, const Scalar& b, const std::size_t& nbc)
{
  const auto a = alpha();

  std::vector<Scalar> frequencies(nbc);
  std::size_t i = 0;
  std::for_each(frequencies.rbegin(),frequencies.rend(),
                [&](auto& x){x = -a + (fh+a)*std::exp(b*i++);});

  return frequencies;
}

#endif // GAMMATONE_POLICY_CHANNELS_BASE_HPP
