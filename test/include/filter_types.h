#ifndef GAMMATONE_TEST_FILTER_TYPES_H
#define GAMMATONE_TEST_FILTER_TYPES_H

#include <gammatone/gammatone.hpp>
#include <gtest/gtest.h>

typedef double T;

// enumeration of all possible filter types (core/policy combination)

typedef gammatone::core::cooke1993<T, gammatone::policy::clipping::off> c1;
typedef gammatone::core::cooke1993<T, gammatone::policy::clipping::on>  c2;
typedef gammatone::core::slaney1993<T>                                  c3;
typedef gammatone::core::convolution<T>                                 c4;

typedef gammatone::policy::bandwidth::glasberg1990<T>  b1;
typedef gammatone::policy::bandwidth::slaney1988<T>    b2;
typedef gammatone::policy::bandwidth::greenwood1990<T> b3;

typedef gammatone::filter<T,c1,b1>  f11;
typedef gammatone::filter<T,c2,b1>  f21;
typedef gammatone::filter<T,c3,b1>  f31;
typedef gammatone::filter<T,c4,b1>  f41;

typedef gammatone::filter<T,c1,b2>  f12;
typedef gammatone::filter<T,c2,b2>  f22;
typedef gammatone::filter<T,c3,b2>  f32;
typedef gammatone::filter<T,c4,b2>  f42;

typedef gammatone::filter<T,c1,b3>  f13;
typedef gammatone::filter<T,c2,b3>  f23;
typedef gammatone::filter<T,c3,b3>  f33;
typedef gammatone::filter<T,c4,b3>  f43;

typedef ::testing::Types< f11,f21,f31,f41,
                          f12,f22,f32,f42,
                          f13,f23,f33,f43
                          > filter_types;

#endif // GAMMATONE_TEST_FILTER_TYPES_H
