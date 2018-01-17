/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        January 17, 2018
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Specification/BaseFunctionsLibrary/MaxFailures.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_MAX_FAILURES_PER_STEP_HH
#define SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_MAX_FAILURES_PER_STEP_HH

#include "Specification/RequirementFunction.hh"
#include "Manipulation/findComponents.hh"

#include "Behaviors/Behaviors.hh"

namespace Specification
{
    class MaxFailuresPerStep : public RequirementFunction
    {
        protected:
            
            MaxFailuresPerStep( const MaxFailuresPerStep & );
            MaxFailuresPerStep & operator=(const MaxFailuresPerStep &);

            void getSubsetsBySize(
                    std::vector< Architecture::NetworkComponent * > nodes,
                    int size,
                    int left,
                    int index,
                    std::vector< 
                    Architecture::NetworkComponent * >& combination,
                    std::list< 
                    std::vector< Architecture::NetworkComponent * > > &results);


            Behaviors::Logics::WellFormedFormula * _getCombinationFormula(
                    std::set< Architecture::NetworkComponent * >& chosen,
                    std::set< Architecture::NetworkComponent * >& nodes,
                    Problem * problem );



        public:

            MaxFailuresPerStep();
            ~MaxFailuresPerStep();

            void resolve( Requirement * req, Problem * problem );
    };
}


#endif //SPECIFICATION_REQUIREMENT_FUNCTION_BASE_LIBRARY_MAX_FAILURES_PER_STEP_HH
