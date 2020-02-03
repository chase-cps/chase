/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Name.hh"

using namespace chase;

Name::Name( std::string s ) :
        ChaseObject(),
        _name(s)
{
    _node_type = name_node;
}

Name::~Name()
{
    // Do nothing.
}

Name::Name( const Name & o ) :
        ChaseObject(),
        _name( o._name )
{
    _node_type = name_node;
}

Name & Name::operator=( const Name & o )
{
    _name = o._name;
    return *this;
}

std::string Name::getString()
{
    return _name;
}

void Name::changeName(std::string name)
{
    _name = name;
}

int Name::accept_visitor( BaseVisitor & v )
{
    return v.visitName(*this);
}

Name * Name::clone() {
    return new Name(_name);
}
