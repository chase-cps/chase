/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include <utility>

#include "chase/representation/Contract.hh"
#include "chase/utilities/ClonedDeclarationVisitor.hh"

using namespace chase;
using namespace std;

using sptr_decl = std::shared_ptr<Declaration>;
using sptr_spec = std::shared_ptr<Specification>;
using sptr_name = std::shared_ptr<Name>;
using sptr_contract = std::shared_ptr<Contract>;

Contract::Contract( std::string name ) :
    _name(new Name(name))
{
    _node_type = contract_node;
}

Contract::~Contract() = default;

int Contract::accept_visitor(chase::BaseVisitor &v) {
    return v.visitContract(*this);
}

std::string Contract::getString() {
    std::string ret("Contract:\n");
    ret += _name->getString();

    ret += "\nDeclarations:\n";

    for(auto dit = declarations.begin(); dit != declarations.end(); ++dit )
    {
        Declaration * d = (*dit).get();
        ret += d->getString();
        ret += "\n";
    }

    ret += "Assumptions:\n";
    std::map< semantic_domain, Specification * >::iterator sit;
    for(auto sit = assumptions.begin(); sit != assumptions.end(); ++sit)
    {

        Specification * s = (*sit).second.get();
        ret += s->getString();
        ret += "\n";
    }

    ret += "Guarantees:\n";
    for(auto sit = guarantees.begin(); sit != guarantees.end(); ++sit)
    {
        Specification * s = (*sit).second.get();
        ret += s->getString();
        ret += "\n";
    }

    ret += "=======================";

    return ret;
}

sptr_name  Contract::getName() const {
    return _name;
}

void Contract::setName(sptr_name  name) {
    _name = name;
}

void Contract::addDeclaration(sptr_decl declaration) {
    declarations.push_back(declaration);
    declaration->setParent(this);
}

void Contract::addAssumptions(semantic_domain domain, sptr_spec spec) {
    std::pair< semantic_domain, sptr_spec > a(domain, spec);
    assumptions.insert(a);
    spec->setParent(this);
}

void Contract::addGuarantees(semantic_domain domain, sptr_spec spec) {
    std::pair< semantic_domain, sptr_spec > g(domain, spec);
    guarantees.insert(g);
    spec->setParent(this);
}



/// \todo Implement the clone method.
sptr_contract Contract::clone() {
    auto ret = make_shared<Contract>(_name->getString());

    // Corresponences maps.
    std::map< sptr_decl, sptr_decl > declaration_map;

    // Declarations.
    for(auto it = declarations.begin(); it != declarations.end(); ++it)
    {
        sptr_decl current = *it;
        auto dec = std::dynamic_pointer_cast<DataDeclaration>(
            current->clone());
        std::pair< sptr_decl , sptr_decl > p(current, dec);
        ret->addDeclaration(dec);
        declaration_map.insert(p);
    }

    // Assumptions.
    for(auto it = assumptions.begin(); it != assumptions.end(); ++it)
    {
        auto spec = (*it).second->clone();
        ret->addAssumptions((*it).first, 
            (std::dynamic_pointer_cast<Specification>(spec)));
    }

    // Guarantees.
    for(auto it = guarantees.begin(); it != guarantees.end(); ++it)
    {
        auto spec = (*it).second->clone();
        ret->addGuarantees((*it).first, 
            (std::dynamic_pointer_cast<Specification>(spec)));
    }

    ClonedDeclarationVisitor c(declaration_map);
    ret->accept_visitor(c);

    return ret;
}





