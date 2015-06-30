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

#ifndef GAMMATONE_FILTER_HPP
#define GAMMATONE_FILTER_HPP

#include <gammatone/detail/interface.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/policy/gain.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/clipping.hpp>

namespace gammatone
{
  //! Gammatone filter implementation
  /*!
    \class filter gammatone/filter.hpp

    This class models a single gammatone filter. Processing core as
    well as bandwidth and clipping policies are templates.

    \tparam Scalar                Type of the scalars.
    \tparam Core                  Type of the processing core.
    \tparam BandwidthPolicy       Policy for computing filter bandwidth.
    \tparam ClippingPolicy        Policy for clipping.
  */
  template
  <
    class Scalar,
    template<class...> class Core         = core::cooke1993,
    template<class> class BandwidthPolicy = policy::bandwidth::glasberg1990,
    class ClippingPolicy                  = policy::clipping::off
    >
  class filter : public detail::interface<Scalar,Scalar>
  {
  public:

    //! Type of this filter
    using type = filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>;

    //! Type of the inherited interface
    using base = detail::interface<Scalar,Scalar>;

    //! Type of the filter core
    using core = Core<Scalar,policy::gain::forall_0dB,ClippingPolicy>;


    
    //! Creates a gammatone filter from explicit parameters.
    /*!
      \param sample_frequency The input signal sample frequency (Hz).
      \param center_frequency The filter center frequency (Hz).
    */
    filter(const Scalar& sample_frequency,
           const Scalar& center_frequency);

    //! Copy constructor
    filter(const type& other);

    //! Move constructor
    filter(type&& other) noexcept;

    //! Assignment operator
    type& operator=(const type& other);

    //! Move operator
    type& operator=(type&& other);

    //! Destructor
    virtual ~filter();

    //! Inherited from interface
    inline Scalar center_frequency() const;
    inline Scalar bandwidth() const;
    inline Scalar gain() const;
    inline void reset();

    //! Compute an output from a scalar input
    /*!
      \param input  The scalar value to be processed.
      \return       The computed output value.
    */
    inline Scalar compute(const Scalar& input);

    //! Compute an input iterator range
    /*!
      Sequentially computes an input range of values and stores the
      result in an output range of values.

      \tparam InputIterator   Iterator on the input range.
      \tparam OutputIterator  Iterator on the output range.

      \param first  Iterator to the initial position of the input range.
      \param last   Iterator to the final position of the input range.
      \param result Iterator to the initial position of the output range.

      \attention This method does not allocate any data. The range
      must include at least as many elements as [first,last).
    */
    template<class InputIterator, class OutputIterator>
    inline void compute(const InputIterator& first,
                        const InputIterator& last,
                        const OutputIterator& result);

    //! Compute scalar values from pointer
    void compute(const std::size_t& size,
                 const Scalar* input,
                 Scalar* output);

  private:

    //! Filter center frequency (Hz)
    Scalar m_center_frequency;

    //! Filter bandwidth (Hz)
    Scalar m_bandwidth;

    //! The underlying processing core
    core m_core;
  };
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
filter(const Scalar& sample_frequency,
       const Scalar& center_frequency)
  : base(sample_frequency),
    m_center_frequency(center_frequency),
    m_bandwidth(BandwidthPolicy<Scalar>::bandwidth(center_frequency)),
    m_core(base::sample_frequency(), m_center_frequency, m_bandwidth)
{}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
filter(const type& other)
  : base(other),
    m_center_frequency(other.m_center_frequency),
    m_bandwidth(other.m_bandwidth),
    m_core(other.m_core)
{}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
filter(type&& other) noexcept
: base(std::move(other)),
  m_center_frequency(std::move(other.m_center_frequency)),
  m_bandwidth(std::move(other.m_bandwidth)),
  m_core(std::move(other.m_core))
{}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>&
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
operator=(const type& other)
{
  base::operator=(other);

  type tmp(other);
  std::swap(m_core, tmp.m_core);
  std::swap(m_center_frequency, tmp.m_center_frequency);
  std::swap(m_bandwidth, tmp.m_bandwidth);

  return *this;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>&
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
operator=(type&& other)
{
  base::operator=(std::move(other));

  m_center_frequency = std::move(other.m_center_frequency);
  m_bandwidth = std::move(other.m_bandwidth);
  m_core = std::move(other.m_core);

  return *this;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
~filter()
{}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
center_frequency() const
{
  return m_center_frequency;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
bandwidth() const
{
  return m_bandwidth;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
gain() const
{
  return m_core.gain();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
void gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
reset()
{
  m_core.reset();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
Scalar gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
compute(const Scalar& input)
{
  return m_core.compute(input);
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
template<class InputIterator, class OutputIterator>
void gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
compute(const InputIterator& first,
        const InputIterator& last,
        const OutputIterator& result)
{
  using T = typename InputIterator::value_type;
  std::transform(first,last,result,[&](const T& x){return this->compute(x);});
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
void gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>::
compute(const std::size_t& size,
        const Scalar* input,
        Scalar* output)
{
  for(std::size_t i=0;i<size;++i) output[i] = compute(input[i]);
}

#endif // GAMMATONE_FILTER_HPP
