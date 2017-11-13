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
 * @file	chase/include/Utilities/Graphs/Graphs.hh
 * @brief
 */



#ifndef GRAPHS_HH
#define GRAPHS_HH

#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <list>

namespace Utilities
{
    namespace Graphs
    {
        typedef std::map< unsigned int,
                std::map< unsigned int, bool > > AdjMatrix;



        class Link
        {
            public:

                Link( int source, int target );
                void GetSourceTargetNodes( int& source, int& target );

            protected:
                int sourceNode;
                int targetNode;
        };


        class Graph
        {
            private:

                std::map< std::pair<int,int>, Link*> link_map;        
                bool _directed;    

            public:

                Graph();
                Graph(int ** matrix, int size, bool directed = false);
                Graph(AdjMatrix matrix, bool directed = false);
                ~Graph();


                void Reset();    
                void setDirected( bool bi );            
                void AddLink( Link* l );
                void AddLink( int s, int d );

                bool LinkExists( int sourceID, int targetID );
                bool isDirected();

                std::vector< int > GetAdjNodeIDs( int n );                 

                static std::string createGraphViz( 
                        AdjMatrix matrix, std::string = std::string("topology"));


                /// @brief This function calculates all the possible paths
                /// between a set of nodes and a node.
                /// @param graph Pointer to the Graph object to analyse.
                /// @param visited nodes of already visited nodes. They are the
                /// starting set of nodes of the search for each call of the
                /// function. The function is recursive, thus, at the first call
                /// it is necessary to store in it the starting node for the 
                /// search.
                /// @param end Final node of the paths to find.
                /// @param result pointer to a list of paths. It contains the 
                /// paths found during the search.
                static void findAllPathsBetweenNodes( 
                        Graph * graph, 
                        std::vector< int >& visited,
                        int end,
                        std::list< std::vector<int> > * result );
                
                static bool ContainsNode( 
                        std::vector< int >& visited, int node );
        };


    }

}





#endif
