#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Chase.hh"

namespace py = pybind11;
using namespace chase;

PYBIND11_MODULE(chasecorebnd, m) {
    m.doc() = "CHASE Core  Python wrapper module";

    /**
    *   TYPES BINDINGS
    */

    // Boolean Type
    py::class_<Boolean, std::shared_ptr<Boolean>>(m, "Boolean")
        .def(py::init<>())
        .def("accept_visitor", &Boolean::accept_visitor,
            py::arg("v"))
        .def("getString", &Boolean::getString)
        .def("clone", &Boolean::clone);

    // Integer Type
    py::class_<Integer, std::shared_ptr<Integer>>(m, "Integer")
        .def(py::init<>())
        .def(py::init<const int &, const int &>(),
            py::arg("l"), py::arg("r"))
        .def(py::init<std::shared_ptr<Range> >(), 
            py::arg("r").none(false))
        .def("isSigned", &Integer::isSigned)
        .def("getRange", &Integer::getRange, py::return_value_policy::reference)
        .def("accept_visitor", &Integer::accept_visitor,
            py::arg("v"))
        .def("getString", &Integer::getString)
        .def("clone", &Integer::clone);

    // Real Type
    py::class_<Real, std::shared_ptr<Real>>(m, "Real")
        .def(py::init<>())
        .def("getRange", &Real::getRange, py::return_value_policy::reference)
        .def("setRange", &Real::setRange,
            py::arg("r"))
        .def("accept_visitor", &Real::accept_visitor,
            py::arg("v"))
        .def("getString", &Real::getString)
        .def("clone", &Real::clone);



    /**
    *   VALUES BINDINGS
    */
   
    // Range Binding
    py::class_<Range, std::shared_ptr<Range>>(m, "Range")
        .def(py::init<>())
        .def(py::init<const int &, const int &>(),
            py::arg("lbound"), py::arg("rbound"))
        .def("getType", &Range::getType, py::return_value_policy::reference)
        .def("setRightValue", &Range::setRightValue,
            py::arg("rbound"))
        .def("setLeftValue", &Range::setLeftValue,
            py::arg("lbound"))
        .def("getLeftValue", &Range::getLeftValue)
        .def("getRightValue", &Range::getRightValue)
        .def("getString", &Range::getString)
        .def("accept_visitor", &Range::accept_visitor,
            py::arg("v"))
        .def("clone", &Range::clone);

    //  BooleanValue Binding
    py::class_<BooleanValue>(m, "BooleanValue")
        .def(py::init<const bool &>(),
            py::arg("value") = false)
        .def(py::init<const BooleanValue &>(),
            py::arg("o"))
        .def("getValue", &BooleanValue::getValue)
        .def("setValue", &BooleanValue::setValue,
            py::arg("v"))
        .def("accept_visitor", &BooleanValue::accept_visitor,
            py::arg("v"))
        .def("getString", &BooleanValue::getString)
        .def("clone", &BooleanValue::clone);

    //  IntegerValue Binding
    py::class_<IntegerValue>(m, "IntegerValue")
        .def(py::init<const int64_t &>(),
            py::arg("value") = 0)
        .def(py::init<const IntegerValue &>(),
            py::arg("o"))
        .def("getValue", &IntegerValue::getValue)
        .def("setValue", &IntegerValue::setValue,
            py::arg("v"))
        .def("accept_visitor", &IntegerValue::accept_visitor,
            py::arg("v"))
        .def("getString", &IntegerValue::getString)
        .def("clone", &IntegerValue::clone);

    //  RealValue Binding
    py::class_<RealValue>(m, "RealValue")
        .def(py::init<const double &>(),
            py::arg("value") = 0)
        .def(py::init<const RealValue &>(),
            py::arg("o"))
        .def("getValue", &RealValue::getValue)
        .def("setValue", &RealValue::setValue,
            py::arg("v"))
        .def("accept_visitor", &RealValue::accept_visitor,
            py::arg("v"))
        .def("getString", &RealValue::getString)
        .def("clone", &RealValue::clone);

    /**
    *   DATA DECLARATION BINDINGS
    */
    py::class_<Constant>(m, "Constant")
        .def(py::init<std::shared_ptr<Type>, std::shared_ptr<Name>,
            std::shared_ptr<Value> &>(), py::arg("type")=nullptr, 
            py::arg("name")=nullptr, py::arg("value")=nullptr)
        .def("getString", &Constant::getString);
}

