/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/Factory.hh"

using namespace chase;

std::shared_ptr<UnaryBooleanFormula> chase::Not(std::shared_ptr<LogicFormula>op) {
    return std::shared_ptr<UnaryBooleanFormula>(new UnaryBooleanFormula(op_not, op));
}

std::shared_ptr<BinaryBooleanFormula> chase::And(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_and, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Or(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_or, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Implies(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_implies, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Iff(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_iff, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Nand(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_nand, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Xor(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_xor, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Nor(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_nor, op1, op2));
}

std::shared_ptr<BinaryBooleanFormula> chase::Xnor(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::shared_ptr<BinaryBooleanFormula>(new BinaryBooleanFormula(op_xnor, op1, op2));
}

std::shared_ptr<UnaryTemporalFormula> chase::Always(std::shared_ptr<LogicFormula>op) {
    return std::shared_ptr<UnaryTemporalFormula>(new UnaryTemporalFormula(op_globally, op));
}

std::shared_ptr<UnaryTemporalFormula> chase::Eventually(std::shared_ptr<LogicFormula>op) {
    return std::shared_ptr<UnaryTemporalFormula>(new UnaryTemporalFormula(op_future, op));
}

std::shared_ptr<UnaryTemporalFormula> chase::Next(std::shared_ptr<LogicFormula>op) {
    return std::shared_ptr<UnaryTemporalFormula>(new UnaryTemporalFormula(op_next, op));
}

std::shared_ptr<Expression> chase::Sum(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_plus, op1, op2));
}

std::shared_ptr<Expression> chase::Sub(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_minus, op1, op2));
}

std::shared_ptr<Expression> chase::Mult(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_multiply, op1, op2));
}

std::shared_ptr<Expression> chase::Div(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_divide, op1, op2));
}

std::shared_ptr<Expression> chase::Eq(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_eq, op1, op2));
}

std::shared_ptr<Expression> chase::NEq(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_neq, op1, op2));
}

std::shared_ptr<Expression> chase::LE(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_le, op1, op2));
}

std::shared_ptr<Expression> chase::LT(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_lt, op1, op2));
}

std::shared_ptr<Expression> chase::GE(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_ge, op1, op2));
}

std::shared_ptr<Expression> chase::GT(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2) {
    return std::shared_ptr<Expression>(new Expression(op_gt, op1, op2));
}

std::shared_ptr<Proposition> chase::Prop( std::shared_ptr<Variable> var )
{
    auto prop = std::shared_ptr<Proposition>(
        new Proposition(std::shared_ptr<Identifier>(new Identifier(var))));
    prop->setName(std::shared_ptr<Name>(new Name(var->getName()->getString())));
    return prop;
}

std::shared_ptr<Proposition> chase::Prop(std::shared_ptr<Expression> exp)
{
    auto prop = std::shared_ptr<Proposition>(new Proposition(exp));
    prop->setName(std::shared_ptr<Name>(new Name(exp->getString())));
    return prop;
}

std::shared_ptr<BooleanConstant> chase::True() {
    return std::shared_ptr<BooleanConstant>(new BooleanConstant(true));
}

std::shared_ptr<BooleanConstant> chase::False() {
    return std::shared_ptr<BooleanConstant>(new BooleanConstant(false));
}

std::shared_ptr<LargeBooleanFormula> chase::LargeAnd(std::vector<std::shared_ptr<LogicFormula>> &formulas) {
    auto ret = std::shared_ptr<LargeBooleanFormula>(new LargeBooleanFormula(op_and));
    for(auto f = formulas.begin(); f != formulas.end(); ++f)
        ret->operands.push_back(*f);
    return ret;
}

std::shared_ptr<LargeBooleanFormula> chase::LargeOr(std::vector<std::shared_ptr<LogicFormula>> &formulas) {
    auto ret = std::make_shared<LargeBooleanFormula>(op_or);
    for(auto f = formulas.begin(); f != formulas.end(); ++f)
        ret->operands.push_back(*f);
    return ret;
}

std::shared_ptr<Identifier> chase::Id( std::shared_ptr<DataDeclaration> declaration ) {
    return std::make_shared<Identifier>(declaration);
}

std::shared_ptr<IntegerValue> chase::IntVal(int n) {
    return std::make_shared<IntegerValue>(n);
}

std::shared_ptr<RealValue> chase::RealVal(double r) {
    return std::make_shared<RealValue>(r);
}

std::shared_ptr<BooleanValue> chase::BoolVal(bool b)
{
    return std::make_shared<BooleanValue>(b);
}

std::shared_ptr<BinaryTemporalFormula> chase::Until(std::shared_ptr<LogicFormula>op1, std::shared_ptr<LogicFormula>op2) {
    return std::make_shared<BinaryTemporalFormula>(op_until, op1, op2);
}



std::shared_ptr<Integer> chase::Int( int l, int r ) {
    return std::make_shared<Integer>(l, r);
}

std::shared_ptr<Variable>
chase::Var(std::shared_ptr<Type> type, std::shared_ptr<Name> name,
    causality_t causality) {
    return std::make_shared<Variable>(type, name, causality);
}

std::shared_ptr<Boolean> chase::Bool() {
    return std::shared_ptr<Boolean>();
}

std::shared_ptr<Name> chase::Nam(std::string n) {
    return std::make_shared<Name>(n);
}

std::shared_ptr<Name> chase::Nam(const char *n) {
    return Nam(std::string(n));
}