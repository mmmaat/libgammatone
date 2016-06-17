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

#include <gammatone/policy/channels.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <test_setup.hpp>
#include <iostream>
using namespace std;

typedef double T;
const T fh = 8000;
const T fl = 100;
const T step_factor = 0.25;
//template<class... X> using B = gammatone::policy::bandwidth::glasberg1990<X...>;

int main()
{
  using fixed_size = gammatone::policy::channels::fixed_size
    < T, gammatone::policy::bandwidth::glasberg1990>;

  using fixed_overlap = gammatone::policy::channels::fixed_overlap
    < T, gammatone::policy::bandwidth::glasberg1990>;

  const auto cf1 = fixed_overlap::setup(fl,fh,step_factor).first;
  const auto cf2 = fixed_size::setup(fl,fh,cf1.size()).first;
  const auto cf3 = fixed_size::setup(fl,fh,30).first;
  const auto cf4 = fixed_overlap::setup(fl,fh,0.5).first;
  const auto cf5 = fixed_size::setup(fl,fh,50).first;

  Gnuplot gp;
  gp << ifstream(gpsetup).rdbuf() << endl
     << "set xlabel 'center frequency (Hz)'" << endl
     << "set ylabel '# channel'" << endl
     << "plot '-' u 1:2 w l ls 11 lw 2 t 'fixed overlap', "
     << "     '-' u 1:2 w l ls 12 lw 1 t 'fixed size', "
     << "     '-' u 1:2 w l ls 13 lw 1 t 'fixed size 30',  "
     << "     '-' u 1:2 w l ls 14 lw 1 t 'fixed overlap 0.5',  "
     << "     '-' u 1:2 w l ls 15 lw 1 t 'fixed size 50'  "
     << endl;

  gp.send1d(make_pair(cf1,utils::range<int>(1,cf1.size())));
  gp.send1d(make_pair(cf2,utils::range<int>(1,cf2.size())));
  gp.send1d(make_pair(cf3,utils::range<int>(1,cf3.size())));
  gp.send1d(make_pair(cf4,utils::range<int>(1,cf4.size())));
  gp.send1d(make_pair(cf5,utils::range<int>(1,cf5.size())));

  return 0;
}
