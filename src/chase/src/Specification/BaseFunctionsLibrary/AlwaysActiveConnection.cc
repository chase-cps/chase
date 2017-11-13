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
 * @file	chase/src/Specification/BaseFunctionsLibrary/AlwaysActiveConnection.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/AlwaysActiveConnection.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;


AlwaysActiveConnection::AlwaysActiveConnection() :
    RequirementFunction("always-active-connection",2)
{
}

AlwaysActiveConnection::~AlwaysActiveConnection()
{
}

void AlwaysActiveConnection::resolve( Requirement * req, Problem * problem )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    std::string par1 = req->_params[0];
    std::string par2 = req->_params[1];

    std::set< Architecture::NetworkComponent * > sinks;
    std::set< Architecture::NetworkComponent * > sources;

    sinks = Manipulation::findComponents( par1, problem->getNetwork() );
    sources = Manipulation::findComponents( par2, problem->getNetwork() );

    // For each sink, if a source is available: a path must exists.

    Architecture::Network * network = problem->getNetwork();
    Utilities::Graphs::AdjMatrix adjmatrix = 
        network->calculateAdjacencyMatrix();

    Utilities::Graphs::Graph * graph =
        new Utilities::Graphs::Graph(adjmatrix, false);


    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = sinks.begin(); it != sinks.end(); ++it )
    {
        Architecture::NetworkComponent * currcomp = *it;

        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                new LogicConstant( lc_false ));

        int ending_index = (int)
            (*(network->componentsMap.find(currcomp))).second;

        std::set< Architecture::NetworkComponent * >::iterator sourceit;
        for( sourceit = sources.begin();
                sourceit != sources.end();
                ++sourceit )
        {
            Architecture::NetworkComponent * currsource = * sourceit;

            int starting_index = (int)
                (*(network->componentsMap.find(currsource))).second;

            std::list< std::vector< int > > result;
            std::vector< int > visited;

            visited.push_back( starting_index );

            Utilities::Graphs::Graph::findAllPathsBetweenNodes(
                    graph,
                    visited,
                    ending_index,
                    &result );

            // Iterate on the found paths.
            std::list< std::vector< int > >::iterator vit;
            for( vit = result.begin();
                    vit != result.end();
                    ++vit )
            {
                std::vector< int > path = *vit;
                wff = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_lor,
                            wff,
                            pathDoesExist(
                                path,
                                problem )));
            }

        }

        wff = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(op_globally, wff));

        WellFormedFormula * guarantee = dynamic_cast< WellFormedFormula * >(
                contract->guarantees );
        assert( guarantee );
        contract->guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    guarantee,
                    wff ));


    }
}

