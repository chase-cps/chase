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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/TimedContactors.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/TimedEPSPatterns.hh"

#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"

using namespace Specification;
using namespace EPSPatterns;
using namespace Behaviors;
using namespace Logics;
using namespace Timing;

using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;
using namespace chase::epstool::EPS;

using namespace Utilities;
using namespace Graphs;

TimedContactors::TimedContactors( std::regex r ) :
    TimedEPSPattern(r)
{
}

std::vector< std::string > TimedContactors::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;
    std::string pattern_1("Contactors requires ");
    std::string pattern_2(" to ");
    std::size_t _end_1 = pattern_1.length();
    std::size_t _begin_2 = text.find(pattern_2);
    std::size_t _end_2 = _begin_2 + pattern_2.length();



    std::string t = text.substr(_end_1, _begin_2 - _end_1);
    std::string action = text.substr(_end_2);

    ret.push_back(t);
    ret.push_back(action);
    return ret;
}

std::vector< WellFormedFormula * > TimedContactors::_createPathFormulae(  
        Contracts::AGContract *,
        std::vector< std::string > )
{
    return std::vector< WellFormedFormula * >();
}


void TimedContactors::_createConstraints(
        Contracts::AGContract *,
        std::vector< Behaviors::Logics::WellFormedFormula * >)
{
}

void TimedContactors::_createTimedConstraints(
        Contracts::AGContract * contract,
        std::vector< std::string > strings )
{
    chase_time t = _getTimeFromString(strings[0]);
    std::string action = strings[1];

    for( size_t i = 0; i < _topology->contactors.size(); ++i )
    {
        PhysicalContactor * phyCon = dynamic_cast< PhysicalContactor * >(
                _topology->contactors[i] );
        if( phyCon == NULL )
        {
            std::cerr << "[ERROR] Not a physical contactor (ideal)" 
                << std::endl;
            exit(-1);
        }

        //Create the Counter.

        std::string counter_name(action);
        counter_name += "_counter_";

        std::stringstream ss;
        ss << phyCon->number;
        std::string n(ss.str());
        counter_name += n;

        LogicProposition * intention = phyCon->intentionVariable;
        LogicProposition * status = phyCon->stateVariable;

        WellFormedFormula * activation;
        WellFormedFormula * desire;

        // Define activation and desired conditions
        if( action == "open" )
        {
            activation = static_cast< WellFormedFormula * >( intention );

            desire = static_cast< WellFormedFormula * >( status );
        }
        else if (action == "close" )
        {
            activation = static_cast< WellFormedFormula * >(
                    new UnaryLogicCombination(
                        op_lnot,
                        intention));


            desire = static_cast< WellFormedFormula * >( 
                    new UnaryLogicCombination(
                        op_lnot,
                        status));
        }
        else
        {
            std::cerr << "[ERROR] Invalid contactor action" << std::endl;
            exit(-1);
        }

        LivenessCounter * lc = new LivenessCounter(counter_name);
        lc->setActivationCondition(activation);
        lc->setDesiredCondition(desire);
        lc->setMaxTimeRequired(t);

        contract->liveness_counters.push_back(lc);
    }

}
/////////////////////////////////////////////////


