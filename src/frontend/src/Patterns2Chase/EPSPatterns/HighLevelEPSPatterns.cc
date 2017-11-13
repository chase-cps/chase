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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.hh"

#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"
#include <algorithm>

using namespace Specification;
using namespace EPSPatterns;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;
using namespace chase::epstool::EPS;

using namespace Utilities;
using namespace Graphs;

void HighLevelEPSPattern::setTopology( EPSTopology * topology )
{
    _topology = topology;
}

std::vector< WellFormedFormula * > HighLevelEPSPattern::createPathFormulae(
        Contracts::AGContract * contract,
        std::vector< std::string > strings )
{
    return(this->_createPathFormulae( contract, strings ));
}

std::vector< int > * HighLevelEPSPattern::_getNodesByType(
        std::string type,
        EPSTopology * topology )
{
    std::vector< int > * ret = new std::vector< int >();
    std::map< std::string, typeStruct >::iterator map_it;
    map_it = typeMap.find(type);
    typeStruct tstruct = (*map_it).second;
    typeSet * subtypes = & tstruct.subtypes;

    for( size_t index = 0 ; index < topology->components.size() ; ++index )
    {
        EPSComponent * current = topology->components[index];
        componentType compType = current->type;

        typeSet::iterator set_it = subtypes->find(compType);
        if( set_it != subtypes->end() )
        {
            ret->push_back(index);
        }

    }
    return ret;
}


WellFormedFormula * HighLevelEPSPattern::pathDoesNotExist(
        std::vector< int > * nodes,
        EPSTopology * topology,
        bool withNodes,
        bool intention )
{
    int ** matrix = topology->getMatrix();
    int path_length = nodes->size();

    // Create the list of the contactors on the path.
    std::vector< int > contactors;
    int prev_node = nodes->at(0);
    int curr_node;
    for( int i = 1; i < path_length; ++i )
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
        LogicProposition * var;
        if( intention )
        {
            PhysicalContactor * c = dynamic_cast< PhysicalContactor * >(
                    topology->contactors[contactors[0]]);
            if( c != NULL )
                var = c->intentionVariable;
            else
                var = topology->contactors[contactors[0]]->stateVariable;
        }
        else
            var = topology->contactors[contactors[0]]->stateVariable;

        to_return = static_cast< WellFormedFormula * >( 
                new UnaryLogicCombination(op_lnot, var ));

        for( size_t i = 1; i < contactors.size(); ++i )
        {
            if( intention )
            {
                PhysicalContactor * c = dynamic_cast< PhysicalContactor * >(
                        topology->contactors[contactors[i]]);
                if( c != NULL )
                    var = c->intentionVariable;
                else
                    var = topology->contactors[contactors[i]]->stateVariable;
            }
            else
                var = topology->contactors[contactors[i]]->stateVariable;


            if( var != NULL )
            {
                UnaryLogicCombination * nf = new UnaryLogicCombination(
                        op_lnot,
                        var );
                to_return = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination( op_lor, to_return, nf ));
            }

        }
    }

    if( withNodes )
    {
        for( size_t i = 0; i < nodes->size(); ++i )
        {
            LogicProposition * var =
                topology->components[nodes->at(i)]->stateVariable;
            if( var != NULL )
            {
                UnaryLogicCombination * nf = new UnaryLogicCombination(
                        op_lnot,
                        var );
                to_return = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(op_lor, to_return, nf ));
            }
        }
    }

    return to_return;
}

WellFormedFormula * HighLevelEPSPattern::pathDoesExist(
        std::vector< int > * nodes,
        EPSTopology * topology,
        bool withNodes,
        bool intention )
{
    int ** matrix = topology->getMatrix();
    int path_length = nodes->size();

    // Create the list of the contactors on the path.


    std::vector< int > contactors;
    int prev_node = nodes->at(0);
    int curr_node;
    for( int i = 1; i < path_length; ++i )
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
        LogicProposition * var;        
        if( intention )
        {
            PhysicalContactor * c = dynamic_cast< PhysicalContactor * >(
                    topology->contactors[contactors[0]]);
            if( c != NULL )
                var = c->intentionVariable;
            else
                var = topology->contactors[contactors[0]]->stateVariable;
        }
        else
            var = topology->contactors[contactors[0]]->stateVariable;


        to_return = static_cast< WellFormedFormula * >( var );

        for( size_t i = 1; i < contactors.size(); ++i )
        {
            if( intention )
            {
                PhysicalContactor * c = dynamic_cast< PhysicalContactor * >(
                        topology->contactors[contactors[i]]);
                if( c != NULL )
                    var = c->intentionVariable;
                else
                    var = topology->contactors[contactors[i]]->stateVariable;
            }
            else
                var = topology->contactors[contactors[i]]->stateVariable;

            if( var != NULL )
            {

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

    return NULL;
}


Behaviors::Logics::WellFormedFormula * HighLevelEPSPattern::_disconnectNode(
        int node, chase::epstool::EPS::EPSTopology * topology )
{
    std::set< int > contactors;
    int size = topology->getSize();
    int ** matrix = topology->getMatrix();

    for( int i = 0; i < size; ++i )
    {
        if( matrix[node][i] > 1 )
            contactors.insert(matrix[node][i] - 2);
        if( matrix[i][node] > 1 )
            contactors.insert(matrix[i][node] - 2);
    }

    EPSComponent * component = topology->components[node];

    WellFormedFormula * toRet;
    //    WellFormedFormula * connection;
    if( component->stateVariable != NULL )
    {
        toRet = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination(op_lnot, component->stateVariable));

        //        connection = static_cast< WellFormedFormula * >( component->stateVariable );

    }

    if( contactors.size() > 0 )
    {

        std::set< int >::iterator cit = contactors.begin();
        int index = *cit;
        WellFormedFormula * implicated = static_cast< WellFormedFormula * >(
                new UnaryLogicCombination( 
                    op_lnot,
                    topology->contactors[index]->stateVariable));

        /*        WellFormedFormula * keepConnected = static_cast< WellFormedFormula * >(
                  topology->contactors[index]->stateVariable);*/

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

            /*            keepConnected = static_cast< WellFormedFormula * >(
                          new BinaryLogicCombination(
                          op_land,
                          keepConnected,
                          topology->contactors[index]->stateVariable));*/
        }

        /*        connection = static_cast< WellFormedFormula * >(
                  new BinaryLogicCombination(
                  op_implies,
                  connection,
                  new UnaryTemporalFormula(
                  op_next,
                  keepConnected )));*/



        toRet = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_implies,
                    toRet,
                    new UnaryTemporalFormula(
                        op_next,
                        implicated )));

        /*        toRet = static_cast< WellFormedFormula * >(
                  new BinaryLogicCombination(
                  op_land,
                  toRet,
                  connection));*/




        return toRet;


    } else {
        std::cerr << "[Warning]\n"
            << "No contactors available. It is not possible to isolate the component"
            << std::endl;
        return static_cast<WellFormedFormula *>(new LogicConstant( lc_false ));
    }


}






void HighLevelEPSPattern::getSubsetBySize(
        std::vector< int > nodes,
        int size,
        int left,
        int index,
        std::vector< int > &combination,
        std::list< std::vector< int > > &results )
{
    if( left == 0 )
    {
        results.push_back(combination);
        return;
    }
    for( int i = index; i < size; ++i )
    {
        combination.push_back(nodes[i]);
        getSubsetBySize(nodes, size, left-1, i+1, combination, results);
        combination.pop_back();
    }
}

std::vector< WellFormedFormula * > HighLevelEPSPattern::_waitDisconnection(
        int node,
        EPSTopology * topology )
{
    std::vector< WellFormedFormula * > toRet;

    std::set< int > contactors;
    int size = topology->getSize();
    int ** matrix = topology->getMatrix();

    for( int i = 0; i < size; ++i )
    {
        if( matrix[node][i] > 1 )
            contactors.insert(matrix[node][i] - 2);
        if( matrix[i][node] > 1 )
            contactors.insert(matrix[i][node] - 2);
    }


    // Create constant part of the formula.
    EPSComponent * component = topology->components[node];

    // Create the exiting star.
    BinaryLogicCombination * radix;

    UnaryLogicCombination * nocomp = new UnaryLogicCombination(
            op_lnot,
            component->stateVariable );

    std::set< int >::iterator sit = contactors.begin();
    WellFormedFormula * yescont = static_cast< WellFormedFormula * >(
            topology->contactors[*sit]->stateVariable);

    for( ++sit; sit != contactors.end(); ++sit )
    {
        int k = *sit;
        yescont = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_lor,
                    yescont,
                    topology->contactors[k]->stateVariable));
    }

    radix = new BinaryLogicCombination(op_land, nocomp, yescont);

    // Find all the contactors not in the star.
    for( size_t i = 0; i < topology->contactors.size(); ++i )
    {
        std::set< int >::iterator found = contactors.find(i);
        if( found == contactors.end() )
        {
            LogicProposition * prop = topology->contactors[i]->stateVariable;    

            UnaryLogicCombination * notcont = new UnaryLogicCombination(
                    op_lnot, prop );

            UnaryTemporalFormula * nextnotcont = new UnaryTemporalFormula(
                    op_next, notcont );

            BinaryLogicCombination * ant = new BinaryLogicCombination(
                    op_land,
                    radix,
                    notcont );

            WellFormedFormula * block = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination(
                        op_implies,
                        ant,
                        nextnotcont ));

            toRet.push_back(block);


        }
    }
    return toRet;



}
