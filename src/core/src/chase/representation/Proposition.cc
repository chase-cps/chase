/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "chase/representation/Proposition.hh"

using namespace chase;
using namespace std;

using sptr_name = std::shared_ptr<Name>;
using sptr_type = std::shared_ptr<Type>;
using sptr_value = std::shared_ptr<Value>;
using sptr_prop = std::shared_ptr<Proposition>;

Proposition::Proposition() :
    _type(new Boolean()),
    _value(nullptr),
    _name(nullptr)
{
    _node_type = proposition_node;
    _type->setParent(this);
}

Proposition::~Proposition()
{
}

Proposition::Proposition(sptr_value v) :
    _type(new Boolean()),
    _value(v),
    _name(nullptr)
{
    _node_type = proposition_node;

    switch( _value->IsA()) {
        case (identifier_node) :
            Identifier *id;
            id = dynamic_cast< Identifier * >(v.get());
            if (id != nullptr) {
                _name = std::shared_ptr<Name>(new Name(id->getDeclaration()->getName()->getString()));
                _name->setParent(this);
            }
            else messageError("Something went wrong in dynamic cast.");
            if (id->getType()->IsA() != boolean_node)
                messageError(
                        "Not a boolean typed variable or constant when creating a proposition",
                        id);
            break;
        case (expression_node) :
            Expression *exp;
            exp = dynamic_cast< Expression * >(v.get());
            if (exp != nullptr)
                if (exp->getType()->IsA() != boolean_node)
                    messageError(
                            "Not a boolean typed expression when creating a proposition",
                            exp);
            break;
        default:
            messageError("Not boolean object when creating a proposition", v.get());
            break;
    }

}

sptr_type Proposition::getType() {
    return _type;
}

std::shared_ptr<Value> Proposition::getValue() {
    return _value;
}

void Proposition::setValue(std::shared_ptr<Value> v) {
    _value = v;
    if(_value->getParent() == nullptr ) 
        _value->setParent(this);
}

std::shared_ptr<Name> Proposition::getName() {
    return _name;
}

void Proposition::setName(std::shared_ptr<Name> n) {
    _name = n;
    if(_name->getParent() == nullptr ) 
        _name->setParent(this);
}

int Proposition::accept_visitor(chase::BaseVisitor &v) {
    return v.visitProposition(*this);
}

std::string Proposition::getString() {
    std::string ret;
    if( _value->IsA() == identifier_node )
        ret = _name->getString();
    else if( _value->IsA() == expression_node )
        ret = _value->getString();
    return ret;
}

sptr_prop Proposition::clone()
{
    auto ret = make_shared<Proposition>();
    if( _name != nullptr ) ret->setName(_name);
    if( _value != nullptr ) ret->setValue(_value);
    return ret;
}
