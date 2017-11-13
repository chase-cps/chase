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
 * @file	backend/include/Chase2TuLiP/manipulation/ResolveCounters.hh
 * @brief
 */



#ifndef RESOLVE_COUNTERS_HH
#define RESOLVE_COUNTERS_HH

#include "Behaviors/Timing/Timing.hh"
#include "Contracts/Contracts.hh"

#include <sstream>
#include <cmath>

namespace chase
{
    namespace backend
    {

        namespace manipulation
        {
            class ResolveCounters
            {
                public:
                    ResolveCounters(
                            Contracts::AGContract * contract );

                    ~ResolveCounters();

                    void resolve();

                protected:

                    Contracts::AGContract * _contract;

                    ResolveCounters( const ResolveCounters & );
                    ResolveCounters & operator=( const ResolveCounters & );

                    void _resolveLivenessCounter( 
                            Behaviors::Timing::LivenessCounter * counter );

                    void _resolveSafetyCounter(
                            Behaviors::Timing::SafetyCounter * counter );


            };
        }
    }
}


#endif // RESOLVE_COUNTERS_HH
