#include <iostream>
#include <vector>

#include <gammatone/filterbank.hpp>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
namespace py = boost::python;

using scalar = double;
using filterbank = gammatone::filterbank<scalar>;

BOOST_PYTHON_MODULE(_filterbank)
{
    py::class_<std::vector<scalar>>("std_vector_double")
        .def(py::vector_indexing_suite<std::vector<scalar>>());

    py::class_<filterbank>(
        "Filterbank", py::init<scalar, scalar, scalar, filterbank::channels::param_type>())
        .def_readonly("sample_frequency", &filterbank::sample_frequency)
        .def_readonly("center_frequency", &filterbank::center_frequency)
        .def_readonly("bandwidth", &filterbank::bandwidth)
        .def_readonly("gain", &filterbank::gain)
        .def("reset", &filterbank::reset)
        .def("_compute", &filterbank::compute_allocate);
}


// // C++ code
// typedef std::vector<std::string> MyList;
// class MyClass {
//   MyList myFuncGet();
//   void myFuncSet(const Mylist& list);
//   //       stuff
// };
// // Wrapper code
// #include <boost/python/suite/indexing/vector_indexing_suite.hpp>
// using namespace boost::python;
// BOOST_PYTHON_MODULE(mymodule)
// {
//     class_<MyList>("MyList")
//         .def(vector_indexing_suite<MyList>() );
//     class_<myClass>("MyClass")
//         .def("myFuncGet", &MyClass::myFuncGet)
//         .def("myFuncSet", &MyClass::myFuncSet)
//         ;
// }


// // http://www.shocksolution.com/python-basics-tutorials-and-examples/boostpython-numpy-example/
// #include <boost/python.hpp>
// #include "boost/python/extract.hpp"
// #include "boost/python/numeric.hpp"
// #include <iostream>

// using namespace boost::python;

// // Functions to demonstrate extraction
// void setArray(boost::python::numeric::array data) {

// 	// Access a built-in type (an array)
// 	boost::python::numeric::array a = data;
// 	// Need to <extract> array elements because their type is unknown
// 	std::cout << "First array item: " << extract<int>(a[0]) << std::endl;
// }


// // Expose classes and methods to Python
// BOOST_PYTHON_MODULE(TestNumPy) {
// 	boost::python::numeric::array::set_module_and_type("numpy", "ndarray");

// 	def("setArray", &setArray);

// }



// class filter_python: public
// {
// public:
//     filter_python(const scalar& sample_frequency,
//                   const scalar& center_frequency):
//         gammatone::filter<scalar>(sample_frequency, center_frequency)
//     {}

//     std::vector<scalar> compute_vector()//std::vector<scalar> input)
//     {
//         std::vector<scalar> output{1.0, 20.};//(input.size());
//         // for(int i = 0; i <input.size(); i++)
//         // {
//         //     this->compute(input[i], output[i]);
//         // }
//         return output;
//     }

//     // numeric::array compute_array(numeric::array& input)
//     // {
//     //     object shape = input.attr("shape");
//     //     int rows = extract<int>(shape[0]);
//     //     int size = extract<int>(input.attr("size"));
//     //     std::cout << "size is " << size << " " << rows << std::endl;

//     //     numeric::array a(input); //(make_tuple(3, 2));
//     //     return a;
//     // }
// };
