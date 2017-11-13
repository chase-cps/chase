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
 * @file	chase/src/Utilities/Graphs/Graphs.cc
 * @brief
 */



#include "Utilities/Graphs/Graphs.hh"

using namespace Utilities;
using namespace Graphs;

Link::Link( int source, int target ) :
    sourceNode(source),
    targetNode(target)
{
}

void Link::GetSourceTargetNodes( int & source, int & target )
{
    source = sourceNode;
    target = targetNode;
}


Graph::Graph() :
    link_map(),
    _directed(false)
{
}

Graph::Graph( int ** matrix, int size, bool undirected) :
    link_map(),
    _directed(undirected)
{
    for(int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size; ++j)
        {
            if(matrix[i][j] > 0)
            {
                AddLink(i,j);
            }
        }
    }
}

Graph::Graph(AdjMatrix matrix, bool undirected) :
    link_map(),
    _directed(undirected)
{
    unsigned int size = matrix.size();
    for(unsigned int i = 0; i < size; ++i)
    {
        for(unsigned int j = 0; j < size; ++j)
        {
            if(matrix[i][j] > 0)
            {
                AddLink(i,j);
            }
        }

    }
}


Graph::~Graph()
{
}

void Graph::Reset()
{
}

void Graph::setDirected( bool bi )
{
    _directed = bi;
}

std::string Graph::createGraphViz( AdjMatrix matrix, std::string name )
{

    unsigned int size = matrix.size();
    std::string ret = "graph " + name + "{\n";

    for( unsigned int i = 0; i < size; ++i )
    {
        for( unsigned int j = 0; j < size; ++j )
        {
            if( matrix[i][j] > 0 )
            {
                ret += "\t" + std::to_string(i) + " -- " + 
                    std::to_string(j) + ";\n";
            }
        }
    }
    ret += "}";
    return ret;
}

void Graph::AddLink( Link * l )
{
    int source, target;
    l->GetSourceTargetNodes( source, target );
    if( LinkExists( target, source ) == false || _directed == false )
    {
        std::pair< std::pair<int,int>, Link * > p(
                std::pair<int,int>(source,target), l
                );

        link_map.insert(p);
    }
}

void Graph::AddLink(int s, int d)
{
    Link * l = new Link(s,d);
    AddLink(l);
}

bool Graph::isDirected()
{
    return _directed;
}

bool Graph::LinkExists( int sourceID, int targetID )
{
    std::map< std::pair<int, int>, Link *>::iterator it;
    bool found = false;
    for ( it = link_map.begin(); it != link_map.end(); ++it )
    {
        std::pair< std::pair< int, int >, Link * > curr = *it;
        std::pair< int, int > p = curr.first;
        if( p.first == sourceID && p.second == targetID )
        {
            found = true;
            break;
        }
    }
    return found;
}

std::vector< int > Graph::GetAdjNodeIDs( int n )
{
    std::vector<int> adjList;
    std::map< std::pair<int,int>, Link *>::iterator it;
    for( it = link_map.begin();
            it != link_map.end();
            ++it )
    {
        std::pair< std::pair<int,int>, Link * > p = *it;
        std::pair<int,int> c = p.first;
        if(c.first == n )
        {
            adjList.push_back(c.second);
        }
        if( _directed && c.second == n )
        {
            adjList.push_back(c.first);
        }
    }
    return adjList;
}

void Graph::findAllPathsBetweenNodes(
        Graph * graph,
        std::vector< int >& visited,
        int end,
        std::list< std::vector< int > > * result )
{
    int back = visited.back();

    std::vector< int > adjNode = graph->GetAdjNodeIDs(back);

    for( std::vector< int >::iterator node_it = adjNode.begin();
            node_it != adjNode.end();
            ++node_it )
    {
        int node = (*node_it);
        if( ContainsNode( visited, node )) 
        {
            continue;
        }

        if( node == end )
        {
            visited.push_back( *node_it );
            int hops = (int) visited.size();

            // Save the found path into the list of the results.
            std::vector< int > found_path;
            for( int i = 0; i < hops; ++i )
            {
                found_path.push_back(visited[i]);
            }
            result->push_back(found_path);

            int n = (int) visited.size() - 1;
            visited.erase( visited.begin() + n );

            break;
        }
    }

    for( std::vector< int >::iterator node_it = adjNode.begin();
            node_it != adjNode.end();
            ++node_it )
    {
        int node = (*node_it);
        if( ContainsNode( visited, node ) || node == end )
            continue;

        visited.push_back(node);
        Graph::findAllPathsBetweenNodes( 
                graph, visited, end, result );

        int n = (int) visited.size() - 1;
        visited.erase( visited.begin() + n );
    }

}


bool Graph::ContainsNode(
        std::vector< int >& visited,
        int node )
{
    bool found = false;
    std::vector< int >::iterator it;
    for( it = visited.begin();
            it != visited.end();
            ++it )
    {
        int n = *it;
        if( n == node )
        {
            found = true;
            return found;
        }
    }

    return found;
}

