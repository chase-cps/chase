/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/BooleanConstant.hh"

using namespace chase;

BooleanConstant::BooleanConstant( bool value ) :
    _value(value)
{
    _node_type = booleanConstant_node;
}

BooleanConstant::~BooleanConstant() = default;

int BooleanConstant::accept_visitor(chase::BaseVisitor &v) {
    return v.visitBooleanConstant(*this);
}

std::string BooleanConstant::getString() {
    if(_value) return std::string("TRUE");
    else return std::string("FALSE");
}

bool BooleanConstant::getValue() {
    return _value;
}

BooleanConstant *BooleanConstant::clone()
{
    return new BooleanConstant(_value);
}
