/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        6/20/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/representation/Enumeration.hh"

#include "utilities/Factory.hh"


using namespace chase;

Enumeration::Enumeration(std::string name) :
    CustomType(name, new Integer())
{
    _node_type = enumeration_node;
}

Enumeration::Enumeration(Name *name) :
    CustomType(name, new Integer())
{
}

Enumeration::~Enumeration() = default;

void Enumeration::addItem(std::string item) {
    if(getPositionByName(item) == -1)
    {
        int pos = _values.size();
        auto v = new Constant(
                new Integer(), new Name(item), IntVal(pos));
        _values.push_back(v);
    }
    else
        messageError(
                "Duplicated item " + item +
                " in enumeration " + _name->getString() + ".");
}

Constant * Enumeration::getItemInPosition(size_t position) {
    if(position >= _values.size())
        return nullptr;
    else
        return _values[position];
}

int Enumeration::getPositionByName(std::string name)
{
    for(size_t i = 0; i < _values.size(); ++i)
    {
        if(name == _values[i]->getName()->getString())
            return i;
    }
    return -1;
}

std::string Enumeration::getString() {
    std::string ret("ENUM: " + _name->getString());
    for(size_t i = 0; i < _values.size(); ++i)
    {
        ret += "\n\t" + std::to_string(i) + " " +
                _values[i]->getName()->getString();
    }
    ret +="\n";
    return ret;
}

int Enumeration::accept_visitor(BaseVisitor &v) {
    return v.visitEnumeration(*this);
}

Enumeration *Enumeration::clone()
{
    return new Enumeration(_name->clone());
}


