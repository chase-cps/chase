/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/BinaryBooleanFormula.hh"


using namespace chase;

chase::BinaryBooleanFormula::BinaryBooleanFormula(
        BooleanOperator op, LogicFormula *op1, LogicFormula *op2) :
        _op(op),
        _op1(op1),
        _op2(op2)
{
    _node_type = binaryBooleanOperation_node;
}

BooleanOperator BinaryBooleanFormula::getOp() const {
    return _op;
}

void BinaryBooleanFormula::setOp(BooleanOperator op) {
    _op = op;
}

LogicFormula *BinaryBooleanFormula::getOp1() const {
    return _op1;
}

void BinaryBooleanFormula::setOp1(LogicFormula *op1) {
    _op1 = op1;
}

LogicFormula *BinaryBooleanFormula::getOp2() const {
    return _op2;
}

void BinaryBooleanFormula::setOp2(LogicFormula *op2) {
    _op2 = op2;
}

int BinaryBooleanFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitBinaryBooleanOperation(*this);
}

std::string BinaryBooleanFormula::getString() {
    std::string ret = "(";
    ret += _op1->getString();
    ret += to_string(_op);
    ret += _op2->getString();
    ret += ")";
    return ret;
}

BinaryBooleanFormula *BinaryBooleanFormula::clone()
{
    return new BinaryBooleanFormula(_op, _op1->clone(), _op2->clone());
}

BinaryBooleanFormula::~BinaryBooleanFormula() = default;
