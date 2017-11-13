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
 * @file	chase/include/Specification/RequirementFunction.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_HH

#include "Specification/Problem.hh"
#include "Behaviors/Timing/Timing.hh"

namespace Specification
{
    class RequirementFunction
    {
        protected:
            const std::string _name;
            const unsigned int _ariety;

            RequirementFunction( const RequirementFunction & );
            RequirementFunction & operator=( const RequirementFunction & );

            RequirementFunction(
                    std::string name,
                    unsigned int ariety );

        public:

            RequirementFunction();
            ~RequirementFunction();

            std::string getName();
            unsigned int getAriety();

            ///
            /// @brief The function retrieves all the timing specifications
            /// specified within the pattern. The function should be overriden
            /// by the specific RequirementFunction if and only if the
            /// function uses timing specifications
            /// @return NULL if no timing is specified. The list of timing
            /// specification in CHASE time format otherwise.
            std::list< Behaviors::Timing::chase_time > * 
                getTiming( Requirement * req ); 

            virtual void resolve( Requirement *, Problem * ) = 0;



    };
}


#endif // SPECIFICATION_REQUIREMENT_FUNCTION_HH
