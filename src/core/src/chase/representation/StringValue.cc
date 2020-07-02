/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        7/2/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/representation/StringValue.hh"
#include "chase/representation/String.hh"

using namespace chase;

StringValue::StringValue( std::string value ) :
    _value(value)
{
    _node_type = stringValue_node;
    _type = new String();
}

std::string StringValue::getValue() {
    return _value;
}

void StringValue::setValue(std::string v) {
    _value = v;
}

StringValue::StringValue(const StringValue &o) :
    _value(o._value)
{
}

StringValue &StringValue::operator=(const StringValue &o) {
    _value = o._value;
    return *this;
}

int StringValue::accept_visitor(BaseVisitor &v) {
    return v.visitStringValue(*this);
}

std::string StringValue::getString() {
    return _value;
}

StringValue *StringValue::clone() {
    return new StringValue(_value);
}

StringValue::~StringValue() = default;
