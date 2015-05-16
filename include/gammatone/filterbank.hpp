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

#ifndef GAMMATONE_FILTERBANK_HPP
#define GAMMATONE_FILTERBANK_HPP

#include <gammatone/interface.hpp>
#include <gammatone/filter.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <utility>
#include <vector>
#include <list>

namespace gammatone
{
  //! Gammatone filterbank
  /*!
    \class filterbank

    Implementation of a gammatone filterbank.

    \tparam Scalar           Type of scalar values.
    \tparam Core             Type of the filter core. See gammatone::core.
    \tparam BandwidthPolicy  Policy for computing filters bandwidth. See policy::bandwidth.
    \tparam ChannelsPolicy   Policy for computing center frequencies. See policy::channels.
  */
  template<class Scalar,
           class Core = core::cooke1993<Scalar>,
           class BandwidthPolicy = policy::bandwidth::glasberg1990<Scalar>,
           class ChannelsPolicy = policy::channels::fixed_size<Scalar> >
  class filterbank : public interface<Scalar, std::vector<Scalar> >
  {
  public:

    //! Type of the output container
    typedef std::vector<Scalar> container_type;

    //! Type of the underlying gammatone filter
    typedef filter<Scalar,Core,BandwidthPolicy> filter_type;

    //! Type of the underlying bank of filters
    typedef std::list<filter_type> bank_type;

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
    filterbank(const Scalar& sample_frequency,
               const Scalar& low_frequency,
               const Scalar& high_frequency);


    //! Create a gammatone filterbank from explicit parameters.
    /*!
      \param sample_frequency    The sample frequency of the input signal (Hz)
      \param low_frequency       The lowest center frequency in the filterbank (Hz)
      \param high_frequency      The highest center frequency in the filterbank (Hz)
      \param channels_parameter  If channels::fixed_size is used,
      channels_parameters is the number of frequency channels in the
      filterbank. If channels::fixed_overlap is used, it corresponds
      to the overlap factor.
    */
    filterbank(const Scalar& sample_frequency,
               const Scalar& low_frequency,
               const Scalar& high_frequency,
               const typename ChannelsPolicy::param_type& channels_parameter);

    //! Copy constructor
    filterbank(const filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>& other);

    //! Assignment operator
    filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>& operator=
    (const filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>& other);

    //! Destructor.
    virtual ~filterbank();

    //! Compute a multichannel output state from a scalar input.
    /*!
      \tparam PostProcessingPolicy Type of policy for output
      postprocessing. See policy::postprocessing.

      \param input  The scalar value to be processed by the filterbank.
    */
    template <class PostProcessingPolicy = policy::postprocessing::off<Scalar> >
    inline container_type compute(const Scalar& input);


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
      \tparam NestedContainer  Type of the result container. Must be a container of containers.

      \param input The input signal to be processed
      \return The output processed signal
    */
    template<class PostProcessingPolicy = policy::postprocessing::off<Scalar>,
             class NestedContainer = std::vector<container_type> >
    NestedContainer compute(const typename NestedContainer::value_type& input);


    //! Accessor to the number of frequency channels in the bank.
    /*!
      \return The number of channels in the filterbank.
    */
    std::size_t nb_channels() const;


    // methods inherited from interface
    Scalar sample_frequency() const;
    container_type center_frequency() const;
    container_type bandwidth() const;
    container_type gain() const;
    void reset();

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

  protected:

    //! The number of frequency channels
    std::size_t m_nb_channels;

    //! The underlying gammatone filter array
    bank_type m_bank;
  };
}


template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy>
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
filterbank(const Scalar& sample_frequency,
           const Scalar& low_cf,
           const Scalar& high_cf)
{
  const std::vector<Scalar> cf = ChannelsPolicy::template center_frequency<BandwidthPolicy>(low_cf,high_cf);
  m_nb_channels = cf.size();

  std::for_each(cf.begin(),cf.end(), [&](const auto& f)
                {m_bank.push_back( filter_type( sample_frequency, f));});
}


template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy>
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
filterbank(const Scalar& sample_frequency,
           const Scalar& low_cf,
           const Scalar& high_cf,
           const typename ChannelsPolicy::param_type& channels_parameter)
{
  const std::vector<Scalar> cf =
    ChannelsPolicy::template center_frequency<BandwidthPolicy>(low_cf,high_cf,channels_parameter);

  m_nb_channels = cf.size();
  std::for_each(cf.begin(),cf.end(), [&](const auto& f)
                {m_bank.push_back( filter_type( sample_frequency, f));});
}


template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy>
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::filterbank
(const gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>& other)
  : m_nb_channels( other.m_nb_channels ),
    m_bank( other.m_bank )
{}

template<class Scalar, class Core, class BandwidthPolicy, class ChannelsPolicy>
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>&
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::operator=
(const gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>& other)
{
  gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy> tmp( other );

  std::swap( m_nb_channels, other.m_nb_channels );
  std::swap( m_bank, other.m_bank );

  return *this;
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::~filterbank()
{}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
void gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::reset()
{
  for(auto& b:m_bank) b.reset();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
Scalar gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
sample_frequency() const
{
  return begin()->sample_frequency();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
std::vector<Scalar> gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
center_frequency() const
{
  std::vector<Scalar> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](const auto& f){return f.center_frequency();});
  return std::move(out);
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
std::vector<Scalar> gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
bandwidth() const
{
  std::vector<Scalar> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](const auto& f){return f.bandwidth();});
  return std::move(out);
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
std::size_t gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
nb_channels() const
{
  return m_nb_channels;
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
std::vector<Scalar> gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
gain() const
{
  std::vector<Scalar> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](const auto& f){return f.gain();});
  return std::move(out);
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::const_iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::begin() const
{
  return m_bank.begin();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::const_iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::end() const
{
  return m_bank.end();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::const_reverse_iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::rbegin() const
{
  return m_bank.rbegin();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::const_reverse_iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::rend() const
{
  return m_bank.rend();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::begin()
{
  return m_bank.begin();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::end()
{
  return m_bank.end();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::reverse_iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::rbegin()
{
  return m_bank.rbegin();
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
typename gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::reverse_iterator
gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::rend()
{
  return m_bank.rend();
}



template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy> template<class PostProcessingPolicy>
std::vector<Scalar> gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
compute(const Scalar& input)
{
  std::vector<Scalar> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](auto& f){return f.template compute<PostProcessingPolicy>(input);});
  return std::move(out);
}


template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
template<class PostProcessingPolicy, class InputIterator, class OutputIterator>
void gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
compute(const InputIterator& first, const InputIterator& last, const OutputIterator& result)
{
  std::transform(first,last,result,[&](const auto& x){return this->compute<PostProcessingPolicy>(x);});
}

template<class Scalar,class Core,class BandwidthPolicy, class ChannelsPolicy>
template<class PostProcessingPolicy, class NestedContainer>
NestedContainer gammatone::filterbank<Scalar,Core,BandwidthPolicy,ChannelsPolicy>::
compute(const typename NestedContainer::value_type& input)
{
  // Allocation of output nested container
  NestedContainer out(input.size());
  std::for_each(out.begin(),out.end(),[&](auto& x){x.assign(this->nb_channels(),0);});

  this->compute<PostProcessingPolicy>(input.begin(),input.end(),out.begin());
  return std::move(out);
}


#endif // GAMMATONE_FILTERBANK_HPP
