/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include <utility>

#include "chase/representation/System.hh"

using namespace chase;

System::System( std::string name ) :
    _name(name)
{
}

System::~System() = default;

void System::addDeclaration(Declaration *declaration)
{
    _declarations.insert(declaration);
    declaration->setParent(this);
}

void System::addContract(Contract *contract)
{
    _contracts.insert(contract);
    contract->setParent(this);
}

std::set<Declaration *> &System::getDeclarationsSet() {
    return _declarations;
}

std::set<Contract *> &System::getContractsSet() {
    return _contracts;
}




int System::accept_visitor(chase::BaseVisitor &v) {
    return v.visitSystem(*this);
}

std::string System::getString() {
    std::string ret("SYSTEM:\t" + _name + "\n");

    ret += "DECLARATIONS:\n";
    for(auto it = _declarations.begin(); it != _declarations.end(); ++it)
    {
        ret += (*it)->getString();
        ret += "\n";
    }
    ret += "\n";

    ret += "CONTRACTS:\n";
    for(auto it = _contracts.begin(); it != _contracts.end(); ++it)
    {
        ret += (*it)->getString();
        ret += "\n";
    }

    return ret;
}

System *System::clone() {
    auto ret = new System(_name);

    for(auto it = _declarations.begin(); it != _declarations.end(); ++it)
        ret->addDeclaration((*it)->clone());

    for(auto it = _contracts.begin(); it != _contracts.end(); ++it)
        ret->addContract((*it)->clone());

    return ret;
}