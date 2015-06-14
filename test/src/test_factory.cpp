#include <gammatone/filter/concrete.hpp>
#include <gammatone/policy/postprocessing.hpp>
#include <gammatone/detail/impulse_response.hpp>

#include <boost/mpl/string.hpp>
#include <boost/mpl/map.hpp>
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <boost/fusion/include/pair.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include <test_setup.hpp>

namespace mpl = boost::mpl;
namespace pp = gammatone::policy::postprocessing;

template<class F1, class F2>
void plot_ir(F1 f1, F2 f2)
{
  using T = typename F1::element_type::scalar_type;
  const T duration = 0.01;

  using ir = gammatone::impulse_response;
  const auto t = ir::time(f1->sample_frequency(),duration);
  const auto ir1 = ir::implemented(*f1,t.begin(),t.end());
  const auto ir2 = ir::implemented(*f2,t.begin(),t.end());

  Gnuplot gp;
  gp << std::ifstream(gpsetup).rdbuf() << std::endl
     << "set xlabel 'time (s)'" << std::endl
     << "set ylabel 'amplitude'" << std::endl
     << "set xrange [0:"<<duration<<"]" << std::endl
     << "plot '-' u 1:2 w l ls 11 t 'f1', "
     << "     '-' u 1:2 w l ls 12 t 'f2'"
     << std::endl;
  gp.send1d(std::make_pair(t,ir1));
  gp.send1d(std::make_pair(t,ir2));
}


// class pp_off : private boost::noncopyable
// {
// public:
//template<class T> using type = gammatone::policy::postprocessing::off<T>;
  //using t_name = mpl::string<'o','f','f'>;
  //static const std::string s_name;
// };


// class pp_hwr : private boost::noncopyable
// {
// public:
//using type = gammatone::policy::postprocessing::hwr;
  //  using t_name = mpl::string<'h','w','r'>;
  //static const std::string s_name;
// };

// const std::string pp_off::s_name = "off"; 
// const std::string pp_hwr::s_name = "hwr";


// template<class T>
// using pp_map =  mpl::map< mpl::pair<pp_off::t_name, pp_off::type<T> >,
// 			  mpl::pair<pp_hwr::t_name, pp_hwr::type<T> > >;

// template<class T, class Name>
// using get_type = typename mpl::at<pp_map<T>,Name >::type;

template<class T> using type_off = gammatone::policy::postprocessing::off<T>;
template<class T> using type_hwr = gammatone::policy::postprocessing::hwr<T>;


template<class T>
std::shared_ptr<gammatone::filter::interface<T> >
create_filter(const T& fs, const T& fc, const std::string param = "")
{

  // using pp_type = std::conditional
  //   <"" == "hwr", type_hwr, type_off>::type;
  
  using namespace gammatone;
  using concrete = filter::concrete
    <T,
     core::cooke1993,
     policy::bandwidth::glasberg1990,
     policy::gain::forall_0dB,
     policy::clipping::off,
     type_off
     //     get_type<double,pp_off::t_name>::type
     >;
  
  return boost::factory<std::shared_ptr<concrete> >()(fs,fc);
}


template<class T>
using off_pair = boost::fusion::pair<gammatone::policy::postprocessing::off<T>,std::string>;
	

int main(int argc, char** argv)
{
  using T = double;
  const T fs = 44100, fc = 3000;

  //  off_pair<T>::first_type::process(1);
  
  //  auto off_pair = boost::fusion::make_pair<type_off<T>>("oo");
  
  using namespace gammatone;
  using concrete1 = filter::concrete
    <T,
     core::cooke1993,
     policy::bandwidth::glasberg1990,
     policy::gain::forall_0dB,
     policy::clipping::off,
     off_pair::first_type
     >;
  auto f1 = boost::factory<std::shared_ptr<concrete1> >()(fs,fc);

  // using concrete2 = filter::concrete
  //   <T,
  //    core::cooke1993,
  //    policy::bandwidth::glasberg1990,
  //    policy::gain::forall_0dB,
  //    policy::clipping::off,
  //    get_type<T,pp_hwr::t_name>::type
  //    >;
  // auto ctor = boost::factory<std::shared_ptr<concrete2> >();
  // std::shared_ptr<gammatone::filter::interface<T> > f2 = ctor(fs,fc);
  //std::shared_ptr<gammatone::filter::interface<T> > f3 = create_filter(fs,fc);

  // // auto f1 = create_filter<T>(44100,2000,"off");
  // // auto f2 = create_filter<T>(44100,2000,"hwr");
  // plot_ir(f1,f2);

  return 0;
}
