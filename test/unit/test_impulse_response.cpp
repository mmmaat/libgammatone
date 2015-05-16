#include <filter_types.h>

using gammatone::impulse_response;

template<class Filter>
class impulse_response_test : public testing::Test
{
protected:
  impulse_response_test() : m_filter(m_sample_frequency,m_center_frequency){}

  typedef typename Filter::scalar_type T;
  const T m_sample_frequency = 44100;
  const T m_center_frequency = 2000;
  const T m_duration = 0.01;
  Filter m_filter;
};

TYPED_TEST_CASE(impulse_response_test, filter_types);

TYPED_TEST( impulse_response_test, time_works )
{
  const auto time = impulse_response::time(this->m_filter.sample_frequency(), this->m_duration);
  const auto mm = std::minmax_element(time.begin(),time.end());

  EXPECT_EQ( std::adjacent_find(time.begin(), time.end(), std::greater<int>()), time.end());
  EXPECT_EQ( time.size(), this->m_sample_frequency*this->m_duration+1);
  EXPECT_EQ( *mm.first, 0);
  EXPECT_EQ( *time.begin(), 0);
  EXPECT_FLOAT_EQ( *mm.second, this->m_duration);
  EXPECT_FLOAT_EQ( *time.rbegin(), this->m_duration);
}


TYPED_TEST( impulse_response_test, theorical_works )
{
  const auto ir = impulse_response::theorical(this->m_filter,this->m_duration);

  EXPECT_NE( utils::absmax(ir.cbegin(),ir.cend()), 0.0);
  EXPECT_LE( *std::min_element(ir.cbegin(),ir.cend()), 0.0);
  EXPECT_GE( *std::max_element(ir.cbegin(),ir.cend()), 0.0);
}


TYPED_TEST( impulse_response_test, implemented_works )
{
  const auto ir = impulse_response::implemented(this->m_filter,this->m_duration);

  EXPECT_NE( utils::absmax(ir.cbegin(),ir.cend()), 0.0);
  EXPECT_LE( *std::min_element(ir.cbegin(),ir.cend()), 0.0);
  EXPECT_GE( *std::max_element(ir.cbegin(),ir.cend()), 0.0);
}


TYPED_TEST( impulse_response_test, polymorphism_works )
{
  const auto t  = impulse_response::time(this->m_filter.sample_frequency(), this->m_duration);
  const auto it = impulse_response::theorical(this->m_filter, t.begin(), t.end());
  const auto pt = impulse_response::theorical(this->m_filter,this->m_duration);
  const auto et = impulse_response::theorical(this->m_center_frequency,
                                              this->m_filter.bandwidth(),
                                              this->m_sample_frequency, this->m_duration);

  const auto pi = impulse_response::implemented(this->m_filter,this->m_duration);
  const auto ii = impulse_response::implemented(this->m_filter, t.begin(), t.end());

  EXPECT_EQ(t.size(), ii.size());
  EXPECT_EQ(t.size(), pi.size());

  EXPECT_EQ(t.size(), it.size());
  EXPECT_EQ(t.size(), pt.size());
  EXPECT_EQ(t.size(), et.size());

  EXPECT_FALSE(t.size() == 0);

  for(std::size_t n=0; n<t.size();n++)
    {
      EXPECT_FLOAT_EQ(it[n], pt[n]);
      EXPECT_FLOAT_EQ(it[n], et[n]);
      EXPECT_FLOAT_EQ(ii[n], pi[n]);
    }
}
