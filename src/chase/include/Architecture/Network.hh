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
 * @file	chase/include/Architecture/Network.hh
 * @brief
 */



#ifndef ARCHITECTURE_NETWORK_HH
#define ARCHITECTURE_NETWORK_HH

#include <map>

#include "Architecture/NetworkComponent.hh"
#include "Architecture/DomainSpecificType.hh"
#include "Architecture/ComponentsGroup.hh"
#include "Architecture/NetworkConnection.hh"

#include "Architecture/DomainSpecificBusType.hh"
#include "Architecture/DomainSpecificConverterType.hh"
#include "Architecture/DomainSpecificGenericType.hh"
#include "Architecture/DomainSpecificSinkType.hh"
#include "Architecture/DomainSpecificSourceType.hh"
#include "Architecture/DomainSpecificSwitchType.hh"


#include "Utilities/Graphs/Graphs.hh"

namespace Architecture
{

    class Network
    {
        protected:
            Network( const Network & );
            Network & operator=( const Network & );


            Utilities::Graphs::AdjMatrix _adjacency_matrix;
            Utilities::Graphs::AdjMatrix _inverse_adjacency_matrix;

        public:
            std::map< NetworkComponent *, unsigned int > componentsMap;
            std::map< unsigned int, NetworkComponent * > indexesMap;

            std::list< DomainSpecificType * > types;
            std::list< ComponentsGroup * > groups; 
            std::list< NetworkComponent * > components;
            std::list< NetworkComponent * > switches;
            std::list< NetworkConnection * > connections;

            std::map< 
                ComponentsGroup *, std::list< NetworkComponent * > > groupsmap;

            Network();
            ~Network();

            Utilities::Graphs::AdjMatrix calculateAdjacencyMatrix();
            Utilities::Graphs::AdjMatrix calculateInverseAdjacencyMatrix();

            Utilities::Graphs::AdjMatrix getAdjacencyMatrix();
            Utilities::Graphs::AdjMatrix getInverseAdjacencyMatrix();



    };
}

#endif // ARCHITECTURE_NETWORK_HH
