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

#ifndef GAMMATONE_FILTERBANK_HPP
#define GAMMATONE_FILTERBANK_HPP

#include <gammatone/interface.hpp>
#include <gammatone/filter.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/gain.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/clipping.hpp>
#include <vector>

namespace gammatone
{
  //! Gammatone filterbank implementation
  /*!
    \class filterbank gammatone/filterbank.hpp

    Implementation of a gammatone filterbank.

    \tparam Scalar           Type of scalar values
    \tparam Core             See gammatone::core
    \tparam ChannelsPolicy   See policy::channels
    \tparam GainPolicy       See policy::gain
    \tparam BandwidthPolicy  See policy::bandwidth
    \tparam ClippingPolicy   See policy::clipping
  */
  template
  <
    class Scalar,
    template<class...> class Core                              = gammatone::core::cooke1993,
    template<class,template<class> class> class ChannelsPolicy = gammatone::policy::channels::fixed_size,
    class GainPolicy                                           = gammatone::policy::gain::forall_0dB,
    template<class> class BandwidthPolicy                      = gammatone::policy::bandwidth::glasberg1990,
    class ClippingPolicy                                       = gammatone::policy::clipping::off
    >
  class filterbank : public gammatone::interface<Scalar, std::vector<Scalar> >
  {
  public:

    //! Type of *this
    using type = filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>;

    //! Type of the channels policy
    using channels = ChannelsPolicy<Scalar,BandwidthPolicy>;

    //! Type of the underlying gammatone filters
    using filter = gammatone::filter<Scalar,Core,BandwidthPolicy,ClippingPolicy>;


    //! Type of the output container
    using output_type = typename gammatone::interface<Scalar,std::vector<Scalar> >::output_type;

    //! Type of the underlying bank of filters
    using bank_type = std::vector<filter >;

    //! Const iterator on filters
    using const_iterator = typename bank_type::const_iterator;

    //! Const reverse iterator on filters
    using const_reverse_iterator = typename bank_type::const_reverse_iterator;

    //! Iterator on filters
    using iterator = typename bank_type::iterator;

    //! Reverse iterator on filters
    using reverse_iterator = typename bank_type::reverse_iterator;


    //! Create a gammatone filterbank from explicit parameters.
    /*!
      \param sample_frequency    The sample frequency of the input signal (Hz)
      \param low_frequency       The lowest center frequency in the filterbank (Hz)
      \param high_frequency      The highest center frequency in the filterbank (Hz)

      \param channels_parameter If channels::fixed_size is used,
      channels_parameters is the number of frequency channels in the
      filterbank. If channels::fixed_overlap is used, it corresponds
      to the overlap factor.
    */
    filterbank(const Scalar& sample_frequency,
               const Scalar& low_frequency,
               const Scalar& high_frequency,
               const typename channels::param_type& channels_parameter = channels::default_parameter());

    //! Copy constructor
    filterbank(const type& other);

    //! Move constructor
    filterbank(type&& other);

    //! Assignment operator
    type& operator=(const type& other) noexcept;

    //! Move operator
    type& operator=(type&& other);

    //! Destructor.
    virtual ~filterbank();

    // methods inherited from filterbank::interface
    Scalar sample_frequency() const;
    output_type center_frequency() const;
    output_type bandwidth() const;
    output_type gain() const;
    void reset();


    //! The number of frequency channels in the filterbank.
    /*!
      \return The number of channels in the filterbank.
    */
    std::size_t nb_channels() const;


    //! The bandwidth overlap factor between two successive filters.
    /*!

      An overlap factor close to zero indicate filters nearly
      completly overlapped, an overlap factor of 0.5 means that each
      frequency in the input signal is "sampled" by two cochlear
      channels, and an overlap factor of 1 means that there is almost
      no overlap between channels.

      \return The filterbank overlap factor

      \see policy::channels
    */
    Scalar overlap() const;

    //! Const iterator to begin
    const_iterator begin() const;

    //! Const iterator to end
    const_iterator end() const;

    //! Reverse const iterator on begin
    const_reverse_iterator rbegin() const;

    //! Reverse const iterator on end
    const_reverse_iterator rend() const;

    //! Iterator on begin
    iterator begin();

    //! Iterator on end
    iterator end();

    //! Reverse iterator on begin
    reverse_iterator rbegin();

    //! Reverse iterator on end
    reverse_iterator rend();


  private:
    // inherited from interface
    inline output_type compute_internal(const Scalar& input);

    //! The number of frequency channels
    std::size_t m_nb_channels;

    //! The filterbank overlap factor
    Scalar m_overlap;

    //! The underlying gammatone filter array
    bank_type m_bank;
  };
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
filterbank(const Scalar& sample_frequency,
           const Scalar& low_cf,
           const Scalar& high_cf,
           const typename channels::param_type& channels_parameter)
{
  const auto p = ChannelsPolicy<Scalar,BandwidthPolicy>::setup(low_cf,high_cf,channels_parameter);
  const auto& cf = p.first;

  this->m_overlap = p.second;
  this->m_nb_channels = cf.size();

  std::for_each(cf.begin(),cf.end(), [&](const auto& f)
                {this->m_bank.push_back( filter( sample_frequency, f));});
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
filterbank(const type& other)
  : m_nb_channels(other.m_nb_channels),
    m_overlap(other.m_overlap),
    m_bank(other.m_bank)
{}


template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
filterbank(type&& other)
  : m_nb_channels(std::move(other.m_nb_channels)),
    m_overlap(std::move(other.m_overlap)),
    m_bank(std::move(other.m_bank))
{}


template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>&
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
operator=(const type& other) noexcept
{
  type tmp( other );

  std::swap( this->m_nb_channels, tmp.m_nb_channels );
  std::swap( this->m_overlap, tmp.m_overlap );
  std::swap( this->m_bank, tmp.m_bank );

  return *this;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>&
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
operator=(type&& other)
{
  this->m_nb_channels = std::move(other.m_nb_channels);
  this->m_overlap = std::move(other.m_overlap);
  this->m_bank = std::move(other.m_bank);

  return *this;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
~filterbank()
{}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
void gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
reset()
{
  for(auto& b:this->m_bank) b.reset();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
Scalar gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
sample_frequency() const
{
  return this->begin()->sample_frequency();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::output_type
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
center_frequency() const
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](const auto& f){return f.center_frequency();});
  return std::move(out);
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::output_type
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
bandwidth() const
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](const auto& f){return f.bandwidth();});
  return std::move(out);
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::output_type
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
gain() const
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](const auto& f){return f.gain();});
  return std::move(out);
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
std::size_t
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
nb_channels() const
{
  return m_nb_channels;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
Scalar
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
overlap() const
{
  return m_overlap;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::const_iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
begin() const
{
  return m_bank.begin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::const_iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
end() const
{
  return m_bank.end();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
const_reverse_iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
rbegin() const
{
  return m_bank.rbegin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
const_reverse_iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
rend() const
{
  return m_bank.rend();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
begin()
{
  return m_bank.begin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
end()
{
  return m_bank.end();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::reverse_iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
rbegin()
{
  return m_bank.rbegin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::reverse_iterator
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
rend()
{
  return m_bank.rend();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class,template<class> class> class ChannelsPolicy,
  class GainPolicy,
  template<class> class BandwidthPolicy,
  class ClippingPolicy
  >
typename gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::output_type
gammatone::filterbank<Scalar,Core,ChannelsPolicy,GainPolicy,BandwidthPolicy,ClippingPolicy>::
compute_internal(const Scalar& input)
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](auto& f){return f.compute(input);});
  return std::move(out);
}

#endif // GAMMATONE_FILTERBANK_HPP
