/*
  Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>

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

#include <gammatone/detail/interface.hpp>
#include <boost/test/unit_test.hpp>

template<class Scalar>
class child : public gammatone::detail::interface<Scalar,Scalar>
{
public:
    child(const Scalar& s) : gammatone::detail::interface<Scalar,Scalar>(s) {}

private:
    Scalar center_frequency() const {return Scalar();}
    Scalar bandwidth() const {return Scalar();}
    Scalar gain() const {return Scalar();}
    void reset(){}
    void compute(const Scalar& x, Scalar& y){}
    void compute_ptr(const std::size_t& s, const Scalar* x, Scalar* y){}
};

BOOST_AUTO_TEST_SUITE(interface_test)

//================================================

BOOST_AUTO_TEST_CASE(copy_works)
{
  child<int> c1(1);
  child<int> c2(c1);
  BOOST_CHECK_EQUAL(c1.sample_frequency(),c2.sample_frequency());
}

BOOST_AUTO_TEST_CASE(move_works)
{
  child<int> c1(1);
  child<int> c2(std::move(c1));
  BOOST_CHECK_EQUAL(1,c2.sample_frequency());
}


//================================================

BOOST_AUTO_TEST_CASE(sample_frequency_works)
{
  child<int> a(0), b(1), c(2);
  BOOST_CHECK_EQUAL(a.sample_frequency(),0);
  BOOST_CHECK_EQUAL(b.sample_frequency(),1);
  BOOST_CHECK_EQUAL(c.sample_frequency(),2);

  child<std::vector<bool> > d({true,false,true});
  BOOST_CHECK_EQUAL(d.sample_frequency().size(),3);
  BOOST_CHECK_EQUAL(d.sample_frequency()[0],true);
  BOOST_CHECK_EQUAL(d.sample_frequency()[1],false);
  BOOST_CHECK_EQUAL(d.sample_frequency()[2],true);
}

BOOST_AUTO_TEST_SUITE_END()
