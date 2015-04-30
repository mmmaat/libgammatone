#include <gammatone/impulse_response.hpp>
#include <gammatone/filter.hpp>
#include <gtest/gtest.h>

using namespace gammatone;

typedef double T;


class impulse_response_test : public testing::Test
{
protected:
  impulse_response_test() : m_filter(m_sample_frequency,m_center_frequency){}

  const T m_sample_frequency = 44100;
  const T m_center_frequency = 2000;
  const T m_duration = 0.01;
  filter<T> m_filter;
};

TEST_F( impulse_response_test, time_works )
{
  const auto time = impulse_response::time(m_filter, m_duration);
  const auto mm = std::minmax_element(time.begin(),time.end());

  EXPECT_EQ( time.size(), m_sample_frequency*m_duration+1);
  EXPECT_EQ( *mm.first, 0);
  EXPECT_EQ( *time.begin(), 0);
  EXPECT_FLOAT_EQ( *mm.second, m_duration);
  EXPECT_FLOAT_EQ( *time.rbegin(), m_duration);
}


TEST_F( impulse_response_test, polymorphism_works )
{
  const auto t  = impulse_response::time(m_filter, m_duration);
  const auto it = impulse_response::theorical(m_filter, t.begin(), t.end());
  const auto ii = impulse_response::implemented(m_filter, t.begin(), t.end());

  const auto pt = impulse_response::theorical(m_filter,m_duration);
  const auto pi = impulse_response::implemented(m_filter,m_duration);

  EXPECT_EQ(t.size(), it.size());
  EXPECT_EQ(t.size(), ii.size());
  EXPECT_EQ(t.size(), pt.first.size());
  EXPECT_EQ(t.size(), pi.first.size());
  EXPECT_EQ(pt.first.size(), pt.second.size());
  EXPECT_EQ(pi.first.size(), pi.second.size());

  for(std::size_t n=0; n<t.size();n++)
    {
      EXPECT_FLOAT_EQ(t[n], pt.first[n]);
      EXPECT_FLOAT_EQ(it[n], pt.second[n]);
      EXPECT_FLOAT_EQ(t[n], pi.first[n]);
      EXPECT_FLOAT_EQ(ii[n], pi.second[n]);
    }
}
