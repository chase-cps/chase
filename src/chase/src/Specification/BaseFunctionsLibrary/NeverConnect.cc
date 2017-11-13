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
 * @file	chase/src/Specification/BaseFunctionsLibrary/NeverConnect.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/NeverConnect.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

NeverConnect::NeverConnect() :
    RequirementFunction("never-connect",UINT_MAX)
{
}

NeverConnect::~NeverConnect()
{
}

void NeverConnect::resolve( Requirement * req, Problem * problem )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    size_t ariety = req->_params.size();

    // Find the set of components to consider in the problem topology.
    std::set< Architecture::NetworkComponent * > components;


    for( size_t i = 0; i < ariety; ++i )
    {
        std::set< Architecture::NetworkComponent * > currset;
        std::string par = req->_params[i];

        currset = Manipulation::findComponents( par, problem->getNetwork() );
        std::set< Architecture::NetworkComponent * >::iterator it;
        for( it = currset.begin();
                it != currset.end();
                ++it )
        {
            components.insert(*it);
        }
    }

    Architecture::Network * network = getSubNetwork(
            problem->getNetwork(),
            components );

    Utilities::Graphs::AdjMatrix adjmatrix = 
        network->calculateAdjacencyMatrix();

    Utilities::Graphs::Graph * graph = 
        new Utilities::Graphs::Graph(adjmatrix, true);

    std::set< Architecture::NetworkComponent * > froms =
        Manipulation::findComponents( req->_params[0], problem->getNetwork() );

    std::set< Architecture::NetworkComponent * > tos =
        Manipulation::findComponents( req->_params[1], problem->getNetwork() );

    std::set< Architecture::NetworkComponent * >::iterator i;
    std::set< Architecture::NetworkComponent * >::iterator j;
    for( i = froms.begin(); i != froms.end(); ++i )
    {
        int starting_index = (int)
            (*(network->componentsMap.find(*i))).second;

        for( j = tos.begin(); j != tos.end(); ++j )
        {

            int ending_index = (int)
                (*(network->componentsMap.find(*j))).second;

            if( starting_index != ending_index )
            {

                std::list< std::vector< int > > result;
                std::vector< int > visited;

                visited.push_back( starting_index );


                Utilities::Graphs::Graph::findAllPathsBetweenNodes(
                        graph,
                        visited,
                        ending_index,
                        &result );

                std::vector< int > translated;
                std::list< std::vector< int > >::iterator vit;
                for( vit = result.begin(); vit != result.end(); ++vit )
                {
                    std::vector< int > res = *vit;
                    for( size_t k = 0; k < res.size(); ++k )
                    {
                        Architecture::Network * net = problem->getNetwork();
                        Architecture::NetworkComponent * currcomp;


                        currcomp = 
                            (*(network->indexesMap.find(res[k]))).second;

                        int z = (*(net->componentsMap.find(currcomp))).second;
                        translated.push_back(z);
                    }

                    WellFormedFormula * wff = pathDoesNotExist( 
                            translated,
                            problem,
                            false );

                    wff = static_cast< WellFormedFormula * >(
                            new UnaryTemporalFormula(
                                op_globally,
                                wff ));

                    WellFormedFormula * guarantees = 
                        dynamic_cast< WellFormedFormula * >(
                                contract->guarantees );

                    assert(guarantees);

                    contract->guarantees = static_cast< WellFormedFormula * >(
                            new BinaryLogicCombination(
                                op_land,
                                guarantees,
                                wff ));
                }
            } 
        }
    }
}

