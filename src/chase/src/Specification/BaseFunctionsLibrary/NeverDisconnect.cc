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
 * @file	chase/src/Specification/BaseFunctionsLibrary/NeverDisconnect.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/NeverDisconnect.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Behaviors::Timing;


NeverDisconnect::NeverDisconnect() :
    RequirementFunction("never-disconnect",4)
{
}

NeverDisconnect::~NeverDisconnect()
{
}

void NeverDisconnect::resolve( Requirement * req, Problem * problem )
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

void NeverDisconnect::_timedResolve(
        Requirement * req,
        Problem * problem,
        std::list< Behaviors::Timing::chase_time > * timing )
{
    // Contract to manipulate.
    Contracts::AGContract * contract = problem->getContract();

    std::string par1 = req->getParameter(0);
    std::string par2 = req->getParameter(1);

    Timing::chase_time tolerance = timing->front();

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
        WellFormedFormula * pre =  static_cast< WellFormedFormula * >(
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

                pre = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_lor,
                            pre,
                            pathDoesExist(
                                path,
                                problem)));
            }

        }

        wff = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination(
                    op_lnot,
                    wff));

        std::string component_name(currcomp->getName());
        std::string counter_name = component_name + "_disconnected_counter";
        for(std::string::iterator it = counter_name.begin(); 
                it != counter_name.end(); ++it) 
        {
            if( *it == ' ' )
            {
                *it = '_';
            }
        }
        SafetyCounter * counter = new SafetyCounter(counter_name);

        counter->setUnsafeCondition(wff);
        counter->setSafetyCondition(pre);
        counter->setCommand(new LogicConstant(lc_true));
        counter->setUnsafeTolerance(tolerance);

        contract->safety_counters.push_back(counter);
    }
}

void NeverDisconnect::_untimedResolve( Requirement * req, Problem * problem )
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
        WellFormedFormula * pre =  static_cast< WellFormedFormula * >(
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

                pre = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_lor,
                            pre,
                            pathDoesExist(
                                path,
                                problem, 
                                false )));

            }

        }

        WellFormedFormula * condition = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination(
                    op_lnot,
                    pre ));

        WellFormedFormula * consequence = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(
                    op_next,
                    wff ));

        WellFormedFormula * complete = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula(
                    op_globally,
                    new BinaryLogicCombination(
                        op_implies,
                        condition,
                        consequence)));

        WellFormedFormula * guarantee = dynamic_cast< WellFormedFormula * >(
                contract->guarantees );
        assert( guarantee );
        contract->guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    guarantee,
                    complete ));

    }
}




    std::list< Behaviors::Timing::chase_time > * 
NeverDisconnect::getTiming( Requirement * req )
{
    if( req->getAriety() == 4 )
    {
        std::list< Behaviors::Timing::chase_time > * ret;
        ret = new std::list< Behaviors::Timing::chase_time >();

        unsigned int par1 = atoi(req->getParameter(2).c_str());
        std::string par2 = req->getParameter(3);

        Timing::chase_time t(par1, par2);
        ret->push_back(t);

        return ret;
    }
    else
    {
        return NULL;
    }
} 



