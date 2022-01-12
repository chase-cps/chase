/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.\n
 *              Copyright (c) 2019 by Singapore University of Technology and Design.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @brief       Implementation of the base visitor.
 */


#include "BaseVisitor.hh"

using namespace chase;

BaseVisitor::BaseVisitor()
{
}

BaseVisitor::~BaseVisitor()
{
}
/// @cond

int BaseVisitor::visitRange( Range & ){ return 0;}
int BaseVisitor::visitSystem( System & ){ return 0;}
int BaseVisitor::visitIntegerValue( IntegerValue & ){ return 0;}
int BaseVisitor::visitRealValue( RealValue & ){ return 0;}
int BaseVisitor::visitBooleanValue( BooleanValue & ){return 0;}
int BaseVisitor::visitExpression(Expression & ){return 0;}
int BaseVisitor::visitIdentifier( Identifier & ){return 0;}
int BaseVisitor::visitInteger( Integer & ){return 0;}
int BaseVisitor::visitReal( Real & ){return 0;}
int BaseVisitor::visitBoolean( Boolean & ){return 0;}
int BaseVisitor::visitEnumeration(Enumeration &) {return 0;}
int BaseVisitor::visitCustomType(CustomType &) {return 0;}
int BaseVisitor::visitName( Name & ){ return 0;}
int BaseVisitor::visitVariable( Variable & ){return 0;}
int BaseVisitor::visitConstant( Constant & ){return 0;}
int BaseVisitor::visitProposition(Proposition &) {return 0;}
int BaseVisitor::visitUnaryBooleanOperation(UnaryBooleanFormula &){return 0;}
int BaseVisitor::visitBinaryBooleanOperation(BinaryBooleanFormula &){return 0;}
int BaseVisitor::visitBooleanConstant(BooleanConstant &) {return 0;}
int BaseVisitor::visitModalFormula(ModalFormula &) {return 0;}
int BaseVisitor::visitBinaryTemporalOperation(BinaryTemporalFormula &) {
    return 0;
}
int BaseVisitor::visitUnaryTemporalOperation(UnaryTemporalFormula &) {
    return 0;
}
int BaseVisitor::visitContract(Contract &) {return 0;}
int BaseVisitor::visitEdge(Edge &) {return 0;}
int BaseVisitor::visitVertex(Vertex &) {return 0;}
int BaseVisitor::visitGraph(Graph &){return 0;}
int BaseVisitor::visitLargeBooleanFormula(LargeBooleanFormula &) {return 0;}
int BaseVisitor::visitComponentDefinition(ComponentDefinition &) {return 0;}
int BaseVisitor::visitParameter(Parameter &) {return 0;}
int BaseVisitor::visitComponent(Component &) {return 0;}
int BaseVisitor::visitStringValue(StringValue &) {return 0;}
int BaseVisitor::visitString(String &) {return 0;}
int BaseVisitor::visitInterval(Interval &) {return 0;}
int BaseVisitor::visitMatrix(Matrix &) {return 0;}
int BaseVisitor::visitDistribution(Distribution &) {return 0;}
int BaseVisitor::visitFunction(Function &) {return 0;}
int BaseVisitor::visitQuantifiedFormula(QuantifiedFormula &) {return 0;}
int BaseVisitor::visitLibrary(Library &) {return 0;}
int BaseVisitor::visitFunctionCall(FunctionCall &) {return 0;}
int BaseVisitor::visitDesignProblem(DesignProblem &) {return 0;}
int BaseVisitor::visitConstraint(Constraint &) {return 0;}
int BaseVisitor::visitProbabilityFunction(ProbabilityFunction &) {return 0;}









/// @endcond
