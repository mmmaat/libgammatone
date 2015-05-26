#ifndef GAMMATONE_TEST_FILTERBANK_TYPES_H
#define GAMMATONE_TEST_FILTERBANK_TYPES_H

#include <gammatone/gammatone.hpp>
#include <boost/mpl/list.hpp>

typedef double T;

// enumeration of all possible filterbank types (core/policy combination)

typedef gammatone::policy::gain::old_cooke1993 g1;

typedef gammatone::core::cooke1993<T, g1, gammatone::policy::clipping::off> c1;
typedef gammatone::core::cooke1993<T, g1, gammatone::policy::clipping::on>  c2;
typedef gammatone::core::slaney1993<T>                                  c3;
typedef gammatone::core::convolution<T>                                 c4;

typedef gammatone::policy::bandwidth::glasberg1990<T>  b1;
typedef gammatone::policy::bandwidth::slaney1988<T>    b2;
typedef gammatone::policy::bandwidth::greenwood1990<T> b3;

typedef gammatone::policy::order::decreasing   o1;
typedef gammatone::policy::order::increasing   o2;

typedef gammatone::policy::channels::fixed_size<T,o1>    n1;
typedef gammatone::policy::channels::fixed_size<T,o2>    n2;
typedef gammatone::policy::channels::fixed_overlap<T,o1> n3;
typedef gammatone::policy::channels::fixed_overlap<T,o2> n4;

typedef gammatone::filterbank::concrete<T,c1,b1,n1> f111;
typedef gammatone::filterbank::concrete<T,c2,b1,n1> f211;
typedef gammatone::filterbank::concrete<T,c3,b1,n1> f311;
typedef gammatone::filterbank::concrete<T,c4,b1,n1> f411;

typedef gammatone::filterbank::concrete<T,c1,b2,n1> f121;
typedef gammatone::filterbank::concrete<T,c2,b2,n1> f221;
typedef gammatone::filterbank::concrete<T,c3,b2,n1> f321;
typedef gammatone::filterbank::concrete<T,c4,b2,n1> f421;

typedef gammatone::filterbank::concrete<T,c1,b3,n1> f131;
typedef gammatone::filterbank::concrete<T,c2,b3,n1> f231;
typedef gammatone::filterbank::concrete<T,c3,b3,n1> f331;
typedef gammatone::filterbank::concrete<T,c4,b3,n1> f431;


typedef gammatone::filterbank::concrete<T,c1,b1,n2> f112;
typedef gammatone::filterbank::concrete<T,c2,b1,n2> f212;
typedef gammatone::filterbank::concrete<T,c3,b1,n2> f312;
typedef gammatone::filterbank::concrete<T,c4,b1,n2> f412;

typedef gammatone::filterbank::concrete<T,c1,b2,n2> f122;
typedef gammatone::filterbank::concrete<T,c2,b2,n2> f222;
typedef gammatone::filterbank::concrete<T,c3,b2,n2> f322;
typedef gammatone::filterbank::concrete<T,c4,b2,n2> f422;

typedef gammatone::filterbank::concrete<T,c1,b3,n2> f132;
typedef gammatone::filterbank::concrete<T,c2,b3,n2> f232;
typedef gammatone::filterbank::concrete<T,c3,b3,n2> f332;
typedef gammatone::filterbank::concrete<T,c4,b3,n2> f432;



typedef gammatone::filterbank::concrete<T,c1,b1,n3> f113;
typedef gammatone::filterbank::concrete<T,c2,b1,n3> f213;
typedef gammatone::filterbank::concrete<T,c3,b1,n3> f313;
typedef gammatone::filterbank::concrete<T,c4,b1,n3> f413;

typedef gammatone::filterbank::concrete<T,c1,b2,n3> f123;
typedef gammatone::filterbank::concrete<T,c2,b2,n3> f223;
typedef gammatone::filterbank::concrete<T,c3,b2,n3> f323;
typedef gammatone::filterbank::concrete<T,c4,b2,n3> f423;

typedef gammatone::filterbank::concrete<T,c1,b3,n3> f133;
typedef gammatone::filterbank::concrete<T,c2,b3,n3> f233;
typedef gammatone::filterbank::concrete<T,c3,b3,n3> f333;
typedef gammatone::filterbank::concrete<T,c4,b3,n3> f433;



typedef gammatone::filterbank::concrete<T,c1,b1,n4> f114;
typedef gammatone::filterbank::concrete<T,c2,b1,n4> f214;
typedef gammatone::filterbank::concrete<T,c3,b1,n4> f314;
typedef gammatone::filterbank::concrete<T,c4,b1,n4> f414;

typedef gammatone::filterbank::concrete<T,c1,b2,n4> f124;
typedef gammatone::filterbank::concrete<T,c2,b2,n4> f224;
typedef gammatone::filterbank::concrete<T,c3,b2,n4> f324;
typedef gammatone::filterbank::concrete<T,c4,b2,n4> f424;

typedef gammatone::filterbank::concrete<T,c1,b3,n4> f134;
typedef gammatone::filterbank::concrete<T,c2,b3,n4> f234;
typedef gammatone::filterbank::concrete<T,c3,b3,n4> f334;
typedef gammatone::filterbank::concrete<T,c4,b3,n4> f434;


typedef boost::mpl::list< f111,f211,f311,f411,
                          f121,f221,f321,f421,
                          f131,f231,f331,f431

                          // f112,f212,f312,f412,
                          // f122,f222,f322,f422,
                          // f132,f232,f332,f432,

                          // f113,f213,f313,f413,
                          // f123,f223,f323,f423,
                          // f133,f233,f333,f433,

                          // f114,f214,f314,f414,
                          // f124,f224,f324,f424,
                          // f134,f234,f334,f434
                          > filterbank_types;

#endif // GAMMATONE_TEST_FILTERBANK_TYPES_H
