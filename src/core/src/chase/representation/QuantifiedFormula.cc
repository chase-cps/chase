/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/22/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/QuantifiedFormula.hh"

using namespace chase;

QuantifiedFormula::QuantifiedFormula(
        logic_quantifier quantifier,
        Variable *variable,
        LogicFormula *formula) :
    _quantifier(quantifier),
    _variable(variable),
    _formula(formula)
{
    _node_type = quantified_formula_node;
    _variable->setParent(this);
    _formula->setParent(this);
}

QuantifiedFormula::~QuantifiedFormula() {
    delete(_variable);
    delete(_formula);
}

int QuantifiedFormula::accept_visitor(BaseVisitor &v) {
    return v.visitQuantifiedFormula(*this);
}

std::string QuantifiedFormula::getString() {
    std::string ret("");
    return ret;
}

QuantifiedFormula *QuantifiedFormula::clone() {
    auto ret = new QuantifiedFormula();
    return ret;
}

logic_quantifier QuantifiedFormula::getQuantifier() const {
    return _quantifier;
}

void QuantifiedFormula::setQuantifier(logic_quantifier quantifier) {
    _quantifier = quantifier;
}

Variable *QuantifiedFormula::getVariable() const {
    return _variable;
}

void QuantifiedFormula::setVariable(Variable *variable) {
    _variable = variable;
    _variable->setParent(this);
}

LogicFormula *QuantifiedFormula::getFormula() const {
    return _formula;
}

void QuantifiedFormula::setFormula(LogicFormula *formula) {
    _formula = formula;
    _formula->setParent(this);
}


