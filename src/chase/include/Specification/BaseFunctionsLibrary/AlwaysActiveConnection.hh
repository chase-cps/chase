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
 * @file	chase/include/Specification/BaseFunctionsLibrary/AlwaysActiveConnection.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_ALWAYS_ACTIVE_CONNECTION_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_ALWAYS_ACTIVE_CONNECTION_HH

#include "Specification/RequirementFunction.hh"
#include "Manipulation/findComponents.hh"
#include "Specification/GraphBasedFunctions.hh"


namespace Specification
{
    class AlwaysActiveConnection : public RequirementFunction
    {
        protected:
            
            AlwaysActiveConnection( const AlwaysActiveConnection & );
            AlwaysActiveConnection & operator=(const AlwaysActiveConnection &);

        public:

            AlwaysActiveConnection();
            ~AlwaysActiveConnection();

            void resolve( Requirement * req, Problem * problem );
    };
}


#endif // SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_ALWAYS_ACTIVE_CONNECTION_HH
