/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include <utility>

#include "chase/representation/Contract.hh"
#include "chase/utilities/ClonedDeclarationVisitor.hh"
#include "chase/utilities/Factory.hh"

using namespace chase;
using namespace std;

using sptr_contract = std::shared_ptr<Contract>;
using sptr_lform = std::shared_ptr<LogicFormula>;
using sptr_decl = std::shared_ptr<Declaration>;

void Contract:: mergeDeclarations(
        sptr_contract c1,
        sptr_contract c2,
        sptr_contract r,
        names_projection_map &correspondences,
        std::map< sptr_decl, sptr_decl >& declaration_map
        )
{
    /// \todo Implement the type checking.

    for(auto i = c1->declarations.begin(); i != c1->declarations.end(); ++i)
    {
        auto original = (*i);
        auto cloned = dynamic_pointer_cast<Declaration>(original->clone());
        std::pair< sptr_decl, sptr_decl> p(original, cloned);
        declaration_map.insert(p);

        r->declarations.push_back(cloned);
    }


    for(auto i = c2->declarations.begin(); i != c2->declarations.end(); ++i)
    {
        auto original = (*i);
        std::string name = original->getName()->getString();
        auto found = correspondences.find(name);

        if( found == correspondences.end())
        {
            // Check for name clashing between C1 and C2.
            for(auto j = r->declarations.begin();
                    j != r->declarations.end();++j)
            {
                if( name == (*j)->getName()->getString() )
                    messageError("Name clashing in composition.");
            }

            auto cloned = dynamic_pointer_cast<Declaration>(original->clone());
            std::pair< sptr_decl, sptr_decl> p(original, cloned);
            declaration_map.insert(p);

            r->declarations.push_back(cloned);
        }
        else
        {
            std::string to_find = found->second;
            for(auto j = r->declarations.begin();
                j != r->declarations.end();++j)
            {
                if((*j)->getName()->getString() == to_find )
                {
                    std::pair< sptr_decl, sptr_decl> p(original,*j);
                    declaration_map.insert(p);
                }
            }
        }
    }
}


sptr_contract Contract::composition(
        sptr_contract c1, sptr_contract c2,
        names_projection_map & correspondences,
        std::string name)
{
    auto composed = make_shared<Contract>(name);

    std::map< sptr_decl, sptr_decl > declaration_map;
    mergeDeclarations(c1, c2, composed, correspondences, declaration_map);

    composeLogic(c1, c2, composed);

    ClonedDeclarationVisitor v(declaration_map);
    composed->accept_visitor(v);

    return composed;
}

void Contract::composeLogic(
        sptr_contract c1,
        sptr_contract c2,
        sptr_contract r)
{
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second.get());
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second.get());
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second.get());
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second.get());
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }

    sptr_lform assumptions = nullptr;
    sptr_lform guarantees = nullptr;

    if( g1 != nullptr && g2 != nullptr ) {
        guarantees = And(dynamic_pointer_cast<LogicFormula>(g1->clone()),
            dynamic_pointer_cast<LogicFormula>(g2->clone()));
    } else if(g1 == nullptr && g2 == nullptr) {
        guarantees = True();
    } else if(g1 == nullptr && g2 != nullptr) {
        guarantees = dynamic_pointer_cast<LogicFormula>(g2->clone());
    } else {
        guarantees = dynamic_pointer_cast<LogicFormula>(g1->clone());
    }

    if( a1 != nullptr && a2 != nullptr ) {
        assumptions = And(dynamic_pointer_cast<LogicFormula>(a1->clone()),
        dynamic_pointer_cast<LogicFormula>(a2->clone()));
    } else if(a1 == nullptr && a2 == nullptr) {
        assumptions = True();
    } else if(a1 == nullptr && a2 != nullptr) {
        assumptions = dynamic_pointer_cast<LogicFormula>(a2->clone());
    } else {
        assumptions = dynamic_pointer_cast<LogicFormula>(a1->clone());
    }
    assumptions = Or(assumptions, Not(dynamic_pointer_cast<LogicFormula>
        (guarantees->clone())));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}

sptr_contract Contract::conjunction(
        sptr_contract c1, sptr_contract c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = make_shared<Contract>(name);

    std::map< sptr_decl, sptr_decl > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    conjoinLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}

void Contract::conjoinLogic(sptr_contract c1, sptr_contract c2, 
    sptr_contract r)
{
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second.get());
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second.get());
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second.get());
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second.get());
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }

    sptr_lform assumptions = nullptr;
    sptr_lform guarantees = nullptr;

    if( g1 != nullptr && g2 != nullptr ) {
        guarantees = And(dynamic_pointer_cast<LogicFormula>(g1->clone()), 
        dynamic_pointer_cast<LogicFormula>(g2->clone()));
    } else if(g1 == nullptr && g2 == nullptr) {
        guarantees = True();
    } else if(g1 == nullptr && g2 != nullptr) {
        guarantees = dynamic_pointer_cast<LogicFormula>(g2->clone());
    } else {
        guarantees = dynamic_pointer_cast<LogicFormula>(g1->clone());
    }

    if( a1 != nullptr && a2 != nullptr ) {
        assumptions = Or(dynamic_pointer_cast<LogicFormula>(a1->clone()),
            dynamic_pointer_cast<LogicFormula>(a2->clone()));
    } else if(a1 == nullptr && a2 == nullptr) {
        assumptions = True();
    } else if(a1 == nullptr && a2 != nullptr) {
        assumptions = dynamic_pointer_cast<LogicFormula>(a2->clone());
    } else {
        assumptions = dynamic_pointer_cast<LogicFormula>(a1->clone());
    }


    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);


}



void Contract::saturate(sptr_contract c)
{
    saturateLogic(c);
}

void Contract::saturateLogic(sptr_contract c )
{
    auto a = c->assumptions.find(logic);
    auto g = c->guarantees.find(logic);

    sptr_lform assumptions = nullptr;
    sptr_lform guarantees = nullptr;

    if(a != c->assumptions.end())
    {
        assumptions = dynamic_pointer_cast<LogicFormula>(a->second);
        if(assumptions == nullptr)
            messageError("Non logic formula in temporal logic domain");
    }

    if(g != c->guarantees.end())
    {
        guarantees = dynamic_pointer_cast<LogicFormula>(g->second);
        if(guarantees == nullptr)
            messageError("Non logic formula in temporal logic domain");
    }

    sptr_lform saturation = nullptr;
    if(assumptions != nullptr ) {
        saturation = Not(dynamic_pointer_cast<LogicFormula>(
            assumptions->clone()));
    }else{
        return; // No saturation necessary.
    }

    if( guarantees == nullptr )
        // Then, create new guarantees negating the
        // assumptions.
    {
        std::pair< semantic_domain, Specification * > p(
                logic, saturation.get());
        c->guarantees.insert(p);
    }
    else
    {
        auto parent = guarantees->getParent();
        guarantees = Or(guarantees, saturation);
        g->second = guarantees;
        guarantees->setParent(parent);
    }
}



