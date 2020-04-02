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
using namespace std;

using sptr_name = std::shared_ptr<Name>;
using sptr_value = std::shared_ptr<Value>;
using sptr_type = std::shared_ptr<Type>;
using sptr_const = std::shared_ptr<Constant>;

Constant::Constant( sptr_type type, sptr_name name, 
    sptr_value value ) :
    DataDeclaration( type, name ),
    _value(value)
{
    _node_type = constant_node;

    _name->setParent(this);
    _type->setParent(this);
}

Constant::~Constant()
{
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

void Constant::setValue(sptr_value value) {
    _value = value;
}

sptr_value Constant::getValue() {
    return _value;
}

sptr_const Constant::clone() {
    return make_shared<Constant>(_type, _name, _value);
}
