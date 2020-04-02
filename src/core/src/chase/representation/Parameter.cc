/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Parameter.hh"

using namespace chase;
using namespace std;

using sptr_type = std::shared_ptr<Type>;
using sptr_name = std::shared_ptr<Name>;
using sptr_param = std::shared_ptr<Parameter>;

Parameter::Parameter(sptr_type type, 
    sptr_name name) :
    DataDeclaration(type, name)
{
    _node_type = parameter_node;
}

Parameter::~Parameter()
{
}

int Parameter::accept_visitor(chase::BaseVisitor &v)
{
    return v.visitParameter(*this);
}

std::string Parameter::getString() {
    return std::__cxx11::string();
}

sptr_param Parameter::clone() {
    return nullptr;
}


