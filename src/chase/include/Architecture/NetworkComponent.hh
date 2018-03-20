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
 * @file	chase/include/Architecture/NetworkComponent.hh
 * @brief
 */



#ifndef ARCHITECTURE_NETWORK_COMPONENT_HH
#define ARCHITECTURE_NETWORK_COMPONENT_HH

#include "Architecture/NetworkDefinitions.hh"
#include "Architecture/DomainSpecificType.hh"
#include "AST/BaseObject.hh"

namespace Architecture
{
    class NetworkComponent : public chase::BaseObject
    {
        protected:

            std::string _name;
            DomainSpecificType * _type;
            std::string _abbreviation;

            std::set< NetworkConnection * > _incomingEdges;
            std::set< NetworkConnection * > _outgoingEdges;



            NetworkComponent( const NetworkComponent & );
            NetworkComponent & operator=( const NetworkComponent & );

        public:

            NetworkComponent( 
                    std::string name, 
                    DomainSpecificType * dst );
            NetworkComponent( 
                    std::string name,
                    DomainSpecificType * dst,
                    std::string abbreviation );

            ~NetworkComponent();

            /// @brief Add an edge to the set of incoming edges of the node
            /// representing the component. It also perform a semantic check 
            /// to see if the insertion of the edge is legal w.r.t. the contract
            /// induced by the component type.
            /// @param edge Pointer to the edge to insert.
            /// @return True if the edge insertion respects the implicit contract
            /// imposed by the type of the component. False otherwise.
            bool addIncomingEdge( NetworkConnection * edge );

            /// @brief Add an edge to the set of outgoing edges of the node
            /// representing the component. It also perform a semantic check 
            /// to see if the insertion of the edge is legal w.r.t. the contract
            /// induced by the component type.
            /// @param edge Pointer to the edge to insert.
            /// @return True if the edge insertion respects the implicit contract
            /// imposed by the type of the component. False otherwise.
            bool addOutgoingEdge( NetworkConnection * edge );

            /// @brief Return the set of incoming edges of the node representing
            /// the component.
            /// @return The set of edges (Network Connections) where the node is
            /// the head of the edge.
            std::set< NetworkConnection * > getIncomingEdges();
            /// @brief Return the set of outgoing edges of the node representing
            /// the component.
            /// @return The set of edges (Network Connections) where the node is
            /// the tail of the edge.
            std::set< NetworkConnection * > getOutgoingEdges();

            DomainSpecificType * getType();
            void setType( DomainSpecificType * dst );

            std::string getName();
            void setName( std::string name );

            std::string getAbbreviation();
            void setAbbreviation( std::string name );
    };
}


#endif // ARCHITECTURE_NETWORK_COMPONENT_HH
