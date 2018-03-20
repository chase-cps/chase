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
 * @file	chase/include/Architecture/DomainSpecificType.hh
 * @brief
 */



#ifndef ARCHITECTURE_DOMAIN_SPECIFIC_TYPE_HH
#define ARCHITECTURE_DOMAIN_SPECIFIC_TYPE_HH

#include "Architecture/NetworkDefinitions.hh"
#include "AST/BaseObject.hh"

namespace Architecture
{
    ///
    /// @brief A Domain Specific Type (DST) is a component type that is specific
    /// of a particular design domain. E.g., considerin an electrical network,
    /// a DST for the network may be the type Rectifier, that represents a
    /// node that converts a DC flow to an AC flow. The DST is characterized by
    /// a Generic Network Type (GNT), that is a generalization. E.g., considering
    /// the rectifier example, its GNT is "converter": as such, it uses the 
    /// component type ct_converter.
    class DomainSpecificType : public chase::BaseObject
    {
        protected:

            std::string _name;
            component_types _generic_type;
            
            degree_range * _indegree_range;
            degree_range * _outdegree_range;

            unsigned int _flow_params;

            DomainSpecificType( const DomainSpecificType & );
            DomainSpecificType & operator=( const DomainSpecificType & );
            
            FlowParameter * _flow_parameters;

        public:

            DomainSpecificType( std::string name );
            DomainSpecificType( std::string name, component_types type);
            ~DomainSpecificType();

            bool addParameter(unsigned int position, FlowParameter param);

            degree_range * getInDegreeRange();
            degree_range * getOutDegreeRange();

            ///
            /// @brief It sets the generic type of a Domain Specific Type.
            /// It allows to specify the ranges for the in and out degree of the
            /// nodes belonging to the type. However, the setup of the degrees is
            /// considered only when allowed, i.e., whenever the implicit contract
            /// of the generic type does not already fix a constraint.
            /// @param type The generic node type in a network.
            /// @param indegree_min the Minimum value allowed as indegree for the
            /// node type.
            /// @param indegree_max the Maximum value allowed as indegree for the
            /// node type.
            /// @param outdegree_min the Minimum value allowed as outdegree for the
            /// node type.
            /// @param indegree_max the Maximum value allowed as outdegree for the
            /// node type.
            void setGenericType( 
                    component_types type,
                    unsigned int indegree_min = 0,
                    unsigned int indegree_max = UINT_MAX,
                    unsigned int outdegree_min = 0,
                    unsigned int outdegree_max = UINT_MAX );

            ///
            /// @brief Retrieve the generic type that the domain specific type
            /// instantiate.
            /// @return the generic type of the DST.
            component_types getGenericType();

            /// @brief Retrieve the name of the type.
            /// @return The type name.
            std::string getName();
    };
}



#endif // ARCHITECTURE_DOMAIN_SPECIFIC_TYPE_HH
