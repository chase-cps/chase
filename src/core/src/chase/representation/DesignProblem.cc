/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/DesignProblem.hh"
#include "chase/representation/Library.hh"
#include "chase/representation/Contract.hh"
#include "chase/representation/System.hh"

using namespace chase;

DesignProblem::DesignProblem() :
    ChaseObject()
{
    _node_type = design_problem_node;
}

DesignProblem::~DesignProblem() {
}

int DesignProblem::accept_visitor(BaseVisitor &v) {
    return v.visitDesignProblem(*this);
}

std::string DesignProblem::getString() {
    std::string ret("");
    return ret;
}

DesignProblem *DesignProblem::clone() {
    auto ret = new DesignProblem();
    for(auto it : libraries)
        ret->libraries.insert(it->clone());
    return ret;
}

System *DesignProblem::getSystem() const {
    return _system;
}

void DesignProblem::setSystem(System *system) {
    _system = system;
    system->setParent(this);
}
