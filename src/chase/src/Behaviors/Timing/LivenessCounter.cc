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
 * @file	chase/src/Behaviors/Timing/LivenessCounter.cc
 * @brief
 */



#include "Behaviors/Timing/LivenessCounter.hh"

using namespace Behaviors;
using namespace Timing;

LivenessCounter::LivenessCounter() :
    Counter()
{
}

LivenessCounter::LivenessCounter( std::string name ) :
    Counter(name)
{
}

LivenessCounter::~LivenessCounter()
{
}
                                                                 
void LivenessCounter::setActivationCondition(
        Behaviors::Logics::WellFormedFormula * c )
{
    _setTrigger(c);
}

Behaviors::Logics::WellFormedFormula * LivenessCounter::getActivationCondition()
{
    return _getTrigger();
}
                                                                 
void LivenessCounter::setDesiredCondition(
        Behaviors::Logics::WellFormedFormula * c )
{
    _setReset(c);
}

Behaviors::Logics::WellFormedFormula * LivenessCounter::getDesiredCondition()
{
    return _getReset();
}
                                                                 
void LivenessCounter::setMaxTimeRequired( chase_time t )
{
    _setTimeOut(t);
}

chase_time LivenessCounter::getMaxTimeRequired()
{
    return _getTimeOut();
}


