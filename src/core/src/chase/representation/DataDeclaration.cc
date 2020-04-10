/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/DataDeclaration.hh"

using namespace chase;

DataDeclaration::DataDeclaration( Type * t, Name * n ) :
    Declaration(n),
    _type(t)
{
    _node_type = dataDeclaration_node;

    _type->setParent(this);
}

Type * DataDeclaration::getType()
{
    return _type;
}

void DataDeclaration::setType( Type * t )
{
    _type = t;
    _type->setParent(this);
}

