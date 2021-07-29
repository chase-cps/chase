/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/BinaryTemporalFormula.hh"

using namespace chase;

BinaryTemporalFormula::BinaryTemporalFormula(
        TemporalOperator op,
        LogicFormula * op1,
        LogicFormula * op2,
        Interval * interval
) :
    _op(op),
    _formula1(op1),
    _formula2(op2),
    _interval(interval)
{
    _node_type = binaryTemporalOperation_node;

    if(_formula1->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula1);
    else _formula1->setParent(this);

    if(_formula2->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula2);
    else _formula2->setParent(this);

    if(_interval != nullptr) {
        if(_interval->getParent() != nullptr){
            messageWarning("Interval with already set parent", _interval);
        } else _interval->setParent(this);
    }
}

TemporalOperator BinaryTemporalFormula::getOp() const {
    return _op;
}

void BinaryTemporalFormula::setOp(TemporalOperator op) {
    _op = op;
}

LogicFormula *BinaryTemporalFormula::getFormula1() const {
    return _formula1;
}

void BinaryTemporalFormula::setFormula1(LogicFormula *formula1) {
    _formula1 = formula1;
}

LogicFormula *BinaryTemporalFormula::getFormula2() const {
    return _formula2;
}

void BinaryTemporalFormula::setFormula2(LogicFormula *formula2) {
    _formula2 = formula2;
}

int BinaryTemporalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitBinaryTemporalOperation(*this);
}

std::string BinaryTemporalFormula::getString() {
    std::string ret(_formula1->getString());
    ret += to_string(_op);
    if(_interval != nullptr)
        ret += _interval->getString();
    ret += _formula2->getString();
    return ret;
}

BinaryTemporalFormula *BinaryTemporalFormula::clone()
{
    return new BinaryTemporalFormula(
            _op, _formula1->clone(), _formula2->clone());
}

Interval *BinaryTemporalFormula::getInterval() const {
    return _interval;
}

void BinaryTemporalFormula::setInterval(Interval *interval) {
    _interval = interval;
}

BinaryTemporalFormula::~BinaryTemporalFormula() = default;
