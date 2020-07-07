/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Parameter.hh"

using namespace chase;

Parameter::Parameter(Type * type, Name * name) :
    DataDeclaration(type, name)
{
    _node_type = parameter_node;
}

Parameter::~Parameter()
{
    delete _name;
    delete _type;
}

int Parameter::accept_visitor(chase::BaseVisitor &v)
{
    return v.visitParameter(*this);
}

std::string Parameter::getString() {
    return std::string("Parameter: ") + _name->getString();
}

Parameter *Parameter::clone() {
    return nullptr;
}


