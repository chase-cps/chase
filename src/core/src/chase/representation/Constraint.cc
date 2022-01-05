/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Constraint.hh"

using namespace chase;

Constraint::Constraint() :
    Specification()
{
    _node_type = constraint_node;
}

Constraint::~Constraint() {
    delete(_expression);
}

int Constraint::accept_visitor(BaseVisitor &v) {
    return v.visitConstraint(*this);
}

std::string Constraint::getString()
{
    std::string ret(_expression->getString());
    return ret;
}

Constraint *Constraint::clone() {
    auto ret = new Constraint();
    ret->setExpression(_expression);
    return ret;
}

Expression *Constraint::getExpression() const {
    return _expression;
}

void Constraint::setExpression(Expression *expression) {
    _expression = expression;
    _expression->setParent(this);
}
