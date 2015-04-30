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

#ifndef GAMMATONE_POLICY_CHANNELS_HPP
#define GAMMATONE_POLICY_CHANNELS_HPP


#include <algorithm>

namespace gammatone
{
  namespace policy
  {
    //! Policy for sorting frequency channels in increasing or decreasing order
    /*!
      \namespace gammatone::policy::channels

      This namespace provides a policy allowing to sort channels of a
      filterbank by increasing or decreasing frequency order. Whithin
      cochlea, the basilar membran decomposes sounds from high to low
      frequencies, from base to apex respectively. This results in
      frequency channels sorted in decreasing order. Nevertheless it
      can be more conveniant to have channels sorted in increasing
      order.

      - This policy is built on the classes channels::increasing and
      channels::decreasing giving access to an adapted iterator on
      channels.

      - Use this policy policy as a template parameter of filterbank

      ~~~
      using namespace gammatone;
      typedef filterbank<double, core::cooke1993<double>, policy::channels::increasing> increasing_filterbank;
      typedef filterbank<double, core::cooke1993<double>, policy::channels::decreasing> decreasing_filterbank;
      ~~~
    */
    namespace channels
    {
      //! Sort frequency channels by increasing order
      class increasing
      {
      public:
        //! Returns an iterator on the first (lowest) frequency channel
        /*!  
	  \tparam Container The type container on which iterate. Must
	  define a bidirectionnal iterator.

          \param container   The container on which iterate
          \return an iterator on the first frequency channel
        */
        template<class Container>
        static inline typename Container::const_reverse_iterator begin(const Container& container);

        //! Returns an iterator on the last (highest) frequency channel
        /*!
          \tparam Container  The type container on which iterate. Must
	  define a bidirectionnal iterator.

          \param container   The container on which iterate
          \return an iterator on the last frequency channel
        */
        template<class Container>
        static inline typename Container::const_reverse_iterator end(const Container& container);
      };

      //! Sort frequency channels by decreasing order
      class decreasing
      {
      public:
        //! Returns an iterator on the first (highest) frequency channel
        /*!
          \tparam Container  The type container on which iterate. Must
	  define a forward iterator.

          \param container   The container on which iterate
          \return an iterator on the first frequency channel
        */
        template<class Container>
        static inline typename Container::const_iterator begin(const Container& container);

        //! Returns an iterator on the last (lowest) frequency channel
        /*!
          \tparam Container  The type container on which iterate. Must
	  define a forward iterator.

          \param container   The container on which iterate
          \return an iterator on the last frequency channel
        */
        template<class Container>
        static inline typename Container::const_iterator end(const Container& container);
      };
    }
  }
}

template<class Container>
typename Container::const_reverse_iterator gammatone::policy::channels::increasing::
begin(const Container& container)
{
  return container.rbegin();
}

template<class Container>
typename Container::const_reverse_iterator gammatone::policy::channels::increasing::
end(const Container& container)
{
  return container.rend();
}

template<class Container>
typename Container::const_iterator gammatone::policy::channels::decreasing::begin(const Container& container)
{
  return container.begin();
}

template<class Container>
typename Container::const_iterator gammatone::policy::channels::decreasing::end(const Container& container)
{
  return container.end();
}



#endif // GAMMATONE_POLICY_CHANNELS_HPP
