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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/TimedKeepPowered.cc
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

TimedKeepPowered::TimedKeepPowered( std::regex r ) :
    TimedEPSPattern(r)
{
}


// KEEP CONNECTED
std::vector< std::string > TimedKeepPowered::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;
    std::string pattern_1(" must never be unpowered for more than ");
    std::string pattern_2(" from ");
    int pattern_1_length = pattern_1.length();
    int pattern_2_length = pattern_2.length();

    std::size_t _begin_1 = text.find(pattern_1);
    std::size_t _end_1 = _begin_1 + pattern_1_length;
    std::size_t _begin_2 = text.find(pattern_2);
    std::size_t _end_2 = _begin_2 + pattern_2_length;

    std::string comp1 = text.substr(0, _begin_1);
    std::string comp2 = text.substr(_end_1, _begin_2 - _end_1);
    std::string comp3 = text.substr(_end_2);

    ret.push_back(comp1);
    ret.push_back(comp3);
    ret.push_back(comp2);

    return ret;
}

std::vector< WellFormedFormula * > TimedKeepPowered::_createPathFormulae(  
        Contracts::AGContract *,
        std::vector< std::string > )
{
    return std::vector< WellFormedFormula * >();
}

void TimedKeepPowered::_createConstraints(
        Contracts::AGContract *,
        std::vector< Behaviors::Logics::WellFormedFormula * >)
{
}

void TimedKeepPowered::_createTimedConstraints(
        Contracts::AGContract * contract,
        std::vector< std::string > strings )
{

    int ** matrix = _topology->getInverseMatrix();

    Graph * graph = new Graph(matrix, _topology->getSize(), true); 

    std::vector< int > * sinkNodes = 
        HighLevelEPSPattern::_getNodesByType( strings[0], _topology );

    std::vector< int > * sourceNodes =
        HighLevelEPSPattern::_getNodesByType( strings[1], _topology );

    // For every sink, create a Safety Counter.
    //
    for( size_t i = 0; i < sinkNodes->size(); ++i )
    {
        std::string node((_topology->components[sinkNodes->at(i)])->name);
        std::string counterName("counter_unpowererd_");
        counterName += node;

        SafetyCounter * counter = new SafetyCounter(counterName);


        // Create the Command;

        std::list< std::vector<int> > * paths =
            new std::list< std::vector< int > >();
        for( size_t j = 0; j < sourceNodes->size(); ++j )
        {
            std::vector< int > visited;
            visited.push_back(sinkNodes->at(i));

            std::list< std::vector<int> >* current = 
                new std::list< std::vector< int > >();

            Graph::findAllPathsBetweenNodes(
                    graph,
                    visited,
                    sourceNodes->at(j),
                    current );

            std::list< std::vector<int> >::iterator it;
            for( it = current->begin();
                    it != current->end();
                    ++it )
            {
                paths->push_back(*it);
            }
            delete(current);
        }

        // Create the formula for the node connected to the generator in
        // position i

        std::list< std::vector< int > >::iterator lit = paths->begin();
        std::vector<int> * v = &(*lit);

        WellFormedFormula * command = static_cast< WellFormedFormula * >(
                _commandPower( v, _topology, true ));

        for( ++lit; lit != paths->end(); ++lit )
        {
            v = &(*lit);

            command = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_lor,
                        command,
                        _commandPower( v, _topology, true )));
        }
        counter->setCommand(command);

        // Create Unsafe Condition: the node is unpowered.

        lit = paths->begin();
        v = &(*lit);

        WellFormedFormula * i_paths = static_cast< WellFormedFormula * >(
                pathDoesExist( v, _topology, true ));

        for( ++lit; lit != paths->end(); ++lit )
        {
            v = &(*lit);

            i_paths = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_lor,
                        i_paths,
                        pathDoesExist( v, _topology, true )));
        }

        lit = paths->begin();
        v = &(*lit);

        WellFormedFormula * n_paths = static_cast< WellFormedFormula * >(
                pathDoesExist( v, _topology, true ));

        for( ++lit; lit != paths->end(); ++lit )
        {
            v = &(*lit);

            n_paths = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_lor,
                        n_paths,
                        pathDoesExist( v, _topology, true )));
        }
        
        WellFormedFormula * unsafe = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination(op_lnot, n_paths));

        counter->setUnsafeCondition(unsafe);

        // Create the safety condition.

        WellFormedFormula * safe = n_paths;
        counter->setSafetyCondition(safe);

        // Create timing.

        chase_time tolerance = _getTimeFromString(strings[2]);
        counter->setUnsafeTolerance(tolerance);
        contract->safety_counters.push_back(counter);

    }
}

