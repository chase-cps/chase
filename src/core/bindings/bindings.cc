#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Chase.hh"
#include "utilities/GraphUtilities.hh"


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
            py::arg("v").none(false))
        .def("getString", &Name::getString)
        .def("changeName", &Name::changeName, 
            py::arg("name"))
        .def("clone", &Name::clone);

    py::class_<Contract, std::shared_ptr<Contract>,
        ChaseObject>(m, "Contract");

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
            py::arg("v").none(false))
        .def("getString", &Boolean::getString)
        .def("clone", &Boolean::clone);

    // Integer Type
    py::class_<Integer, std::shared_ptr<Integer>, 
        SimpleType>(m, "Integer")
        .def(py::init<>())
        .def(py::init<const int &, const int &>(),
            py::arg("l").none(false), py::arg("r").none(false))
        .def(py::init<std::shared_ptr<Range> >(), 
            py::arg("r").none(false))
        .def("isSigned", &Integer::isSigned)
        .def("getRange", &Integer::getRange, 
            py::return_value_policy::reference)
        .def("accept_visitor", &Integer::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Integer::getString)
        .def("clone", &Integer::clone);

    // Real Type
    py::class_<Real, std::shared_ptr<Real>, SimpleType>(m, "Real")
        .def(py::init<>())
        .def("getRange", &Real::getRange, 
            py::return_value_policy::reference)
        .def("setRange", &Real::setRange,
            py::arg("r").none(false))
        .def("accept_visitor", &Real::accept_visitor,
            py::arg("v").none(false))
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
            py::arg("lbound").none(false), 
            py::arg("rbound").none(false))
        .def("getType", &Range::getType, 
            py::return_value_policy::reference)
        .def("setRightValue", &Range::setRightValue,
            py::arg("rbound").none(false))
        .def("setLeftValue", &Range::setLeftValue,
            py::arg("lbound").none(false))
        .def("getLeftValue", &Range::getLeftValue)
        .def("getRightValue", &Range::getRightValue)
        .def("getString", &Range::getString)
        .def("accept_visitor", &Range::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Range::clone);

    //  Identifier Binding
    py::class_<Identifier, std::shared_ptr<Identifier>, 
        Value>(m, "Identifier")
        .def(py::init<std::shared_ptr<DataDeclaration>>(),
            py::arg("d")=nullptr)
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
            std::shared_ptr<Value>>(), py::arg("op").none(false),
            py::arg("op1").none(false), py::arg("op2")=nullptr)
        .def("getOperator", &Expression::getOperator)
        .def("getOp1", &Expression::getOp1)
        .def("getOp2", &Expression::getOp2)
        .def("setOperator", &Expression::setOperator, 
            py::arg("op").none(false))
        .def("setOp1", &Expression::setOp1,
            py::arg("op").none(false))
        .def("setOp2", &Expression::setOp2,
            py::arg("op").none(false))
        .def("getString", &Expression::getString)
        .def("getType", &Expression::getType)
        .def("accept_visitor", &Expression::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Expression::clone);


    //  BooleanValue Binding
    py::class_<BooleanValue, std::shared_ptr<BooleanValue>, 
        NumericValue>(m, "BooleanValue")
        .def(py::init<const bool &>(),
            py::arg("value") = false)
        .def(py::init<const BooleanValue &>(),
            py::arg("o").none(false))
        .def("getValue", &BooleanValue::getValue)
        .def("setValue", &BooleanValue::setValue,
            py::arg("v").none(false))
        .def("accept_visitor", &BooleanValue::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BooleanValue::getString)
        .def("clone", &BooleanValue::clone);

    //  IntegerValue Binding
    py::class_<IntegerValue, std::shared_ptr<IntegerValue>, 
        NumericValue>(m, "IntegerValue")
        .def(py::init<const int64_t &>(),
            py::arg("value") = 0)
        .def(py::init<const IntegerValue &>(),
            py::arg("o").none(false))
        .def("getValue", &IntegerValue::getValue)
        .def("setValue", &IntegerValue::setValue,
            py::arg("v").none(false))
        .def("accept_visitor", &IntegerValue::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &IntegerValue::getString)
        .def("clone", &IntegerValue::clone);

    //  RealValue Binding
    py::class_<RealValue, std::shared_ptr<RealValue>, 
        NumericValue>(m, "RealValue")
        .def(py::init<const double &>(),
            py::arg("value") = 0)
        .def(py::init<const RealValue &>(),
            py::arg("o").none(false))
        .def("getValue", &RealValue::getValue)
        .def("setValue", &RealValue::setValue,
            py::arg("v").none(false))
        .def("accept_visitor", &RealValue::accept_visitor,
            py::arg("v").none(false))
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
            py::arg("value").none(false))
        .def("accept_visitor", &Constant::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Constant::clone);

    // Causality Enum
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
            py::arg("name"), py::arg("causality_t")=generic)
        .def("getString", &Variable::getString)
        .def("getCausality", &Variable::getCausality)
        .def("setCausality", &Variable::setCausality, 
            py::arg("causality").none(false))
        .def("accept_visitor", &Variable::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Variable::clone);


    /**
    *  LOGIC FORMULA BINDINGS
    */

    py::class_<Specification, std::shared_ptr<Specification>, 
        ChaseObject>(m, "Specification");
    py::class_<LogicFormula, std::shared_ptr<LogicFormula>, 
        Specification>(m, "LogicFormula");

    // BinaryBooleanFormula
    py::class_<BinaryBooleanFormula, 
        std::shared_ptr<BinaryBooleanFormula>, 
        LogicFormula>(m, "BinaryBooleanFormula")
        .def(py::init<chase::BooleanOperator, 
            std::shared_ptr<LogicFormula>, 
            std::shared_ptr<LogicFormula>>(),
            py::arg("op")=op_and, 
            py::arg("op1")=nullptr,
            py::arg("op2")=nullptr)
        .def("getOp", &BinaryBooleanFormula::getOp)
        .def("getOp1", &BinaryBooleanFormula::getOp1)
        .def("getOp2", &BinaryBooleanFormula::getOp2)
        .def("setOp", &BinaryBooleanFormula::setOp,
            py::arg("op").none(false))
        .def("setOp1", &BinaryBooleanFormula::setOp1,
            py::arg("op1").none(false))
        .def("setOp2", &BinaryBooleanFormula::setOp2,
            py::arg("op2").none(false))
        .def("accept_visitor", &BinaryBooleanFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BinaryBooleanFormula::getString)
        .def("clone", &BinaryBooleanFormula::clone);
    
    // BinaryTemporalFormula
    py::class_<BinaryTemporalFormula, 
        std::shared_ptr<BinaryTemporalFormula>, 
        LogicFormula>(m, "BinaryTemporalFormula")
        .def(py::init<chase::TemporalOperator, 
            std::shared_ptr<LogicFormula>, 
            std::shared_ptr<LogicFormula>>(),
            py::arg("op")=op_until, 
            py::arg("op1")=nullptr,
            py::arg("op2")=nullptr)
        .def("getOp", &BinaryTemporalFormula::getOp)
        .def("getFormula1", &BinaryTemporalFormula::getFormula1)
        .def("getFormula2", &BinaryTemporalFormula::getFormula2)
        .def("setOp", &BinaryTemporalFormula::setOp,
            py::arg("op").none(false))
        .def("setFormula1", &BinaryTemporalFormula::setFormula1,
            py::arg("formula1").none(false))
        .def("setFormula2", &BinaryTemporalFormula::setFormula2,
            py::arg("formula2").none(false))
        .def("getString", &BinaryTemporalFormula::getString)
        .def("accept_visitor", &BinaryTemporalFormula::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &BinaryTemporalFormula::clone);
    
    // Boolean Constant
    py::class_<BooleanConstant,
        std::shared_ptr<BooleanConstant>, 
        LogicFormula>(m, "BooleanConstant")
        .def(py::init<bool &>(), py::arg("value")=true)
        .def("getValue", &BooleanConstant::getValue)
        .def("accept_visitor", &BooleanConstant::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BooleanConstant::getString)
        .def("clone", &BooleanConstant::clone);
    
    // LargeBooleanFormula
    py::class_<LargeBooleanFormula,
        std::shared_ptr<LargeBooleanFormula>,
        LogicFormula>(m, "LargeBooleanFormula")
        .def(py::init<BooleanOperator &>(),
            py::arg("op")=op_and)
        .def("getOp", &LargeBooleanFormula::getOp)
        .def("setOp", &LargeBooleanFormula::setOp,
            py::arg("op").none(false))
        .def("addOperand", &LargeBooleanFormula::addOperand,
            py::arg("f").none(false))
        .def("getString", &LargeBooleanFormula::getString)
        .def("accept_visitor", &LargeBooleanFormula::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &LargeBooleanFormula::clone);

    // ModalFormula
    py::class_<ModalFormula,
        std::shared_ptr<ModalFormula>,
        LogicFormula>(m, "ModalFormula")
        .def(py::init<ModalOperator &,
            std::shared_ptr<LogicFormula>>(),
            py::arg("op")=op_square, 
            py::arg("formula")=nullptr)
        .def("getOperator", &ModalFormula::getOperator)
        .def("getFormula", &ModalFormula::getFormula)
        .def("setOperator", &ModalFormula::setOperator,
            py::arg("op").none(false))
        .def("setFormula", &ModalFormula::setFormula,
            py::arg("formula").none(false))
        .def("getString", &ModalFormula::getString)
        .def("clone", &ModalFormula::clone);

    // Proposition
    py::class_<Proposition, 
        std::shared_ptr<Proposition>,
        LogicFormula>(m, "Proposition")
        .def(py::init<>())
        .def(py::init<std::shared_ptr<Value> &>(),
            py::arg("v").none(false))
        .def("getType", &Proposition::getType)
        .def("getValue", &Proposition::getValue)
        .def("getName", &Proposition::getName)
        .def("setValue", &Proposition::setValue,
            py::arg("v").none(false))
        .def("setName", &Proposition::setName,
            py::arg("n").none(false))
        .def("accept_visitor", &Proposition::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Proposition::getString)
        .def("clone", &Proposition::clone);

    // UnaryBooleanFormula
    py::class_<UnaryBooleanFormula,
        std::shared_ptr<UnaryBooleanFormula>,
        LogicFormula>(m, "UnaryBooleanFormula")
        .def(py::init<BooleanOperator &,
            std::shared_ptr<LogicFormula> &>(),
            py::arg("op")=op_not,
            py::arg("op1")=nullptr)
        .def("getOp", &UnaryBooleanFormula::getOp)
        .def("getOp1", &UnaryBooleanFormula::getOp1)
        .def("setOp", &UnaryBooleanFormula::setOp,
            py::arg("op").none(false))
        .def("setOp1", &UnaryBooleanFormula::setOp1,
            py::arg("op1").none(false))
        .def("accept_visitor", &UnaryBooleanFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &UnaryBooleanFormula::getString)
        .def("clone", &UnaryBooleanFormula::clone);

    // UnaryTemporalFormula
    py::class_<UnaryTemporalFormula,
        std::shared_ptr<UnaryTemporalFormula>,
        LogicFormula>(m, "UnaryTemporalFormula")
        .def(py::init<TemporalOperator &,
            std::shared_ptr<LogicFormula> &>(),
            py::arg("op")=op_globally,
            py::arg("op1")=nullptr)
        .def("getOp", &UnaryTemporalFormula::getOp)
        .def("getFormula", &UnaryTemporalFormula::getFormula)
        .def("setOp", &UnaryTemporalFormula::setOp,
            py::arg("op").none(false))
        .def("setFormula", &UnaryTemporalFormula::setFormula,
            py::arg("formula").none(false))
        .def("accept_visitor", &UnaryTemporalFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &UnaryTemporalFormula::getString)
        .def("clone", &UnaryTemporalFormula::clone);

    
    /**
    * GRAPH BINDINGS
    */ 

    // Edge  
    py::class_<Edge, std::shared_ptr<Edge>, ChaseObject>
        (m, "Edge")
        .def(py::init<unsigned int &, unsigned int &>(),
            py::arg("source").none(false), 
            py::arg("target").none(false))
        .def("getSource", &Edge::getSource)
        .def("setSource", &Edge::setSource,
            py::arg("source").none(false))
        .def("getTarget", &Edge::getTarget)
        .def("setTarget", &Edge::setTarget,
            py::arg("target").none(false))
        .def("accept_visitor", &Edge::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Edge::getString)
        .def("clone", &Edge::clone);

    // WeightedEdge
    py::class_<WeightedEdge, 
        std::shared_ptr<WeightedEdge>, Edge>
        (m, "WeightedEdge")
        .def(py::init<unsigned int &, 
            unsigned int &,
            std::shared_ptr<Value> & >(),
            py::arg("source").none(false), 
            py::arg("target").none(false),
            py::arg("weight").none(false))
        .def("getWeight", &WeightedEdge::getWeight)
        .def("setWeight", &WeightedEdge::setWeight,
            py::arg("weight").none(false))
        .def("getString", &Edge::getString)
        .def("clone", &Edge::clone);
    
    // Vertex
    py::class_<Vertex, std::shared_ptr<Vertex>,
        ChaseObject>(m, "Vertex")
        .def(py::init<std::shared_ptr<Name> &>(),
            py::arg("name")=nullptr)
        .def("getName", &Vertex::getName)
        .def("setName", &Vertex::setName,
            py::arg("name").none(false))
        .def("accept_visitor", &Vertex::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Vertex::getString)
        .def("clone", &Vertex::clone);
    
    // Graph
    py::class_<Graph, std::shared_ptr<Graph>,
        Specification>(m, "Graph")
        .def(py::init<unsigned int &, 
            bool &, std::shared_ptr<Name> &>(),
            py::arg("size").none(false), 
            py::arg("directed")=false,
            py::arg("name")=std::make_shared<Name>("GenericGraph"))
        .def("accept_visitor", &Graph::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Graph::getString)
        .def("associateVertex", &Graph::associateVertex,
            py::arg("index").none(false),
            py::arg("vertex").none(false))
        .def("addEdge", &Graph::addEdge,
            py::arg("edge").none(false))
        .def("isDirected", &Graph::isDirected)
        .def("getEdge", &Graph::getEdge,
            py::arg("source").none(false),
            py::arg("target").none(false))
        .def("getVertex", &Graph::getVertex,
            py::arg("vertex_id").none(false))
        .def("getSize", &Graph::getSize)
        .def("getVertexIndex", &Graph::getVertexIndex,
            py::arg("name").none(false))
        .def("getName", &Graph::getName)
        .def("setName", &Graph::setName,
            py::arg("name"))
        .def("getGraphViz", &Graph::getGraphViz)
        .def("getAdjacentNodes", &Graph::getAdjacentNodes,
            py::arg("id").none(false))
        .def("clone", &Graph::clone);

    m.def("getSubGraph", &chase::getSubGraph,
        py::arg("graph"), py::arg("vertexes"));
    m.def("findAllPathsBetweenNodes", &chase::findAllPathsBetweenNodes,
        py::arg("graph"), py::arg("visited"),
        py::arg("end"), py::arg("result"));
}

