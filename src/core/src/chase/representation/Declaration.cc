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
using namespace std;

using sptr_name = std::shared_ptr<Name>;
using sptr_decl = std::shared_ptr<Declaration>;

Declaration::Declaration() :
    _name(nullptr)
{
    _node_type = declaration_node;
}

Declaration::~Declaration()
{
}

Declaration::Declaration( sptr_name n ) :
    _name(n)
{
    _node_type = declaration_node;
}

Declaration::Declaration( std::string n ) :
    _name( new Name(n) )
{
    _node_type = declaration_node;
    _name->setParent(this);
}

sptr_name Declaration::getName()
{
    return _name;
}

void Declaration::setName( sptr_name n )
{
    _name = n;
    _name->setParent(this);
}

