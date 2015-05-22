#ifndef UTILS_WAV_HPP
#define UTILS_WAV_HPP

#include <sndfile.h>
#include <vector>
#include <string>


namespace utils
{
  //! Static methods to read and write audio wav files
  /*!
    Input from and output to wav files, both mono and stereo. Use
    libsndfile.

    \see http://www.mega-nerd.com/libsndfile/
  */
  namespace wav
  {
    //! Load a mono wav file in a container
    /*!
      \param filename  input mono wav file
      \param data      output double array
      \param sample_frequency sample frequency of the input file

      \return 0 on success, -1 on file errors
    */
    template<class Container = std::vector<double> >
    static int read_mono(const std::string& filename,
                         Container& data,
                         double& sample_frequency);

    //! Load a stereo wav file in a container
    /*!
      \param filename  input stereo wav file
      \param data      output stereo double array
      \param sample_frequency sample frequency of the input file

      \return 0 on success, -1 on file errors
    */
    template<class Container = std::pair<std::vector<double>,std::vector<double> > >
    static int read_stereo(const std::string& filename,
                           Container& data,
                           double& sample_frequency);


    // //! Write a mono wav file from data
    // /*!
    //   \param filename  output mono wav file
    //   \param data      input container
    //   \param sample_frequency sample frequency of the output file

    //   \return 0 on success, -1 on file errors
    // */
    // template<class Container = std::vector<double> >
    // static int write_mono(const std::string& filename,
    //                       const Container& data,
    //                       const double& sample_frequency );

    // //! Write a stereo wav file from data
    // /*!
    //   \param filename  output stereo wav file
    //   \param data      input stereo double array
    //   \param sample_frequency sample frequency of the output file

    //   \return 0 on success, -1 on file errors
    // */
    // static int write_stereo(const std::string& filename,
    //                         const Container& data,
    //                         const double& sample_frequency );
  }
}

template<class Container>
int utils::wav::read_mono(const std::string& filename,
                          Container& data,
                          double& sample_frequency)
{
  SNDFILE *infile;
  SF_INFO sfinfo;

  // open wav file and check it is mono
  if(!(infile = sf_open(filename.c_str(), SFM_READ, &sfinfo)))
    return  -1 ;

  if(sfinfo.channels != 1)
    return -2;

  // get audio sample frequency
  sample_frequency = sfinfo.samplerate;

  // read audio data from file buffer
  data.clear();

  float tmp[1024];
  int readcount;
  while((readcount = sf_read_float(infile, tmp, 1024)))
    for(int i = 0; i < readcount; i++)
      data.push_back(static_cast<typename Container::value_type>(tmp[i]));

  //! \todo Check for an error code here
  sf_close(infile);

  return 0;
}

template<class Container>
int utils::wav::read_stereo(const std::string& filename,
                            Container& data,
                            double& sample_frequency)
{
  SNDFILE *infile;
  SF_INFO sfinfo;

  // open wav file and check it is stereo
  if(!(infile = sf_open(filename.c_str(), SFM_READ, &sfinfo)))
    return  -1 ;

  if(sfinfo.channels != 2)
    return -2;

  // get audio sample frequency
  sample_frequency = sfinfo.samplerate;

  // read audio data from file buffer
  data.first.clear();
  data.second.clear();

  float tmp[1024];
  int readcount;
  while((readcount = sf_read_float(infile, tmp, 1024)))
    {
      for(int i=0; i<readcount; i=i+2)
        {
          data.first.push_back(tmp[i]);
          data.second.push_back(tmp[i+1]);
        }
    }

  sf_close(infile);

  return 0;
}


// template<class Container>
// int utils::wav::write_mono( const string& filename,
//                             const Container& data,
//                             const double& sample_frequency )
// {
//   SF_INFO sfinfo;
//   sfinfo.samplerate = sample_frequency;
//   sfinfo.frames = data.size();
//   sfinfo.channels = 1;
//   sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_16);
//   SNDFILE* file = sf_open(filename.c_str(), SFM_WRITE, &sfinfo);

//   // ecriture des données
//   float* ptr = new float[data.size()];
//   for(int i = 0; i < (int)data.size(); i++)
//     ptr[i] = data[i];
//   sf_write_float(file, ptr, data.size());


//   if(sf_close(file) != 0)
//     {
//       cout<<"utils::wav : can't close "<<filename<<endl;
//       return -1;
//     }

//   return 0;
// }


// template<class Container>
// int utils::wav::write_stereo(const string& filename,
//                              const Container& data,
//                              const double& sample_frequency)
// {
//   // check for left and right size
//   if( data.left.size() != data.right.size() )
//     {
//       cerr << "utils::wav : can't write in stereo " << filename << endl;
//       return -1;
//     }
//   int size = data.left.size();

//   SF_INFO sfinfo;
//   sfinfo.samplerate = sample_frequency;
//   sfinfo.frames = size;
//   sfinfo.channels = 2;
//   sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_16);
//   SNDFILE* file = sf_open(filename.c_str(), SFM_WRITE, &sfinfo);

//   // ecriture des données
//   float* ptr = new float[size*2];
//   for(int i = 0; i < size; i++)
//     {
//       ptr[2*i] = data.right[i];
//       ptr[2*i+1] = data.left[i];
//     }
//   sf_writef_float(file, ptr, size);

//   if(sf_close(file) != 0)
//     {
//       cerr<<"utils::wav : can't close "<<filename<<endl;
//       return -1;
//     }

//   return 0;
// }

#endif // UTILS_WAV_HPP
