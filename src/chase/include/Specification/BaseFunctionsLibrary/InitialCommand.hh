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
 * @file	chase/include/Specification/BaseFunctionsLibrary/InitialCommand.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_INITIAL_COMMAND_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_INITIAL_COMMAND_HH

#include "Specification/RequirementFunction.hh"
#include "Manipulation/findComponents.hh"

namespace Specification
{
    class InitialCommand : public RequirementFunction
    {
        protected:
            
            InitialCommand( const InitialCommand & );
            InitialCommand & operator=(const InitialCommand &);

        public:

            InitialCommand();
            ~InitialCommand();

            void resolve( Requirement * req, Problem * problem );
    };
}


#endif // SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_INITIAL_COMMAND_HH
