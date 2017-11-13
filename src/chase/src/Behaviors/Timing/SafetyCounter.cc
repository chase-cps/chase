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
 * @file	chase/src/Behaviors/Timing/SafetyCounter.cc
 * @brief
 */



#include "Behaviors/Timing/SafetyCounter.hh"

using namespace Behaviors;
using namespace Timing;

SafetyCounter::SafetyCounter() :
    Counter(),
    _command(NULL)
{
}

SafetyCounter::SafetyCounter( std::string name ) :
    Counter(name),
    _command(NULL)
{
}

SafetyCounter::~SafetyCounter()
{
}
                                                             
void SafetyCounter::setUnsafeCondition(
        Behaviors::Logics::WellFormedFormula * c )
{
   _setTrigger(c);
}

Behaviors::Logics::WellFormedFormula * SafetyCounter::getUnsafeCondition()
{
    return _getTrigger();
}
                                                             
void SafetyCounter::setSafetyCondition(
        Behaviors::Logics::WellFormedFormula * c )
{
    _setReset(c);
}

Behaviors::Logics::WellFormedFormula * SafetyCounter::getSafetyCondition()
{
    return _getReset();
}
                                                             
void SafetyCounter::setUnsafeTolerance( chase_time t )
{
    _setTimeOut(t);
}

chase_time SafetyCounter::getUnsafeTolerance()
{
    return _getTimeOut();
}

void SafetyCounter::setCommand( Behaviors::Logics::WellFormedFormula * c )
{
    _command = c;
}

Behaviors::Logics::WellFormedFormula * SafetyCounter::getCommand()
{
    return _command;
}

std::string SafetyCounter::print()
{
    std::string ret = Counter::print();
    ret += "\nCOMMAND:\n";
    ret += _command->print();
    return ret;
}
