/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/BinaryTemporalFormula.hh"

using namespace chase;
using namespace std;

using sptr_bintempform = std::shared_ptr<BinaryTemporalFormula>;
using sptr_logicform = std::shared_ptr<LogicFormula>;

BinaryTemporalFormula::BinaryTemporalFormula(
        TemporalOperator op,
        sptr_logicform op1,
        sptr_logicform op2
) :
    _op(op),
    _formula1(op1),
    _formula2(op2)
{
    _node_type = binaryTemporalOperation_node;

    if(_formula1->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula1.get());
    else _formula1->setParent(this);

    if(_formula2->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula2.get());
    else _formula2->setParent(this);
}

TemporalOperator BinaryTemporalFormula::getOp() const {
    return _op;
}

void BinaryTemporalFormula::setOp(TemporalOperator op) {
    _op = op;
}

sptr_logicform BinaryTemporalFormula::getFormula1() const {
    return _formula1;
}

void BinaryTemporalFormula::setFormula1(sptr_logicform formula1) {
    _formula1 = formula1;
}

sptr_logicform BinaryTemporalFormula::getFormula2() const {
    return _formula2;
}

void BinaryTemporalFormula::setFormula2(sptr_logicform formula2) {
    _formula2 = formula2;
}

int BinaryTemporalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitBinaryTemporalOperation(*this);
}

std::string BinaryTemporalFormula::getString() {
    std::string ret(_formula1->getString());
    ret += to_string(_op);
    ret += _formula2->getString();
    return ret;
}

sptr_bintempform BinaryTemporalFormula::clone()
{
    return make_shared<BinaryTemporalFormula>(
            _op, _formula1, _formula2);
}

BinaryTemporalFormula::~BinaryTemporalFormula() = default;
