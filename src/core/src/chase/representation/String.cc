/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        7/2/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/representation/String.hh"

using namespace chase;

String::String()
{
    _node_type = string_node;
}

String::~String() = default;

int String::accept_visitor(BaseVisitor &v) {
    return v.visitString(*this);
}

String *String::clone() {
    return new String();
}

std::string String::getString() {
    return "string";
}