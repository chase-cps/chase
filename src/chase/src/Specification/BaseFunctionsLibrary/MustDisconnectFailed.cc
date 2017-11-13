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
 * @file	chase/src/Specification/BaseFunctionsLibrary/MustDisconnectFailed.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/MustDisconnectFailed.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Behaviors::Timing;

MustDisconnectFailed::MustDisconnectFailed() :
    RequirementFunction("must-disconnect-failed",3)
{
}

MustDisconnectFailed::~MustDisconnectFailed()
{
}

void MustDisconnectFailed::resolve( Requirement * req, Problem * problem )
{
    std::list< Behaviors::Timing::chase_time > * lst = getTiming(req);
    if( lst == NULL || lst->size() != 1 )
    {
        _untimedResolve(req, problem);
    } 
    else
    {
        _timedResolve( req, problem, lst );
    }
}

void MustDisconnectFailed::_timedResolve(
        Requirement * req,
        Problem * problem,
        std::list< Behaviors::Timing::chase_time > * timing )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    // Retrieve the parameter used to indicate the set of components
    // that must always be disconnected after failure.
    std::string par0 = req->getParameter(0);

    Timing::chase_time tolerance = timing->front();

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );

    // For each component in the set, find the entering and exiting
    // network connections. Collect all the switches.
    std::set< Architecture::NetworkComponent * >::iterator it;

    for( it = components.begin();
            it != components.end();
            ++it )
    {


        Architecture::NetworkComponent * currcomp = *it;

        // Find all the involved switches for currcomp.
        std::vector< Architecture::NetworkComponent * > switches;
        std::list< Architecture::NetworkConnection * >::iterator cit;
        for( cit = problem->getNetwork()->connections.begin();
                cit != problem->getNetwork()->connections.end();
                ++cit )
        {
            Architecture::NetworkConnection * currconn = *cit;
            if( currconn->getTarget() == currcomp || 
                    currconn->getSource() == currcomp )
            {
                if( currconn->getSwitch() != NULL )
                {
                    switches.push_back(currconn->getSwitch());
                }
            }
        }

        // Create the formula for the currcomp Component.
        if( switches.size() > 0 )
        {
            Behaviors::Variable * statevar =
                problem->getStateVariable( switches[0] );

            WellFormedFormula * implied = static_cast< WellFormedFormula * >(
                    new UnaryLogicCombination(
                        op_lnot,
                        new LogicProposition(statevar->GetName())));

            for( size_t i = 1; i < switches.size(); ++i )
            {
                statevar = problem->getStateVariable( switches[i] );
                implied = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            implied,
                            new UnaryLogicCombination(
                                op_lnot,
                                new LogicProposition(statevar->GetName()))));
            }

            statevar = problem->getStateVariable( currcomp );
            WellFormedFormula * condition = static_cast< WellFormedFormula * >(
                    new UnaryLogicCombination(
                        op_lnot,
                        new LogicProposition(statevar->GetName())));

            std::string component_name(currcomp->getName());
            std::string counter_name = component_name + "_disconnect_counter";
            for(std::string::iterator it = counter_name.begin(); 
                    it != counter_name.end(); ++it) 
            {
                if( *it == ' ' )
                {
                    *it = '_';
                }
            }
            SafetyCounter * counter = new SafetyCounter(counter_name);

            counter->setUnsafeCondition(condition);
            counter->setSafetyCondition(implied);
            counter->setCommand(new LogicConstant(lc_true));
            counter->setUnsafeTolerance(tolerance);

            contract->safety_counters.push_back(counter);
        }

    }
}











void MustDisconnectFailed::_untimedResolve( 
        Requirement * req, 
        Problem * problem )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();


    // Retrieve the parameter used to indicate the set of components
    // that must always be disconnected after failure.
    std::string par0 = req->getParameter(0);

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components =  
        Manipulation::findComponents( par0, problem->getNetwork() );

    // For each component in the set, find the entering and exiting
    // network connections. Collect all the switches.
    std::set< Architecture::NetworkComponent * >::iterator it;

    for( it = components.begin();
            it != components.end();
            ++it )
    {


        Architecture::NetworkComponent * currcomp = *it;

        // Find all the involved switches for currcomp.
        std::vector< Architecture::NetworkComponent * > switches;
        std::list< Architecture::NetworkConnection * >::iterator cit;
        for( cit = problem->getNetwork()->connections.begin();
                cit != problem->getNetwork()->connections.end();
                ++cit )
        {
            Architecture::NetworkConnection * currconn = *cit;
            if( currconn->getTarget() == currcomp || 
                    currconn->getSource() == currcomp )
            {
                if( currconn->getSwitch() != NULL )
                {
                    switches.push_back(currconn->getSwitch());
                }
            }
        }

        // Create the formula for the currcomp Component.
        if( switches.size() > 0 )
        {
            Behaviors::Variable * statevar =
                problem->getStateVariable( switches[0] );

            WellFormedFormula * implied = static_cast< WellFormedFormula * >(
                    new UnaryLogicCombination(
                        op_lnot,
                        new LogicProposition(statevar->GetName())));

            for( size_t i = 1; i < switches.size(); ++i )
            {
                statevar = problem->getStateVariable( switches[i] );
                implied = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            implied,
                            new UnaryLogicCombination(
                                op_lnot,
                                new LogicProposition(statevar->GetName()))));
            }

            statevar = problem->getStateVariable( currcomp );
            WellFormedFormula * condition = static_cast< WellFormedFormula * >(
                    new UnaryLogicCombination(
                        op_lnot,
                        new LogicProposition(statevar->GetName())));

            WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                    new UnaryTemporalFormula(
                        op_globally,
                        new BinaryLogicCombination(
                            op_implies,
                            condition,
                            new UnaryTemporalFormula(
                                op_next,
                                implied))));

            WellFormedFormula * guarantees = 
                dynamic_cast< WellFormedFormula * >(contract->guarantees);

            contract->guarantees = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_land,
                        guarantees,
                        wff ));
        }
    }
}



    std::list< Behaviors::Timing::chase_time > * 
MustDisconnectFailed::getTiming( Requirement * req )
{
    if( req->getAriety() == 3 )
    {
        std::list< Behaviors::Timing::chase_time > * ret;
        ret = new std::list< Behaviors::Timing::chase_time >();

        unsigned int par1 = atoi(req->getParameter(1).c_str());
        std::string par2 = req->getParameter(2);

        Timing::chase_time t(par1, par2);
        ret->push_back(t);

        return ret;
    }
    else
    {
        return NULL;
    }
} 



