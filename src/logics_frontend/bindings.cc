/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        8/3/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Chase.hh"

#include "LTLSpecsBuilder.hh"

namespace py = pybind11;
using namespace chase;
using namespace logics_frontend;

template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

PYBIND11_MODULE(pychase_logics_frontend, m)
{
    m.doc() = "CHASE Logics Frontend  Python wrapper module";

    py::class_<LTLSpecsBuilder,
                std::unique_ptr<LTLSpecsBuilder,
                py::nodelete> >(m, "LTLSpecsBuilder")
        .def(py::init<>())
        .def("getSystem", &LTLSpecsBuilder::getSystem)
        .def("parseSpecificationFile", &LTLSpecsBuilder::parseSpecificationFile,
             py::arg("infile").none(false));
}