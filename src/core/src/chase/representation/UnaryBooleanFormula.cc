/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/UnaryBooleanFormula.hh"


using namespace chase;
using namespace std;

using sptr_logicform = std::shared_ptr<LogicFormula>;
using sptr_ubinform = std::shared_ptr<UnaryBooleanFormula>;

chase::UnaryBooleanFormula::UnaryBooleanFormula(
        BooleanOperator op, sptr_logicform op1) :
        _op(op),
        _op1(op1)
{
    _node_type = unaryBooleanOperation_node;
}

BooleanOperator UnaryBooleanFormula::getOp() const {
    return _op;
}

void UnaryBooleanFormula::setOp(BooleanOperator op) {
    _op = op;
}

sptr_logicform UnaryBooleanFormula::getOp1() const {
    return _op1;
}

void UnaryBooleanFormula::setOp1(sptr_logicform op1) {
    _op1 = op1;
}

int UnaryBooleanFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitUnaryBooleanOperation(*this);
}

std::string UnaryBooleanFormula::getString() {
    std::string ret = to_string(_op);
    ret += "(";
    ret += _op1->getString();
    ret += ")";
    return ret;
}

sptr_ubinform UnaryBooleanFormula::clone()
{
    return make_shared<UnaryBooleanFormula>(_op, _op1);
}

UnaryBooleanFormula::~UnaryBooleanFormula() = default;
