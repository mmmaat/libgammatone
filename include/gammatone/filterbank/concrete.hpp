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

#ifndef GAMMATONE_FILTERBANK_CONCRETE_HPP
#define GAMMATONE_FILTERBANK_CONCRETE_HPP

#include <gammatone/filterbank/interface.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/order.hpp>
#include <utility>
#include <vector>

namespace gammatone
{
  namespace filterbank
  {
    //! Concrete gammatone filterbank
    /*!
      \class concrete

      Implementation of a gammatone filterbank.

      \tparam Scalar  Type of scalar values.
      \tparam Core    Type of the filter core. See gammatone::core.
      \tparam BandwidthPolicy  Policy for computing filters bandwidth. See policy::bandwidth.
      \tparam ChannelsPolicy   Policy for computing center frequencies. See policy::channels.
      \tparam PostProcessingPolicy  Type of policy for output postprocessing. Default is to do nothing.
    */
    template
    <
      class Scalar,
      template<class...> class Core               = core::cooke1993,
      template<class> class BandwidthPolicy       = policy::bandwidth::glasberg1990,
      template<class,template<class> class,class> class ChannelsPolicy     = policy::channels::fixed_size,
      class OrderPolicy                           = policy::order::increasing,
      class GainPolicy                            = policy::gain::forall_0dB,
      class ClippingPolicy                        = policy::clipping::off,
      template<class> class PostProcessingPolicy  = policy::postprocessing::off
      >
    class concrete : public gammatone::filterbank::interface<Scalar>
    {
    private:

      //! Type of *this
      using concrete_type =
        concrete<Scalar,
                 Core,
                 BandwidthPolicy,
                 ChannelsPolicy,
                 OrderPolicy,
                 GainPolicy,
                 ClippingPolicy,
                 PostProcessingPolicy>;

      //! Type of the parameter for channels computation
      using param_type = typename ChannelsPolicy<Scalar,BandwidthPolicy,OrderPolicy>::param_type;

    public:

      //! Type of the underlying gammatone filters
      using filter_type = gammatone::filter::
        concrete<Scalar,
                 Core,
                 BandwidthPolicy,
                 GainPolicy,
                 ClippingPolicy,
                 PostProcessingPolicy>;

      //! Type of the output container
      using output_type = typename gammatone::filterbank::interface<Scalar>::output_type;

      //! Type of the underlying bank of filters
      using bank_type = std::vector<filter_type >;

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
      concrete(const Scalar& sample_frequency,
               const Scalar& low_frequency,
               const Scalar& high_frequency,
               const param_type& channels_parameter = ChannelsPolicy<Scalar,BandwidthPolicy,OrderPolicy>::default_parameter());

      //! Copy constructor
      concrete(const concrete_type& other);

      //! Assignment operator
      concrete_type& operator=(const concrete_type& other);

      //! Destructor.
      virtual ~concrete();

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
      // inherited from filterbank::interface
      inline output_type compute_internal(const Scalar& input);

      //! The number of frequency channels
      std::size_t m_nb_channels;

      //! The filterbank overlap factor
      Scalar m_overlap;

      //! The underlying gammatone filter array
      bank_type m_bank;
    };
  }
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
concrete(const Scalar& sample_frequency,
         const Scalar& low_cf,
         const Scalar& high_cf,
         const param_type& channels_parameter)
{
  const auto p = ChannelsPolicy<Scalar,BandwidthPolicy,OrderPolicy>::setup(low_cf,high_cf,channels_parameter);
  const auto& cf = p.first;

  this->m_overlap = p.second;
  this->m_nb_channels = cf.size();

  std::for_each(cf.begin(),cf.end(), [&](const auto& f)
                {this->m_bank.push_back( filter_type( sample_frequency, f));});
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
concrete(const concrete_type& other)
: m_nb_channels(other.m_nb_channels),
  m_overlap(other.m_overlap),
  m_bank(other.m_bank)
{}


template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>&
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
operator=(const concrete_type& other)
{
  concrete_type tmp( other );

  std::swap( this->m_nb_channels, other.m_nb_channels );
    std::swap( this->m_overlap, other.m_overlap );
  std::swap( this->m_bank, other.m_bank );

  return *this;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
~concrete()
{}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
void gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
reset()
{
  for(auto& b:this->m_bank) b.reset();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
Scalar gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
sample_frequency() const
{
  return this->begin()->sample_frequency();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::output_type
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
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
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::output_type
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
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
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::output_type
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
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
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
std::size_t gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
nb_channels() const
{
  return m_nb_channels;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
Scalar gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
overlap() const
{
  return m_overlap;
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::const_iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
begin() const
{
  return m_bank.begin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::const_iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
end() const
{
  return m_bank.end();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::const_reverse_iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
rbegin() const
{
  return m_bank.rbegin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::const_reverse_iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
rend() const
{
  return m_bank.rend();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
begin()
{
  return m_bank.begin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
end()
{
  return m_bank.end();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::reverse_iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
rbegin()
{
  return m_bank.rbegin();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::reverse_iterator
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
rend()
{
  return m_bank.rend();
}

template
<
  class Scalar,
  template<class...> class Core,
  template<class> class BandwidthPolicy,
  template<class,template<class> class,class> class ChannelsPolicy,
  class OrderPolicy,
  class GainPolicy,
  class ClippingPolicy,
  template<class> class PostProcessingPolicy
  >
typename gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::output_type
gammatone::filterbank::
concrete<Scalar,
         Core,
         BandwidthPolicy,
         ChannelsPolicy,
         OrderPolicy,
         GainPolicy,
         ClippingPolicy,
         PostProcessingPolicy>::
compute_internal(const Scalar& input)
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](auto& f){return f.compute(input);});
  return std::move(out);
}

#endif // GAMMATONE_FILTERBANK_CONCRETE_HPP
