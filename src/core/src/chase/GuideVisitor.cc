/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */



#include "GuideVisitor.hh"

chase::GuideVisitor::~GuideVisitor() = default;

chase::GuideVisitor::GuideVisitor(int rv) :
    _rv(rv)
{
}

int chase::GuideVisitor::visitRange(chase::Range &o) {
    return BaseVisitor::visitRange(o);
}

int chase::GuideVisitor::visitIntegerValue(chase::IntegerValue &o) {
    return BaseVisitor::visitIntegerValue(o);
}

int chase::GuideVisitor::visitRealValue(chase::RealValue &o) {
    return BaseVisitor::visitRealValue(o);
}

int chase::GuideVisitor::visitBooleanValue(chase::BooleanValue &o) {
    return BaseVisitor::visitBooleanValue(o);
}

int chase::GuideVisitor::visitExpression(chase::Expression &o) {
    return BaseVisitor::visitExpression(o);
}

int chase::GuideVisitor::visitIdentifier(chase::Identifier &o) {
    return BaseVisitor::visitIdentifier(o);
}

int chase::GuideVisitor::visitInteger(chase::Integer &o) {
    return BaseVisitor::visitInteger(o);
}

int chase::GuideVisitor::visitReal(chase::Real &o) {
    return BaseVisitor::visitReal(o);
}

int chase::GuideVisitor::visitBoolean(chase::Boolean &o) {
    return BaseVisitor::visitBoolean(o);
}

int chase::GuideVisitor::visitName(chase::Name &o) {
    return BaseVisitor::visitName(o);
}

int chase::GuideVisitor::visitVariable(chase::Variable &o) {
    return BaseVisitor::visitVariable(o);
}

int chase::GuideVisitor::visitConstant(chase::Constant &o) {
    return BaseVisitor::visitConstant(o);
}

int chase::GuideVisitor::visitProposition(chase::Proposition &o) {
    return BaseVisitor::visitProposition(o);
}

int chase::GuideVisitor::visitBooleanConstant(chase::BooleanConstant &o) {
    return BaseVisitor::visitBooleanConstant(o);
}

int chase::GuideVisitor::visitBinaryBooleanOperation(
        chase::BinaryBooleanFormula &o) {
    return BaseVisitor::visitBinaryBooleanOperation(o);
}

int
chase::GuideVisitor::visitUnaryBooleanOperation(chase::UnaryBooleanFormula &o) {
    return BaseVisitor::visitUnaryBooleanOperation(o);
}

int
chase::GuideVisitor::visitLargeBooleanFormula(chase::LargeBooleanFormula &o) {
    return BaseVisitor::visitLargeBooleanFormula(o);
}

int chase::GuideVisitor::visitModalOperation(chase::ModalFormula &o) {
    return BaseVisitor::visitModalOperation(o);
}

int chase::GuideVisitor::visitUnaryTemporalOperation(
        chase::UnaryTemporalFormula &o) {
    return BaseVisitor::visitUnaryTemporalOperation(o);
}

int chase::GuideVisitor::visitBinaryTemporalOperation(
        chase::BinaryTemporalFormula &o) {
    return BaseVisitor::visitBinaryTemporalOperation(o);
}

int chase::GuideVisitor::visitContract(chase::Contract &o) {
    return BaseVisitor::visitContract(o);
}

int chase::GuideVisitor::visitEdge(chase::Edge &o) {
    return BaseVisitor::visitEdge(o);
}

int chase::GuideVisitor::visitVertex(chase::Vertex &o) {
    return BaseVisitor::visitVertex(o);
}

int chase::GuideVisitor::visitGraph(chase::Graph &o) {
    return BaseVisitor::visitGraph(o);
}
