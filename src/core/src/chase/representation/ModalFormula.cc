/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/ModalFormula.hh"

using namespace chase;
using namespace std;

using sptr_modform = std::shared_ptr<ModalFormula>;
using sptr_logicform = std::shared_ptr<LogicFormula>;

ModalFormula::ModalFormula(ModalOperator op, sptr_logicform formula) :
    _operator(op),
    _formula(formula)
{
    _node_type = modalFormula_node;
    if( formula->getParent() != nullptr )
        messageWarning("Formula with parent already set:", formula.get());
    formula->setParent(this);
}

ModalFormula::~ModalFormula() {
}

ModalOperator ModalFormula::getOperator()
{
    return _operator;
}

void ModalFormula::setOperator(ModalOperator op) {
    _operator = op;
}

sptr_logicform ModalFormula::getFormula()
{
    return _formula;
}

void ModalFormula::setFormula(sptr_logicform formula)
{
    _formula = formula;
}

int ModalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitModalFormula(*this);
}

std::string ModalFormula::getString() {
    std::string ret(to_string(_operator));
    ret += "(";
    ret += _formula->getString();
    ret += ")";
    return ret;
}

sptr_modform ModalFormula::clone()
{
    return make_shared<ModalFormula>(
        _operator, _formula);
}


