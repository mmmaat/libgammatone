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

#ifndef UTILS_RANGE_HPP
#define UTILS_RANGE_HPP

#include <algorithm>
#include <vector>
#include <random>
#include <memory>

namespace utils
{
  //! Generate a linear sequence of values
  /*!
    \tparam Value      Type of values
    \tparam Container  Type of the returned container

    \param min   minimum value of the sequence
    \param max   maximum value of the sequence
    \param step  Interval between two successive values

    \return The generated linear sequence
  */
  template<class Value,
           class Container = std::vector<Value> >
  inline Container range(const Value& min,
                         const Value& max,
                         const Value& step = 1);

  //! Generate a linear space between min and max
  /*!
    \tparam Value      Type of values
    \tparam Container  Type of the returned container

    \param min   minimum value of the sequence
    \param max   maximum value of the sequence
    \param size  number of values in the sequence

    \return  The generated linear space
  */
  template<class Value,
           class Container = std::vector<Value> >
  inline Container linspace(const Value& min,
                            const Value& max,
                            const std::size_t& size = 50);

  //! Generate a logarithmic space between base^min and base^max
  /*!
    \tparam Value      Type of values
    \tparam Container  Type of the returned container

    \param min   minimum value of the sequence
    \param max   maximum value of the sequence
    \param size  number of values in the sequence
    \param base  base for the power computation

    \return  The generated logarithmic space
  */
  template<class Value,
           class Container = std::vector<Value> >
  inline Container logspace(const Value& min,
                            const Value& max,
                            const std::size_t& size = 50,
                            const Value& base = 10);


  //! Generate a range of random values
  /*!
    \tparam Value         Type of values
    \tparam Distribution  Type of random distribution
    \tparam Container     Type of the returned container

    \param min   minimum value of the sequence
    \param max   maximum value of the sequence
    \param size  number of values in the sequence

    \return  The generated random space
   */
  template<class Value,
           class Distribution = std::uniform_real_distribution<Value>,
           class Container = std::vector<Value> >
  inline Container random(const Value& min,
                          const Value& max,
                          const std::size_t& size = 50);
}


template<class T, class C>
inline C utils::range(const T& min, const T& max, const T& step)
{
  size_t size = (max-min)/step + 1;
  C out(size);

  auto val = static_cast<typename C::value_type>(min);
  auto s = static_cast<typename C::value_type>(step);
  std::for_each(out.begin(),out.end(),[&](auto& x){x=val;val+=s;});
  return std::move(out);
}


template<class Value, class Container>
Container utils::linspace(const Value& min,
			  const Value& max,
			  const std::size_t& size)
{
  Container data(size);
  Value current = min;
  Value step = (max - min) / static_cast<Value>(size-1);
  std::for_each(data.begin(),data.end(),[&](auto& x){x = current; current += step;});
  return std::move(data);
}


template<class Value, class Container>
Container utils::logspace(const Value& min,
			  const Value& max,
			  const std::size_t& size,
			  const Value& base)
{
  auto data = linspace(min, max, size);
  std::for_each(data.begin(),data.end(),[&](auto& x){x = pow(base,x);});
  return std::move(data);
}


template<class Value, class Distribution, class Container>
Container utils::random(const Value& min,
                        const Value& max,
                        const std::size_t& size)
{
  std::default_random_engine generator;
  Distribution distribution(min,max);
  Container out(size);
  std::for_each(out.begin(),out.end(),[&](auto& x){x = distribution(generator);});
  return std::move(out);
}


#endif // UTILS_RANGE_HPP
