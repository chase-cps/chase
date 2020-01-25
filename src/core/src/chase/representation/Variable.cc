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

Variable::Variable( Type * type, Name * name, causality_t causality ) :
    DataDeclaration(type, name),
    _causality(causality)
{
    _node_type = variable_node;
    _name->setParent(this);
    _type->setParent(this);
}

Variable::~Variable()
{
    delete _name;
    delete _type;
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

Variable *Variable::clone()
{
    return new Variable(_type->clone(), _name->clone(), _causality);
}
