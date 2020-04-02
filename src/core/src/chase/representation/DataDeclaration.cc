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
using namespace std;

using sptr_name = std::shared_ptr<Name>;
using sptr_type = std::shared_ptr<Type>;
using sptr_datadecl = std::shared_ptr<DataDeclaration>;
    

DataDeclaration::DataDeclaration( sptr_type t, 
    sptr_name n ) :
    Declaration(n),
    _type(t)
{
    _node_type = dataDeclaration_node;

    _type->setParent(this);
}

sptr_type DataDeclaration::getType()
{
    return _type;
}

void DataDeclaration::setType( sptr_type t )
{
    _type = t;
    _type->setParent(this);
}

