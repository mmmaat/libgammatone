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

#ifndef UTILS_CHRONO_HPP
#define UTILS_CHRONO_HPP

//#include <utils/duration.h>
#include <chrono>

namespace utils
{
  //! A chronometer based on the system clock
  class chrono
  {
  public:
    
    //! Default constructor
    /*!
      Initialize a chronometer at zero
    */
    chrono() : m_total( 0 ) {}

    //! Destructor
    ~chrono(){}

    //! Start the chronometer now
    inline void start()
    {
      m_start = std::chrono::system_clock::now();
    }

    //! Stop the chronometer now
    /*!
      \post The ellapsed time can now be retrieved by get()
    */
    inline void stop()
    {
      m_end = std::chrono::system_clock::now();
      m_total += m_end - m_start;
    }

    //! Restart the chronometer from zero
    inline void reset()
    {
      m_total -= m_total;
      start();
    }

    //! Return the ellapsed time between start() and stop() in ms
    /*!
      \return 0 if stop() have not been called, total ellapsed time (ms) else.
    */
    inline double count() const
    {
      return m_total.count();
    }

  private:
    //! The begin timestamp
    std::chrono::time_point<std::chrono::system_clock> m_start;

    //! The end timestamp
    std::chrono::time_point<std::chrono::system_clock> m_end;

    //! The total ellapsed time
    std::chrono::duration<double,std::milli> m_total;

  };
}

#endif // UTILS_CHRONO_HPP
