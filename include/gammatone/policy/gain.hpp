/*
  Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>

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

#ifndef GAMMATONE_POLICY_GAIN_HPP
#define GAMMATONE_POLICY_GAIN_HPP

#include <gammatone/policy/policy.hpp>
#include <cmath>

namespace gammatone
{
  namespace policy
  {
    //! Policies for filter gain computation
    /*!
      \namespace gammatone::policy::gain

      This namespace provides a set of policies used to set up the
      gain of a gammatone filter.
    */
    namespace gain
    {
      //! Old cooke1993 gain implementation
      class old_cooke1993 : public gammatone::policy::policy
      {
      public:
        template<class Scalar>
        static inline Scalar gain(const Scalar& input,
                                  const Scalar& sample_frequency,
                                  const Scalar& center_frequency,
                                  const std::size_t& order);
      };

      //! Disable gain policy
      class off : public gammatone::policy::policy
      {
      public:
        template<class Scalar>
        static inline Scalar gain(const Scalar& input,
                                  const Scalar& sample_frequency,
                                  const Scalar& center_frequency,
                                  const std::size_t& order);
      };

      //! 0dB gain for all channels
      class forall_0dB : public gammatone::policy::policy
      {
      public:
        template<class Scalar>
        static inline Scalar gain(const Scalar& input,
                                  const Scalar& sample_frequency,
                                  const Scalar& center_frequency,
                                  const std::size_t& order);
      };

      //! Gain falling by 6dB gain per octave
      class peroctave_6dB : public gammatone::policy::policy
      {
      public:
        template<class Scalar>
        static inline Scalar gain(const Scalar& input,
                                  const Scalar& sample_frequency,
                                  const Scalar& center_frequency,
                                  const std::size_t& order);
      };
    }
  }
}

template<class Scalar>
Scalar gammatone::policy::gain::off::
gain(const Scalar& input,
     const Scalar&, const Scalar&, const std::size_t&)
{
  return input;
}

template<class Scalar>
Scalar gammatone::policy::gain::old_cooke1993::
gain(const Scalar& input,
     const Scalar&, const Scalar&,
     const std::size_t& order)
{
  return std::pow(input,order) / 3.0;
}

template<class Scalar>
Scalar gammatone::policy::gain::forall_0dB::
gain(const Scalar& input,
     const Scalar&, const Scalar&,
     const std::size_t& order)
{
  return std::pow(input,order);
}

template<class Scalar>
Scalar gammatone::policy::gain::peroctave_6dB::
gain(const Scalar& input,
     const Scalar& sample_frequency,
     const Scalar& center_frequency,
     const std::size_t& order)
{
  return gammatone::policy::gain::forall_0dB::gain(input,sample_frequency,center_frequency,order) *
    sample_frequency / (order * center_frequency);
}


#endif // GAMMATONE_POLICY_GAIN_HPP
