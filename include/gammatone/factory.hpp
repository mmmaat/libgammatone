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

#ifndef GAMMATONE_FACTORY_HPP
#define GAMMATONE_FACTORY_HPP

#include <gammatone/filter/interface.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/slaney1993.hpp>
#include <utils/singleton/creation.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <memory>
#include <string>
#include <map>

namespace gammatone
{
  template<class Scalar>
  class factory : private boost::noncopyable
  {
  public:
    //! Type returned by the factory
    typedef std::shared_ptr<filter::interface<Scalar> > return_type;

    //! Keys type of the factory map
    typedef std::string key_type;

    //! Create a filter from a given string
    /*!
      \return A pointer to the created filter
     */
    return_type create(const Scalar& sample_frequency,
                       const Scalar& center_frequency,
                       const key_type& key);

  private:
    friend class utils::singleton::creation::with_new<factory<Scalar> >;
    factory();
    virtual ~factory();
    
    //! Value type of the factory map
    typedef boost::function<return_type(const Scalar&, const Scalar&)>  value_type;

    //! Type of the factory map
    typedef std::map<key_type,value_type> factory_map;

    template<class ConcreteFilter>
    bool record(const key_type& key);

    bool unrecord(const key_type& key);

    factory_map m_map;
  };
}

template<class Scalar>
gammatone::factory<Scalar>::factory()
{
  using namespace gammatone;
  record<filter::concrete<Scalar,core::cooke1993<Scalar> > >( "cooke1993");
  record<filter::concrete<Scalar,core::slaney1993<Scalar> > >("slaney1993");
}

template<class Scalar>
gammatone::factory<Scalar>::~factory()
{}

template<class Scalar>
template<class ConcreteFilter>
bool gammatone::factory<Scalar>::
record(const key_type& key)
{
  const auto f = boost::factory<std::shared_ptr<ConcreteFilter> >();
  return gammatone::factory<Scalar>::m_map.insert(typename factory_map::value_type(key, f)).second;
}

template<class Scalar>
bool gammatone::factory<Scalar>::
unrecord(const key_type& key)
{
  return gammatone::factory<Scalar>::m_map.erase(key) == 1;
}

template<class Scalar>
typename gammatone::factory<Scalar>::return_type gammatone::factory<Scalar>::
create(const Scalar& sample_frequency,
       const Scalar& center_frequency,
       const std::string& key)
{
  return gammatone::factory<Scalar>::m_map.at(key)(sample_frequency,center_frequency);
}

#endif // GAMMATONE_FACTORY_HPP
