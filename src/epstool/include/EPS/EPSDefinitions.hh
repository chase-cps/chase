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
 * @file	chase/include/Architecture/EPS/EPSDefinitions.hh
 * @brief
 */



#ifndef ARCHITECTURE_EPS_DEFS_HH
#define ARCHITECTURE_EPS_DEFS_HH

#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>

#include "Behaviors/Logics/Logics.hh"
#include "Behaviors/Timing/Timing.hh"
#include "Architecture/Network.hh"

namespace chase { namespace epstool {
    /**
     * @brief Collection of definition of datatypes for the EPS architecture.
     * @todo transform structs in classes (only the component: done for
     * contactors)?
     */

    namespace EPS
    {

        /// @brief Enumeration of the different type of components
        // that can be used in a EPS.
        enum componentType
        {
            generator_t,
            left_generator_t,
            right_generator_t,
            auxiliary_generator_t,
            ac_bus_t,
            left_ac_bus_t,
            right_ac_bus_t,
            rectifier_t,
            left_rectifier_t,
            right_rectifier_t,
            dc_bus_t,
            left_dc_bus_t,
            right_dc_bus_t,
            load_t,
            left_load_t,
            right_load_t,
            COMPONENT_TYPE_SIZE
        };

        /// @brief Datatype representing a component. A component
        // is a couple name/type. Where the name is the "name" of
        // the component in the instance of a topology. The type
        // is a value of the enumeration defined above. 
        typedef struct _component
        {
            std::string name;
            componentType type;
            Behaviors::Logics::LogicProposition * stateVariable;
        } EPSComponent;

        /// @brief Since the contactor is not a "special" kind
        // of component, that is represented by a edge in the
        // topology graph. Thus, it can be seen as a connection
        // between two EPSComponent.
        // A Contactor is represented by an integer, and two
        // pointers to EPSComponents (nodes of the graph).
        // Furthermore, a contactor can be uni- or bi-directional,
        // depending on the types of the nodes connected. So far,
        // only bus-to-bus connections are bi-directional. If the
        // Contactor is uni-directional, node_1 is the source and
        // node_2 is the target of the connection.
        class Contactor
        {
            public:
                unsigned int number;
                EPSComponent * node_1;
                EPSComponent * node_2;
                Behaviors::Logics::LogicProposition * stateVariable;

                Contactor( 
                        unsigned int n = 0,
                        EPSComponent * n1 = NULL,
                        EPSComponent * n2 = NULL,
                        Behaviors::Logics::LogicProposition * sv = NULL );

                virtual ~Contactor();

            protected:
                Contactor( const Contactor & );
                Contactor & operator=( const Contactor & );

        };


        /// @brief implementation of the physically accurate contactor.
        // It introduces a double variable to indicate the intention and the
        // status. Furthermore, it introduces the use of the counters to model
        // the latency required to open and close the contactor. 
        class PhysicalContactor : public Contactor
        {
            public:
                Behaviors::Logics::LogicProposition * intentionVariable;
                Behaviors::Timing::Counter * openingCounter;
                Behaviors::Timing::Counter * closingCounter;

                unsigned int openingTime;
                unsigned int closingTime;

                PhysicalContactor(
                        unsigned int n = 0,
                        EPSComponent * n1 = NULL,
                        EPSComponent * n2 = NULL,
                        Behaviors::Logics::LogicProposition * sv = NULL,
                        Behaviors::Logics::LogicProposition * iv = NULL,
                        Behaviors::Timing::Counter * oc = NULL,
                        Behaviors::Timing::Counter * cc = NULL,
                        unsigned int ot = 1,
                        unsigned int ct = 1 );

                ~PhysicalContactor();

            protected:
                PhysicalContactor( const PhysicalContactor & );
                PhysicalContactor & operator=( const PhysicalContactor & );

        };

        typedef std::set<EPS::componentType> typeSet;
        typedef struct
        {
            std::string name_radix;
            typeSet subtypes;
        } typeStruct;

        typedef std::pair< std::string, typeStruct > typePair;

        static std::map< std::string, typeStruct > typeMap = 
        {
            {
                std::string("generators"),
                {
                    std::string("generator"),
                    {
                        EPS::generator_t,
                        EPS::left_generator_t, 
                        EPS::right_generator_t, 
                        EPS::auxiliary_generator_t
                    }
                }
            },            
            {
                std::string("left_generators"),
                {
                    std::string("left_generator"),
                    {
                        EPS::left_generator_t
                    }
                }
            },
            {
                std::string("right_generators"),
                {
                    std::string("right_generator"),
                    {
                        EPS::right_generator_t
                    }
                }
            },
            {
                std::string("auxiliary_generators"),
                {
                    std::string("auxiliary_generator"),
                    {
                        EPS::auxiliary_generator_t
                    }
                }
            },
            {
                std::string("ac_buses"),
                {
                    std::string("ac_bus"),
                    {
                        EPS::ac_bus_t,
                        EPS::left_ac_bus_t, 
                        EPS::right_ac_bus_t
                    }
                }
            },
            {
                std::string("left_ac_buses"),
                {
                    std::string("left_ac_bus"),
                    {
                        EPS::left_ac_bus_t
                    }
                }
            },
            {
                std::string("right_ac_buses"),
                {
                    std::string("right_ac_bus"),
                    {
                        EPS::right_ac_bus_t
                    }
                }
            },
            {
                std::string("rectifiers"),
                {
                    std::string("rectifier"),
                    {
                        EPS::rectifier_t,
                        EPS::left_rectifier_t, 
                        EPS::right_rectifier_t
                    }
                }
            },
            {
                std::string("left_rectifiers"),
                {
                    std::string("left_rectifier"),
                    {
                        EPS::left_rectifier_t
                    }
                }
            },
            {
                std::string("right_rectifiers"),
                {
                    std::string("right_rectifier"),
                    {
                        EPS::right_rectifier_t
                    }
                }
            },

            {
                std::string("dc_buses"),
                {
                    std::string("dc_bus"),
                    {
                        EPS::left_dc_bus_t, 
                        EPS::right_dc_bus_t
                    }
                }
            },
            {
                std::string("left_dc_buses"),
                {
                    std::string("left_dc_bus"),
                    {
                        EPS::left_dc_bus_t
                    }
                }
            },
            {
                std::string("right_dc_buses"),
                {
                    std::string("right_dc_bus"),
                    {
                        EPS::right_dc_bus_t
                    }
                }
            },

            {
                std::string("loads"),
                {
                    std::string("load"),
                    {
                        EPS::load_t,
                        EPS::left_load_t, 
                        EPS::right_load_t
                    }
                }
            },
            {
                std::string("left_loads"),
                {
                    std::string("left_load"),
                    {
                        EPS::left_load_t
                    }
                }
            },
            {
                std::string("right_loads"),
                {
                    std::string("right_load"),
                    {
                        EPS::right_load_t
                    }
                }
            }
        };
    }
}}


#endif // ARCHITECTURE_EPS_DEFS_HH
