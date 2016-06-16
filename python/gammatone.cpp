#include <boost/python.hpp>

#include <vector>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <gammatone/filter.hpp>
#include <gammatone/filterbank.hpp>

namespace py = boost::python;

using scalar = double;

void export_filter();
void export_filterbank();


BOOST_PYTHON_MODULE(_gammatone)
{
    // specify that this module is actually a package
    py::object package = py::scope();
    package.attr("__path__") = "gammatone";

    export_filter();
    export_filterbank();
}


void export_filter()
{
    // we export the default filter type
    using filter = gammatone::filter<scalar>;

    py::class_<filter>("Filter", py::init<scalar, scalar>())
        .def_readonly("sample_frequency", &filter::sample_frequency)
        .def_readonly("center_frequency", &filter::center_frequency)
        .def_readonly("bandwidth", &filter::bandwidth)
        .def_readonly("gain", &filter::gain)
        .def("reset", &filter::reset)
        .def("_compute", &filter::compute_allocate);
}


void export_filterbank()
{
    py::class_<std::vector<scalar>>("std_vector_double")
        .def(py::vector_indexing_suite<std::vector<scalar>>());

    // we export the default filterbank type
    using filterbank = gammatone::filterbank<scalar>;

    py::class_<filterbank>(
        "Filterbank", py::init<scalar, scalar, scalar, filterbank::channels::param_type>())
        .def_readonly("sample_frequency", &filterbank::sample_frequency)
        .def_readonly("center_frequency", &filterbank::center_frequency)
        .def_readonly("nb_channels", &filterbank::nb_channels)
        .def_readonly("bandwidth", &filterbank::bandwidth)
        .def_readonly("gain", &filterbank::gain)
        .def("reset", &filterbank::reset)
        .def("_compute", &filterbank::compute_allocate);
}
