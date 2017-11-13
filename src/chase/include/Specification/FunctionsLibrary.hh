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
 * @file	chase/include/Specification/FunctionsLibrary.hh
 * @brief
 */



#ifndef SPECIFICATION_FUNCTIONS_LIBRARY_HH
#define SPECIFICATION_FUNCTIONS_LIBRARY_HH

#include <list>

#include "Specification/RequirementFunction.hh"
#include "Specification/Problem.hh"

namespace Specification
{
    class FunctionsLibrary
    {
        protected:
            std::list< RequirementFunction * > _functions;

            FunctionsLibrary( const FunctionsLibrary & );
            FunctionsLibrary & operator=( const FunctionsLibrary & );

        public:
            FunctionsLibrary();
            ~FunctionsLibrary();

            void resolveRequirement(
                    Requirement * req,
                    Problem * problem );



    };
}

#endif // SPECIFICATION_FUNCTIONS_LIBRARY_HH
