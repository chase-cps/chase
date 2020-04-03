/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include <utility>

#include "chase/representation/System.hh"
#include "chase/utilities/ClonedDeclarationVisitor.hh"

using namespace chase;
using namespace std;

using sptr_decl = std::shared_ptr<Declaration>;
using sptr_contract = std::shared_ptr<Contract>;
using sptr_comp = std::shared_ptr<Component>;
using sptr_name = std::shared_ptr<Name>;
using sptr_sys = std::shared_ptr<System>;

System::System( std::string name ) :
    _name(make_shared<Name>(name))
{
    _name->setParent(this);
}

System::~System() = default;

void System::addDeclaration(sptr_decl declaration)
{
    _declarations.insert(declaration);
    declaration->setParent(this);
}

void System::addContract(sptr_contract contract)
{
    _contracts.insert(contract);
    contract->setParent(this);
}

std::set<sptr_decl> &System::getDeclarationsSet() {
    return _declarations;
}

std::set<sptr_contract> &System::getContractsSet() {
    return _contracts;
}




int System::accept_visitor(chase::BaseVisitor &v) {
    return v.visitSystem(*this);
}

std::string System::getString() {
    std::string ret("SYSTEM:\t" + _name->getString() + "\n");

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

sptr_sys System::clone()
{
    auto ret = make_shared<System>(_name->getString());

    std::map< sptr_decl, sptr_decl > declaration_map;

    for(auto it = _declarations.begin(); it != _declarations.end(); ++it)
    {
        Declaration * current = (*it).get();
        auto dec = current->clone();
        std::pair< Declaration *, Declaration * > p(current, 
            dynamic_cast<Declaration *>(dec.get()));
        ret->addDeclaration(dynamic_pointer_cast<Declaration>(dec));
        declaration_map.insert(p);
    }

    for(auto it = _contracts.begin(); it != _contracts.end(); ++it) {
        ret->addContract((*it)->clone());
    }

    for(auto it = _components.begin(); it != _components.end(); ++it)
    {
        ret->addComponent((*it)->clone());
    }


    ClonedDeclarationVisitor c(declaration_map);
    ret->accept_visitor(c);

    return ret;
}

sptr_name System::getName()
{
    return _name;
}

void System::setName(sptr_name name)
{
    _name = name;
    _name->setParent(this);
}

void System::addComponent(sptr_comp component)
{
    _components.insert(component);
    component->setParent(this);
}

std::set<sptr_comp> &System::getComponentsSet() {
    return _components;
}
