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

#ifndef LIBGAMMATONE_TEST_UTILS_HPP
#define LIBGAMMATONE_TEST_UTILS_HPP

// Some utilities function to generate tests

#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <boost/math/constants/constants.hpp>


template<class Value,
         class Distribution = std::uniform_real_distribution<Value>,
         class Container = std::vector<Value> >
Container random(const Value& min,
                 const Value& max,
                 const std::size_t& size = 100)
{
  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  Distribution distribution(min,max);
  Container out(size);
  std::for_each(out.begin(),out.end(),[&](auto& x){x = distribution(generator);});
  return std::move(out);
}


template<class Container = std::vector<double> >
Container make_sinus(const typename Container::value_type& fs,
                     const typename Container::value_type& f,
		     const std::size_t size = 10000)
{
  typedef typename Container::value_type T;

  Container sinus(size);
  T t = 0;
  for_each(sinus.begin(),sinus.end(),[&](T& x){x=sin(2*boost::math::constants::pi<T>()*f*t++/fs);});
  return std::move(sinus);
}


//! Generate a linear sequence of values
/*!

  Generates a linear sequances of values in [min,max], each
  successive values separated by a given step. We must have min <
  max and step > 0.

  \tparam Value      Type of values
  \tparam Container  Type of the returned container

  \param min   minimum value of the sequence
  \param max   maximum value of the sequence
  \param step  Interval between two successive values

  \return The generated linear sequence
*/
template<class Value,
         class Container = std::vector<Value> >
Container range(const Value& min,
                const Value& max,
                const Value& step = 1)
{
  // detect incoherent parameters
  if(step <= 0 || min > max)
    return Container();

  // val is incremented by s until we reach max
  auto val = static_cast<typename Container::value_type>(min);
  const auto s = static_cast<typename Container::value_type>(step);

  // push values up to max
  Container out;
  while(val<=max)
    {
      out.push_back(val);
      val += s;
    }

  return std::move(out);
}


//! Generate a logarithmic space between base^min and base^max
/*!

  The logspace function generates a container of logarithmically
  spaced values. This is a logarithmic equivalent of utils::linspace.

  \tparam Value      Type of values
  \tparam Container  Type of the returned container

  \param min   minimum value of the sequence
  \param max   maximum value of the sequence
  \param size  number of values in the sequence
  \param base  base for the power computation

  \return  The generated logarithmic space
*/
template<class Value, class Container>
Container logspace(const Value& min,
                   const Value& max,
                   const std::size_t& size = 100,
                   const Value& base = 10)
{
  // detect silly base (avoid computation of 0^0 below)
  if(base == 0)
    return Container(size);

  // Compute underlying linspace
  auto space = linspace(min, max, size);

  // Raise to logspace
  std::for_each(space.begin(),space.end(),[&](auto& x){x = std::pow(base,x);});

  return std::move(space);
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
template<class Iterator1, class Iterator2>
typename Iterator1::value_type mean_squared_error(const Iterator1& xfirst,
                                                  const Iterator1& xlast,
                                                  const Iterator2& yfirst)
{
  typedef typename Iterator1::value_type T;

  // detect empty range
  if(xfirst==xlast) return 0;

  // Accumulation in out of squarred differences (x_i - y_i)^2
  T out = 0, size = 0; auto yit = yfirst;
  std::for_each(xfirst,xlast,[&](const auto& x){out += std::pow(x - *(yit++), 2);size++;});

  // normalize by the number of elements
  return std::move(out/size);
}

#endif // LIBGAMMATONE_TEST_UTILS_HPP
