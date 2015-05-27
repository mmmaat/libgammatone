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

#ifndef GAMMATONE_POLICY_CHANNELS_FIXED_OVERLAP_HPP
#define GAMMATONE_POLICY_CHANNELS_FIXED_OVERLAP_HPP

#include <gammatone/policy/channels/base.hpp>

namespace gammatone
{
  namespace policy
  {
    namespace channels
    {
      //! Computes channel spacing with a fixed overlap factor
      /*!
        \tparam Scalar           Type of scalar values.
        \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.
        \tparam OrderPolicy      Policy for ordering frequency channels. See policy::order.
      */
      template
      <
        class Scalar,
        template<class> class BandwidthPolicy,
        class OrderPolicy
        >
      class fixed_overlap : private base<Scalar,BandwidthPolicy,OrderPolicy>
      {
      public:

        //! Type of the 3rd parameter of setup()
        using param_type = Scalar;

        //! Default overlap factor
        static const param_type default_overlap;

        // Return the default overlap factor
        static param_type default_parameter();


        //! Computes center frequencies and overlap factor of a filterbank
        /*!
          - Let \f$k\f$ be the overlap factor, \f$Q\f$ the bandwidth ear
          quality, this method returns a result equivalent to
          channels::base::setup(\f$ f_l,f_h,-k/Q,n \f$).

          - The number of channels \f$n\f$ is given by \f$n = -Q\beta / k\f$.

          \param low_frequency   The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency  The highest desired center frequency in the filterbank (Hz)
          \param overlap_factor  The number of frequency channels in the filterbank.

          \return A pair p, where p.first is the vector of center
          frequencies (Hz) and p.second the overlap factor. The number
          of channels is thus p.first.size()
        */
        static typename gammatone::policy::channels::base<Scalar,BandwidthPolicy,OrderPolicy>::setup_type
        setup(const Scalar& low_frequency,
              const Scalar& high_frequency,
              const param_type& overlap_factor);
      };
    }
  }
}

template
<
  class Scalar,
  template<class> class BandwidthPolicy,
  class OrderPolicy
  >
const typename gammatone::policy::channels::fixed_overlap<Scalar,BandwidthPolicy,OrderPolicy>::param_type
gammatone::policy::channels::fixed_overlap<Scalar,BandwidthPolicy,OrderPolicy>::
default_overlap = 0.25;

template
<
  class Scalar,
  template<class> class BandwidthPolicy,
  class OrderPolicy
  >
typename gammatone::policy::channels::fixed_overlap<Scalar,BandwidthPolicy,OrderPolicy>::param_type
gammatone::policy::channels::fixed_overlap<Scalar,BandwidthPolicy,OrderPolicy>::
default_parameter()
{
  return default_overlap;
}

template
<
  class Scalar,
  template<class> class BandwidthPolicy,
  class OrderPolicy
  >
typename gammatone::policy::channels::base<Scalar,BandwidthPolicy,OrderPolicy>::setup_type
gammatone::policy::channels::fixed_overlap<Scalar,BandwidthPolicy,OrderPolicy>::
setup(const Scalar& fl, const Scalar& fh, const Scalar& overlap_factor)
{
  using base = base<Scalar,BandwidthPolicy,OrderPolicy>;
  
  const Scalar b = -overlap_factor / BandwidthPolicy<Scalar>::earq;
  const std::size_t nbc = base::beta(fl,fh) / b;

  return std::make_pair(base::setup(fl,fh,b,nbc), overlap_factor);
}

#endif // GAMMATONE_POLICY_CHANNELS_FIXED_OVERLAP_HPP
