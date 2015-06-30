/*
  Copyright (C) 2015 Mathieu Bernard <mathieu_bernard@laposte.net>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more utilss.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMATONE_DETAIL_UTILS_HPP
#define GAMMATONE_DETAIL_UTILS_HPP

#include <vector>
#include <algorithm>
#include <cmath>

namespace gammatone
{
  namespace detail
  {
    //! Generate a linear space between min and max
    /*!

      Generate a container of *size* points linearly spaced between
      and including *min* and *max*. For *size* = 1, linspace returns *max*.

      \tparam Value      Type of values
      \tparam Container  Type of the returned container

      \param min   minimum value of the sequence
      \param max   maximum value of the sequence
      \param size  number of values in the sequence

      \return  The generated linear space
    */
    template<class Value,
             template<class...> class Container = std::vector >
    inline Container<Value> linspace(const Value& min,
                                     const Value& max,
                                     const std::size_t& size = 100);

    //! Return the maximum absolute value in a range
    /*!
      \tparam Iterator A forward iterator. Can be
      const_iterator. std::abs is used so it's works on complex.

      \param first  First position in the range
      \param last   Last position in the range

      \return The maximum absolute value in the range
      [first,last). Returns 0 if first == last.
    */
    template<class Iterator>
    typename Iterator::value_type absmax(const Iterator& first,
                                         const Iterator& last);

    //! In place normalization of a range of values.
    /*!

      Normalizes the range [first,last) in the interval [-1,1].

      \tparam Iterator  A forward iterator

      \param first  Forst iterator of the range
      \param last   Last iterator of the range
    */
    template<class Iterator>
    inline void normalize(const Iterator& first,
                          const Iterator& last);
  }
}


template<class Value,
         template<class...> class Container>
Container<Value> gammatone::detail::linspace(const Value& min,
                                             const Value& max,
                                             const std::size_t& size)
{
  // check for special case
  if(size == 1)
  return Container<Value>(1,max);

  // Build linspace
  Container<Value> space(size);
  const Value k = (max-min)/(size-1);
  for(size_t i=0; i<size; i++) space[i] = min + k*i;

  return space;
}


template<class Iterator>
typename Iterator::value_type gammatone::detail::
absmax(const Iterator& first, const Iterator& last)
{
  // detect empty range
  if(first==last) return 0;

  // find min and max
  const auto minmax = std::minmax_element(first,last);

  // return max(|min|,|max|)
  return std::max(std::abs(*minmax.first),std::abs(*minmax.second));
}

template<class Iterator>
void gammatone::detail::
normalize(const Iterator& first, const Iterator& last)
{
  // first pass to find range absmax
  const auto max = absmax(first,last);

  // detect range of zeros and empty range
  if(max == 0) return;

  // normalization factor
  const auto factor = 1.0 / max;

  // second pass to normalize the range by this factor
  using T = typename Iterator::value_type;
  std::for_each(first,last,[&](T& v){v*=factor;});
}

#endif // GAMMATONE_DETAIL_UTILS_HPP
