/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/IntegerValue.hh"
#include "representation/Integer.hh"
#include <stdint.h>


using namespace chase;

IntegerValue::IntegerValue( int64_t value ) :
    NumericValue(),
    _value(value)
{
    _node_type = integerValue_node;
    setType(new Integer());
}

IntegerValue::~IntegerValue()
{
    delete _type;
}

IntegerValue::IntegerValue( const IntegerValue & o ) :
    NumericValue(),
    _value(o._value)
{
    _node_type = integer_node;
}

IntegerValue & IntegerValue::operator=( const IntegerValue &o )
{
    _value = o._value;
    return *this;
}

int64_t IntegerValue::getValue()
{
    return _value;
}

void IntegerValue::setValue( const int64_t v )
{
    _value = v;
}

int IntegerValue::accept_visitor( BaseVisitor &v )
{
    return v.visitIntegerValue( *this );
}

std::string IntegerValue::getString()
{
    return std::to_string(_value);
}

IntegerValue * IntegerValue::clone() {
    auto ret = new IntegerValue(_value);
    ret->setType(_type->clone());
    return ret;
}

