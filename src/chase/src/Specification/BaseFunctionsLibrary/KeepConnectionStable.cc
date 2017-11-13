/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4th October 2017
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/src/Specification/BaseFunctionsLibrary/KeepConnectionStable.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/KeepConnectionStable.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Behaviors::Timing;

KeepConnectionStable::KeepConnectionStable() :
    RequirementFunction("keep-connection-stable",2)
{
}

KeepConnectionStable::~KeepConnectionStable()
{
}

void KeepConnectionStable::resolve( Requirement * req, Problem * problem )
{
    std::string source = req->getParameter(0);
    std::string sink = req->getParameter(1);

    Architecture::Network * network = problem->getNetwork();

    std::set< Architecture::NetworkComponent * > sources =
        Manipulation::findComponents( source, network );
    std::set< Architecture::NetworkComponent * > sinks =
        Manipulation::findComponents( sink, network );

    Utilities::Graphs::AdjMatrix adjmatrix =
        network->calculateAdjacencyMatrix();
    Utilities::Graphs::Graph * graph =
        new Utilities::Graphs::Graph(adjmatrix, false);

    // Find all the possible live paths from sources to sinks.
    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = sinks.begin(); it != sinks.end(); ++it )
    {
        //For each sink, find all the paths incoming from sources.
        Architecture::NetworkComponent * currcomp = *it;

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

            std::list< std::vector<int> > result;
            std::vector< int > visited;
            visited.push_back(starting_index);

            Utilities::Graphs::Graph::findAllPathsBetweenNodes(
                    graph,
                    visited,
                    ending_index,
                    &result );

            std::list< std::vector< int > >::iterator vit;
            for( vit = result.begin(); vit != result.end(); ++vit )
            {
                WellFormedFormula * f = _createConstraintForPath(
                        problem, *vit );
                Contracts::AGContract * contract = problem->getContract();

                WellFormedFormula * g = static_cast< WellFormedFormula * >(
                        contract->guarantees);

                contract->guarantees = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            g,
                            f)
                        );
            }
        }
    }
}





Behaviors::Logics::WellFormedFormula * 
KeepConnectionStable::_createConstraintForPath(
        Problem * problem, 
        std::vector< int > path )
{
    WellFormedFormula * livepath = pathDoesExist( path, problem );
    WellFormedFormula * activatepath = activatePath( path, problem );

    WellFormedFormula * f = static_cast< WellFormedFormula * >(
            new UnaryTemporalFormula(
                op_globally,
                new BinaryLogicCombination(
                    op_implies,
                    livepath,
                    activatepath )
                )
            );

    return f;
}

