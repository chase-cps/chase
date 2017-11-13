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
 * @file	chase/src/Specification/BaseFunctionsLibrary/SwitchOnTime.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/SwitchOnTime.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

SwitchOnTime::SwitchOnTime() :
    RequirementFunction("switch-on-time",3)
{
}

SwitchOnTime::~SwitchOnTime()
{
}

void SwitchOnTime::resolve( Requirement * req, Problem * problem )
{

    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    // Retrieve the parameters used
    std::string par0 = req->getParameter(0);


    std::list< Behaviors::Timing::chase_time > * lst = getTiming(req);
    if( lst == NULL || lst->size() != 1 )
    {
        std::cerr << "ERROR: wrong timing specification" << std::endl;
        exit(-1);
    }

    Timing::chase_time t = lst->front();

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );

    // For each component in the list, create a counter.
    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = components.begin();
            it != components.end();
            ++it )
    {
        Variable * intentionvar = problem->getIntentionVariable(*it);
        Variable * statevar = problem->getStateVariable(*it);

        problem->physical_switch_on.insert(*it);


        std::string component_name((*it)->getName());
        std::string counter_name = component_name + "_switch_on_counter";
        for(std::string::iterator it = counter_name.begin(); 
                it != counter_name.end(); ++it) 
        {
            if( *it == ' ' )
            {
                *it = '_';
            }
        }

        Timing::LivenessCounter * counter = 
            new Timing::LivenessCounter(counter_name);

        counter->setActivationCondition(
                new LogicProposition(intentionvar->GetName()));

        counter->setDesiredCondition(
                new LogicProposition(statevar->GetName()));

        counter->setMaxTimeRequired(t);

        contract->liveness_counters.push_back(counter);
    }
}

std::list< Behaviors::Timing::chase_time > * 
    SwitchOnTime::getTiming( Requirement * req )
{
    std::list< Behaviors::Timing::chase_time > * ret;
    ret = new std::list< Behaviors::Timing::chase_time >();

    unsigned int par1 = atoi(req->getParameter(1).c_str());
    std::string par2 = req->getParameter(2);

    Timing::chase_time t(par1, par2);
    ret->push_back(t);

    return ret;
}

