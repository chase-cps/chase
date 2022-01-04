/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/17/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Function.hh"

using namespace chase;

Function::Function(Type * type, Name * name, unsigned int arity) :
    DataDeclaration(type, name),
    _arity(arity)
{
}

Function::~Function() = default;

int Function::accept_visitor(BaseVisitor &v) {
    return v.visitFunction(*this);
}

std::string Function::getString() {
    std::string ret("");
    return ret;
}

Function *Function::clone() {
    auto ret = new Function(_type->clone(), _name->clone(), _arity);
    for(size_t it = 0; it < parameters.size(); ++it)
        ret->parameters[it] = parameters[it];
    for(size_t it = 0; it < _domain.size(); ++it)
        ret->setDomainOfParameter(it, _domain[it]->clone());
    return ret;
}

unsigned int Function::getArity() const {
    return _arity;
}

void Function::setArity(unsigned int arity) {
    _arity = arity;
}

Type *Function::getDomainOfParameter(unsigned int position) {
    if(position <= _arity)
        return _domain[position - 1];
    else{
        messageWarning(std::string("Position requested: ")
            + std::to_string(position) +
            "\tArity: " + std::to_string(_arity));
        return nullptr;
    }
}

void Function::setDomainOfParameter(unsigned int position, Type *type) {
    if(position > _arity)
        messageWarning(std::string("Position requested: ")
                       + std::to_string(position) +
                       "\tArity: " + std::to_string(_arity));
    else
        _domain[position] = type;
}
