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
 * @file	chase/include/Architecture/ComponentsGroup.hh
 * @brief   File containing the description of the ComponentsGroup class.
 */



#ifndef ARCHITECTURE_COMPONENT_GROUP_HH
#define ARCHITECTURE_COMPONENT_GROUP_HH

#include "Architecture/NetworkDefinitions.hh"
#include "Architecture/DomainSpecificType.hh"

#include "AST/BaseObject.hh"

namespace Architecture
{
    /// @brief Represents a group of components through their attributes and parameters.
    //
    class ComponentsGroup : public chase::BaseObject
    {
        protected:
            DomainSpecificType * _type;

            ComponentsGroup(const ComponentsGroup & );
            ComponentsGroup & operator=(const ComponentsGroup & );

        public:

            std::list< TopologyParameter > parameters;

            ComponentsGroup(DomainSpecificType * type);
            ComponentsGroup(
                    DomainSpecificType * type,
                    std::list< TopologyParameter > parameters );
            ~ComponentsGroup();

            DomainSpecificType * getDomainSpecificType();
            bool equals( ComponentsGroup & o);
    }; 
}

#endif // ARCHITECTURE_COMPONENT_GROUP_HH
