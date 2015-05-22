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

#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <vector>
#include <algorithm>
#include <cmath>


namespace utils
{

  //! Return the maximum absolute value in a range
  /*!
    \tparam I Type of the iterator. Can be const_iterator. std::abs is
    used so it's works on complex.

    \param begin Begin iterator of the range
    \param end   End iterator of the range
    \return The maximum absolute value in a range
  */
  template<class I>
  inline typename I::value_type absmax(const I& begin, const I& end);


  //! In place normalization of a range of values.
  /*!
    Normalizes a range of values in the interval [-1,1].

    \tparam I Type of the iterator

    \param begin Begin iterator of the range
    \param end   End iterator of the range

    \post utils::absmax(begin,end) == 1
  */
  template<class I>
  inline void normalize(const I& begin, const I& end);


  template<class Iterator>
  inline void decibel(const Iterator& first, const Iterator& last)
  {
    normalize(first,last);
    std::for_each(first, last, [&](auto& x){x = 20*std::log(x);});
  }

  /*!
    Return the Mean Squared Error from two range of values.

    The two ranges must have the same size. Let X and Y be 2 ranges of
    n values. MSE = $\frac{1}{n}\sum_{i=1}^n(X_i-Y_i)^2$.

    \tparam I Type of the iterator. Can be const_iterator.

    \param xbegin Begin iterator of the first range
    \param xend   End iterator of the first range
    \param ybegin Begin iterator of the second range
    \return The Mean Squared Error
  */
  template<class I1, class I2>
  inline typename I1::value_type mean_squared_error(const I1& xbegin, const I1& xend, const I2& ybegin);


  //! Find the position where a signal reachs a given attenuation
  /*!
    \tparam Signal The type of the signal container. Must have
    bidirectionnal iterator.

    \param signal  The processed input signal
    \param level   The attenuation level to search for (dB). Must be negative.

    \return A const iterator to the signal position where attenuation reach
    the given level.

    \todo test it !
  */
  template<class Container>
  typename Container::const_iterator find_attenuation(const Container& signal,
                                                      const typename Container::value_type& level)
  {
    typedef typename Container::value_type T;

    // compute partial max from end to begining of the IR
    std::vector<T> partial_max(signal.size());
    T max = 0.0;
    std::transform(signal.rbegin(),signal.rend(),partial_max.rbegin(),
                   [&](const auto& x){if(std::abs(x)>max) max = std::abs(x); return max;});

    // normalize the max
    std::for_each(partial_max.begin(),partial_max.end(),[&](auto& x){x/=max;});

    // find cutoff position in partial_max
    const T cutoff = std::pow(10,level/20.0);
    const auto cutoff_it = std::find_if(partial_max.begin(),partial_max.end(),
                                        [&](const auto& x){return x <= cutoff;});

    // convert it to an iterator on signal
    return signal.cbegin() + std::distance(partial_max.begin(),cutoff_it);
  }


  template<class Container>
  void shrink_to_attenuation(Container& signal,
                             const typename Container::value_type& level)
  {
    const auto it = utils::find_attenuation(signal, level);

    if(it != signal.cbegin())
      {
        signal.assign(signal.cbegin(),it);
        signal.shrink_to_fit();
      }
  }
}



template<class I>
typename I::value_type utils::absmax(const I& begin, const I& end)
{
  const auto minmax = std::minmax_element(begin,end);
  return std::max(std::abs(*minmax.first),std::abs(*minmax.second));
}


template<class I>
void utils::normalize(const I& begin, const I& end)
{
  // first pass to find normalization factor in range
  const auto factor = 1.0 / absmax(begin,end);

  // second pass to normalize the range by this factor
  std::for_each(begin,end,[&](auto& v){v*=factor;});
}


template<class I1, class I2>
typename I1::value_type utils::mean_squared_error(const I1& xbegin, const I1& xend, const I2& ybegin)
{
  typedef typename I1::value_type T;
  T out = 0;
  T size=0;
  auto yit = ybegin;

  std::for_each(xbegin,xend,[&](const auto& x){out += std::pow(x - *(yit++), 2);size++;});

  return std::move(out/size);
}


#endif // UTILS_UTILS_HPP
