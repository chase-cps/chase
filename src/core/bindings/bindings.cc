#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Chase.hh"

namespace py = pybind11;
using namespace chase;

PYBIND11_MODULE(chasecorebnd, m) {
    m.doc() = "CHASE Core  Python wrapper module";

    py::class_<ChaseObject, std::shared_ptr<ChaseObject>>
        (m, "ChaseObject");

    py::class_<Name, std::shared_ptr<Name>, ChaseObject>
        (m, "Name")
        .def(py::init<std::string &>(), py::arg("s"))
        .def(py::init<const Name &>(), py::arg("o"))
        .def("accept_visitor", &Name::accept_visitor,
            py::arg("v"))
        .def("getString", &Name::getString)
        .def("changeName", &Name::changeName, 
            py::arg("name"))
        .def("clone", &Name::clone);

    /**
    *  OPERATORS 
    */
    
    py::enum_<chase::Operator>(m, "Operator")
        .value("op_none", chase::Operator::op_none)
        .value("op_plus", chase::Operator::op_plus)
        .value("op_minus", chase::Operator::op_minus)
        .value("op_multiply", chase::Operator::op_multiply)
        .value("op_divide", chase::Operator::op_divide)
        .value("op_mod", chase::Operator::op_mod)
        .value("op_eq", chase::Operator::op_eq)
        .value("op_neq", chase::Operator::op_neq)
        .value("op_lt", chase::Operator::op_lt)
        .value("op_gt", chase::Operator::op_gt)
        .value("op_le", chase::Operator::op_le)
        .value("op_ge", chase::Operator::op_ge)
        .export_values();

    py::enum_<chase::BooleanOperator>(m, "BooleanOperator")
        .value("op_not", chase::BooleanOperator::op_not)
        .value("op_and", chase::BooleanOperator::op_and)
        .value("op_or", chase::BooleanOperator::op_or)
        .value("op_implies", chase::BooleanOperator::op_implies)
        .value("op_iff", chase::BooleanOperator::op_iff)
        .value("op_xor", chase::BooleanOperator::op_xor)
        .value("op_nand", chase::BooleanOperator::op_nand)
        .value("op_nor", chase::BooleanOperator::op_nor)
        .value("op_xnor", chase::BooleanOperator::op_xnor)
        .export_values();
    
    py::enum_<chase::ModalOperator>(m, "ModalOperator")
        .value("op_square", chase::ModalOperator::op_square)
        .value("op_diamond", chase::ModalOperator::op_diamond)
        .export_values();

    py::enum_<chase::TemporalOperator>(m, "TemporalOperator")
        .value("op_globally", chase::TemporalOperator::op_globally)
        .value("op_future", chase::TemporalOperator::op_future)
        .value("op_next", chase::TemporalOperator::op_next)
        .value("op_until", chase::TemporalOperator::op_until)
        .value("op_release", chase::TemporalOperator::op_release)
        .export_values();


    /**
    *   TYPES BINDINGS
    */

    py::class_<Type, std::shared_ptr<Type>>(m, "Type");
    py::class_<SimpleType, Type, std::shared_ptr<SimpleType>>
    (m, "SimpleType");

    // Boolean Type
    py::class_<Boolean, std::shared_ptr<Boolean>, SimpleType>
        (m, "Boolean")
        .def(py::init<>())
        .def("accept_visitor", &Boolean::accept_visitor,
            py::arg("v"))
        .def("getString", &Boolean::getString)
        .def("clone", &Boolean::clone);

    // Integer Type
    py::class_<Integer, std::shared_ptr<Integer>, 
        SimpleType>(m, "Integer")
        .def(py::init<>())
        .def(py::init<const int &, const int &>(),
            py::arg("l"), py::arg("r"))
        .def(py::init<std::shared_ptr<Range> >(), 
            py::arg("r").none(false))
        .def("isSigned", &Integer::isSigned)
        .def("getRange", &Integer::getRange, 
            py::return_value_policy::reference)
        .def("accept_visitor", &Integer::accept_visitor,
            py::arg("v"))
        .def("getString", &Integer::getString)
        .def("clone", &Integer::clone);

    // Real Type
    py::class_<Real, std::shared_ptr<Real>, SimpleType>(m, "Real")
        .def(py::init<>())
        .def("getRange", &Real::getRange, 
            py::return_value_policy::reference)
        .def("setRange", &Real::setRange,
            py::arg("r"))
        .def("accept_visitor", &Real::accept_visitor,
            py::arg("v"))
        .def("getString", &Real::getString)
        .def("clone", &Real::clone);



    /**
    *   VALUES BINDINGS
    */

    py::class_<Value, std::shared_ptr<Value>, 
        ChaseObject>(m, "Value");
    py::class_<NumericValue, std::shared_ptr<NumericValue>, 
        Value>(m, "NumericValue");


    // Range Binding
    py::class_<Range, std::shared_ptr<Range>, Value>(m, "Range")
        .def(py::init<>())
        .def(py::init<const int &, const int &>(),
            py::arg("lbound"), py::arg("rbound"))
        .def("getType", &Range::getType, 
            py::return_value_policy::reference)
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

    //  Identifier Binding
    py::class_<Identifier, std::shared_ptr<Identifier>, 
        Value>(m, "Identifier")
        .def(py::init<std::shared_ptr<DataDeclaration>>(),
            py::arg("d"))
        .def(py::init<const Identifier &>(), py::arg("i"))
        .def("getDeclaration", &Identifier::getDeclaration)
        .def("setDeclaration", &Identifier::setDeclaration, 
            py::arg("d"))
        .def("getString", &Identifier::getString)
        .def("getType", &Identifier::getType)
        .def("clone", &Identifier::clone);

    // Expression Binding
    py::class_<Expression, std::shared_ptr<Expression>, 
    Value>(m, "Expression")
        .def(py::init<>())
        .def(py::init<Operator, std::shared_ptr<Value>,
            std::shared_ptr<Value>>(), py::arg("op"),
            py::arg("op1"), py::arg("op2"))
        .def("getOperator", &Expression::getOperator)
        .def("getOp1", &Expression::getOp1)
        .def("getOp2", &Expression::getOp2)
        .def("setOperator", &Expression::setOperator, 
            py::arg("op"))
        .def("setOp1", &Expression::setOp1,
            py::arg("op"))
        .def("setOp2", &Expression::setOp2,
            py::arg("op"))
        .def("getString", &Expression::getString)
        .def("getType", &Expression::getType)
        .def("accept_visitor", &Expression::accept_visitor,
            py::arg("v"))
        .def("clone", &Expression::clone);


    //  BooleanValue Binding
    py::class_<BooleanValue, std::shared_ptr<BooleanValue>, 
        NumericValue>(m, "BooleanValue")
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
    py::class_<IntegerValue, std::shared_ptr<IntegerValue>, 
        NumericValue>(m, "IntegerValue")
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
    py::class_<RealValue, std::shared_ptr<RealValue>, 
        NumericValue>(m, "RealValue")
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

    py::class_<Declaration, std::shared_ptr<Declaration>, 
        ChaseObject>(m, "Declaration");
    py::class_<DataDeclaration, std::shared_ptr<DataDeclaration>, 
        Declaration>(m, "DataDeclaration");

    // Constant Binding
    py::class_<Constant, std::shared_ptr<Constant>, 
        DataDeclaration>(m, "Constant")
        .def(py::init< std::shared_ptr<Type>, std::shared_ptr<Name>,
            std::shared_ptr<Value> >(), py::arg("type"), 
            py::arg("name"), py::arg("value"))
        .def("getString", &Constant::getString)
        .def("getValue", &Constant::getValue)
        .def("setValue", &Constant::setValue,
            py::arg("value"))
        .def("clone", &Constant::clone);

    py::enum_<chase::causality_t>(m, "causality_t")
        .value("generic", chase::causality_t::generic)
        .value("input", chase::causality_t::input)
        .value("output", chase::causality_t::output)
        .value("inout", chase::causality_t::inout)
        .value("internal", chase::causality_t::internal)
        .export_values();
        
    // Variable Binding
    py::class_<Variable, std::shared_ptr<Variable>, 
        DataDeclaration>(m, "Variable")
        .def(py::init<std::shared_ptr<Type> &, 
            std::shared_ptr<Name> &,
            causality_t &>(), py::arg("type"), 
            py::arg("name"), py::arg("causality_t"))
        .def("getString", &Variable::getString)
        .def("getCausality", &Variable::getCausality)
        .def("setCausality", &Variable::setCausality, 
            py::arg("causality"))
        .def("clone", &Variable::clone);

}

