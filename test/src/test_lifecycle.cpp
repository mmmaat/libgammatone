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

#include <gammatone/filter.hpp>
#include <gammatone/filterbank.hpp>
#include <iostream>
using namespace std;

// this function has been tested with success as a protected member of
// filter::concrete to assert lifecycle in the concrete as well
void write(const std::string msg)
{
  cout << msg << endl;
}

class filter : public gammatone::filter<double>
{
  using concrete = gammatone::filter<double>;

public:
  filter(const double& fs, const double& fc)
    : concrete::filter(fs,fc)
  { write("explicit"); }

  filter(const filter& other)
    : concrete::filter(other)
  { write("copy ctor"); }

  filter(filter&& other)
    : concrete::filter(std::move(other))
  { write("move ctor"); }

  virtual ~filter()
  { write("dtor");}

};

int main()
{
  const double fs = 20000, fc = 3000;

  filter f(fs,fc);
  filter f2(f);
  filter f3(std::move(f));

  return 0;
}
