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
 * @file	chase/src/Contracts/Contract.cc
 * @brief
 */



#include "Contracts/Contract.hh"
#include <cmath>

using namespace Contracts;

void Contract::findRequiredTimePrecision()
{
    std::list< Behaviors::Timing::SafetyCounter * >::iterator sit;
    std::list< Behaviors::Timing::LivenessCounter * >::iterator lit;
    const size_t counters = safety_counters.size() + liveness_counters.size();

    unsigned int * time_constraints;
    time_constraints = new unsigned int[counters];

    Behaviors::Timing::chase_time_unit min_time_unit =  
        Behaviors::Timing::CHASE_SEC;

    // Find the minimum unit measure (that it is actually the maximum)
    for( sit = safety_counters.begin();
            sit != safety_counters.end();
            ++sit )
    {
        Behaviors::Timing::SafetyCounter * curr = *sit;
        Behaviors::Timing::chase_time t = curr->getUnsafeTolerance(); 
        
        if( t.unit > min_time_unit )
            min_time_unit = t.unit;
    }

    for( lit = liveness_counters.begin();
            lit != liveness_counters.end();
            ++lit )
    {
        Behaviors::Timing::LivenessCounter * curr = *lit;
        Behaviors::Timing::chase_time t = curr->getMaxTimeRequired(); 
        
        if( t.unit > min_time_unit )
            min_time_unit = t.unit;
    }


    unsigned int i = 0;

    // Conversion among different time units. After having found the minimum
    // time unit, for every counter: check the difference between the time units
    // w.r.t the enumeration. Then, multiply the amount to 10 power 3*diff.
    // E.g.
    // If the minimum is CHASE_NS (nanoseconds), and a value is expressed in 
    // CHASE_MS (milliseconds):
    // CHASE_NS = 3, CHASE_MS = 1, then diff = 2. If the amount is 5, for 
    // instance:
    // the new amount will be 5 * 10^3*2 CHASE_NS = 5 * 10^6 CHASE_NS, that is
    // 5000000 CHASE_NS.
    //
    for( sit = safety_counters.begin();
            sit != safety_counters.end();
            ++sit )
    {
        Behaviors::Timing::SafetyCounter * curr = *sit;
        Behaviors::Timing::chase_time t = curr->getUnsafeTolerance(); 
        
        unsigned int diff = min_time_unit - t.unit;
        time_constraints[i] = t.amount * pow(10,(3*diff));
        ++i;
    }

    for( lit = liveness_counters.begin();
            lit != liveness_counters.end();
            ++lit )
    {
        Behaviors::Timing::LivenessCounter * curr = *lit;
        Behaviors::Timing::chase_time t = curr->getMaxTimeRequired(); 
        
        unsigned int diff = min_time_unit - t.unit;
        time_constraints[i] = t.amount * pow(10,(3*diff));
        ++i;
    }

    // Find the amount for the precision. The unit will be the minimum.

    unsigned int amount = 
        Utilities::Timing::getGreaterCommonFactor( time_constraints, counters  );

    time_precision.amount = amount;
    time_precision.unit = min_time_unit;

    delete[] time_constraints;

}

void Contract::createVariablesFromCounters()
{
    std::list< Behaviors::Timing::SafetyCounter * >::iterator sit;
    std::list< Behaviors::Timing::LivenessCounter * >::iterator lit;

    for( sit = safety_counters.begin();
            sit != safety_counters.end();
            ++sit )
    {
        Behaviors::Timing::SafetyCounter * curr = *sit;
        Behaviors::Timing::chase_time t = curr->getUnsafeTolerance();

        unsigned int unit_diff = time_precision.unit - t.unit;
        unsigned int abs_value = t.amount * pow(10, (3*unit_diff));

        unsigned int max_value = 
            (abs_value / time_precision.amount) + 1;

        Behaviors::Variable * var = 
            new Behaviors::Variable( 
                    curr->getName(),
                    Behaviors::t_int,
                    0.0,
                    (double) max_value);

        system_variables.push_back(var);
        curr->setVariable(var);
    }

    for( lit = liveness_counters.begin();
            lit != liveness_counters.end();
            ++lit )
    {
        Behaviors::Timing::LivenessCounter * curr = *lit;
        Behaviors::Timing::chase_time t = curr->getMaxTimeRequired();

        unsigned int unit_diff = time_precision.unit - t.unit;
        unsigned int abs_value = t.amount * pow(10, (3*unit_diff));

        unsigned int max_value = 
            (abs_value / time_precision.amount) + 1;

        Behaviors::Variable * var = 
            new Behaviors::Variable( 
                    curr->getName(),
                    Behaviors::t_int,
                    0.0,
                    (double) max_value);

        environment_variables.push_back(var);
        curr->setVariable(var);
    }
}



