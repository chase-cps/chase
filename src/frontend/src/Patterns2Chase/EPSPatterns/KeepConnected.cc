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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/KeepConnected.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.hh"

#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"

using namespace Specification;
using namespace EPSPatterns;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;
using namespace chase::epstool::EPS;

using namespace Utilities;
using namespace Graphs;

// KEEP CONNECTED
std::vector< std::string > KeepConnected::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;
    std::string pattern(" must never be disconnected from ");
    int pattern_length = pattern.length();

    std::size_t _begin = text.find(pattern);
    std::size_t _end = _begin + pattern_length;

    std::string comp1 = text.substr(0, _begin);
    std::string comp2 = text.substr(_end);

    ret.push_back(comp1);
    ret.push_back(comp2);

    return ret;
}

std::vector< WellFormedFormula * > KeepConnected::_createPathFormulae(  
        Contracts::AGContract *,
        std::vector< std::string > strings )
{

    std::vector< WellFormedFormula * > ret;

    int ** matrix = _topology->getInverseMatrix();

    Graph * graph = new Graph(matrix, _topology->getSize(), true); 

    std::string sinks = strings[0];
    std::string sources = strings[1];

    std::vector< int > * sinkNodes = _getNodesByType( sinks, _topology );
    std::vector< int > * sourceNodes = _getNodesByType( sources, _topology );

    // Find all the needed paths.
    for( size_t i = 0; i < sinkNodes->size(); ++i )
    {
        std::list< std::vector<int> > * paths = 
            new std::list< std::vector< int > >();
        for( size_t j = 0; j < sourceNodes->size(); ++j )
        {
            std::vector< int > visited;
            visited.push_back(sinkNodes->at(i));

            std::list< std::vector<int> > * current =
                new std::list< std::vector<int> >();

                Graph::findAllPathsBetweenNodes(
                        graph,
                        visited,
                        sourceNodes->at(j),
                        current);

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
        // position i.

        std::list< std::vector<int> >::iterator lit = paths->begin();
        std::vector<int> * v = &(*lit);
        
        WellFormedFormula * i_paths = static_cast< WellFormedFormula * >(
                pathDoesExist( v, _topology, false ));

        for( ++lit; lit != paths->end(); ++lit )
        {
            v = &(*lit);

            i_paths = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_lor,
                        i_paths,
                        pathDoesExist( v, _topology, false )));
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

        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination(
                    op_lnot,
                    i_paths));

        wff = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_implies,
                    wff,
                    new UnaryTemporalFormula(
                        op_next,
                        n_paths)));

        ret.push_back(wff);
    }


    delete(matrix);
    delete(graph);

    return ret;

}

void KeepConnected::_createConstraints(
        Contracts::AGContract * contract,
        std::vector< Behaviors::Logics::WellFormedFormula * > formulae )
{
    WellFormedFormula * guarantees = dynamic_cast< WellFormedFormula * >(
            contract->guarantees );

    if( guarantees == NULL )
    {
        std::cerr << "[ERROR]\n"
            << "The behaviors are not expressed using logics in this contract."
            << std::endl;
        exit(-1);
    }


    for( size_t i = 0; i < formulae.size(); ++i )
    {
        WellFormedFormula * wff = formulae[i];

        UnaryTemporalFormula * always =
            new UnaryTemporalFormula( op_globally, wff );

        guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( 
                    op_land, 
                    static_cast< WellFormedFormula * >(contract->guarantees),
                    always ));

        contract->guarantees = static_cast< Behavior * >( guarantees );

    }

}



