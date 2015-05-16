/*
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

#ifndef GAMMATONE_FILTER_HPP
#define GAMMATONE_FILTER_HPP

#include <gammatone/interface.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/policy/postprocessing.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <utility>

namespace gammatone
{
  //! Gammatone filter
  /*!
    \class filter

    This class models a single gammatone filter. Processing is
    delegated to a specific core and bandwidth computation is
    specified by policy.

    \tparam Scalar           Type of scalar values.
    \tparam Core             Type of the filter core. See gammatone::core.
    \tparam BandwidthPolicy  Policy for computing filter bandwidth. See policy::bandwidth.
  */
  template<class Scalar,
           class Core = core::cooke1993<Scalar>,
           class BandwidthPolicy = policy::bandwidth::glasberg1990<Scalar> >
  class filter : public interface<Scalar,Scalar>
  {
  public:
    
    //! Creates a gammatone filter from explicit parameters.
    /*!
      \param sample_frequency The sample frequency of the input signal
      (Hz). Must be positive.

      \param center_frequency The center frequency of the filter
      (Hz). Must be positive. Practically, this is a non-sense to have
      \f$f_c > f_s/2\f$.
    */
    filter(const Scalar& sample_frequency, const Scalar& center_frequency);

    //! Copy constructor
    filter(const filter<Scalar,Core,BandwidthPolicy>& other);

    //! Assignment operator
    filter<Scalar,Core,BandwidthPolicy>& operator=(const filter<Scalar,Core,BandwidthPolicy>& other);

    //! Destructor
    virtual ~filter();

    
    //! Compute a output state from a scalar input.
    /*!
      \tparam PostProcessingPolicy Type of policy for output
      postprocessing. Default is to do nothing.

      \param input  The scalar value to be processed by the filter.
    */
    template<class PostProcessingPolicy = policy::postprocessing::off<Scalar> >
    inline Scalar compute(const Scalar& input);

    
    //! Compute preallocated output from a range of scalar inputs.
    /*!
      Sequentially computes a range of input values and stores the
      result in an output range.

      \tparam PostProcessingPolicy  Type of policy for output
      postprocessing. Default is to do nothing.
      \tparam InputIterator   Iterator on the input range.
      \tparam OutputIterator  Iterator on the output range.

      \param first  Iterator to the initial position of the input range.
      \param last   Iterator to the final position of the input range.
      \param result Iterator to the initial position of the output
      range. The range must include at least as many elements as
      [first,last).
    */
    template<class PostProcessingPolicy = policy::postprocessing::off<Scalar>,
             class InputIterator, class OutputIterator>
    void compute(const InputIterator& first, const InputIterator& last, const OutputIterator& result);

    
    //! Compute output from a range of scalar inputs.
    /*!
      Sequentially computes a range of input values and returns the result.

      \tparam PostProcessingPolicy  Type of policy for output
      postprocessing. Default is to do nothing.
      \tparam Container  Type of the result container.

      \param input The input signal to be processed
      \return The output processed signal
    */
    template<class PostProcessingPolicy = policy::postprocessing::off<Scalar>,
	     class Container = std::vector<Scalar> >
    Container compute(const Container& input);


    // methods inherited from interface
    inline Scalar sample_frequency() const;
    inline Scalar center_frequency() const;
    inline Scalar bandwidth() const;
    inline Scalar gain() const;
    virtual inline void reset();

  protected:

    //! Sample frequency of the filter (Hz)
    const Scalar m_sample_frequency;

    //! center frequency of the filter (Hz)
    const Scalar m_center_frequency;

    //! Bandwidth of the filter (Hz)
    const Scalar m_bandwidth;

    //! The underlying processing core
    Core m_core;
  };
}


template<class Scalar, class Core, class BandwidthPolicy>
gammatone::filter<Scalar,Core,BandwidthPolicy>::
filter(const Scalar& sample_frequency,
       const Scalar& center_frequency)
  : m_sample_frequency( sample_frequency ),
    m_center_frequency( center_frequency ),
    m_bandwidth( BandwidthPolicy::bandwidth(center_frequency) ),
    m_core( m_sample_frequency, m_center_frequency, m_bandwidth )
{}

template<class Scalar, class Core, class BandwidthPolicy>
gammatone::filter<Scalar,Core,BandwidthPolicy>::
filter(const gammatone::filter<Scalar,Core,BandwidthPolicy>& other)
  : m_sample_frequency( other.m_sample_frequency ),
    m_center_frequency( other.m_center_frequency ),
    m_bandwidth( other.m_bandwidth ),
    m_core( other.m_core )
{}

template<class Scalar, class Core, class BandwidthPolicy>
gammatone::filter<Scalar,Core,BandwidthPolicy>& gammatone::filter<Scalar,Core,BandwidthPolicy>::
operator=(const gammatone::filter<Scalar,Core,BandwidthPolicy>& other)
{
  gammatone::filter<Scalar,Core,BandwidthPolicy> tmp(other);

  std::swap( m_sample_frequency, tmp.m_sample_frequency );
  std::swap( m_center_frequency, tmp.m_center_frequency );
  std::swap( m_bandwidth, tmp.m_bandwidth );
  std::swap( m_core, tmp.m_core );

  return *this;
}


template<class Scalar, class Core, class BandwidthPolicy>
gammatone::filter<Scalar,Core,BandwidthPolicy>::~filter()
{}

template<class Scalar, class Core, class BandwidthPolicy>
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy>::center_frequency() const
{
  return m_center_frequency;
}

template<class Scalar, class Core, class BandwidthPolicy>
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy>::sample_frequency() const
{
  return m_sample_frequency;
}

template<class Scalar, class Core, class BandwidthPolicy>
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy>::bandwidth() const
{
  return m_bandwidth;
}

template<class Scalar, class Core, class BandwidthPolicy>
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy>::gain() const
{
  return m_core.gain();
}

template<class Scalar, class Core, class BandwidthPolicy>
void gammatone::filter<Scalar,Core,BandwidthPolicy>::reset()
{
  m_core.reset();
}

template<class Scalar, class Core, class BandwidthPolicy>
template<class PostProcessingPolicy>
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy>::
compute(const Scalar& input)
{
  return PostProcessingPolicy::process(m_core.compute(input));
}

template<class Scalar, class Core, class BandwidthPolicy>
template <class PostProcessingPolicy, class InputIterator, class OutputIterator>
void gammatone::filter<Scalar,Core,BandwidthPolicy>::
compute(const InputIterator& first, const InputIterator& last, const OutputIterator& result)
{
  std::transform(first,last,result,[&](const auto& x){return this->compute<PostProcessingPolicy>(x);});
}

template<class Scalar, class Core, class BandwidthPolicy>
template<class PostProcessingPolicy, class Container>
Container gammatone::filter<Scalar,Core,BandwidthPolicy>::
compute(const Container& input)
{
  Container out(input.size());
  this->compute<PostProcessingPolicy>(input.begin(),input.end(),out.begin());
  return std::move(out);
}

#endif // GAMMATONE_FILTER_HPP
