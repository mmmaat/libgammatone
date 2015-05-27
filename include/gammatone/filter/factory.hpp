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

#ifndef GAMMATONE_FILTER_FACTORY_HPP
#define GAMMATONE_FILTER_FACTORY_HPP

#include <gammatone/filter/interface.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <gammatone/detail/singleton.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <memory>
#include <string>
#include <map>

namespace gammatone
{
  namespace filter
  {
    template<class Scalar>
    class factory : private boost::noncopyable
    {
    public:
      //! Shared pointer to concrete filter
      typedef std::shared_ptr<filter::interface<Scalar> > shared_ptr;
      
      //! Creates a concrete filter from a parameter string
      /*!
        \param sample_frequency  The processing sample frequency (Hz)
        \param center_frequency  The filter center frequency (Hz)
        \param params            Parameters string

        \return A shared pointer to the created filter
      */
      shared_ptr create(const Scalar& sample_frequency,
                        const Scalar& center_frequency,
                        const std::string& params);

    private:
      // singleton design
      friend class detail::singleton<factory>;
      factory();
      virtual ~factory();

      //! Keys type of the factory map
      typedef std::string key_type;

      //! Value type of the factory map
      typedef boost::function<shared_ptr(const Scalar&, const Scalar&)>  value_type;

      //! Type of the factory map
      typedef std::map<key_type,value_type> factory_map;

      template<class ConcreteType>
      bool register_type(const key_type& key);

      bool unregister_type(const key_type& key);

      key_type parse(const std::string& key) const
      {
        if(key=="slaney1993")
          return key;
        return "cooke1993";
      }

      factory_map m_map;
    };
  }
}

template<class Scalar>
gammatone::filter::factory<Scalar>::factory()
{
  using namespace gammatone;
  register_type<filter::concrete<Scalar,core::cooke1993 > >( "cooke1993");
  register_type<filter::concrete<Scalar,core::slaney1993 > >("slaney1993");
}

template<class Scalar>
gammatone::filter::factory<Scalar>::~factory()
{}

template<class Scalar>
template<class ConcreteFilter>
bool gammatone::filter::factory<Scalar>::register_type(const key_type& key)
{
  const auto f = boost::factory<std::shared_ptr<ConcreteFilter> >();
  return m_map.insert(typename factory_map::value_type(key, f)).second;
}

template<class Scalar>
bool gammatone::filter::factory<Scalar>::unregister_type(const key_type& key)
{
  return m_map.erase(key) == 1;
}

template<class Scalar>
typename gammatone::filter::factory<Scalar>::shared_ptr gammatone::filter::factory<Scalar>::
create(const Scalar& sample_frequency,
       const Scalar& center_frequency,
       const std::string& key)
{
  return m_map.at(parse(key))(sample_frequency,center_frequency);
}

#endif // GAMMATONE_FILTER_FACTORY_HPP
