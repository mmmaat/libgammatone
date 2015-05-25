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

#include <boost/test/unit_test.hpp>
#include <gammatone/detail/singleton.hpp>

bool dummy_created = false;

class dummy : boost::noncopyable
{
private:
  friend class gammatone::detail::singleton<dummy>;
  dummy(){dummy_created = true;}
  ~dummy(){dummy_created = false;}
};

typedef gammatone::detail::singleton<dummy> singleton;

BOOST_AUTO_TEST_SUITE(detail_singleton)

//================================================

  BOOST_AUTO_TEST_CASE(instance_test)
{
  BOOST_CHECK_EQUAL(false, dummy_created);
  dummy& d1 = singleton::instance();
  BOOST_CHECK_EQUAL(true, dummy_created);
  dummy& d2 = singleton::instance();
  BOOST_CHECK_EQUAL(&d1,&d2);
  BOOST_CHECK_EQUAL(true, dummy_created);
}

BOOST_AUTO_TEST_SUITE_END()
