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

#ifndef GAMMATONE_FILTER_CONCRETE_HPP
#define GAMMATONE_FILTER_CONCRETE_HPP

#include <gammatone/filter/interface.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/policy/postprocessing.hpp>
#include <gammatone/policy/bandwidth.hpp>

namespace gammatone
{
  namespace filter
  {
    //! Concrete gammatone filter
    /*!
      \class concrete

      This class models a single gammatone filter. Processing core,
      bandwidth computation and postprocessing are specified by
      template policies.

      \tparam Scalar                Type of the scalars.
      \tparam Core                  Type of the processing core.
      \tparam BandwidthPolicy       Policy for computing filter bandwidth.
      \tparam PostProcessingPolicy  Policy for output postprocessing.
    */
    template
    <
      class Scalar,
      class Core = core::cooke1993<Scalar>,
      class BandwidthPolicy = policy::bandwidth::glasberg1990<Scalar>,
      class PostProcessingPolicy = policy::postprocessing::off<Scalar>
      >
    class concrete : public ::gammatone::filter::interface<Scalar>
    {
    public:

      //! Creates a gammatone filter from explicit parameters.
      /*! 
	\param sample_frequency The input signal sample frequency (Hz).
        \param center_frequency The filter center frequency (Hz).
      */
      concrete(const Scalar& sample_frequency, const Scalar& center_frequency);

      //! Copy constructor
      concrete(const concrete<Scalar,Core,BandwidthPolicy, PostProcessingPolicy>& other);

      //! Assignment operator
      concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>&
      operator=(const concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>& other);

      //! Destructor
      virtual ~concrete();

      // Methods inherited from filter::interface
      inline Scalar sample_frequency() const;
      inline Scalar center_frequency() const;
      inline Scalar bandwidth() const;
      inline Scalar gain() const;
      inline void reset();

    protected:

      // Method inherited from filter::interface
      inline Scalar compute_internal(const Scalar& input);
      
      //! Sample frequency of the filter (Hz)
      const Scalar m_sample_frequency;

      //! Center frequency of the filter (Hz)
      const Scalar m_center_frequency;

      //! Bandwidth of the filter (Hz)
      const Scalar m_bandwidth;

      //! The underlying processing core
      Core m_core;
    };
  }
}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
concrete(const Scalar& sample_frequency,
       const Scalar& center_frequency)
  : m_sample_frequency( sample_frequency ),
    m_center_frequency( center_frequency ),
    m_bandwidth( BandwidthPolicy::bandwidth(center_frequency) ),
    m_core( m_sample_frequency, m_center_frequency, m_bandwidth )
{}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
concrete(const concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>& other)
  : m_sample_frequency( other.m_sample_frequency ),
    m_center_frequency( other.m_center_frequency ),
    m_bandwidth( other.m_bandwidth ),
    m_core( other.m_core )
{}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>&
gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
operator=(const concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>& other)
{
  concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy> tmp(other);

  std::swap( m_sample_frequency, tmp.m_sample_frequency );
  std::swap( m_center_frequency, tmp.m_center_frequency );
  std::swap( m_bandwidth, tmp.m_bandwidth );
  std::swap( m_core, tmp.m_core );

  return *this;
}


template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
~concrete()
{}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
Scalar gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
center_frequency() const
{
  return m_center_frequency;
}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
Scalar gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
sample_frequency() const
{
  return m_sample_frequency;
}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
Scalar gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
bandwidth() const
{
  return m_bandwidth;
}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
Scalar gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
gain() const
{
  return m_core.gain();
}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
void gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
reset()
{
  m_core.reset();
}

template<class Scalar, class Core, class BandwidthPolicy, class PostProcessingPolicy>
Scalar gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy>::
compute_internal(const Scalar& input)
{
  return PostProcessingPolicy::process(m_core.compute(input));
}

#endif // GAMMATONE_FILTER_CONCRETE_HPP
