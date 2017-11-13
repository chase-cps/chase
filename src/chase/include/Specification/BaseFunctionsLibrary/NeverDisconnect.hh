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
 * @file	chase/include/Specification/BaseFunctionsLibrary/NeverDisconnect.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_NEVER_DISCONNECT_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_NEVER_DISCONNECT_HH

#include "Specification/RequirementFunction.hh"
#include "Manipulation/findComponents.hh"
#include "Specification/GraphBasedFunctions.hh"


namespace Specification
{
    class NeverDisconnect : public RequirementFunction
    {
        protected:
            
            NeverDisconnect( const NeverDisconnect & );
            NeverDisconnect & operator=(const NeverDisconnect &);

            void _untimedResolve( Requirement * req, Problem * problem );
            void _timedResolve( 
                    Requirement * req, 
                    Problem * problem,
                    std::list< Behaviors::Timing::chase_time > * timing );

        public:

            NeverDisconnect();
            ~NeverDisconnect();

            void resolve( Requirement * req, Problem * problem );
            std::list< Behaviors::Timing::chase_time > * 
                getTiming( Requirement * req ); 

    };
}


#endif // SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_NEVER_DISCONNECT_HH
