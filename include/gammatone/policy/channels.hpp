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

#ifndef GAMMATONE_POLICY_CHANNELS_HPP
#define GAMMATONE_POLICY_CHANNELS_HPP

#include <gammatone/policy/order.hpp>
#include <algorithm>

namespace gammatone
{
  namespace policy
  {
    //! Policy for frequency channels spacing
    /*!
      \namespace gammatone::policy::channels

      Classes in this namespace are in charge of frequency sampling
      and attribution of a center frequency to each filter in a
      filterbank. This computation depends of the number of channels
      you want in the bank, or equivalently to an overlap factor
      between successive filters. An overlap factor close to zero
      indicate filters nearly completly overlapped, an overlap factor
      of 0.5 means that each frequency in the input signal is
      "sampled" by two cochlear channels, and an overlap factor of 1
      means that there is almost no overlap between channels.  Thoses
      algorithm are implemented from \cite Slaney1993 .

      - The class channels::fixed_size let the user determines the
      number of frequency channels in the bank and setup the related
      overlap.

      - The class channels::fixed_overlap let the user determines the
      overlap between successive filters in the bank. Also estimates
      the related number of channels.

      - A channel spacing policy accept a policy::order as template
      parameter. The default is to use policy::order::increasing.

      - Use this policy as a template parameter of gammatone::filterbank.
      ~~~
      using namespace gammatone;

      // Define core and bandwidth policy
      typedef core::cooke1993<double> core;
      typedef policy::bandwidth::glasberg1990<double> bandwidth;

      // Define a filterbank with fixed number of channels
      typedef policy::channels::fixed_size channels1;
      typedef filterbank<double,core,bandwidth,channels1> filterbank1;

      // Define a filterbank with fixed overlap factor
      typedef policy::channels::fixed_overlap channels2;
      typedef filterbank<double,core,bandwidth,channels2> filterbank2;

      // Define a filterbank with fixed overlap factor and decreasing channels
      typedef policy::channels::fixed_overlap<policy::order::decreasing> channels3;
      typedef filterbank<double,core,bandwidth,channels3> filterbank3;

      ~~~
    */
    namespace channels
    {
      //! Base class of channel spacing policies
      /*!
        The base class of channels::fixed_size and channels::fixed_overlap.

        \attention  This class contains only protected static methods and
        should not be called directly.
      */
      class base
      {
      protected:
        //! Usefull constant
        /*!
          Given a bandwidth policy with it's related ear quality
          \f$Q\f$ and minimal bandwidth \f$b_m\f$, computes the
          following constant:

          \f[\alpha = Q b_m\f]

          \tparam Scalar           Type of scalar values.
          \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.

          \return The constant \f$\alpha = Q b_m\f$
        */
        template<class Scalar, class BandwidthPolicy>
        static inline const Scalar alpha();

        //! Usefull constant
        /*!
          Given a bandwidth policy and \f$\alpha\f$ as previously,
          computes the following constant in function of lowest and
          hightest frequencies \f$f_l \f$ and  \f$f_h \f$ respectively:

          \f[\beta = log(\frac{f_l+\alpha}{f_h+\alpha})\f]

          \tparam Scalar  Type of scalar values.
          \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.

          \param low_frequency   The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency  The highest desired center frequency in the filterbank (Hz)
        */
        template<class Scalar, class BandwidthPolicy>
        static inline const Scalar beta(const Scalar& low_frequency, const Scalar& high_frequency);


        //! Computes center frequencies of a filterbank
        /*!
          - General form for computing center frequencies of a
          filterbank called from derived classes.

          - Given lowest and hightest frequencies, \f$f_l \f$ and
          \f$f_h \f$ respectively, a bandwidth related coefficient
          \f$b\f$ and a number of channels \f$n\f$, this method
          computes an array of center frequencies
          \f$(f_k)_{k\in[1,n]}\f$ such as:
          \f[f_k = -\alpha+(f_h+\alpha)e^{kb}\f]

          \tparam Scalar           Type of scalar values.
          \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.
          \tparam OrderingPolicy   Policy for ordering frequency channels. See policy::order.

          \param low_frequency      The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency     The highest desired center frequency in the filterbank (Hz)
          \param bandwidth_related  A bandwidth related parameter. See derived classes.
          \param nb_channels        The number of frequency channels in the filterbank.

          \return The vector \f$(f_k)_{k\in[1,n]}\f$ containing the center frequencies (Hz).
        */
        template<class Scalar, class BandwidthPolicy, class OrderPolicy>
        static std::vector<Scalar> center_frequency(const Scalar& low_frequency,
                                                    const Scalar& high_frequency,
                                                    const Scalar& bandwidth_related,
                                                    const std::size_t& nb_channels);
      };

      //! Computes channel spacing with a fixed number of channels
      /*!
        \tparam OrderPolicy  Policy for ordering frequency channels. See policy::order.
      */
      template
      <
	class Scalar,
	class OrderPolicy = order::increasing
	>
      class fixed_size : public base
      {
      public:

        //! Type of the 3rd parameter of center_frequency
        typedef std::size_t param_type;

        //! Computes center frequencies of a filterbank
        /*!
          - This method returns a result equivalent to
          channels::base::center_frequency(\f$ f_l,f_h,\beta / n,n \f$).

          - The corresponding overlap factor \f$k\f$ is given as \f$ k
          = -Q\beta / n \f$.

          \tparam Scalar           Type of scalar values.
          \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.

          \param low_frequency      The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency     The highest desired center frequency in the filterbank (Hz)
          \param nb_channels        The number of frequency channels in the filterbank.

          \return A vector containing the center frequencies (Hz).
        */
        template<class BandwidthPolicy>
        static std::vector<Scalar> center_frequency(const Scalar& low_frequency,
                                                    const Scalar& high_frequency,
                                                    const std::size_t& nb_channels);

        //! Computes center frequencies of a filterbank
        /*!
          - "Default version" when the number of channels is not
          specified. Use channels::fixed_overlap with the default
          overlap.

          - This method returns a result equivalent to
          channels::fixed_overlap::center_frequency(\f$ f_l,f_h \f$).

          \tparam Scalar           Type of scalar values.
          \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.

          \param low_frequency      The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency     The highest desired center frequency in the filterbank (Hz)

          \return A vector containing the center frequencies (Hz).
        */
        template<class BandwidthPolicy>
        static std::vector<Scalar> center_frequency(const Scalar& low_frequency,
                                                    const Scalar& high_frequency);
      };
      

      //! Computes channel spacing with a fixed overlap factor
      /*!
        \tparam OrderPolicy  Policy for ordering frequency channels. See policy::order.
      */
      template
      <
	class Scalar,
	class OrderPolicy = order::increasing
	>
      class fixed_overlap : public base
      {
      public:

        //! Type of the 3rd parameter of center_frequency
        typedef Scalar param_type;

        //! Default overlap factor
        static const Scalar default_overlap;

        //! Computes center frequencies of a filterbank
        /*!
          - Let \f$k\f$ be the overlap factor, \f$Q\f$ the bandwidth ear
          quality, this method returns a result equivalent to
          channels::base::center_frequency(\f$ f_l,f_h,-k/Q,n \f$).

          - The number of channels \f$n\f$ is given by \f$n = -Q\beta / k\f$.

          \tparam Scalar           Type of scalar values.
          \tparam BandwidthPolicy  Policy for bandwith constants. See policy::bandwidth.

          \param low_frequency   The lowest desired center frequency in the filterbank (Hz)
          \param high_frequency  The highest desired center frequency in the filterbank (Hz)
          \param overlap_factor  The number of frequency channels in the filterbank.

          \return A vector *cf* containing the center frequencies (Hz).
        */
        template<class BandwidthPolicy>
        static std::vector<Scalar> center_frequency(const Scalar& low_frequency,
                                                    const Scalar& high_frequency,
                                                    const Scalar& overlap_factor = default_overlap);
      };
    }
  }
}

/*
  channels::base
*/

template<class Scalar, class BandwidthPolicy>
const Scalar gammatone::policy::channels::base::alpha()
{
  return BandwidthPolicy::earq * BandwidthPolicy::minbw;
}


template<class Scalar, class BandwidthPolicy>
const Scalar gammatone::policy::channels::base::beta(const Scalar& fl, const Scalar& fh)
{
  const Scalar a = alpha<Scalar,BandwidthPolicy>();
  return std::log((fl+a) / (fh+a));
}


template<class Scalar, class BandwidthPolicy, class OrderPolicy>
std::vector<Scalar> gammatone::policy::channels::base::
center_frequency(const Scalar& fl, const Scalar& fh, const Scalar& b, const std::size_t& nbc)
{
  const auto a = alpha<Scalar,BandwidthPolicy>();

  std::vector<Scalar> frequencies(nbc);
  std::size_t i = 0;
  std::for_each(OrderPolicy::begin(frequencies),OrderPolicy::end(frequencies),
                [&](auto& x){x = -a + (fh+a)*std::exp(b*i++);});

  return std::move(frequencies);
}


/*
  channels::fixed_size
*/


template<class Scalar, class OrderPolicy> template<class BandwidthPolicy>
std::vector<Scalar> gammatone::policy::channels::fixed_size<Scalar,OrderPolicy>::
center_frequency(const Scalar& fl, const Scalar& fh, const std::size_t& nbc)
{
  const auto b = base::beta<Scalar,BandwidthPolicy>(fl,fh) / static_cast<Scalar>(nbc);
  return std::move(base::center_frequency<Scalar,BandwidthPolicy,OrderPolicy>(fl,fh,b,nbc));
}


template<class Scalar, class OrderPolicy> template<class BandwidthPolicy>
std::vector<Scalar> gammatone::policy::channels::fixed_size<Scalar,OrderPolicy>::
center_frequency(const Scalar& fl, const Scalar& fh)
{
  return std::move(fixed_overlap<Scalar,OrderPolicy>::template center_frequency<BandwidthPolicy>(fl,fh));
}


/*
  channels::fixed_overlap
*/


template<class Scalar, class OrderPolicy>
const Scalar gammatone::policy::channels::fixed_overlap<Scalar,OrderPolicy>::default_overlap = 0.25;


template<class Scalar, class OrderPolicy> template<class BandwidthPolicy>
std::vector<Scalar> gammatone::policy::channels::fixed_overlap<Scalar,OrderPolicy>::
center_frequency(const Scalar& fl, const Scalar& fh, const Scalar& overlap_factor)
{
  const auto b = -overlap_factor / BandwidthPolicy::earq;
  const std::size_t nbc = base::beta<Scalar,BandwidthPolicy>(fl,fh) / b;
  return std::move(base::center_frequency<Scalar,BandwidthPolicy,OrderPolicy>(fl,fh,b,nbc));
}

#endif // GAMMATONE_POLICY_CHANNELS_HPP
