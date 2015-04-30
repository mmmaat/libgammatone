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
#include <gammatone/policy/spacing.hpp>
#include <gammatone/policy/channels.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <vector>
#include <list>

namespace gammatone
{
  //! Gammatone filterbank
  /*!
    \class filterbank

    Implementation of a gammatone filterbank.

    \tparam Scalar  Type of scalars
    \tparam Core Type of the filter core
    \tparam ChannelPolicy Policy for sorting frequency channels (by increasing or decreasing order)
  */
  template<class Scalar,
	   class Core = core::cooke1993<Scalar>,
	   class ChannelPolicy = policy::channels::increasing >
  class filterbank : public interface<Scalar, std::vector<Scalar> >
  {
    //! Container type
    typedef std::vector<Scalar> Container;

    //! Underlying gammatone filter type
    typedef filter<Scalar,Core> Filter;

    //! Underlying filterbank
    typedef std::list<Filter> Bank;

  public:
    /*!
      Create a gammatone filterbank form explicit parameters.

      \param sample_frequency  The sample frequency of the input signal (Hz)
      \param low_frequency     The lowest center frequency in the filterbank (Hz)
      \param high_frequency    The highest center frequency in the filterbank (Hz)
      \param nb_channels       Number of frequency channels in the filterbank.
    */
    filterbank( const Scalar& sample_frequency,
                const Scalar& low_frequency,
                const Scalar& high_frequency,
                const std::size_t& nb_channels);

    //! Destructor.
    virtual ~filterbank();

    /*!
      Compute a multichannel output state from a scalar input.

      \tparam PostProcessingPolicy Type of policy for output
      postprocessing. Default is to do nothing.

      \param input  The scalar value to be processed by the filterbank.
    */
    template <class PostProcessingPolicy = policy::postprocessing::off<Scalar> >
    inline Container compute(const Scalar& input);

    /*!
      Accessor to the number of frequency channels in the bank.
      \return The number of channels in the filterbank.
    */
    std::size_t nb_channels() const;

    // inherited from interface
    Scalar sample_frequency() const;
    Container center_frequency() const;
    Container bandwidth() const;
    Container gain() const;
    void reset();

    // definition of iterators on filters
    typedef typename Bank::const_iterator const_iterator;
    typedef typename Bank::const_reverse_iterator const_reverse_iterator;
    typedef typename Bank::iterator iterator;
    typedef typename Bank::reverse_iterator reverse_iterator;

    const_iterator begin() const { return m_bank.begin(); }
    const_iterator end() const { return m_bank.end(); }

    const_reverse_iterator rbegin() const { return m_bank.rbegin(); }
    const_reverse_iterator rend() const { return m_bank.rend(); }

    iterator begin() { return m_bank.begin(); }
    iterator end() { return m_bank.end(); }

    reverse_iterator rbegin() { return m_bank.rbegin(); }
    reverse_iterator rend() { return m_bank.rend(); }

  protected:
    //! The number of frequency channels
    std::size_t m_nb_channels;

    //! The underlying gammatone filter array
    Bank m_bank;
  };
}


template<class T, class Core, class ChannelPolicy>
gammatone::filterbank<T,Core,ChannelPolicy>::filterbank
( const T& sample_frequency,
  const T& low_cf,
  const T& high_cf,
  const std::size_t& nb_channels)
  : m_nb_channels( nb_channels )
{
  const auto cf = gammatone::policy::spacing::fixed_size<T>::center_frequency(low_cf,high_cf,nb_channels);
  std::for_each(ChannelPolicy::begin(cf),ChannelPolicy::end(cf),
                [&](const auto f){m_bank.push_back( filter<T,Core>( sample_frequency, f));});
}


template<class T,class C,class O>
gammatone::filterbank<T,C,O>::~filterbank()
{}


template<class T,class C,class O>
void gammatone::filterbank<T,C,O>::reset()
{
  for(auto& b:m_bank) b.reset();
}


template<class T,class C,class O>
T gammatone::filterbank<T,C,O>::sample_frequency() const
{
  return begin()->sample_frequency();
}


template<class T,class C,class O>
std::vector<T> gammatone::filterbank<T,C,O>::center_frequency() const
{
  std::vector<T> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](const auto& f){return f.center_frequency();});
  return std::move(out);
}


template<class T,class C,class O>
std::vector<T> gammatone::filterbank<T,C,O>::bandwidth() const
{
  std::vector<T> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](const auto& f){return f.bandwidth();});
  return std::move(out);
}


template<class T,class C,class O>
std::size_t gammatone::filterbank<T,C,O>::nb_channels() const
{
  return m_nb_channels;
}


template<class T,class C,class O>
std::vector<T> gammatone::filterbank<T,C,O>::gain() const
{
  std::vector<T> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](const auto& f){return f.gain();});
  return std::move(out);
}


template<class T,class C,class O> template<class PostProcessingPolicy>
std::vector<T> gammatone::filterbank<T,C,O>::compute(const T& input)
{
  std::vector<T> out(nb_channels());
  std::transform(begin(),end(),out.begin(),[&](auto& f){return f.compute<PostProcessingPolicy>(input);});
  return std::move(out);
}

#endif // GAMMATONE_FILTERBANK_HPP
