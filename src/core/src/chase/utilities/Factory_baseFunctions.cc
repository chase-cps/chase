/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/Factory.hh"

using namespace chase;

UnaryBooleanFormula * chase::Not(LogicFormula *op) {
    return new UnaryBooleanFormula(op_not, op);
}

BinaryBooleanFormula * chase::And(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_and, op1, op2);
}

BinaryBooleanFormula * chase::Or(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_or, op1, op2);
}

BinaryBooleanFormula * chase::Implies(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_implies, op1, op2);
}

BinaryBooleanFormula * chase::Iff(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_iff, op1, op2);
}

BinaryBooleanFormula * chase::Nand(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_nand, op1, op2);
}

BinaryBooleanFormula * chase::Xor(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_xor, op1, op2);
}

BinaryBooleanFormula * chase::Nor(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_nor, op1, op2);
}

BinaryBooleanFormula * chase::Xnor(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryBooleanFormula(op_xnor, op1, op2);
}

UnaryTemporalFormula * chase::Always(LogicFormula *op) {
    return new UnaryTemporalFormula(op_globally, op);
}

UnaryTemporalFormula * chase::Eventually(LogicFormula *op) {
    return new UnaryTemporalFormula(op_future, op);
}

UnaryTemporalFormula * chase::Next(LogicFormula *op) {
    return new UnaryTemporalFormula(op_next, op);
}

Expression * chase::Sum(Value *op1, Value *op2) {
    return new Expression(op_plus, op1, op2);
}

Expression * chase::Sub(Value *op1, Value *op2) {
    return new Expression(op_minus, op1, op2);
}

Expression * chase::Mult(Value *op1, Value *op2) {
    return new Expression(op_multiply, op1, op2);
}

Expression * chase::Div(Value *op1, Value *op2) {
    return new Expression(op_divide, op1, op2);
}

Expression * chase::Eq(Value *op1, Value *op2) {
    return new Expression(op_eq, op1, op2);
}

Expression * chase::NEq(Value *op1, Value *op2) {
    return new Expression(op_neq, op1, op2);
}

Expression * chase::LE(Value *op1, Value *op2) {
    return new Expression(op_le, op1, op2);
}

Expression * chase::LT(Value *op1, Value *op2) {
    return new Expression(op_lt, op1, op2);
}

Expression * chase::GE(Value *op1, Value *op2) {
    return new Expression(op_ge, op1, op2);
}

Expression * chase::GT(Value *op1, Value *op2) {
    return new Expression(op_gt, op1, op2);
}

Proposition * chase::Prop( Variable * var )
{
    auto prop = new Proposition(new Identifier(var));
    prop->setName(new Name(var->getName()->getString()));
    return prop;
}

Proposition * chase::Prop(Expression * exp)
{
    auto prop = new Proposition(exp);
    prop->setName(new Name(exp->getString()));
    return prop;
}

BooleanConstant * chase::True() {
    return new BooleanConstant(true);
}

BooleanConstant * chase::False() {
    return new BooleanConstant(false);
}

LargeBooleanFormula * chase::LargeAnd(std::vector<LogicFormula *> &formulas) {
    auto ret = new LargeBooleanFormula(op_and);
    for(auto f = formulas.begin(); f != formulas.end(); ++f)
        ret->operands.push_back(*f);
    return ret;
}

LargeBooleanFormula * chase::LargeOr(std::vector<LogicFormula *> &formulas) {
    auto ret = new LargeBooleanFormula(op_or);
    for(auto f = formulas.begin(); f != formulas.end(); ++f)
        ret->operands.push_back(*f);
    return ret;
}

Identifier * chase::Id( DataDeclaration *declaration ) {
    return new Identifier(declaration);
}

IntegerValue * chase::IntVal(int n) {
    return new IntegerValue(n);
}

RealValue * chase::RealVal(double r) {
    return new RealValue(r);
}

BooleanValue * chase::BoolVal(bool b)
{
    return new BooleanValue(b);
}

BinaryTemporalFormula *chase::Until(LogicFormula *op1, LogicFormula *op2) {
    return new BinaryTemporalFormula(op_until, op1, op2);
}

Constraint * chase::arithmeticConstraint(Operator op,
                                  Value * op1, Value * op2)
{
    auto c = new Constraint();
    auto e = new Expression(op, op1, op2);
    c->setExpression(e);
    return c;
}