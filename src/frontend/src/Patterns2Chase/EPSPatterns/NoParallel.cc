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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/NoParallel.cc
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

/////////////////////////////////////////////////////////////////

// NO PARALLEL

std::vector< std::string > NoParallel::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;
    std::string pattern(" are never connected in parallel through ");
    std::size_t _begin = text.find(pattern);
    std::size_t _end = _begin + pattern.length();

    std::string components = text.substr(0,_begin);

    // The boundary is the last component to use as a boundary
    // for the subpart of the system that can create a parallel
    // connection.
    // At the moment is the last component to consider.
    /// \todo Extend to specify the type of components that "breaks"
    /// the flow (e.g., Rectifiers wrt AC power).
    std::string boundary = text.substr(_end);

    ret.push_back(components);
    ret.push_back(boundary);

    return ret;
}

std::vector< WellFormedFormula * > NoParallel::_createPathFormulae( 
        Contracts::AGContract *,
        std::vector< std::string > strings )
{
    std::string components(strings[0]);
    std::string boundary(strings[1]);

    // Find the nodes of the components to use.
    std::vector< int > * compList = _getNodesByType( components, _topology );
    std::vector< int > * boundaryList = _getNodesByType( boundary, _topology );

    std::vector< int > * nodes = new std::vector< int >();

    // the vector nodes is using all the nodes to use.
    for( size_t k = 0; k < compList->size(); ++k )
    {
        nodes->push_back(compList->at(k));
    }

    for( size_t k = 0; k < boundaryList->size(); ++k )
    {
        nodes->push_back(boundaryList->at(k));
    }

    // create the adjacency matrix of the sub-graph to consider.
    size_t size = nodes->size();
    int ** new_matrix = new int*[size];
    for( size_t i = 0; i < size; ++i )
    {
        new_matrix[i] = new int[size];
    }

    int ** old_matrix = _topology->getMatrix();

    std::map< int, int > index_map;

    for( size_t i = 0; i < nodes->size(); ++i )
    {
        std::pair< int, int > p(i, nodes->at(i));
        index_map.insert(p);
        for( size_t j = 0; j < nodes->size(); ++j )
        {
            new_matrix[i][j] = 
                old_matrix[nodes->at(i)][nodes->at(j)];
        }
    }

    Graph * graph = new Graph(new_matrix, size, true);

    std::list< std::vector< int > > * paths = 
        new std::list< std::vector< int > >();

    for( size_t z = 0; z < compList->size(); ++z )
    {
        for( size_t w = z; w < compList->size(); ++w )
        {
            std::vector<int> visited;
            visited.push_back(z);

            std::list< std::vector<int> > * cpath =
                new std::list< std::vector< int > >();

            Graph::findAllPathsBetweenNodes(
                    graph,
                    visited,
                    w,
                    cpath);

            std::list< std::vector<int> >::iterator it;
            for( it = cpath->begin();
                    it != cpath->end();
                    ++it )
            {
                std::vector<int> cv = *it;
                paths->push_back(cv);
            }


        }
    }

    std::list< std::vector<int> >::iterator lit = paths->begin();

    std::vector<int> v = *lit;


    WellFormedFormula * conj = static_cast< WellFormedFormula * >(
            HighLevelEPSPattern::pathDoesNotExist(&v, _topology, false, false));

    for( ++lit; lit != paths->end(); ++lit )
    {
        v = (*lit);

        WellFormedFormula * disj = static_cast< WellFormedFormula * >(
                pathDoesNotExist(&v, _topology, false, false));

        conj = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( op_land, conj, disj ));
    }

    std::vector< WellFormedFormula * > ret;
    ret.push_back(conj);

    delete(compList);
    delete(boundaryList);
    delete(nodes);
    delete(graph);
    delete(paths);

    return ret;

}




void NoParallel::_createConstraints(
        Contracts::AGContract * contract,
        std::vector< Behaviors::Logics::WellFormedFormula * > formulae )
{
   
    WellFormedFormula * toAdd = formulae[0];

    WellFormedFormula * guarantees = dynamic_cast< WellFormedFormula * >(
            contract->guarantees);

    if( guarantees == NULL )
    {
        std::cerr << "[ERROR]\n"
            << "The behaviors are not expressed using logics in this contract."
            << std::endl;
        exit(-1);
    }

    toAdd = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(op_globally, toAdd));

    guarantees = static_cast< WellFormedFormula * >(
            new BinaryLogicCombination(
                op_land, 
                static_cast< WellFormedFormula * >(contract->guarantees), 
                toAdd ));

    contract->guarantees = static_cast< Behavior * >( guarantees );
}

//////////////////////////////////////////////////

