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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/TimedEPSPatterns.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/TimedEPSPatterns.hh"

#include <sstream>

using namespace chase;

using namespace Specification;
using namespace EPSPatterns;

using namespace Behaviors;
using namespace Logics;
using namespace Timing;

using namespace Architecture;
using namespace epstool;
using namespace EPS;



TimedEPSPattern::TimedEPSPattern( std::regex _regex ) :
    Specification::EPSPatterns::HighLevelEPSPattern( _regex )
{
}

chase_time TimedEPSPattern::_getTimeFromString( std::string s )
{
    chase_time toReturn;

    std::string n("");
    bool started = false;
    for(size_t i = 0; i < s.size(); ++i )
    {
        if( s[i] >= '0' && s[i] <= '9' )
        {
            started = true;
            n += s[i];
        }
        else
        {
            if( started )
            {
                break;
            }
        }
    }

    toReturn.amount = atoi(n.c_str());

    if( s.find("sec") != std::string::npos )
    {
        toReturn.unit = CHASE_SEC;
    }
    else if( s.find("ms") != std::string::npos )
    {
        toReturn.unit = CHASE_MS;
    }
    else if( s.find("us") != std::string::npos )
    {
        toReturn.unit = CHASE_US;
    }
    else if( s.find("ns") != std::string::npos )
    {
        toReturn.unit = CHASE_NS;
    }
    else if( s.find("ps") != std::string::npos )
    {
        toReturn.unit = CHASE_PS;
    }
    else if( s.find("fs") != std::string::npos )
    {
        toReturn.unit = CHASE_FS;
    }


    return toReturn;

}

WellFormedFormula * TimedEPSPattern::_commandDisconnection(
        int node,
        EPSTopology * topology )
{
    std::set< int > contactors;
    size_t size = topology->getSize();
    int ** matrix = topology->getMatrix();

    for( size_t i = 0; i < size; ++i )
    {
        if( matrix[node][i] > 1 )
            contactors.insert(matrix[node][i] - 2);
        if( matrix[i][node] > 1 )
            contactors.insert(matrix[i][node] - 2);
    }

    /// @todo Check why it is unused!
    // EPSComponent * component = topology->components[node];

    WellFormedFormula * toRet;

    if( contactors.size() > 0 )
    {
        std::set< int >::iterator cit = contactors.begin();
        int index = *cit;

        Contactor * cc = topology->contactors[index];
        PhysicalContactor * currContactor = 
            dynamic_cast< PhysicalContactor * >(cc);
        if( currContactor == NULL )
        {
            std::cerr << "ERROR: not a physical contactor (but Ideal)" 
                << std::endl;
            exit(-1);
        }

        toRet = static_cast< WellFormedFormula * >( new UnaryLogicCombination(
                    op_lnot, currContactor->intentionVariable ));

        for( ++cit; cit != contactors.end(); ++cit )
        {
            index = *cit;
            currContactor = dynamic_cast< PhysicalContactor * >(
                    topology->contactors[index]);
            if( currContactor == NULL )
            {
                std::cerr << "ERROR: not a physical contactor (but Ideal)" 
                    << std::endl;
                exit(-1);
            }

            toRet = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination( op_land,
                        toRet,
                        new UnaryLogicCombination( 
                            op_lnot,
                            currContactor->intentionVariable )));
        }

        return toRet;

    }
    return static_cast< WellFormedFormula * >( new LogicConstant(lc_true));
}

Behaviors::Logics::WellFormedFormula * TimedEPSPattern::_commandPower(
        std::vector<int> * nodes, EPSTopology * topology, bool withNodes )
{
    int ** matrix = topology->getMatrix();
    size_t path_length = nodes->size();

    // Create the list of the contactors on the path.


    std::vector< int > contactors;
    int prev_node = nodes->at(0);
    int curr_node;
    for( size_t i = 1; i < path_length; ++i )
    {
        curr_node = nodes->at(i);

        if( matrix[prev_node][curr_node] > 1 
                || matrix[curr_node][prev_node] > 1  )
        {
            contactors.push_back( std::max(
                        matrix[prev_node][curr_node],
                        matrix[curr_node][prev_node]) - 2 );
        }
        prev_node = curr_node;
    }


    if ( contactors.size() <= 0 )
    {
        std::cout << "[Warning]\n\tIt is not possible to disactivate a path."
            << "\nOnly solid connections in the path." << std::endl;
        return new LogicConstant( lc_false );
    }

    WellFormedFormula * to_return = NULL;
    if( contactors.size() > 0 )
    {
        PhysicalContactor * currCon = dynamic_cast< PhysicalContactor * >(
                topology->contactors[contactors[0]]);
        if( currCon == NULL )
        {
            std::cerr << "[ERROR] not a physical contactor (ideal)"
                << std::endl;
            exit(-1);
        }
        LogicProposition * var = currCon->intentionVariable;
        to_return = static_cast< WellFormedFormula * >( var );

        for( size_t i = 1; i < contactors.size(); ++i )
        {
            if( var != NULL )
            {
                currCon = dynamic_cast< PhysicalContactor * >(
                        topology->contactors[contactors[0]]);
                if( currCon == NULL )
                {
                    std::cerr << "[ERROR] not a physical contactor (ideal)"
                        << std::endl;
                    exit(-1);
                }
                var = currCon->intentionVariable;
                to_return = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination( op_land, to_return, var ));
            }

        }
    }

    // In case 
    if( withNodes )
    {
        for( size_t i = 0; i < nodes->size(); ++i )
        {
            LogicProposition * var =
                topology->components[nodes->at(i)]->stateVariable;
            if( var != NULL )
            {
                to_return = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination( op_land, to_return, var ));
            }
        }
    }

    return to_return;


}

void TimedEPSPattern::createTimedConstraints(
        Contracts::AGContract * contract,
        std::vector< std::string > string )
{
    this->_createTimedConstraints(contract, string);
}


Behaviors::Logics::WellFormedFormula * TimedEPSPattern::_nodeDisconnected(
        int node, chase::epstool::EPS::EPSTopology * topology )
{
    std::set< int > contactors;
    size_t size = topology->getSize();
    int ** matrix = topology->getMatrix();

    for( size_t i = 0; i < size; ++i )
    {
        if( matrix[node][i] > 1 )
            contactors.insert(matrix[node][i] - 2);
        if( matrix[i][node] > 1 )
            contactors.insert(matrix[i][node] - 2);
    }

    if( contactors.size() > 0 )
    {

        std::set< int >::iterator cit = contactors.begin();
        int index = *cit;
        WellFormedFormula * implicated = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination( 
                    op_lnot,
                    topology->contactors[index]->stateVariable));

        for( ++cit; cit != contactors.end(); ++cit )
        {
            index = *cit;
            implicated = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_land,
                        implicated,
                        new UnaryLogicCombination(
                            op_lnot,
                            topology->contactors[index]->stateVariable)));
        }
        return implicated;
    }

    return( new LogicConstant(lc_true));
} 


