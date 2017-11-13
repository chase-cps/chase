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
 * @file	chase/src/Behaviors/Timing/Counter.cc
 * @brief
 */



#include "Behaviors/Timing/Counter.hh"

using namespace Behaviors;
using namespace Timing;

Counter::Counter() :
    _name(""),
    _trigger(NULL),
    _time_out(1, CHASE_SEC),
    _variable(NULL)
{
}

Counter::Counter( std::string name ) :
    _name(name),
    _trigger(NULL),
    _reset(NULL),
    _time_out(1, CHASE_SEC),
    _variable( new Variable( name, t_int ))
{
}

Counter::~Counter()
{
}

void Counter::setName( std::string n )
{
    _name = n;
}

std::string Counter::getName()
{
    return _name;
}

void Counter::setVariable( Behaviors::Variable * v )
{
    _variable = v;
}

Behaviors::Variable * Counter::getVariable()
{
    return _variable;
}


void Counter::_setTrigger( Behaviors::Logics::WellFormedFormula * t )
{
    _trigger = t;
}

void Counter::_setReset( Behaviors::Logics::WellFormedFormula * r )
{
    _reset = r;
}

void Counter::_setTimeOut( chase_time t )
{
    _time_out = t;
}

Behaviors::Logics::WellFormedFormula * Counter::_getTrigger()
{
    return _trigger;
}

Behaviors::Logics::WellFormedFormula * Counter::_getReset()
{
    return _reset;
}

chase_time Counter::_getTimeOut()
{
    return _time_out;
}

std::string Counter::print()
{
    std::string ret("\n");
    ret += "PERIOD:\t";
    std::stringstream ss;
    ss << _time_out.amount;
    ret += ss.str();
    ret += " ";
    switch( _time_out.unit )
    {
        case CHASE_SEC:
            ret += "sec\n";
            break;
        case CHASE_MS:
            ret += "ms\n";
            break;
        case CHASE_US:
            ret += "us\n";
            break;
        case CHASE_NS:
            ret += "ns\n";
            break;
        case CHASE_PS:
            ret += "ps\n";
            break;
        case CHASE_FS:
            ret += "fs\n";
            break;
        default:
            break;
    }
    ret += "TRIGGER:\n";
    ret += _trigger->print();
    ret += "\nRESET:\n";
    ret += _reset->print();
    return ret;
}


