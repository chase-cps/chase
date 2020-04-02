/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/BooleanValue.hh"
#include "representation/Boolean.hh"

using namespace chase;
using namespace std; 

using sptr_boolval = std::shared_ptr<BooleanValue>;
using sptr_type = std::shared_ptr<Type>;

BooleanValue::BooleanValue( bool value ) :
    _value(value)
{
    _node_type = booleanValue_node;
    setType(make_shared<Boolean>());
}

BooleanValue::~BooleanValue()
{
}

BooleanValue::BooleanValue( const BooleanValue & o ) :
    NumericValue(),
    _value(o._value)
{
    _node_type = booleanValue_node;
}

BooleanValue & BooleanValue::operator=( const BooleanValue &o )
{
    _value = o._value;
    return *this;
}

bool BooleanValue::getValue()
{
    return _value;
}

void BooleanValue::setValue( const bool v )
{
    _value = v;
}

int BooleanValue::accept_visitor( BaseVisitor &v )
{
    return v.visitBooleanValue( *this );
}

std::string BooleanValue::getString()
{
    if( _value ) return std::string("true");
    else return std::string("false");
}

sptr_boolval BooleanValue::clone() {
    return make_shared<BooleanValue>(_value);
}
