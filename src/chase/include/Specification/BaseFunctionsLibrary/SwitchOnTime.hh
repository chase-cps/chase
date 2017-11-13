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
 * @file	chase/include/Specification/BaseFunctionsLibrary/SwitchOnTime.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_SWITCH_ON_TIME_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_SWITCH_ON_TIME_HH

#include "Specification/RequirementFunction.hh"
#include "Manipulation/findComponents.hh"
#include "Behaviors/Timing/Timing.hh"

namespace Specification
{
    class SwitchOnTime : public RequirementFunction
    {
        protected:
            
            SwitchOnTime( const SwitchOnTime & );
            SwitchOnTime & operator=(const SwitchOnTime &);

        public:

            SwitchOnTime();
            ~SwitchOnTime();

            void resolve( Requirement * req, Problem * problem );
            std::list< Behaviors::Timing::chase_time > * 
                getTiming( Requirement * req ); 

    };
}


#endif // SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_SWITCH_ON_TIME_HH
