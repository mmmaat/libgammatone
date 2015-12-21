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

#ifndef GAMMATONE_DETAIL_DECIBEL_HPP
#define GAMMATONE_DETAIL_DECIBEL_HPP

#include <gammatone/detail/utils.hpp>
#include <vector>
#include <algorithm>

namespace gammatone
{
    namespace detail
    {

        //! Convert values in the range [first,last) in a decibel scale
        /*!  

          Given \f$ X=(x_i) \f$ the values in [first,last) and its
          absolute max \f$ M = max_i(|x_i|) \f$, this function converts the
          range \f$X\f$ in its decibel representation: \f$ \forall x_i \in X,
          x_i \leftarrow 20log\frac{x_i}{M}\f$.

          \attention the values in \f$X\f$ must be positive. Else an error
          occurs (see std::log).

          \tparam Iterator  An iterator to the range to convert.

          \param first  Iterator to the first position in range
          \param last  Iterator to the last position in range      
        */
        template<class Iterator>
        inline void decibel(const Iterator& first, const Iterator& last){
            using T = typename Iterator::value_type;
  
            gammatone::detail::normalize(first,last);
            std::for_each(first, last, [&](T& x){x = 20*std::log(x);});
        }


        //! Find the position in a signal where it reaches a given attenuation (in dB)
        /*!

          Given a signal \f$X=(x_i)\f$ and its absolute max \f$ M =
          max_i(|x_i|)\f$, this function returns the index \f$I_\alpha\f$
          where the signal envelope reach the attenuation level \f$ \alpha
          \f$ such as \f$I_\alpha = min_i(\{i|\alpha <
          20log\frac{|x_i|}{M}\})\f$

          \tparam Container The type of the signal container. Must have
          bidirectional iterator.

          \param signal  The processed input signal \f$ X=\{x_i\} \f$
          \param level The attenuation level \f$ \alpha \f$ to search for
          (dB). No effect if positive.

          \return A const iterator where the signal attenuation reach the
          given level. If the attenuation is not reached, return signal.cend()
        */
        template<class Container>
        typename Container::const_iterator find_attenuation(const Container& signal,
                                                            const typename Container::value_type& level){
            using T = typename Container::value_type;

            // deal with silly cases
            if(level>0) return signal.cend();

            // compute partial max from end to beginning of the IR
            std::vector<T> partial_max(signal.size());
            T max = 0.0;
            std::transform(signal.rbegin(),signal.rend(),partial_max.rbegin(),
                           [&](const T& x){if(std::abs(x)>max) max = std::abs(x); return max;});

            // normalize the max
            std::for_each(partial_max.begin(),partial_max.end(),[&](T& x){x/=max;});

            // find cutoff position in partial_max
            const T cutoff = std::pow(10,level/20.0);
            const auto cutoff_it = std::find_if(partial_max.begin(),partial_max.end(),
                                                [&](const T& x){return x <= cutoff;});

            // convert it to an iterator on signal
            auto res_it = signal.cbegin() + std::distance(partial_max.begin(),cutoff_it);

            // If the attenuation level is not reached, return signal.cend()
            if(res_it == signal.cbegin())
                return signal.cend();
            return res_it;
        }


        //! Shrink a signal up to a given attenuation (in dB)
        /*!

          Reduce the signal up to the position returned by
          find_attenuation(signal,level)
      
          \tparam Container Type of container to be processed

          \param signal  The processed input signal \f$ X=\{x_t\} \f$
          \param level The attenuation level \f$ \alpha \f$ to search for
          (dB). No effect if positive.

        */
        template<class Container>
        void shrink_to_attenuation(Container& signal,
                                   const typename Container::value_type& level){
            const auto it = gammatone::detail::find_attenuation(signal, level);

            if(it != signal.cbegin())
            {
                signal.assign(signal.cbegin(),it);
                signal.shrink_to_fit( );
            }
        }

    }
}

#endif // GAMMTONE_DETAIL_DECIBEL_HPP
