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
 * @file	chase/src/Specification/BaseFunctionsLibrary/InitialState.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/InitialState.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;



InitialState::InitialState() :
    RequirementFunction("initial-state",2)
{
}

InitialState::~InitialState()
{
}

void InitialState::resolve( Requirement * req, Problem * problem )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    // Retrieve the parameters used
    std::string par0 = req->getParameter(0);
    unsigned int par1 = atoi(req->getParameter(1).c_str());

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );

    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = components.begin();
            it != components.end();
            ++it )
    {
        Behaviors::Variable * intention = problem->getStateVariable(*it);
        WellFormedFormula * wff;
        if( par1 > 0 )
            wff = static_cast< WellFormedFormula * >(
                    new LogicProposition(intention->GetName()));
        else
            wff = static_cast< WellFormedFormula * >(
                    new UnaryLogicCombination(
                        op_lnot,
                        new LogicProposition(intention->GetName())));

        WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
                contract->assumptions );
        assert(assumptions);

        contract->assumptions = new BinaryLogicCombination(
                op_land,
                assumptions,
                wff );
    }   
}








