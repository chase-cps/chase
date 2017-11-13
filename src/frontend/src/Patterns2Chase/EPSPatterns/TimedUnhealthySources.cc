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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/TimedUnhealthySources.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/TimedEPSPatterns.hh"

#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"

using namespace Specification;
using namespace EPSPatterns;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Timing;
using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;
using namespace chase::epstool::EPS;

using namespace Utilities;
using namespace Graphs;

TimedUnhealthySources::TimedUnhealthySources( std::regex r ) :
    TimedEPSPattern(r)
{
}


// UNHEALTHY SOURCES
std::vector< std::string > TimedUnhealthySources::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;
    std::string pattern(" must be disconnected when unhealthy within ");
    std::size_t _begin = text.find(pattern);
    std::size_t _end = _begin + pattern.length();

    std::string components = text.substr(0,_begin);
    ret.push_back(components);

    std::string timing = text.substr(_end);
    ret.push_back(timing);

    return ret;
}


void TimedUnhealthySources::_createTimedConstraints(
        Contracts::AGContract * contract,
        std::vector< std::string > strings )
{
    std::vector< int > * nodes = 
        HighLevelEPSPattern::_getNodesByType( strings[0], _topology );

    chase_time tolerance = _getTimeFromString(strings[1]);

    size_t num_of_nodes = nodes->size();

    // Create a Safety Counter for every 
    for( size_t i = 0; i < num_of_nodes; ++i )
    {
        std::string node((_topology->components[nodes->at(i)])->name);
        std::string counterName("counter_disconnection_");
        counterName += node;

        SafetyCounter * counter = new SafetyCounter(counterName);

        // Unsafe condition: the node is broken
        WellFormedFormula * unsafe = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination(
                    op_lnot,
                    (_topology->components[nodes->at(i)])->stateVariable));

        counter->setUnsafeCondition( unsafe );


        // Safety condition: the node is disconnected;
        
        WellFormedFormula * safe = _nodeDisconnected(
                nodes->at(i), _topology );

        counter->setSafetyCondition(safe);
        
        // Command: de-activate all the connections;
        
        WellFormedFormula * command = _commandDisconnection(
                nodes->at(i), _topology);

        counter->setCommand(command);

        // Unsafe Tolerance: the time specified in the pattern
        counter->setUnsafeTolerance(tolerance);

        // Insert the counter within the contract.
        contract->safety_counters.push_back(counter); 
        
    }
}

std::vector< WellFormedFormula * > TimedUnhealthySources::_createPathFormulae(  
        Contracts::AGContract *,
        std::vector< std::string > )
{
    return std::vector< WellFormedFormula * >();
}


void TimedUnhealthySources::_createConstraints(
        Contracts::AGContract *,
        std::vector< Behaviors::Logics::WellFormedFormula * >)
{
}


/////////////////////////////////////////////////


