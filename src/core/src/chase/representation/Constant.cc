/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Constant.hh"
#include "representation/Value.hh"

using namespace chase;

Constant::Constant( Type * type, Name * name, Value * value ) :
    DataDeclaration( type, name ),
    _value(value)
{
    _node_type = constant_node;

    _name->setParent(this);
    _type->setParent(this);
}

Constant::~Constant()
{
    if(_name != nullptr) delete _name;
    if(_type != nullptr) delete _type;
}

Constant::Constant( const Constant &o ) :
    DataDeclaration( o._type, o._name )
{
    _node_type = constant_node;
    _name->setParent(this);
    _type->setParent(this);
}

Constant & Constant::operator=(const Constant & )
{
    return *this;
}

std::string Constant::getString()
{
    std::string ret("constant: ");
    if( _name != nullptr && _type != nullptr )
    {
        ret += _name->getString();
        ret += " (";
        ret += _type->getString();
        ret += ") = ";
        ret += _value->getString();

    }
    else{
        ret = std::string("NULL CONSTANT");
    }
    return ret;
}

int Constant::accept_visitor( BaseVisitor &v )
{
    return v.visitConstant(*this);
}

void Constant::setValue(Value *value) {
    _value = value;
}

Value *Constant::getValue() {
    return _value;
}

Constant * Constant::clone() {
    return new Constant(
            _type->clone(), _name->clone(), _value->clone());
}
