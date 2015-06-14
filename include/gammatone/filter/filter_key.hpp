#ifndef GAMMATONE_FILTER_KEY_HPP
#define GAMMATONE_FILTER_KEY_HPP

#include <string>

namespace gammatone
{
  namespace filter
  {
    //! Represente template arguments of a filter::concrete by strings
    class filter_key
    {
    public:
      filter_key(const std::string& params = "");
      filter_key(const filter_key& other);
      filter_key(filter_key&& other);
      filter_key& operator=(const filter_key& other);
      filter_key& operator=(filter_key&& other);
      virtual ~filter_key();

      void core(const std::string& s);
      void bandwidth(const std::string& s);
      void gain(const std::string& s);
      void clipping(const std::string& s);
      void postprocessing(const std::string& s);

      std::string core() const;
      std::string bandwidth() const;
      std::string gain() const;
      std::string clipping() const;
      std::string postprocessing() const;

      void parse(const std::string& params);

    private:

      std::string m_core;
      std::string m_bandwidth;
      std::string m_gain;
      std::string m_clipping;
      std::string m_postprocessing;

    };
  }
}

#endif // GAMMATONE_FILTER_KEY_HPP
