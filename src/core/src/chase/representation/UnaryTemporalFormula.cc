/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/UnaryTemporalFormula.hh"

using namespace chase;

UnaryTemporalFormula::UnaryTemporalFormula(
        TemporalOperator op,
        LogicFormula * formula
        ) :
        _op(op),
        _formula(formula)
{
    _node_type = unaryTemporalOperation_node;
    if(_formula->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula);
    else _formula->setParent(this);
}

TemporalOperator UnaryTemporalFormula::getOp() const {
    return _op;
}

void UnaryTemporalFormula::setOp(TemporalOperator op) {
    _op = op;
}

LogicFormula *UnaryTemporalFormula::getFormula() const {
    return _formula;
}

void UnaryTemporalFormula::setFormula(LogicFormula *formula) {
    _formula = formula;
}

int UnaryTemporalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitUnaryTemporalOperation(*this);
}

std::string UnaryTemporalFormula::getString() {
    std::string ret(to_string(_op));

    ret += "(";
    ret += _formula->getString();
    ret += ")";

    return ret;
}

UnaryTemporalFormula *UnaryTemporalFormula::clone() {
    return new UnaryTemporalFormula(_op, _formula->clone());
}

UnaryTemporalFormula::~UnaryTemporalFormula() = default;