#include <gammatone/filter.hpp>

#include <boost/python.hpp>
using namespace boost::python;

using scalar = double;
using filter = gammatone::filter<scalar>;
using iterator = std::vector<scalar>::iterator;
using const_iterator = std::vector<scalar>::const_iterator;

auto compute_range = &filter::base::compute_range<iterator, iterator>;

BOOST_PYTHON_MODULE(gammatone)
{
    // class_<filter_base>("FilterInterface", init<const scalar&>())
    //     ;
    
    //class_<filter, bases<filter::base> >("Filter", init<const scalar&, const scalar&>())
    class_<filter>("Filter", init<scalar, scalar>())
        .def_readonly("sample_frequency", &filter::sample_frequency)
        .def_readonly("center_frequency", &filter::center_frequency)
        .def_readonly("bandwidth", &filter::bandwidth)
        .def_readonly("gain", &filter::gain)
        .def("reset", &filter::reset)
        .def("compute", &filter::base::compute_allocate)
        .def("compute_range", &compute_range)
        ;
}
