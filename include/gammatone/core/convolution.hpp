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

#ifndef GAMMATONE_CORE_CONVOLUTION_HPP
#define GAMMATONE_CORE_CONVOLUTION_HPP

#include <gammatone/impulse_response.hpp>

namespace gammatone
{
  //! Namespace composed of different gammatone filter implementations
  /*!
    \namespace 
   */
  namespace core
  {
    //! Gammatone filter implementation by convolution from theorical impulse response
    template<class T>
    class convolution
    {
    public:
     void reset();

      template<class F>
      void init(const F& filter);

      template<class F>
      T find_gain(const F& filter) const;

      template<class F>
      inline T compute(const F& filter, const T& input);
    };
  }
}



template<class T> template<class F>
void gammatone::core::convolution<T>::init(const F& filter)
{

}

template<class T> template<class F>
T gammatone::core::convolution<T>::find_gain(const F& filter) const
{
  return 1;
}

template<class T>
void gammatone::core::convolution<T>::reset()
{
  
}


template<class T> template<class F>
T gammatone::core::convolution<T>::compute(const F& filter, const T& input)
{
  return 0;
}


#endif // GAMMATONE_CORE_CONVOLUTION_HPP
