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
 * @file	chase/include/Contracts/Contract.hh
 * @brief
 */



// Contract.hh
//


/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0

#ifndef CONTRACT_HH
#define CONTRACT_HH

#include "Behaviors/Behaviors.hh"
#include "Behaviors/Timing/Timing.hh"
#include "Utilities/Timing/CounterUtilities.hh"

#include "ChaseASTObject.hh"

#include <list>

#include <iostream>

namespace Contracts
{
    // @brief Abstract class to represent the concept of contract (or requirement)
    class Contract : public chase::ChaseASTObject 
    {
        public:

            std::list< Behaviors::Variable * > environment_variables;
            std::list< Behaviors::Variable * > system_variables;

            std::list< Behaviors::Timing::SafetyCounter * > safety_counters;
            std::list< Behaviors::Timing::LivenessCounter * > liveness_counters;

            Behaviors::Timing::chase_time time_precision;
            

            virtual std::string print() = 0;
            virtual void findRequiredTimePrecision();
            virtual void createVariablesFromCounters();
    };
}

#endif // CONTRACT_HH
