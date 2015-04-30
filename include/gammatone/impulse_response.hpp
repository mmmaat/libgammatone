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

#ifndef GAMMATONE_IMPULSE_RESPONSE_H
#define GAMMATONE_IMPULSE_RESPONSE_H

#include <vector>
#include <algorithm>
#include <boost/math/constants/constants.hpp> // for pi

namespace gammatone
{
  //! Impulse response generation
  /*!
    \class impulse_response

    This class provide static methods for impulse response
    generation. Both theorical and obtained impulse responses are
    provided.
  */
  class impulse_response
  {
  public:
    /*!
      Returns a container of timestamps for a given filter and duration.

      \tparam F Type of the input gammatone filter.
      \tparam U Type of the output container

      \param filter    The gammatone filter to be processed.
      \param duration  The duration for computing the timestamps (in s)

      \return A container of timestamps (in s) of size duration*filter.sample_frequency()+1
    */
    template<class F, class U = std::vector<typename F::scalar_type> >
    static U time(const F& filter, const typename F::scalar_type& duration);


    /*!
      Compute the theorical impulse response of a given filter

      \tparam F Type of the input gammatone filter
      \tparam U Type of the output paired containers

      \param filter   The gammatone filter to be processed
      \param duration The duration for computing the timestamps (in s)

      \return A pair p of containers where p.first is
      time(filter,duration) and p.second is the computed impulse
      response for each timestamp.

      \post p.first.size() == p.second.size()
    */
    template<class F, class U = std::vector<typename F::scalar_type> >
    static typename std::pair<U,U> theorical(const F& filter,
                                             const typename F::scalar_type& duration );


    /*!
      Compute the theorical impulse response of a given filter

      \tparam F Type of the input gammatone filter
      \tparam U Type of the output container

      \param filter The gammatone filter to be processed
      \param begin  Begin iterator on the timestamps container
      \param end    End iterator on the timestamps container

      \return A container c of the computed impulse response for each
      timestamp.

      \post c.size() == time.size()
    */
    template<class F, class U = std::vector<typename F::scalar_type>, class I = typename U::const_iterator >
    static U theorical( const F& filter, const I& begin, const I& end );


    /*!
      Compute the implemented impulse response of a given filter

      \tparam F Type of the input gammatone filter
      \tparam I Type of the timestamps iterator
      \tparam U Type of the output container

      \param filter The gammatone filter to be processed
      \param time   The timestamps or duration

      \return A container c of the computed impulse response for each
      timestamp.

      \post c.size() == time.size()
      \post filter has been reseted
    */
    template<class F, class U = std::vector<typename F::scalar_type>, class I = typename U::iterator >
    static U implemented( F& filter, const I& begin, const I& end );


    /*!
      Compute the implemented impulse response of a given filter

      \tparam F Type of the input gammatone filter
      \tparam U Type of the output paired containers

      \param filter   The gammatone filter to be processed
      \param begin  Begin iterator on the timestamps container
      \param end    End iterator on the timestamps container

      \return A pair p of containers where p.first is
      time(filter,duration) and p.second is the computed impulse
      response for each timestamp.

      \post p.first.size() == p.second.size()
      \post filter has been reseted
    */
    template<class F, class U = std::vector<typename F::scalar_type> >
    static typename std::pair<U,U> implemented( F& filter,const typename F::scalar_type& duration );


  protected:
    /*!
      Compute the theorical impulse response of a gammatone filter.

      \tparam F Type of the input gammatone filter

      \param filter The gammatone filter to be analyzed
      \param time   The timestamp for which to compute the result (s)
      \param phase  The phase for which to compute the result (rad)

      \return The computed impulse response value
    */
    template<class F>
    static inline typename F::scalar_type formula_ir(const F& filter,
                                                     const typename F::scalar_type& time,
                                                     const typename F::scalar_type& phase = 0);
  };
}


template<class F, class U>
U gammatone::impulse_response::time(const F& filter, const typename F::scalar_type& duration)
{
  typedef typename F::scalar_type T;

  // allocation of timestamps container
  U time(filter.sample_frequency()*duration+1);

  // generation of timestamps
  T t=0; const T dt = duration / static_cast<T>(time.size()-1);
  std::for_each(time.begin(),time.end(),[&](auto& x){x = t; t += dt;});

  return std::move(time);
}



template<class F, class U, class I>
U gammatone::impulse_response::theorical(const F& filter, const I& begin, const I& end)
{
  U ir(std::distance(begin,end));
  std::transform(begin,end,ir.begin(),[&](const auto& t){return formula_ir(filter,t);});
  return std::move(ir);
}


template<class F, class U>
std::pair<U,U> gammatone::impulse_response::theorical(const F& filter, const typename F::scalar_type& duration)
{
  U t = time(filter, duration);
  return std::move(std::make_pair(t, theorical(filter,t.begin(), t.end())));
}


template<class F, class U, class I>
U gammatone::impulse_response::implemented(F& filter, const I& begin, const I& end)
{
  // reset the filter before computation
  filter.reset();

  // allocation for output
  U ir(std::distance(begin,end));

  // compute the first sample (= 1)
  auto it = ir.begin();
  *it++ = filter.compute(1.0);

  // compute other samples (= 0)
  std::for_each(it,ir.end(),[&](auto& x){x = filter.compute(0.0);});

  filter.reset();
  return std::move(ir);
}


template<class F, class U>
std::pair<U,U> gammatone::impulse_response::implemented(F& filter, const typename F::scalar_type& duration)
{
  U t = time(filter, duration);
  return std::move(std::make_pair(t, implemented(filter,t.begin(), t.end())));
}


template<class F>
typename F::scalar_type gammatone::impulse_response::formula_ir
(const F& filter, const typename F::scalar_type& time, const typename F::scalar_type& phase)
{
  const int order = 4;
  const auto tt = 2 * boost::math::constants::pi<typename F::scalar_type>() * time;
  return /*filter.gain()* */
    pow(time, order-1) * exp(-tt*filter.bandwidth()) * cos(tt*filter.center_frequency()+phase);
}


#endif // GAMMATONE_IMPULSE_RESPONSE_H
