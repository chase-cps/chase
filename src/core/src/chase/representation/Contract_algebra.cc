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

void Contract:: mergeDeclarations(
        Contract * c1,
        Contract * c2,
        Contract * r,
        names_projection_map &correspondences,
        std::map< Declaration *, Declaration * >& declaration_map
        )
{
    /// \todo Implement the type checking.

    for(auto original : c1->declarations) {
        auto cloned = original->clone();
        std::pair< Declaration*, Declaration *> p(original, cloned);
        declaration_map.insert(p);

        r->declarations.push_back(cloned);
    }

    for(auto original : c2->declarations)
    {
        std::string name = original->getName()->getString();
        std::cout << name << std::endl;
        auto found = correspondences.find(name);

        if( found == correspondences.end())
        {
            // Check for name clashing between C1 and C2.
            for(auto & declaration : r->declarations) {
                if( name == declaration->getName()->getString() )
                    messageError("Name clashing in composition: " + name);
            }

            auto cloned = original->clone();
            std::pair< Declaration*, Declaration *> p(original, cloned);
            declaration_map.insert(p);

            r->declarations.push_back(cloned);
        }
        else {
            std::string to_find = found->second;
            for(auto & declaration : r->declarations) {
                if(declaration->getName()->getString() == to_find ) {
                    std::pair< Declaration*, Declaration *> p(original,declaration);
                    declaration_map.insert(p);
                }
            }
        }
    }
}


Contract * Contract::composition(
        Contract * c1, Contract * c2,
        names_projection_map & correspondences,
        std::string name)
{
    auto composed = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, composed, correspondences, declaration_map);

    // Fix variables causality.
    // If it is output in at least of the composing contracts, then it must be
    // output (i.e., controlled) variable.
    for(auto & dit : composed->declarations)
    {
        if(dit->IsA() == variable_node)
        {
            std::vector< Declaration * > originals;
            for(auto & mit : declaration_map )
            {
                if(mit.second == dit)
                    originals.push_back(mit.first);
            }
            for(auto & vit : originals)
            {
                if(vit->IsA() == variable_node)
                {
                    auto var = reinterpret_cast< Variable * >(dit);
                    auto original =
                            reinterpret_cast< Variable * >(vit);
                    if(original->getCausality() == output && var->getCausality() == input)
                    {
                        var->setCausality(output);
                    }
                }
            }
        }
    }

    composeLogic(c1, c2, composed);

    ClonedDeclarationVisitor v(declaration_map);
    composed->accept_visitor(v);

    return composed;
}

void Contract::composeLogic(
        Contract * c1,
        Contract * c2,
        Contract * r)
{
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }

    LogicFormula * assumptions = nullptr;
    LogicFormula * guarantees = nullptr;

    if( g1 != nullptr && g2 != nullptr ) {
        guarantees = And(g1->clone(), g2->clone());
    } else if(g1 == nullptr && g2 == nullptr) {
        guarantees = True();
    } else if(g1 == nullptr && g2 != nullptr) {
        guarantees = g2->clone();
    } else {
        guarantees = g1->clone();
    }

    if( a1 != nullptr && a2 != nullptr ) {
        assumptions = And(a1->clone(), a2->clone());
    } else if(a1 == nullptr && a2 == nullptr) {
        assumptions = True();
    } else if(a1 == nullptr && a2 != nullptr) {
        assumptions = a2->clone();
    } else {
        assumptions = a1->clone();
    }
    assumptions = Or(assumptions, Not(guarantees->clone()));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}

Contract * Contract::conjunction(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    conjoinLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}




void Contract::conjoinLogic(Contract *c1, Contract *c2, Contract *r)
{
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }

    LogicFormula * assumptions = nullptr;
    LogicFormula * guarantees = nullptr;

    if( g1 != nullptr && g2 != nullptr ) {
        guarantees = And(g1->clone(), g2->clone());
    } else if(g1 == nullptr && g2 == nullptr) {
        guarantees = True();
    } else if(g1 == nullptr && g2 != nullptr) {
        guarantees = g2->clone();
    } else {
        guarantees = g1->clone();
    }

    if( a1 != nullptr && a2 != nullptr ) {
        assumptions = Or(a1->clone(), a2->clone());
    } else if(a1 == nullptr && a2 == nullptr) {
        assumptions = True();
    } else if(a1 == nullptr && a2 != nullptr) {
        assumptions = a2->clone();
    } else {
        assumptions = a1->clone();
    }


    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);


}

Contract * Contract::quotient(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name, bool synthesizable)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res,
                      correspondences, declaration_map);

    quotientLogic(c1, c2, res, synthesizable);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}



void Contract::quotientLogic(
        Contract *c1, Contract *c2, Contract *r, bool synthesizable) {
    LogicFormula *a1 = nullptr;
    LogicFormula *a2 = nullptr;
    LogicFormula *g1 = nullptr;
    LogicFormula *g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if (i != c1->assumptions.end()) {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if (a1 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if (i != c2->assumptions.end()) {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if (a2 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if (i != c1->guarantees.end()) {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if (g1 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if (i != c2->guarantees.end()) {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if (g2 == nullptr) messageError("Wrong format in Logic.");
    }

    LogicFormula *assumptions = nullptr;
    LogicFormula *guarantees = nullptr;

    if (a1 != nullptr && g2 != nullptr)
        assumptions = And(a1->clone(), g2->clone());
    else if (a1 == nullptr && g2 == nullptr)
        assumptions = True();
    else if (a1 == nullptr && g2 != nullptr)
        assumptions = a1->clone();
    else
        assumptions = g2->clone();

    if (a2 != nullptr && g1 != nullptr)
        guarantees = And(a2->clone(), g1->clone());
    else if (a2 == nullptr && g1 == nullptr)
        guarantees = True();
    else if (a2 != nullptr && g1 == nullptr)
        guarantees = a2->clone();
    else
        guarantees = g1->clone();

    if (!synthesizable)
    {
        if(a1 != nullptr)
            guarantees = Or(guarantees, Not(a1->clone()));
        if(g2 != nullptr)
            guarantees = Or(guarantees, Not(g2->clone()));
    }

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);

}




void Contract::saturate(Contract *c)
{
    saturateLogic(c);
}

void Contract::saturateLogic(Contract * c )
{
    auto a = c->assumptions.find(logic);
    auto g = c->guarantees.find(logic);

    LogicFormula * assumptions = nullptr;
    LogicFormula * guarantees = nullptr;

    if(a != c->assumptions.end())
    {
        assumptions = dynamic_cast< LogicFormula * >(a->second);
        if(assumptions == nullptr)
            messageError("Non logic formula in temporal logic domain");
    }

    if(g != c->guarantees.end())
    {
        guarantees = dynamic_cast< LogicFormula * >(g->second);
        if(guarantees == nullptr)
            messageError("Non logic formula in temporal logic domain");
    }

    if(assumptions != nullptr ) {
        guarantees = Implies(assumptions->clone(), guarantees);
    }else{
        return; // No saturation necessary.
    }

    if( guarantees == nullptr )
        // Then, create new guarantees negating the
        // assumptions.
    {
        std::pair< semantic_domain, Specification * > p(
                logic, Not(assumptions->clone()));
        c->guarantees.insert(p);
    }
    else
    {
        auto parent = guarantees->getParent();
        g->second = guarantees;
        guarantees->setParent(parent);
    }
}


Contract *Contract::refinementCheck(Contract *c1, Contract *c2, names_projection_map &correspondences,
                               std::string name) {
    auto rcheck = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, rcheck, correspondences, declaration_map);

    /// \todo Check that variables have the same causality in both contracts.

    refinementCheckLogic(c1, c2, rcheck);

    ClonedDeclarationVisitor v(declaration_map);
    rcheck->accept_visitor(v);

    return rcheck;
}

void Contract::refinementCheckLogic(Contract *c1, Contract *c2, Contract *r) {
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }


    /// \todo Reason about correctness in the case one is a nullptr.
    if( a1 == nullptr )
        a1 = True();
    if( a2 == nullptr )
        a2 = True();
    auto assumptions = Implies(a2, a1);

    if( g1 == nullptr )
        g1 = True();
    if( g2 == nullptr )
        g2 = True();

    g1 = Or(g1, Not(a1->clone()));
    g2 = Or(g2, Not(a2->clone()));

    auto guarantees = Implies(g1, g2);
    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}