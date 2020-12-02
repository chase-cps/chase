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

    for(auto i = c1->declarations.begin(); i != c1->declarations.end(); ++i)
    {
        auto original = (*i);
        auto cloned = original->clone();
        std::pair< Declaration*, Declaration *> p(original, cloned);
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
                    messageError("Name clashing in composition: " + name);
            }

            auto cloned = original->clone();
            std::pair< Declaration*, Declaration *> p(original, cloned);
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
                    std::pair< Declaration*, Declaration *> p(original,*j);
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
            for(size_t vit = 0; vit < originals.size(); ++vit )
            {
                if(originals[vit]->IsA() == variable_node)
                {
                    auto var = reinterpret_cast< Variable * >(dit);
                    auto original =
                            reinterpret_cast< Variable * >(originals[vit]);
                    if(original->getCausality() == output && var->getCausality() == input)
                    {
                        std::cout << "replace: " << var->getName()->getString() << std::endl;
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



