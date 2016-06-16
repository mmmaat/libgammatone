#include <boost/python.hpp>
#include <gammatone/filter.hpp>

namespace py = boost::python;
using scalar = double;
using filter = gammatone::filter<scalar>;

BOOST_PYTHON_MODULE(_filter)
{
    py::class_<filter>("Filter", py::init<scalar, scalar>())
        .def_readonly("sample_frequency", &filter::sample_frequency)
        .def_readonly("center_frequency", &filter::center_frequency)
        .def_readonly("bandwidth", &filter::bandwidth)
        .def_readonly("gain", &filter::gain)
        .def("reset", &filter::reset)
        .def("_compute", &filter::compute_allocate);
}
