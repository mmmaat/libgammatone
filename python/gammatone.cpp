/*
  Copyright (C) 2015, 2016 Mathieu Bernard <mathieu_bernard@laposte.net>

  This file is part of libgammatone

  libgammatone is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with libgammatone. If not, see <http://www.gnu.org/licenses/>.
*/
#include <vector>
#include <boost/python.hpp>
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
