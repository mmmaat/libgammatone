#include <gammatone/filterbank/concrete.hpp>
#include <utils/range.hpp>
#include <wav.hpp>
#include <chrono.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <gnuplot-iostream.h>
#include <string>
#include <iostream>
#include <array>

using namespace std;

typedef double T;

// The type of filterbank we are using...
typedef gammatone::core::cooke1993<T>                        core;
typedef gammatone::policy::bandwidth::glasberg1990<T>        bandwidth;
typedef gammatone::policy::channels::fixed_size<T>           channels;
typedef gammatone::policy::postprocessing::hwr<T>            postprocessing;
typedef gammatone::filterbank::concrete<T,core,bandwidth,channels,postprocessing> filterbank;

// WILL BE
//typedef gammatone::filterbank::holder<T> filterbank;
//filterbank fb(fs,fl,fh,"--core cooke1993 --bandwidth glasberg1990 --channels fixed --postprocessing hwr");

//! Gnuplot script file
string gnuplot_script = "/home/mathieu/dev/libgammatone/share/cochleogram.gp";

//! Input audio file name
string input_file;

//! Output PDF file
string output_file = "output.pdf";

//! Name of this program executable
string program_name;

//! Verbose flag
bool verb = false;

//! Normalization flag
bool normalize = false;



//! Parse command line parameters
int parse_parameters(int argc, char** argv);

//! Normalize each channel of the cochleogramm within [-1,1]
template<class T>
void normalization(T& data);




int main(int argc, char** argv)
{
  // exit if fails parsing the parameters
  if(parse_parameters(argc,argv))
    return -1;

  // open audio file
  vector<double> audio_data;
  double sample_frequency;

  // try to open input file as mono
  if(int error = utils::wav::read_mono(input_file, audio_data, sample_frequency))
    {
      // if not mono, try stereo and take left channel
      pair<vector<double>,vector<double> > stereo;
      if((error = utils::wav::read_stereo(input_file, stereo, sample_frequency)))
        return error;
      audio_data = stereo.first;
      cerr << input_file << "is a stereo file.\nRead left channel only." << endl;
    }

  // duration of the audio file (in s)
  const double duration = audio_data.size()/sample_frequency;

  if(verb)
    cout << "Input file:" << endl
         << "   " << input_file << endl
         << "   " << static_cast<float>(duration) << "s sampled at " << sample_frequency << " Hz."
         << endl;

  // filter input signal
  filterbank fb(sample_frequency,100,10000); 

  utils::chrono chrono;
  if( verb )
    {
      cout << "Gammatone processing:" << endl
           << "   " << fb.nb_channels() << " channels from "
           << (int)fb.begin()->center_frequency() << "Hz to "
           << (int)fb.rbegin()->center_frequency() << "Hz." << endl;
      chrono.start();
    }

  // processing
  vector<vector<double> > cochleogram(audio_data.size(),vector<double>(fb.nb_channels()));
  fb.compute(audio_data.begin(),audio_data.end(),cochleogram.begin());
  
  // normalization
  if(normalize)
    {
      if(verb) cout << "   normalization..." << endl;
      normalization(cochleogram);
    }


  if(verb)
    {
      chrono.stop();
      cout << "   processing took " << chrono.count() << " ms." << endl;
      cout << "Sending data to gnuplot..." << flush;
      chrono.reset();
    }

  // xaxis is time (in s)
  const auto xaxis = utils::linspace(0.0, duration, audio_data.size());

  // yaxis are center frequencies (in Hz)
  const auto yaxis = fb.center_frequency();

  // reshape data in a gnuplot compliant format (see gnuplot splot data-file)
  typedef vector<vector<array<double,3> > > data_type;
  data_type data;
  for(size_t i=0;i<xaxis.size();i++)
    {
      data_type::value_type tmp;
      for(size_t j=0;j<yaxis.size();j++)
        {
          data_type::value_type::value_type a;
          a[0] = xaxis[i];
          a[1] = yaxis[j];
          a[2] = cochleogram[i][j];
          tmp.push_back(a);
        }
      data.push_back(tmp);
    }

  // write gnuplot script and send data

  const auto xlimits = minmax_element( xaxis.begin(), xaxis.end() );
  //const auto ylimits = minmax_element( yaxis.begin(), yaxis.end() );
  Gnuplot gp;
  gp << std::ifstream(gnuplot_script).rdbuf() << std::endl
     << "set output '" << output_file << "'" << std::endl
     << "set xrange [" << *xlimits.first << " : " << *xlimits.second << "]" << endl
    //     << "set yrange [" << *ylimits.first << " : " << *ylimits.second << "]" << endl
     << "splot '-'" << endl
     << endl;

  gp.send2d(data);

  if(verb)
    {
      chrono.stop();

      cout << " took " << chrono.count()/1000 << " s." << endl
           << "Writing " << output_file << endl;
    }

  return 0;
}


int parse_parameters(int argc, char** argv)
{
  namespace po = boost::program_options;
  namespace fs = boost::filesystem;

  program_name = argv[0];

  // Declare the supported options
  po::options_description desc("Allowed OPTIONS");
  desc.add_options()
    ( "help,h", "produce help message" )
    ( "verbose,v", "enable verbosity" )
    ( "normalize,n", "normalize output" )
    ( "gnuplot-script,g", po::value<string>(), "path to the Gnuplot script" )
    ( "output-file,o", po::value<string>(), "output PDF file to write" )
    ( "input-file", po::value<string>(), "input audio file to read" )
    ;

  po::positional_options_description p;
  p.add("input-file", -1);

  try
    {
      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
      po::notify(vm);

      if(vm.count("help"))
        {
          cout << "usage: " << program_name << " [OPTIONS]... FILE" << endl
               << "Plot a cochleogram from an input audio FILE" << endl
               << desc;
          return 0;
        }

      if(vm.count("gnuplot-script"))
        gnuplot_script = fs::canonical(vm["gnuplot-script"].as<string>()).string();

      if(vm.count("verbose"))
        verb = true;

      if(vm.count("normalize"))
        normalize = true;

      if(vm.count("output-file"))
        output_file = vm["output-file"].as<string>();

      if(vm.count("input-file"))
        input_file = fs::canonical(vm["input-file"].as<string>()).string();
      else
        {
          // Input file must be specified: throw an exception
          struct oops:std::exception{const char* what() const noexcept{return "Input file unspecified !";}};
          throw(oops());
        }
    }
  catch(const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return -1;
    }

  return 0;
}


template<class T>
void normalization(T& data)
{
  typedef typename T::value_type::value_type Scalar;
  
  // time axis
  const size_t xsize = data.size();

  // frequency axis
  const size_t ysize = data.begin()->size();

  // for each frequency channel
  for(size_t y=0; y<ysize; y++)
    {
      // 1st pass: find the absolute max
      Scalar absmax = 0;
      for(size_t x=0; x<xsize; x++)
	absmax = max(absmax, abs(data[x][y]));
      
      const Scalar factor = 1.0/absmax;
      
      // 2nd pass: normalization
      for(size_t x=0; x<xsize; x++)
	data[x][y] *= factor;
    }
}

