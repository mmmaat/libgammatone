#include <gammatone/filterbank.hpp>
#include <gtest/gtest.h>

typedef double T;

class filterbank_test : public testing::Test
{
protected:

  const T m_sample_frequency = 44100;
  const T m_nb_channels = 20;
  const T m_low = 500, m_high = 8000;
};

TEST_F( filterbank_test, accessors_works )
{
  gammatone::filterbank<T> m_filterbank( m_sample_frequency,m_low,m_high,m_nb_channels);

  EXPECT_EQ( m_sample_frequency, m_filterbank.sample_frequency() );
  EXPECT_EQ( m_nb_channels, m_filterbank.nb_channels() );

  size_t i=0;
  for(const auto& f:m_filterbank)
    {
      EXPECT_GT( f.bandwidth(), 0.0 );
      EXPECT_GT( f.gain(), 0.0 );

      EXPECT_EQ( m_filterbank.center_frequency()[i], f.center_frequency() );
      EXPECT_EQ( m_filterbank.bandwidth()[i], f.bandwidth() );
      EXPECT_EQ( m_filterbank.gain()[i], f.gain() );
      i++;      
    }
}


TEST_F( filterbank_test, center_frequency_works )
{
  using namespace gammatone;
  typedef filterbank<T,core::cooke1993<T>,policy::channels::increasing> increasing;
  typedef filterbank<T,core::cooke1993<T>,policy::channels::decreasing> decreasing;

  increasing fi(m_sample_frequency,m_low,m_high,m_nb_channels);
  EXPECT_LE( m_low, fi.begin()->center_frequency() );
  EXPECT_EQ( m_high, fi.rbegin()->center_frequency() );

  decreasing fd(m_sample_frequency,m_low,m_high,m_nb_channels);
  EXPECT_EQ( m_high, fd.begin()->center_frequency() );
  EXPECT_LE( m_low, fd.rbegin()->center_frequency() );

  auto it=fi.begin(); auto it2=fd.rbegin();
  while(it!=fi.end())
    EXPECT_EQ(it++->center_frequency(), it2++->center_frequency());

}
