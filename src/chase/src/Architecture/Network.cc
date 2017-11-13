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
 * @file	chase/src/Architecture/Network.cc
 * @brief
 */



#include "Architecture/Network.hh"

using namespace Architecture;

Network::Network() :
    _adjacency_matrix(),
    _inverse_adjacency_matrix(),
    componentsMap(),
    indexesMap(),
    types(),
    groups(),
    components(),
    switches(),
    connections()
{
}

Network::~Network()
{
    while(!types.empty()) delete types.front(), types.pop_front();
    while(!groups.empty()) delete groups.front(), groups.pop_front();
    while(!components.empty()) delete components.front(), components.pop_front();
    while(!switches.empty()) delete switches.front(), switches.pop_front();
    while(!connections.empty()) delete connections.front(), connections.pop_front();
}

Utilities::Graphs::AdjMatrix Network::calculateAdjacencyMatrix()
{

    componentsMap.clear();
    _adjacency_matrix.clear();

    // Create the maps relating each component to its index.
    unsigned int index = 0;
    std::list< NetworkComponent * >::iterator it;
    for( it = components.begin(); it != components.end(); ++it )
    {
        Architecture::NetworkComponent * curr = *it;
        std::pair< NetworkComponent *, unsigned int > p(curr, index);
        std::pair< unsigned int, NetworkComponent * > i(index, curr);
        componentsMap.insert(p);
        indexesMap.insert(i);
        ++index;
    }
    for( it = switches.begin(); it != switches.end(); ++it )
    {
        std::pair< NetworkComponent *, unsigned int > p(*it, index);
        std::pair< unsigned int, NetworkComponent * > i(index, *it);
        componentsMap.insert(p);
        indexesMap.insert(i);
        ++index;
    }

    /// Create adjacency matrix.

    unsigned int matrix_size = componentsMap.size();
    for( unsigned int i = 0; i < matrix_size; ++i )
    {
        std::map< unsigned int, bool > inner_map;
        for( unsigned int j = 0; j < matrix_size; ++j )
        {
            std::pair< unsigned int, bool > p(j, false);
            inner_map.insert(p);
        }
        std::pair< 
            unsigned int,
                     std::map< unsigned int, bool > > outer(i, inner_map);
        _adjacency_matrix.insert(outer);
    }

    std::list< NetworkConnection * >::iterator connit;
    for( connit = connections.begin();
            connit != connections.end();
            ++connit )
    {
        NetworkConnection * currconn = *connit;

        NetworkComponent * to = currconn->getTarget();
        NetworkComponent * from = currconn->getSource();
        NetworkComponent * connector = currconn->getSwitch();

        unsigned int to_index = componentsMap.find(to)->second;
        unsigned int from_index = componentsMap.find(from)->second;

        if( connector != NULL )
        {
            unsigned int conn_index = componentsMap.find(connector)->second;
            _adjacency_matrix[from_index][conn_index] = true;
            _adjacency_matrix[conn_index][to_index] = true;
//            if( currconn->getBidirectional() )
//            {
                _adjacency_matrix[to_index][conn_index] = true;
                _adjacency_matrix[conn_index][from_index] = true;
//            }
        }
        else
        {
            _adjacency_matrix[from_index][to_index] = true;
//            if( currconn->getBidirectional() )
                _adjacency_matrix[to_index][from_index] = true;
        }
    }

    return _adjacency_matrix;
}



Utilities::Graphs::AdjMatrix Network::calculateInverseAdjacencyMatrix()
{

    componentsMap.clear();

    // Create the map relating each component to its index.
    unsigned int index = 0;
    std::list< NetworkComponent * >::iterator it;
    for( it = components.begin(); it != components.end(); ++it )
    {
        Architecture::NetworkComponent * curr = *it;
        std::pair< NetworkComponent *, unsigned int > p(curr, index);
        componentsMap.insert(p);
        ++index;
    }
    for( it = switches.begin(); it != switches.end(); ++it )
    {
        std::pair< NetworkComponent *, unsigned int > p(*it, index);
        componentsMap.insert(p);
        ++index;
    }

    /// Create adjacency matrix.

    unsigned int matrix_size = componentsMap.size();
    for( unsigned int i = 0; i < matrix_size; ++i )
    {
        std::map< unsigned int, bool > inner_map;
        for( unsigned int j = 0; j < matrix_size; ++j )
        {
            std::pair< unsigned int, bool > p(j, false);
            inner_map.insert(p);
        }
        std::pair< 
            unsigned int,
                     std::map< unsigned int, bool > > outer(i, inner_map);
        _inverse_adjacency_matrix.insert(outer);
    }





    std::list< NetworkConnection * >::iterator connit;
    for( connit = connections.begin();
            connit != connections.end();
            ++connit )
    {
        NetworkConnection * currconn = *connit;

        NetworkComponent * to = currconn->getTarget();
        NetworkComponent * from = currconn->getSource();
        NetworkComponent * connector = currconn->getSwitch();

        unsigned int to_index = componentsMap.find(to)->second;
        unsigned int from_index = componentsMap.find(from)->second;

        if( connector != NULL )
        {
            unsigned int conn_index = componentsMap.find(connector)->second;
            _inverse_adjacency_matrix[to_index][conn_index] = true;
            _inverse_adjacency_matrix[conn_index][from_index] = true;
//            if( currconn->getBidirectional() )
//            {
                _inverse_adjacency_matrix[from_index][conn_index] = true;
                _inverse_adjacency_matrix[conn_index][to_index] = true;
//            }
        }
        else
        {
            _inverse_adjacency_matrix[to_index][from_index] = true;
//            if( currconn->getBidirectional() )
                _inverse_adjacency_matrix[to_index][from_index] = true;
        }
    }

    return _inverse_adjacency_matrix;
}



Utilities::Graphs::AdjMatrix Network::getAdjacencyMatrix()
{
    if( _adjacency_matrix.empty() == false )
        return _adjacency_matrix;
    else return calculateAdjacencyMatrix();
}

Utilities::Graphs::AdjMatrix Network::getInverseAdjacencyMatrix()
{
    if( _adjacency_matrix.empty() == false )
        return _inverse_adjacency_matrix;
    else return calculateInverseAdjacencyMatrix();
}

