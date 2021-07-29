#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Chase.hh"
#include "utilities/GraphUtilities.hh"
#include "utilities/Factory.hh"
#include "utilities/simplify.hh"





namespace py = pybind11;
using namespace chase;

template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

PYBIND11_MODULE(chasecorebnd, m) {
    m.doc() = "CHASE Core  Python wrapper module";

    py::class_<ChaseObject, std::unique_ptr<ChaseObject, 
        py::nodelete>>
        (m, "ChaseObject");

    py::class_<Name, std::unique_ptr<Name, 
        py::nodelete>, ChaseObject>
        (m, "Name")
        .def(py::init<std::string &>(), py::arg("s"))
        .def(py::init<const Name &>(), py::arg("o"))
        .def("accept_visitor", &Name::accept_visitor,
            py::arg("v").none(false))
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

    py::class_<Type, std::unique_ptr<Type, 
        py::nodelete>>(m, "Type");
    py::class_<SimpleType, Type, std::unique_ptr<SimpleType, 
        py::nodelete>>
    (m, "SimpleType");
    py::class_<CustomType, Type, std::unique_ptr<CustomType,
        py::nodelete>>
    (m, "CustomType");

    // Boolean Type
    py::class_<Boolean, std::unique_ptr<Boolean>, SimpleType>
        (m, "Boolean")
        .def(py::init<>())
        .def("accept_visitor", &Boolean::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Boolean::getString)
        .def("clone", &Boolean::clone);

    // Integer Type
    py::class_<Integer, std::unique_ptr<Integer, 
        py::nodelete>, 
        SimpleType>(m, "Integer")
        .def(py::init<>())
        .def(py::init<const int64_t &, const int64_t &>(),
            py::arg("min").none(false), py::arg("max").none(false))
        .def("isSigned", &Integer::isSigned)
        .def("getMin", &Integer::getMin)
        .def("getMax", &Integer::getMax)
        .def("accept_visitor", &Integer::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Integer::getString)
        .def("clone", &Integer::clone);

    // Real Type
    py::class_<Real, std::unique_ptr<Real, py::nodelete>,
        SimpleType>(m, "Real")
        .def(py::init<>())
        .def(py::init<const double &, const double &>(),
             py::arg("min").none(false), py::arg("max").none(false))
        .def("getMin", &Real::getMin)
        .def("getMax", &Real::getMax)
        .def("accept_visitor", &Real::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Real::getString)
        .def("clone", &Real::clone);



    /**
    *   VALUES BINDINGS
    */

    py::class_<Value, std::unique_ptr<Value, py::nodelete>, 
        ChaseObject>(m, "Value");
    py::class_<NumericValue, std::unique_ptr<NumericValue, 
        py::nodelete>, Value>(m, "NumericValue");




    //  Identifier Binding
    py::class_<Identifier, std::unique_ptr<Identifier, 
        py::nodelete>, Value>(m, "Identifier")
        .def(py::init<DataDeclaration *>(),
            py::arg("d")=nullptr)
        .def(py::init<const Identifier &>(), py::arg("i"))
        .def("getDeclaration", &Identifier::getDeclaration)
        .def("setDeclaration", &Identifier::setDeclaration, 
            py::arg("d"))
        .def("getString", &Identifier::getString)
        .def("getType", &Identifier::getType)
        .def("clone", &Identifier::clone);

    // Expression Binding
    py::class_<Expression, std::unique_ptr<Expression, 
        py::nodelete>, Value>(m, "Expression")
        .def(py::init<>())
        .def(py::init<Operator, Value *,
            Value *>(), py::arg("op").none(false),
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
    py::class_<BooleanValue, std::unique_ptr<BooleanValue, 
        py::nodelete>, NumericValue>(m, "BooleanValue")
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
    py::class_<IntegerValue, std::unique_ptr<IntegerValue, 
        py::nodelete>, NumericValue>(m, "IntegerValue")
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
    py::class_<RealValue, std::unique_ptr<RealValue, 
        py::nodelete>, NumericValue>(m, "RealValue")
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

    // Range Binding
    py::class_<Range, std::unique_ptr<Range,
    py::nodelete>, Value>(m, "Range")
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

    // Interval Binding
    py::class_<Interval, std::unique_ptr<Interval,
    py::nodelete>, Value>(m, "Interval")
    .def(py::init<Value *, Value*, bool, bool>(),
            py::arg("lbound").none(false),
            py::arg("rbound").none(false),
            py::arg("leftOpen").none(false),
            py::arg("rightOpen").none(false))
    .def("getLeftBound", &Interval::getLeftBound,
         py::return_value_policy::reference)
    .def("getRightBound", &Interval::getRightBound,
         py::return_value_policy::reference)
    .def("isLeftOpen", &Interval::isLeftOpen)
    .def("isRightOpen", &Interval::isRightOpen)
    .def("getType", &Interval::getType)
    .def("getString", &Interval::getString)
    .def("accept_visitor", &Interval::accept_visitor,
         py::arg("v").none(false))
    .def("clone", &Interval::clone);
    /**
    *   DECLARATION BINDINGS
    */

    py::class_<Declaration, std::unique_ptr<Declaration, 
        py::nodelete>, ChaseObject>(m, "Declaration");

    // ComponentDefinition Binding
    py::class_<ComponentDefinition, std::unique_ptr<
        ComponentDefinition, py::nodelete>, 
        Declaration>(m, "ComponentDefinition")
        .def(py::init<>())
        .def(py::init<Name *>(),
            py::arg("name").none(false))
        .def(py::init<std::string &>(),
            py::arg("name").none(false))
        .def("accept_visitor", &ComponentDefinition::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &ComponentDefinition::getString)
        .def("clone", &ComponentDefinition::clone)
        .def_readwrite("views", &ComponentDefinition::views)
        .def_readwrite("subcomponents", &ComponentDefinition::subcomponents);
    
    py::class_<DataDeclaration, std::unique_ptr<DataDeclaration, 
        py::nodelete>, Declaration>(m, "DataDeclaration");
    
    //Parameter Binding
    py::class_<Parameter, std::unique_ptr<Parameter, 
        py::nodelete>, DataDeclaration>(m, "Parameter")
        .def(py::init<Type *, Name *>(),
            py::arg("type")=nullptr, 
            py::arg("name")=nullptr)
        .def("accept_visitor", &Parameter::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Parameter::getString)
        .def("clone", &Parameter::clone);

    // Constant Binding
    py::class_<Constant, std::unique_ptr<Constant, 
        py::nodelete>, DataDeclaration>(m, "Constant")
        .def(py::init< Type *, Name *, Value* >(), 
            py::arg("type"), 
            py::arg("name"), 
            py::arg("value"))
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
    py::class_<Variable, std::unique_ptr<Variable, 
        py::nodelete>, DataDeclaration>(m, "Variable")
        .def(py::init<Type *, Name *, causality_t &>(),
            py::arg("type"), 
            py::arg("name"), 
            py::arg("causality_t")=generic)
        .def("getString", &Variable::getString)
        .def("getCausality", &Variable::getCausality)
        .def("setCausality", &Variable::setCausality, 
            py::arg("causality").none(false))
        .def("accept_visitor", &Variable::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Variable::clone);


    /**
    *  SPECIFICATIONS BINDINGS
    */

    py::class_<Specification, std::unique_ptr<Specification, 
        py::nodelete>, ChaseObject>(m, "Specification");
    py::class_<LogicFormula, std::unique_ptr<LogicFormula, 
        py::nodelete>, Specification>(m, "LogicFormula");

    py::enum_<chase::semantic_domain>(m, "semantic_domain")
        .value("logic", chase::semantic_domain::logic)
        .value("graph", chase::semantic_domain::graph)
        .export_values();

    // BinaryBooleanFormula
    py::class_<BinaryBooleanFormula, 
        std::unique_ptr<BinaryBooleanFormula, py::nodelete>, 
        LogicFormula>(m, "BinaryBooleanFormula")
        .def(py::init<chase::BooleanOperator, 
            LogicFormula *, 
            LogicFormula *>(),
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
        std::unique_ptr<BinaryTemporalFormula, py::nodelete>, 
        LogicFormula>(m, "BinaryTemporalFormula")
        .def(py::init<chase::TemporalOperator, 
            LogicFormula *, 
            LogicFormula *>(),
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
        std::unique_ptr<BooleanConstant, py::nodelete>, 
        LogicFormula>(m, "BooleanConstant")
        .def(py::init<bool &>(), py::arg("value")=true)
        .def("getValue", &BooleanConstant::getValue)
        .def("accept_visitor", &BooleanConstant::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BooleanConstant::getString)
        .def("clone", &BooleanConstant::clone);
    
    // LargeBooleanFormula
    py::class_<LargeBooleanFormula,
        std::unique_ptr<LargeBooleanFormula, py::nodelete>,
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
        std::unique_ptr<ModalFormula, py::nodelete>,
        LogicFormula>(m, "ModalFormula")
        .def(py::init<ModalOperator &,
            LogicFormula *>(),
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
        std::unique_ptr<Proposition, py::nodelete>,
        LogicFormula>(m, "Proposition")
        .def(py::init<>())
        .def(py::init<Value *>(),
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
        std::unique_ptr<UnaryBooleanFormula, py::nodelete>,
        LogicFormula>(m, "UnaryBooleanFormula")
        .def(py::init<BooleanOperator &,
            LogicFormula *>(),
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
        std::unique_ptr<UnaryTemporalFormula, py::nodelete>,
        LogicFormula>(m, "UnaryTemporalFormula")
        .def(py::init<TemporalOperator &,
            LogicFormula *>(),
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
    py::class_<Edge, std::unique_ptr<Edge, py::nodelete>, 
        ChaseObject>(m, "Edge")
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
        std::unique_ptr<WeightedEdge, py::nodelete>, Edge>
        (m, "WeightedEdge")
        .def(py::init<unsigned int &, 
            unsigned int &,
            Value *>(),
            py::arg("source").none(false), 
            py::arg("target").none(false),
            py::arg("weight").none(false))
        .def("getWeight", &WeightedEdge::getWeight)
        .def("setWeight", &WeightedEdge::setWeight,
            py::arg("weight").none(false))
        .def("getString", &Edge::getString)
        .def("clone", &Edge::clone);
    
    // Vertex
    py::class_<Vertex, std::unique_ptr<Vertex, py::nodelete>,
        ChaseObject>(m, "Vertex")
        .def(py::init<Name *>(),
            py::arg("name")=nullptr)
        .def("getName", &Vertex::getName)
        .def("setName", &Vertex::setName,
            py::arg("name").none(false))
        .def("accept_visitor", &Vertex::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Vertex::getString)
        .def("clone", &Vertex::clone);
    
    // Graph
    py::class_<Graph, std::unique_ptr<Graph, py::nodelete>,
        Specification>(m, "Graph")
        .def(py::init<unsigned int &, 
            bool &, Name *>(),
            py::arg("size").none(false), 
            py::arg("directed")=false,
            py::arg("name")=new Name("GenericGraph"))
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

    
    /**
    * SYSTEM BINDINGS
    */ 
    py::class_<System, std::unique_ptr<System,
        py::nodelete>, ChaseObject>(m, "System")
        .def(py::init<std::string &>(),
            py::arg("name")="System")
        .def("addDeclaration", &System::addDeclaration,
            py::arg("declaration").none(false))
        .def("addContract", &System::addContract,
            py::arg("contract").none(false))
        .def("addComponent", &System::addComponent,
            py::arg("component").none(false))
        .def("getDeclarationsSet", &System::getDeclarationsSet,
            py::return_value_policy::reference)
        .def("getContractsSet", &System::getContractsSet,
            py::return_value_policy::reference)
        .def("getComponentsSet", &System::getComponentsSet,
            py::return_value_policy::reference)
        .def("accept_visitor", &System::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &System::getString)
        .def("clone", &System::clone)
        .def("getName", &System::getName,
            py::return_value_policy::reference)
        .def("setName", &System::setName,
            py::arg("name").none(false));
    
    /**
    * CONTRACT BINDINGS
    */ 

    py::class_<Contract, std::unique_ptr<Contract, 
        py::nodelete>, ChaseObject>(m, "Contract")
        .def(py::init<std::string >(),
            py::arg("name")="contract")
        .def("getName", &Contract::getName,
            py::return_value_policy::reference)
        .def("setName", &Contract::setName,
            py::arg("name").none(false))
        .def("addDeclaration", &Contract::addDeclaration,
            py::arg("declaration").none(false))
        .def("addAssumptions", &Contract::addAssumptions,
            py::arg("domain").none(false),
            py::arg("spec").none(false))
        .def("addGuarantees", &Contract::addGuarantees,
            py::arg("domain").none(false),
            py::arg("spec").none(false))
        .def("accept_visitor", &Contract::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Contract::getString)
        .def("clone", &Contract::clone)
        .def_static("saturate", &Contract::saturate,
            py::arg("c").none(false))
        .def_static("composition", &Contract::composition,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("correspondences").none(false),
            py::arg("name")="composition")
        .def_static("composeLogic", &Contract::composeLogic,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("r").none(false))
        .def_static("conjunction", &Contract::conjunction,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("correspondences").none(false),
            py::arg("name")="conjunction")
        .def_static("conjoinLogic", &Contract::conjoinLogic,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("r").none(false))
        .def_static("mergeDeclarations", 
            &Contract::mergeDeclarations,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("r").none(false),
            py::arg("correspondences").none(false),
            py::arg("declaration_map").none(false))
        .def_static("saturateLogic", &Contract::saturateLogic,
            py::arg("c").none(false))
        .def_readwrite("declarations", &Contract::declarations)
        .def_readwrite("assumptions", &Contract::assumptions)
        .def_readwrite("guarantees", &Contract::guarantees);

    /**
    * COMPONENT BINDINGS
    */ 
    py::class_<Component, std::unique_ptr<Component, 
        py::nodelete>, ChaseObject>(m, "Component")
        .def(py::init<ComponentDefinition *, std::string &>(),
            py::arg("definition").none(false),
            py::arg("name").none(false))
        .def("getDefinition", &Component::getDefinition)
        .def("setDefinition", &Component::setDefinition,
            py::arg("definition").none(false))
        .def("getName", &Component::getName)
        .def("setName", &Component::setName,
            py::arg("name").none(false))
        .def("getParameterValue", &Component::getParameterValue,
            py::arg("view").none(false),
            py::arg("param").none(false))
        .def("setParameter", &Component::setParameter,
            py::arg("view").none(false),
            py::arg("param").none(false),
            py::arg("value").none(false))
        .def("accept_visitor", &Component::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Component::getString)
        .def("clone", &Component::clone);

    /**
    * CHASE TIME BINDING
    */
    py::enum_<chase::chase_time_unit>(m, "chase_time_unit")
        .value("CHASE_SEC", chase::chase_time_unit::CHASE_SEC)
        .value("CHASE_MS", chase::chase_time_unit::CHASE_MS)
        .value("CHASE_US", chase::chase_time_unit::CHASE_US)
        .value("CHASE_NS", chase::chase_time_unit::CHASE_NS)
        .value("CHASE_PS", chase::chase_time_unit::CHASE_PS)
        .value("CHASE_FS", chase::chase_time_unit::CHASE_FS)
        .export_values();

    py::class_<chase_time, std::unique_ptr<chase_time, 
        py::nodelete>>(m, "chase_time")
        .def(py::init<unsigned int, chase_time_unit>(),
            py::arg("a")=1, py::arg("u")=CHASE_SEC)
        .def(py::init<unsigned int, std::string>(),
            py::arg("a"), py::arg("u"));

    /**
    * UTILITIES
    * BINDINGS
    *
    */

    /**
    * VISITOR BINDINGS
    */ 

    //BaseVisitor Binding
    py::class_<BaseVisitor, std::unique_ptr<BaseVisitor,
        py::nodelete>>(m, "BaseVisitor");
    
    py::class_<GuideVisitor, std::unique_ptr<GuideVisitor, 
        py::nodelete>>(m, "GuideVisitor")
        .def(py::init<int &>(),
            py::arg("rv")=0)
        .def("visitSystem", &GuideVisitor::visitSystem)
        .def("vistiRange", &GuideVisitor::visitRange)
        .def("visitIntegerValue", 
            &GuideVisitor::visitIntegerValue)
        .def("visitRealValue", 
            &GuideVisitor::visitRealValue)
        .def("visitBooleanValue", 
            &GuideVisitor::visitBooleanValue)
        .def("visitExpression", 
            &GuideVisitor::visitExpression)
        .def("visitIdentifier", 
            &GuideVisitor::visitIdentifier)
        .def("visitInteger", &GuideVisitor::visitInteger)
        .def("visitReal", &GuideVisitor::visitReal)
        .def("visitBoolean", &GuideVisitor::visitBoolean)
        .def("visitName", &GuideVisitor::visitName)
        .def("visitVariable", &GuideVisitor::visitVariable)
        .def("visitConstant", &GuideVisitor::visitConstant)
        .def("visitParameter", &GuideVisitor::visitParameter)
        .def("visitComponentDefinition", 
            &GuideVisitor::visitComponentDefinition)
        .def("visitProposition", 
            &GuideVisitor::visitProposition)
        .def("visitBooleanConstant", 
            &GuideVisitor::visitBooleanConstant)
        .def("visitBinaryBooleanOperation", 
            &GuideVisitor::visitBinaryBooleanOperation)
        .def("visitUnaryBooleanOperation", 
            &GuideVisitor::visitUnaryBooleanOperation)
        .def("visitLargeBooleanFormula", 
            &GuideVisitor::visitLargeBooleanFormula)
        .def("visitModalFormula",
            &GuideVisitor::visitModalFormula)
        .def("visitUnaryTemporalOperation",
            &GuideVisitor::visitUnaryTemporalOperation)
        .def("visitBinaryTemporalOperation",
            &GuideVisitor::visitBinaryTemporalOperation)
        .def("visitContract", &GuideVisitor::visitContract)
        .def("visitComponent", &GuideVisitor::visitComponent)
        .def("visitGraph", &GuideVisitor::visitGraph)
        .def("visitEdge", &GuideVisitor::visitEdge)
        .def("visitVertex", &GuideVisitor::visitVertex);

    /**
    * SIMPLIFY
    */

    m.def("simplify", &chase::simplify,
            py::arg("object").none(false),
            py::arg("options").none(false));


    py::class_<simplify_options, std::unique_ptr<simplify_options,
            py::nodelete>>(m, "simplify_options")
            .def(py::init<bool&, bool&>(),
                    py::arg("_nots")=true,
                    py::arg("_temporal_operators")=true)
            .def_readwrite("nots", &simplify_options::nots)
            .def_readwrite("temporal_operators",
                    &simplify_options::temporal_operators);

    /**
    * FACTORY BINDINGS
    */  
    m.def("True", &chase::True);
    m.def("False", &chase::False);
    m.def("Not", &chase::Not,
        py::arg("op").none(false));
    m.def("And", &chase::And,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Or", &chase::Or,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Implies", &chase::Implies,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Iff", &chase::Iff,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Nand", &chase::Nand,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Xor", &chase::Xor,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Nor", &chase::Nor,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Xnor", &chase::Xnor,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("LargeAnd", &chase::LargeAnd,
        py::arg("formulas").none(false));
    m.def("LargeOr", &chase::LargeOr,
        py::arg("formulas").none(false));
    m.def("Always", &chase::Always,
        py::arg("op").none(false));
    m.def("Eventually", &chase::Eventually,
        py::arg("op").none(false));
    m.def("Next", &chase::Next,
        py::arg("op").none(false));
    m.def("Until", &chase::Until,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Sum", &chase::Sum,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Sub", &chase::Sub,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Mult", &chase::Mult,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Div", &chase::Div,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Eq", &chase::Eq,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("NEq", &chase::NEq,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Eq", &chase::Eq,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("LE", &chase::LE,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("LT", &chase::LT,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("GE", &chase::GE,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("GT", &chase::GT,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    m.def("Prop", overload_cast_<Variable*>()
        (&chase::Prop), 
        py::arg("var").none(false));
    m.def("Prop", overload_cast_<Expression*>()
        (&chase::Prop), 
        py::arg("var").none(false));
    m.def("Id", &chase::Id,
        py::arg("declaration").none(false));
    m.def("intVal", &chase::IntVal,
        py::arg("n").none(false));
    m.def("RealVal", &chase::RealVal,
        py::arg("r").none(false));
    m.def("BoolVal", &chase::BoolVal,
        py::arg("b").none(false));
    
    /**
    * BACKENDS BINDINGS
    */  

    //GR1C Printer
    py::class_<GR1CPrinter, std::unique_ptr<GR1CPrinter,
        py::nodelete>, GuideVisitor>(m, "GR1CPrinter")
        .def(py::init<>())
        .def("print", &GR1CPrinter::print,
            py::arg("contract").none(false),
            py::arg("path").none(false))
        .def("visitIntegerValue", 
            &GR1CPrinter::visitIntegerValue,
            py::arg("o").none(false))
        .def("visitExpression", 
            &GR1CPrinter::visitExpression,
            py::arg("o").none(false))
        .def("visitIdentifier", 
            &GR1CPrinter::visitIdentifier,
            py::arg("o").none(false))
        .def("visitProposition", 
            &GR1CPrinter::visitProposition,
            py::arg("o").none(false))
        .def("visitBooleanConstant", 
            &GR1CPrinter::visitBooleanConstant,
            py::arg("o").none(false))
        .def("visitBinaryBooleanOperation", 
            &GR1CPrinter::visitBinaryBooleanOperation,
            py::arg("o").none(false))
        .def("visitUnaryBooleanOperation", 
            &GR1CPrinter::visitUnaryBooleanOperation,
            py::arg("o").none(false))
        .def("visitUnaryTemporalOperation", 
            &GR1CPrinter::visitUnaryTemporalOperation,
            py::arg("o").none(false))
        .def("visitBinaryTemporalOperation", 
            &GR1CPrinter::visitBinaryTemporalOperation,
            py::arg("o").none(false));
    
    //Slugs Printer
    py::class_<SlugsPrinter, std::unique_ptr<SlugsPrinter,
        py::nodelete>, GuideVisitor>(m, "SlugsPrinter")
        .def(py::init<>())
        .def("print", &SlugsPrinter::print,
            py::arg("contract").none(false),
            py::arg("path").none(false))
        .def("visitIntegerValue", 
            &SlugsPrinter::visitIntegerValue,
            py::arg("o").none(false))
        .def("visitExpression", 
            &SlugsPrinter::visitExpression,
            py::arg("o").none(false))
        .def("visitIdentifier", 
            &SlugsPrinter::visitIdentifier,
            py::arg("o").none(false))
        .def("visitProposition", 
            &SlugsPrinter::visitProposition,
            py::arg("o").none(false))
        .def("visitBooleanConstant", 
            &SlugsPrinter::visitBooleanConstant,
            py::arg("o").none(false))
        .def("visitBinaryBooleanOperation", 
            &SlugsPrinter::visitBinaryBooleanOperation,
            py::arg("o").none(false))
        .def("visitUnaryBooleanOperation", 
            &SlugsPrinter::visitUnaryBooleanOperation,
            py::arg("o").none(false))
        .def("visitUnaryTemporalOperation", 
            &SlugsPrinter::visitUnaryTemporalOperation,
            py::arg("o").none(false))
        .def("visitBinaryTemporalOperation", 
            &SlugsPrinter::visitBinaryTemporalOperation,
            py::arg("o").none(false));
}

