/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/UnaryTemporalFormula.hh"

using namespace chase;
using namespace std;

using sptr_logicform = std::shared_ptr<LogicFormula>;
using sptr_utempform = std::shared_ptr<UnaryTemporalFormula>;

UnaryTemporalFormula::UnaryTemporalFormula(
        TemporalOperator op,
        sptr_logicform formula
        ) :
        _op(op),
        _formula(formula)
{
    _node_type = unaryTemporalOperation_node;
    if(_formula->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula.get());
    else _formula->setParent(this);
}

TemporalOperator UnaryTemporalFormula::getOp() const {
    return _op;
}

void UnaryTemporalFormula::setOp(TemporalOperator op) {
    _op = op;
}

sptr_logicform UnaryTemporalFormula::getFormula() const {
    return _formula;
}

void UnaryTemporalFormula::setFormula(sptr_logicform formula) {
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

sptr_utempform UnaryTemporalFormula::clone() {
    return make_shared<UnaryTemporalFormula>(_op, _formula);
}

UnaryTemporalFormula::~UnaryTemporalFormula() = default;
