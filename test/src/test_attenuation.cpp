//#include <gammatone/gammatone.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/detail/impulse_response.hpp>
#include <gnuplot-iostream.h>
#include <string>
#include <iostream>
#include <limits>

using ir = gammatone::impulse_response;

template<class Filter, class Container, class IRBase>
void plot(const Filter& f, const Container& ref, const IRBase& base, const Container& db)
{
  const std::string gnuplot_setup = "/home/mathieu/dev/libgammatone/share/setup.gp";

  Gnuplot gp;//(stdout);
  gp << std::ifstream(gnuplot_setup).rdbuf() << std::endl
     << "set xlabel 'time (s)'" << std::endl
     << "set ylabel 'amplitude'" << std::endl
     << "plot ";

  const auto fs = f.sample_frequency();
  const double duration = (ref.size()-1)/fs;

  // plot reference IR
  gp << "'-' u 1:2 w l ls 11 t 'reference', ";

  // plot with attenuation
  for(std::size_t i = 0; i < base.size()-1; i++)
    gp << "'-' u 1:2 w l ls " << 12+i << " t '" << (int)db[i] << " dB', ";
  gp << "'-' u 1:2 w l ls " << 12+base.size() << " t '" << (int)*db.rbegin() << " dB'" << std::endl;

  // send data to gnuplot
  gp.send1d(std::make_pair(ir::time(fs,duration), ref));
  for(const auto& ir : base)
    gp.send1d(std::make_pair(ir::time(fs,(ir.size()-1)/fs), ir));
}


int main()
{
  // the gammatone filter
  const gammatone::filter::concrete<double> f(44100, 1000);

  // the reference impulse response (0.1 second long)
  const double max_duration = 0.1;
  const auto ir_ref = ir::theorical(f,max_duration);

  // IR for different attenuation levels (in db)
  std::vector<double> db = {-200,-100,-60,-30,-20,-10,-6,-3};
  std::vector<std::vector<double> > ir_db;
  for(auto level : db) ir_db.push_back(ir::theorical_attenuate(f,level,max_duration));

  plot(f,ir_ref,ir_db,db);

  return 0;
}
