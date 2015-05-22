#ifndef GAMMATONE_FILTER_HOLDER_HPP
#define GAMMATONE_FILTER_HOLDER_HPP

#include <gammatone/filter/interface.hpp>
#include <gammatone/filter/concrete.hpp>
#include <gammatone/factory.hpp>
#include <string>

namespace gammatone
{
  namespace filter
  {
    template<class Scalar>
    class holder : public ::gammatone::filter::interface<Scalar>
    {
    public:
      holder(const Scalar& sample_frequency,
             const Scalar& center_frequency,
             const std::string key = "");

      holder(const holder<Scalar>& other);
      holder<Scalar>& operator=(const holder<Scalar>& other);
      virtual ~holder();

      // methods inherited from interface
      inline Scalar sample_frequency() const;
      inline Scalar center_frequency() const;
      inline Scalar bandwidth() const;
      inline Scalar gain() const;
      inline void reset();

    private:
      inline Scalar compute_internal(const Scalar& input);
      
      //! A pointer to the concrete filter
      typename concrete_factory<Scalar>::return_type p_filter;

      //typename concrete_factory<Scalar>::key_type parse_key(const std::string& key);
      
    };
  }
}

template<class Scalar>
gammatone::filter::holder<Scalar>::
holder(const Scalar& sample_frequency,
       const Scalar& center_frequency,
       const std::string key)
  : p_filter(factory<double>::create(sample_frequency,center_frequency,key))
{}

template<class Scalar>
gammatone::filter::holder<Scalar>::
holder(const gammatone::filter::holder<Scalar>& other)
  : p_filter(other.p_filter)
{}

template<class Scalar>
gammatone::filter::holder<Scalar>& gammatone::filter::holder<Scalar>::
operator=(const gammatone::filter::holder<Scalar>& other)
{
  p_filter = other.p_filter;
}

template<class Scalar> gammatone::filter::holder<Scalar>::
~holder()
{}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
center_frequency() const
{
  return p_filter->center_frequency();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
sample_frequency() const
{
  return p_filter->sample_frequency();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
bandwidth() const
{
  return p_filter->bandwidth();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
gain() const
{
  return p_filter->gain();
}

template<class Scalar>
void gammatone::filter::holder<Scalar>::
reset()
{
  p_filter->reset();
}

template<class Scalar>
Scalar gammatone::filter::holder<Scalar>::
compute_internal(const Scalar& input)
{
  return p_filter->compute(input);
}

#endif // GAMMATONE_FILTER_HOLDER_HPP
