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
 * @file	chase/include/Architecture/NetworkDefinitions.hh
 * @brief
 */



#ifndef NETWORK_DEFINITIONS_HH
#define NETWORK_DEFINITIONS_HH

#include <iostream>
#include <string>
#include <set>
#include <list>
#include <map>

#include <climits>

namespace Architecture
{
    /// @brief Forward declarations of Network related classes.
    class ComponentsGroup;
    class DomainSpecificBusType;
    class DomainSpecificConverterType;
    class DomainSpecificGenericType;
    class DomainSpecificSinkType;
    class DomainSpecificSourceType;
    class DomainSpecificSwitchType;
    class Network;
    class NetworkComponent;
    class NetworkConnection;

    enum component_types
    {
        ct_generic,
        ct_source,
        ct_bus,
        ct_converter,
        ct_sink,
        ct_switch,
        ct_error
    };

    typedef std::string FlowParameter;
    typedef std::string TopologyParameter;


    /// @brief Struct to represent a range of admissible values for a certain
    /// component type. If the minimum value is not applicable, then use 0. 
    /// If the maximum is not applicable, then use the system constant UINT_MAX.
    typedef struct degree_range
    {
        public:
            unsigned  int min;
            unsigned  int max;

            /// @brief Standard constructor, it sets to 0 the minimum (min) and
            /// to UINT_MAX the maximum (max) value.
            degree_range();

            /// @brief Constructor with explicit min and max values.
            /// @param m minimum value to set.
            /// @param M maximum value to set.
            degree_range( unsigned  int m, unsigned  int M );
            ~degree_range();

        protected:
            degree_range( const degree_range & );
            degree_range & operator=( const degree_range & );

    } degree_range_t;



}


#endif /// ARCHITECTURE_DEFINITIONS_HH
