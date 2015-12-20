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

#include <gammatone/core/slaney1993.hpp>
#include <gammatone/core/cooke1993.hpp>
#include <gammatone/core/convolution.hpp>
#include <gammatone/policy/bandwidth.hpp>
#include <gammatone/policy/channels/fixed_size.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/timer/timer.hpp>
#include <test_setup.hpp>
#include <vector>
#include <tuple>


// Scalar values are double
using scalar = double;

// Bandwidth policy
template<class X> using bandwidth_policy = gammatone::policy::bandwidth::glasberg1990<X>;

// Store core types in a named tuple
using core1 = gammatone::core::cooke1993<scalar>;
using core2 = gammatone::core::slaney1993<scalar>;
using core3 = gammatone::core::convolution<scalar>;
const auto cores =
    std::make_tuple(boost::fusion::make_pair<core1>("      cooke"),
                    boost::fusion::make_pair<core2>("     slaney"),
                    boost::fusion::make_pair<core3>("convolution"));

// Running at 44.1kHz
const scalar fs = 44100;

// during 100 ms
const scalar duration = 0.1;

const std::size_t nbc = 10;
const scalar fl = 1e2, fh = 1e4;

// Center frequencies tested for the core (Hz)
const std::vector<scalar> fc = gammatone::policy::channels::
    fixed_size<scalar,bandwidth_policy>::setup(fl,fh,nbc).first;

// size in sample of the input
const scalar size = fs * duration;

// core name, mean and variance of core computing time
struct result_type
{
  std::string name;
  std::vector<scalar> mean;
  std::vector<scalar> var;
};

class chrono
{
public:

  //! Setup the chrono for a given number of repetitions
  chrono(const std::size_t repeat = 100)
      : m_repeat(repeat)
    {}

  //! Get the number of repetitions
  std::size_t repeat() const
    {
        return m_repeat;
    }

  //! Compute repeatedly a given input on a given core
  template<class P>
  result_type process(const std::vector<scalar>& input, P& p) const
    {
      using core = typename P::first_type;

      result_type res;
      res.name = p.second;

      for(const scalar& f : fc)
      {
        // Bandwidth of the core (Hz)
        const scalar bw = bandwidth_policy<scalar>::bandwidth(f);

        // Setup the core
        core c(fs,f,bw);

        // Get computing time for each repetition
        std::vector<scalar> elapsed(m_repeat);
        for(std::size_t i=0; i<m_repeat; ++i)
        {
          c.reset();

          boost::timer::cpu_timer timer;
          std::for_each(input.begin(),input.end(),[&](const scalar& x){c.compute(x);});
          elapsed[i] = static_cast<scalar>(timer.elapsed().wall);
        }

        // Return core name, mean and std of computing time (in ms)
        const auto stat = utils::mean_variance(elapsed.begin(),elapsed.end());
        res.mean.push_back(stat.first/1e6);
        res.var.push_back(std::sqrt(stat.second)/1e6);
      }

      return res;
    }

private:
  const std::size_t m_repeat;
};


int main(int argc, char** argv)
{
  using std::get;
  using std::cout;
  using std::endl;

  // init the chrono for repeating 1000 times
  chrono c(1000);

  // setup test input signal
  // const std::vector<scalar> input = utils::pulse<scalar>(size);
  const std::vector<scalar> input = utils::random<scalar>(-1.0, 1.0, size);

  // estimate computing time for each core type
  std::vector<result_type> res;
  res.push_back(c.process(input,get<0>(cores)));
  res.push_back(c.process(input,get<1>(cores)));
  res.push_back(c.process(input,get<2>(cores)));

  // plot computing time in function of center frequency for all cores
  Gnuplot gp;
  gp << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set xlabel 'center frequency (Hz)'" << std::endl
     << "set ylabel 'processing time (ms)'" << std::endl
     << "plot ";

  gp << "'-' u 1:2 w l ls 11 t '" << res[0].name << "', "
     << "'-' u 1:2 w l ls 12 t '" << res[1].name << "', "
     << "'-' u 1:2 w l ls 13 t '" << res[2].name << "'"
     << std::endl;

  gp.send1d(std::make_pair(fc,res[0].mean));
  gp.send1d(std::make_pair(fc,res[1].mean));
  gp.send1d(std::make_pair(fc,res[2].mean));


  // // get faster core
  // std::vector<scalar> t(res.size());
  // std::transform(res.begin(),res.end(),t.begin(),[&](const auto& x){return get<1>(x);});
  // scalar min_time = *std::min_element(t.begin(),t.end());

  // // compute relative results
  // std::vector<scalar> relative(res.size());
  // std::transform(res.begin(),res.end(),relative.begin(),[&](const auto& x){return get<1>(x)/min_time;});

  // display results
  std::size_t idx = nbc/2;
  cout << "duration = " << duration*1e3 << " ms, "
       << "center frequency = " << fc[idx]
       << ", repetitions = " << c.repeat()
       << endl;

  std::for_each(res.begin(),res.end(),[&](const result_type& x)
                {cout << x.name << ": "          // core type
                      << x.mean[idx]             // computing time
                      << " ms " << endl;});      // in microsecond

  return 0;
}
