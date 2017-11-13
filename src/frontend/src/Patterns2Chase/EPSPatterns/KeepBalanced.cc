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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/KeepBalanced.cc
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


// KEEP BALANCED
std::vector< std::string > KeepBalanced::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;

    std::string pattern(" should be powered by ");
    int pattern_length = pattern.length();

    std::size_t _begin = text.find(pattern);
    std::size_t _end = _begin + pattern_length;

    std::string comp1 = text.substr(0, _begin);
    std::string comp2 = text.substr(_end);

    ret.push_back(comp1);
    ret.push_back(comp2);


    return ret;
}


std::vector< WellFormedFormula * > KeepBalanced::_createPathFormulae(  
        Contracts::AGContract *,
        std::vector< std::string > strings )
{
    std::vector< WellFormedFormula * > toRet;
    WellFormedFormula * ret;

    int ** matrix = _topology->getInverseMatrix();

    Graph * graph = new Graph(matrix, _topology->getSize(), true);


    std::string sinks = strings[0];
    std::string sources = strings[1];

    std::vector< int > * sinkNodes = _getNodesByType( sinks, _topology );
    std::vector< int > * sourceNodes = _getNodesByType( sources, _topology );

    // Attention: Neutral element of the disjunction here!
    WellFormedFormula * condition = static_cast< WellFormedFormula * >(
            new LogicConstant(lc_false));
    WellFormedFormula * implicated = static_cast< WellFormedFormula * >(
            new LogicConstant(lc_false));


    std::vector< int >::iterator svit;
    for( svit = sourceNodes->begin();
            svit != sourceNodes->end();
            ++svit )
    {
        int curNodeIndex = *svit;
        chase::epstool::EPS::EPSComponent * component = 
            _topology->components[curNodeIndex];

        LogicProposition * curNodeProp = component->stateVariable;

        condition = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_lor,
                    condition,
                    curNodeProp ));

        std::list< std::vector< int > > * paths = 
            new std::list< std::vector<int> >();

        for( size_t i = 0; i < sinkNodes->size(); ++i )
        {
            std::vector< int > visited;
            visited.push_back(sinkNodes->at(i));

            std::list< std::vector<int> > * current = 
                new std::list< std::vector<int> >();

            Graph::findAllPathsBetweenNodes(
                    graph,
                    visited,
                    curNodeIndex,
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
        
        std::list< std::vector<int> >::iterator lit;
        for( lit = paths->begin(); 
                lit != paths->end();
                ++lit )
        {
            std::vector<int> * v = &(*lit);
            implicated = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_lor,
                        implicated,
                        pathDoesExist(v, _topology, false))); 
        }

    }

    WellFormedFormula * next = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_next,
                implicated));

    ret = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_implies,
                    condition,
                    next ));


    toRet.push_back(ret);

    delete(graph);
    delete(matrix);

    return toRet;

}


void KeepBalanced::_createConstraints(
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

        UnaryTemporalFormula * always =
            new UnaryTemporalFormula( op_globally, formulae[i] );


        guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( 
                    op_land, 
                    static_cast< WellFormedFormula * >(contract->guarantees), 
                    always ));

        contract->guarantees = static_cast< Behavior * >( guarantees );
    }
}

