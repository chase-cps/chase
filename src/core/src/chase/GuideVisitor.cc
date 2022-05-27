/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 */



#include "GuideVisitor.hh"
#include "representation.hh"

chase::GuideVisitor::~GuideVisitor() = default;

chase::GuideVisitor::GuideVisitor(int rv) :
    _rv(rv)
{
}

int chase::GuideVisitor::visitRange(chase::Range &)
{
    return _rv;
}

int chase::GuideVisitor::visitIntegerValue(chase::IntegerValue &o )
{
    return continueVisit(o.getType());
}

int chase::GuideVisitor::visitRealValue(chase::RealValue &o )
{
    return continueVisit(o.getType());
}

int chase::GuideVisitor::visitBooleanValue(chase::BooleanValue &o )
{
    return continueVisit(o.getType());
}

int chase::GuideVisitor::visitExpression(chase::Expression &o)
{
    int rv = continueVisit(o.getOp1());
    rv |= continueVisit(o.getOp2());
    return rv;
}

int chase::GuideVisitor::visitIdentifier(chase::Identifier &o)
{
    return continueVisit(o.getType());
}

int chase::GuideVisitor::visitInteger(chase::Integer & )
{
    return _rv;
}

int chase::GuideVisitor::visitReal(chase::Real & )
{
    return _rv;
}

int chase::GuideVisitor::visitBoolean(chase::Boolean & )
{
    return _rv;
}

int chase::GuideVisitor::visitName(chase::Name & ) {
    return _rv;
}

int chase::GuideVisitor::visitVariable(chase::Variable &o) {
    int rv = visitName(* o.getName());
    rv |= continueVisit(o.getType());
    return rv;
}

int chase::GuideVisitor::visitConstant(chase::Constant &o) {
    int rv = visitName(* o.getName());
    rv |= continueVisit(o.getType());
    rv |= continueVisit(o.getValue());
    return rv;
}

int chase::GuideVisitor::visitProposition(chase::Proposition &o) {
    int rv = visitName(* o.getName());
    rv |= continueVisit(o.getType());
    rv |= continueVisit(o.getValue());
    return rv;
}

int chase::GuideVisitor::visitBooleanConstant(chase::BooleanConstant &) {
    return _rv;
}

int chase::GuideVisitor::visitBinaryBooleanOperation(
        chase::BinaryBooleanFormula &o) {
    int rv = continueVisit(o.getOp1());
    rv |= continueVisit(o.getOp2());
    return rv;
}

int
chase::GuideVisitor::visitUnaryBooleanOperation(chase::UnaryBooleanFormula &o)
{
    return continueVisit(o.getOp1());
}

int
chase::GuideVisitor::visitLargeBooleanFormula(chase::LargeBooleanFormula &o)
{
    return visitVector(o.operands);
}

int chase::GuideVisitor::visitModalFormula(chase::ModalFormula &o)
{
    return continueVisit(o.getFormula());
}

int chase::GuideVisitor::visitUnaryTemporalOperation(
        chase::UnaryTemporalFormula &o)
{
    int rv = 0;
    if(o.getInterval() != nullptr) rv |= visitInterval(* o.getInterval());
    rv |= continueVisit(o.getFormula());
    return rv;
}

int chase::GuideVisitor::visitBinaryTemporalOperation(
        chase::BinaryTemporalFormula &o)
{
    int rv = 0;
    if(o.getInterval() != nullptr) rv |= visitInterval(* o.getInterval());
    rv = continueVisit(o.getFormula1());
    rv |= continueVisit(o.getFormula2());
    return rv;
}

int chase::GuideVisitor::visitContract(chase::Contract &o)
{
    int rv = visitName(*o.getName());

    rv |= visitList(o.declarations);

    for (auto it = o.assumptions.begin(); it != o.assumptions.end(); ++it)
    {
        rv |= continueVisit((*it).second);
    }

    for (auto it = o.guarantees.begin(); it != o.guarantees.end(); ++it)
    {
        rv |= continueVisit((*it).second);
    }

    return rv;
}

int chase::GuideVisitor::visitEdge(chase::Edge &o)
{
    WeightedEdge * w = dynamic_cast< WeightedEdge * >(&o);
    if( w != nullptr )
        return continueVisit( w->getWeight() );
    else
        return _rv;
}

int chase::GuideVisitor::visitVertex(chase::Vertex &o) {
    return visitName(*o.getName());
}

int chase::GuideVisitor::visitGraph(chase::Graph &o) {
    int rv = _rv;
    for(size_t i = 0; i < o.getSize(); ++i)
    {
        rv |= visitVertex(* o.getVertex(i));
    }

    for( size_t i = 0; i < o.getSize(); ++i )
        for( size_t j = 0; j < o.getSize(); ++j )
        {
            Edge * e = o.getEdge(i, j);
            if(e != nullptr)
                rv |= visitEdge(*e);
        }
    return rv;
}






int chase::GuideVisitor::continueVisit(chase::ChaseObject *o)
{
    // If o is Null, then it means that something is missing. In general,
    // it may be ok. Consider the case of a proposition with no value assigned
    // yet: in general it is like that.
    if( o == nullptr ) return _rv;

    switch(o->IsA())
    {
        // Values.
        case range_node:{
            auto v = reinterpret_cast< Range * >(o);
            return v->accept_visitor(*this);
        }
        case integerValue_node:{
            auto v = reinterpret_cast< IntegerValue * >(o);
            return v->accept_visitor(*this);
        }
        case realValue_node:{
            auto v = reinterpret_cast< RealValue * >(o);
            return v->accept_visitor(*this);
        }
        case booleanValue_node:{
            auto v = reinterpret_cast< BooleanValue * >(o);
            return v->accept_visitor(*this);
        }
        case expression_node: {
            auto v = reinterpret_cast< Expression *>(o);
            return v->accept_visitor(*this);
        }
        case identifier_node: {
            auto v = reinterpret_cast< Identifier * > (o);
            return v->accept_visitor(*this);
        }
        case probabilityFunction_node : {
            auto v = reinterpret_cast< ProbabilityFunction * >(o);
            return v->accept_visitor(*this);
        }
        // Types
        case integer_node: {
            auto v = reinterpret_cast< Integer * > (o);
            return v->accept_visitor(*this);
        }
        case real_node: {
            auto v = reinterpret_cast< Real * > (o);
            return v->accept_visitor(*this);
        }
        case boolean_node: {
            auto v = reinterpret_cast< Boolean * > (o);
            return v->accept_visitor(*this);
        }

        // Declaration
        case name_node : {
            auto v = reinterpret_cast< Name * > (o);
            return v->accept_visitor(*this);
        }
        case variable_node : {
            auto v = reinterpret_cast< Variable * > (o);
            return v->accept_visitor(*this);
        }
        case constant_node : {
            auto v = reinterpret_cast< Constant * > (o);
            return v->accept_visitor(*this);
        }

        // Boolean Formulas
        case proposition_node : {
            auto v = reinterpret_cast< Proposition * > (o);
            return v->accept_visitor(*this);
        }
        case booleanConstant_node: {
            auto v = reinterpret_cast< BooleanConstant * > (o);
            return v->accept_visitor(*this);
        }
        case binaryBooleanOperation_node: {
            auto v = reinterpret_cast< BinaryBooleanFormula * > (o);
            return v->accept_visitor(*this);
        }
        case unaryBooleanOperation_node: {
            auto v = reinterpret_cast< UnaryBooleanFormula *>(o);
            return v->accept_visitor(*this);
        }

        case large_boolean_formula_node: {
            auto v = reinterpret_cast< LargeBooleanFormula *>(o);
            return v->accept_visitor(*this);
        }

        case modalFormula_node: {
            auto v = reinterpret_cast< ModalFormula * >(o);
            return v->accept_visitor(*this);
        }

        case unaryTemporalOperation_node: {
            auto v = reinterpret_cast< UnaryTemporalFormula *>(o);
            return v->accept_visitor(*this);
        }
        case binaryTemporalOperation_node: {
            auto v = reinterpret_cast< BinaryTemporalFormula *>(o);
            return v->accept_visitor(*this);
        }

        // Graphs

        case graph_node : {
            auto v = reinterpret_cast< Graph *>(o);
            return v->accept_visitor(*this);
        }
        case graph_edge_node : {
            auto v = reinterpret_cast< Edge *>(o);
            return v->accept_visitor(*this);
        }
        case graph_vertex_node : {
            auto v = reinterpret_cast< Vertex *>(o);
            return v->accept_visitor(*this);
        }


        default:
            messageError("Unsupported formula.");
            break;
    }
    return _rv;
}

int chase::GuideVisitor::visitSystem(chase::System &o)
{
    int rv = 0;
    auto declarations = o.getDeclarationsSet();
    for(auto i = declarations.begin(); i != declarations.end(); ++i)
    {
        rv |= (*i)->accept_visitor(*this);
    }
    auto contracts = o.getContractsSet();
    for(auto i = contracts.begin(); i != contracts.end(); ++i)
    {
        rv |= (*i)->accept_visitor(*this);
    }
    auto components = o.getComponentsSet();
    for(auto i = components.begin(); i != components.end(); ++i)
    {
        rv |= (*i)->accept_visitor(*this);
    }
    return rv;
}

int chase::GuideVisitor::visitComponentDefinition(
        chase::ComponentDefinition &o)
{
    int rv = visitName( * o.getName());
    for( auto i = o.views.begin(); i != o.views.end(); ++i )
    {
        Contract * view = i->second;
        rv |= view->accept_visitor(*this);
    }
    return rv;
}

int chase::GuideVisitor::visitParameter(chase::Parameter &o)
{
    int rv = o.getType()->accept_visitor(*this);
    rv |= o.getName()->accept_visitor(*this);
    return rv;
}

int chase::GuideVisitor::visitComponent(chase::Component &o)
{
    int rv = o.getName()->accept_visitor(*this);

    for(
            auto i =  o.getDefinition()->views.begin();
            i != o.getDefinition()->views.end(); ++i )
    {
        std::string v = i->first;

        auto m = o.getParametersInView(v);
        for(auto j = m.begin(); j != m.end(); ++j )
        {
            rv |= j->second->accept_visitor(*this);
        }
    }
    return rv;
}

int chase::GuideVisitor::visitEnumeration(chase::Enumeration &o) {
    auto rv = o.getName()->accept_visitor(*this);
    return rv;
}

int chase::GuideVisitor::visitCustomType(chase::CustomType &o) {
    auto rv = o.getName()->accept_visitor(*this);
    return rv;
}

int chase::GuideVisitor::visitStringValue(chase::StringValue &value) {
    return BaseVisitor::visitStringValue(value);
}

int chase::GuideVisitor::visitString(chase::String &s) {
    return BaseVisitor::visitString(s);
}

int chase::GuideVisitor::visitInterval(chase::Interval &o ) {
    int rv = o.getLeftBound()->accept_visitor(*this);
    rv |= o.getRightBound()->accept_visitor(*this);
    return rv;
}

int chase::GuideVisitor::visitMatrix(chase::Matrix &matrix) {
    int rv = 1;
    for(size_t i = 1; i <= matrix.getRows(); ++i)
        for(size_t j = 1; j <= matrix.getColumns(); ++j)
            rv |= matrix.at(i,j)->accept_visitor(*this);
    return rv;
}

int chase::GuideVisitor::visitDistribution(chase::Distribution &distribution) {
    int rv = distribution.getName()->accept_visitor(*this);
    rv |= distribution.getType()->accept_visitor(*this);
    for(auto it: distribution.parameters)
    {
        it.second->accept_visitor(*this);
    }
    return 1;
}

int chase::GuideVisitor::visitQuantifiedFormula(
        chase::QuantifiedFormula &qf) {
    int rv = qf.getVariable()->accept_visitor(*this);
    rv |= qf.getFormula()->accept_visitor(*this);
    return rv;
}

int chase::GuideVisitor::visitProbabilityFunction(
        chase::ProbabilityFunction &function) {
    return function.getSpecification()->accept_visitor(*this);
}







