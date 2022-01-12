/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Declaration.hh"

using namespace chase;

Declaration::Declaration() :
    _name(nullptr)
{
    _node_type = declaration_node;
}

Declaration::~Declaration()
{
}

Declaration::Declaration( Name * n ) :
    _name(n)
{
    _node_type = declaration_node;
    _name->setParent(this);
}

Declaration::Declaration( std::string n ) :
    _name( new Name(n) )
{
    _node_type = declaration_node;
    _name->setParent(this);
}

Name * Declaration::getName()
{
    return _name;
}

void Declaration::setName( Name * n )
{
    _name = n;
    _name->setParent(this);
}

