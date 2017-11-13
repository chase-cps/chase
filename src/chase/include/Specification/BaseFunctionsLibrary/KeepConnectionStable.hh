/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4th October 2017
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Specification/BaseFunctionsLibrary/KeepConnectionStable.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_KEEP_CONNECTION_STABLE_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_KEEP_CONNECTION_STABLE_HH

#include "Specification/RequirementFunction.hh"
#include "Manipulation/findComponents.hh"
#include "Specification/GraphBasedFunctions.hh"

namespace Specification
{
    class KeepConnectionStable : public RequirementFunction
    {
        protected:
            
            KeepConnectionStable( const KeepConnectionStable & );
            KeepConnectionStable & operator=(const KeepConnectionStable &);

            Behaviors::Logics::WellFormedFormula * _createConstraintForPath(
                    Problem * problem, 
                    std::vector< int > path );

        public:

            KeepConnectionStable();
            ~KeepConnectionStable();

            void resolve( Requirement * req, Problem * problem );
    };
}


#endif // SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_KEEP_CONNECTION_STABLE_HH
