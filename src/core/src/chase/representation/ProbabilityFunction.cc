/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/11/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/ProbabilityFunction.hh"
#include "chase/representation/Real.hh"

using namespace chase;

ProbabilityFunction::ProbabilityFunction(Specification * specification) :
    Value(),
    _specification(specification)
{
    _node_type  = probabilityFunction_node;
    _type = new Real(0.0, 1.0);
}

ProbabilityFunction::~ProbabilityFunction()
{
    delete _specification;
}

int ProbabilityFunction::accept_visitor(BaseVisitor &v) {
    return v.visitProbabilityFunction(*this);
}

std::string ProbabilityFunction::getString() {
    std::string ret("P(");
    ret += _specification->getString();
    ret += ")";
    return ret;
}

ProbabilityFunction *ProbabilityFunction::clone()
{
    auto ret = new ProbabilityFunction(_specification->clone());
    return ret;
}

Specification *ProbabilityFunction::getSpecification() const
{
    return _specification;
}

void ProbabilityFunction::setSpecification(Specification *specification)
{
    _specification = specification;
}
