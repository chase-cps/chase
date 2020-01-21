/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/ChaseObject.hh"
#include "representation/Boolean.hh"

using namespace chase;

Boolean::Boolean() :
    SimpleType()
{
    _node_type = boolean_node;
}

Boolean::~Boolean()
{
}


int Boolean::accept_visitor( BaseVisitor &v )
{
    return v.visitBoolean(*this);
}

std::string Boolean::getString()
{
    return "boolean";
}

Boolean *Boolean::clone()
{
    return new Boolean();
}


