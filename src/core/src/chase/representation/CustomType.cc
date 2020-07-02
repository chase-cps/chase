/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        6/20/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/representation/CustomType.hh"

using namespace chase;

CustomType::CustomType(std::string name, Type *type) :
    CustomType(new Name(name), type)
{
}

CustomType::CustomType(Name *name, Type *type) :
    Type(),
    _name(name),
    _type(type)
{
    _node_type = customType_node;
}

Type *CustomType::getType() const {
    return _type;
}

void CustomType::setType(Type *type) {
    _type = type;
}

Name *CustomType::getName() const {
    return _name;
}

void CustomType::setName(Name *name) {
    _name = name;
}

std::string CustomType::getString() {
    std::string ret("CUSTOM TYPE: " + _name->getString());
    return ret;
}

int CustomType::accept_visitor(BaseVisitor &v) {
    return v.visitCustomType(*this);
}

Type *CustomType::clone() {
    return new CustomType(_name->clone(), _type->clone());
}

CustomType::~CustomType() = default;
