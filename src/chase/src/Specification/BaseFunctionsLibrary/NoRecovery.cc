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
 * @file	chase/src/Specification/BaseFunctionsLibrary/NoRecovery.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/NoRecovery.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

NoRecovery::NoRecovery() :
    RequirementFunction("no-recovery",1)
{
}

NoRecovery::~NoRecovery()
{
}

void NoRecovery::resolve( Requirement * req, Problem * problem )
{

    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();


    // Retrieve the parameter used to indicate the set of components
    // never recovering after failure.
    std::string par0 = req->getParameter(0);

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );

    // For each found component.
    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = components.begin();
            it != components.end();
            ++it )
    {
        WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
                contract->assumptions );
        assert(assumptions);

        Architecture::NetworkComponent * currcomp = *it;
        // Retrieve the component state variable.

        Behaviors::Variable * statevar = problem->getStateVariable( currcomp );

        UnaryLogicCombination * broken = 
            new UnaryLogicCombination( 
                    op_lnot, 
                    new LogicProposition( statevar->GetName() ));

        UnaryTemporalFormula * next =
            new UnaryTemporalFormula( op_next, broken );

        BinaryLogicCombination * implies =
            new BinaryLogicCombination( op_implies, broken, next );

        UnaryTemporalFormula * globally = 
            new UnaryTemporalFormula(op_globally, implies);

        contract->assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( op_land, assumptions, globally ));
    }

}

