/*
  Copyright (C) 2014, 2015 Mathieu Bernard <mathieu_bernard@laposte.net>

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

#ifndef UTILS_SINGLETON_HPP
#define UTILS_SINGLETON_HPP

#include <boost/noncopyable.hpp>

namespace utils
{
  /*!
    \brief Provides singleton-specific services to a client class.

    \tparam Client The client class to be tuned in singleton

    Usage
    -----

    The singleton is templated by a client class. It ensures that this
    client class have ONE AND ONLY ONE instance running in the
    program, from first access to program end. The client instance is
    accessible at any point with the instance() method. This
    implementation is not thread safe, see reference below for more
    details.
    
    ~~~
    MyClient& client = utils::singleton<MyClient>::instance();
    ~~~

    The client class
    ----------------

    This client class must take all the precautionary measures against
    unattended construction and destruction:

    - The default constructor and destructor, as well as copy
    operators should be made private.

    - You also have to grant friendship to the singleton you use. The
    singleton can then call the private constructor and destructor of
    the client.

    - These protective measures and the friend declaration are all the
    changes needed for a class to work with the singleton. Here is a
    minimal client prototype :

    ~~~ 
    #include <utils/singleton.hpp>
    class MyClient : private boost::noncopyable
    { 
    private:    
    friend class utils::singleton<MyClient> 
    MyClient();
    ~MyClient();    
    ...
    }; 
    ~~~

    \see Modern C++ Design: Generic Programming and Design
    Patterns Applied, Andrei Alexandrescu, Addison Wesley, 2001.
  */
  template<class Client>
  class singleton : private boost::noncopyable
  {
  public:
    //! Returns a reference to the client instance
    static Client& instance();

  private:
    singleton();
    ~singleton();

    //! Destroys the holded instance
    static void destroy_singleton();
    
    //! A pointer to the olded instance
    static Client* p_instance;
  };
}

template<class Client>
Client* utils::singleton<Client>::p_instance = 0;

template<class Client>
Client& utils::singleton<Client>::instance()
{
  if(! p_instance)
    {
      p_instance = new Client;
      atexit(destroy_singleton);
    }

  return *p_instance;
}

template<class Client>
void utils::singleton<Client>::destroy_singleton()
{
  delete p_instance;
  p_instance = 0;
}

#endif // UTILS_SINGLETON_HPP
