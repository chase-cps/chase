/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Variable.hh"

using namespace chase;
using namespace std;

using sptr_type = std::shared_ptr<Type>;
using sptr_name = std::shared_ptr<Name>;
using sptr_var = std::shared_ptr<Variable>;

Variable::Variable(sptr_type type, sptr_name name, 
    causality_t causality ) :
    DataDeclaration(type, name),
    _causality(causality)
{
    _node_type = variable_node;
    _name->setParent(this);
    _type->setParent(this);
}

Variable::~Variable()
{
}

std::string Variable::getString()
{
    std::string ret("variable: ");
    if(_causality == input) ret += " (input):\t";
    if(_causality == output) ret += " (output):\t";
    if( _name != nullptr && _type != nullptr )
    {
        ret += _name->getString();
        ret += " (";
        ret += _type->getString() + ")";
    }
    else{
        ret = std::string("NULL VARIABLE");
    }
    return ret;

}

int Variable::accept_visitor( BaseVisitor &v )
{
    return v.visitVariable(*this);
}

causality_t Variable::getCausality() const {
    return _causality;
}

void Variable::setCausality(causality_t causality)
{
    _causality = causality;
}

sptr_var Variable::clone()
{
    return make_shared<Variable>(_type, _name, _causality);
}
