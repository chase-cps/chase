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

Contract * Contract::composition(
        Contract * C1, Contract * C2,
        names_projection_map & correspondences)
{
    auto composed = C1->clone();
    auto cloned = C2->clone();
    /// \todo Visit the clone to modify it according to the map.


    /// Build the composed contact properly.
    /// At the moment, only for logic.

    return composed;
}

void Contract::saturate(Contract *c)
{
    _saturateTemporalLogic(c);
}

void Contract::_saturateTemporalLogic( Contract * c )
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

    LogicFormula * saturation = nullptr;
    if(assumptions != nullptr ) {
        saturation = Not(assumptions->clone());
    }else{
        return; // No saturation necessary.
    }

    if( guarantees == nullptr )
        // Then, create new guarantees negating the
        // assumptions.
    {
        std::pair< semantic_domain, Specification * > p(
                logic, saturation);
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

