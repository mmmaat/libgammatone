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

#ifndef GAMMATONE_FILTERBANK_CONCRETE_HPP
#define GAMMATONE_FILTERBANK_CONCRETE_HPP

#include <gammatone/filterbank/interface.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/core/cooke1993.hpp>
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
    template<
      class Scalar,
      class Core = core::cooke1993<Scalar>,
      class BandwidthPolicy = policy::bandwidth::glasberg1990<Scalar>,
      class ChannelsPolicy = policy::channels::fixed_size<Scalar>,
      class PostProcessingPolicy = policy::postprocessing::off<Scalar>
      >
    class concrete : public ::gammatone::filterbank::interface<Scalar>
    {
    public:

      //! Type of the underlying gammatone filter
      typedef ::gammatone::filter::concrete<Scalar,Core,BandwidthPolicy,PostProcessingPolicy> filter_type;

      //! Type of the output container
      typedef typename ::gammatone::filterbank::interface<Scalar>::output_type output_type;

      //! Type of the underlying bank of filters
      typedef std::vector<filter_type > bank_type;

      //! Const iterator on filters
      typedef typename bank_type::const_iterator const_iterator;

      //! Const reverse iterator on filters
      typedef typename bank_type::const_reverse_iterator const_reverse_iterator;

      //! Iterator on filters
      typedef typename bank_type::iterator iterator;

      //! Reverse iterator on filters
      typedef typename bank_type::reverse_iterator reverse_iterator;


      //! Create a gammatone filterbank from explicit parameters.
      /*!
        \param sample_frequency    The sample frequency of the input signal (Hz)
        \param low_frequency       The lowest center frequency in the filterbank (Hz)
        \param high_frequency      The highest center frequency in the filterbank (Hz)
      */
      concrete(const Scalar& sample_frequency,
               const Scalar& low_frequency,
               const Scalar& high_frequency);


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
               const typename ChannelsPolicy::param_type& channels_parameter);

      //! Copy constructor
      concrete(const concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>& other);

      //! Assignment operator
      concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>& operator=
      (const concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>& other);

      //! Destructor.
      virtual ~concrete();

      // methods inherited from filterbank::interface
      Scalar sample_frequency() const;
      output_type center_frequency() const;
      output_type bandwidth() const;
      output_type gain() const;
      void reset();


      //! Accessor to the number of frequency channels in the bank.
      /*!
        \return The number of channels in the filterbank.
      */
      std::size_t nb_channels() const;

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

      //! The underlying gammatone filter array
      bank_type m_bank;
    };
  }
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
concrete(const Scalar& sample_frequency,
         const Scalar& low_cf,
         const Scalar& high_cf)
{
  const std::vector<Scalar> cf = ChannelsPolicy::template center_frequency<BandwidthPolicy>(low_cf,high_cf);
  this->m_nb_channels = cf.size();

  std::for_each(cf.begin(),cf.end(), [&](const auto& f)
                {this->m_bank.push_back( filter_type( sample_frequency, f));});
}


template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
concrete(const Scalar& sample_frequency,
         const Scalar& low_cf,
         const Scalar& high_cf,
         const typename ChannelsPolicy::param_type& channels_parameter)
{
  const std::vector<Scalar> cf =
    ChannelsPolicy::template center_frequency<BandwidthPolicy>(low_cf,high_cf,channels_parameter);
  this->m_nb_channels = cf.size();

  std::for_each(cf.begin(),cf.end(), [&](const auto& f)
                {this->m_bank.push_back( filter_type( sample_frequency, f));});
}


template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
concrete(const concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>& other)
  : m_nb_channels(other.m_nb_channels),
    m_bank(other.m_bank)
{}


template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>&
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
operator=(const concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>& other)
{
  concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy> tmp( other );

  std::swap( this->m_nb_channels, other.m_nb_channels );
  std::swap( this->m_bank, other.m_bank );

  return *this;
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
~concrete()
{}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
void gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
reset()
{
  for(auto& b:this->m_bank) b.reset();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
Scalar gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
sample_frequency() const
{
  return this->begin()->sample_frequency();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::output_type
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
center_frequency() const
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](const auto& f){return f.center_frequency();});
  return std::move(out);
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::output_type
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
bandwidth() const
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](const auto& f){return f.bandwidth();});
  return std::move(out);
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::output_type
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
gain() const
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](const auto& f){return f.gain();});
  return std::move(out);
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
std::size_t gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
nb_channels() const
{
  return m_nb_channels;
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::const_iterator
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
begin() const
{
  return m_bank.begin();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::const_iterator
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
end() const
{
  return m_bank.end();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::const_reverse_iterator
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
rbegin() const
{
  return m_bank.rbegin();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::const_reverse_iterator
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
rend() const
{
  return m_bank.rend();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::iterator
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
begin()
{
  return m_bank.begin();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::iterator
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
end()
{
  return m_bank.end();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
reverse_iterator gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
rbegin()
{
  return m_bank.rbegin();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
reverse_iterator gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
rend()
{
  return m_bank.rend();
}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy, class PostProcessingPolicy>
typename gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::output_type
gammatone::filterbank::concrete<Scalar,Core,BandwidthPolicy,ChannelsPolicy,PostProcessingPolicy>::
compute_internal(const Scalar& input)
{
  output_type out(this->nb_channels());
  std::transform(this->begin(),this->end(),out.begin(),
                 [&](auto& f){return f.compute(input);});
  return std::move(out);
}

#endif // GAMMATONE_FILTERBANK_CONCRETE_HPP

// template<class Scalar>
// template<class InputIterator, class OutputIterator>
// void gammatone::filterbank::interface<Scalar>::
// compute(const InputIterator& first, const InputIterator& last, const OutputIterator& result)
// {
//   std::transform(first,last,result,[&](const auto& x){return this->compute(x);});
// }

// template<class Scalar>
// template<class NestedContainer>
// NestedContainer gammatone::filterbank::interface<Scalar>::
// compute(const typename NestedContainer::value_type& input)
// {
//   // Allocation of output nested container
//   NestedContainer out(input.size());
//   std::for_each(out.begin(),out.end(),[&](auto& x){x.assign(this->nb_channels(),0);});

//   this->compute(input.begin(),input.end(),out.begin());
//   return std::move(out);
// }
