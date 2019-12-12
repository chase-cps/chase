/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Type.hh"

using namespace chase;

Type::Type() :
        ChaseObject(),
        _typeVariant(NATIVE_TYPE)
{
    _node_type = type_node;
}

Type::~Type() = default;

Type::TypeVariant Type::getTypeVariant() const
{
    return _typeVariant;
}

void Type::setTypeVariant( const Type::TypeVariant tv )
{
    _typeVariant = tv;
}



std::string Type::getTypeVariantString()
{
    switch(_typeVariant)
    {
        case Type::TypeVariant::NATIVE_TYPE : return std::string("NATIVE_TYPE");
                           break;
        default: break;
    }

    messageError("Unexpected type variant.");
    return std::string("");
}

