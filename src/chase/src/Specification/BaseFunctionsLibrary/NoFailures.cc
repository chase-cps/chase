/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/src/Specification/BaseFunctionsLibrary/NoFailures.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/NoFailures.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

NoFailures::NoFailures() :
    RequirementFunction("no-failures",1)
{
}

NoFailures::~NoFailures()
{
}

/// \todo
// It is necessary to decide if insert this properties.
// Two possibilities are available:
// - Do not insert these constraints and remove the variables.
// - Keep as it is.

void NoFailures::resolve( Requirement * req, Problem * problem )
{
    std::string par0 = req->getParameter(0);
    std::set< Architecture::NetworkComponent * > components =
        Manipulation::findComponents( par0, problem->getNetwork() );

    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = components.begin();
            it != components.end();
            ++it )
    {
        // Register the component as constant.
        Architecture::NetworkComponent * currcomp = *it;
        problem->constant_components.insert(currcomp);

        // Retrieve the contract.
/*
        WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
                contract->assumptions );
        assert(assumptions);

        Behaviors::Variable * statevar = problem->getStateVariable( currcomp );

        LogicProposition * prop = new LogicProposition(statevar->GetName());
        UnaryTemporalFormula * globally = new UnaryTemporalFormula(
                op_globally, prop);

        contract->assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( 
                    op_land, assumptions, globally ));

        assumptions = dynamic_cast< WellFormedFormula * >(
                contract->assumptions );

        contract->assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( op_land, assumptions, prop ));



        WellFormedFormula * guarantees = dynamic_cast< WellFormedFormula * >(
                contract->guarantees );

        Behaviors::Variable * intentvar = 
            problem->getIntentionVariable( currcomp );

        LogicProposition * iprop = new LogicProposition(intentvar->GetName());
        UnaryTemporalFormula * iglobally = new UnaryTemporalFormula(
                op_globally, iprop);

        contract->guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( 
                    op_land, guarantees, iglobally ));

        guarantees = dynamic_cast< WellFormedFormula * >(
                contract->guarantees );

        contract->guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( op_land, guarantees, iprop ));

*/
    }



}

