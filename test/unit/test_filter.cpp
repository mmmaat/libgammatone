#include <gammatone/filter.hpp>
#include <gtest/gtest.h>

typedef double T;
typedef gammatone::filter<T> filter;

class filter_test : public testing::Test
{
protected:

  const T m_sample_frequency = 44100;
  const T m_center_frequency = 5000;
};

TEST_F( filter_test, accessors_works )
{
  filter m_filter( m_sample_frequency, m_center_frequency );

  EXPECT_EQ( m_sample_frequency, m_filter.sample_frequency() );
  EXPECT_EQ( m_center_frequency, m_filter.center_frequency() );
  
  EXPECT_GT( m_filter.bandwidth(), 0.0 );
  EXPECT_GT( m_filter.gain(), 0.0 );
}

