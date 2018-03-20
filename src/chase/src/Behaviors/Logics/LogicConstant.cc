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
 * @file	chase/src/Behaviors/Logics/LogicConstant.cc
 * @brief
 */



#include "Behaviors/Logics/LogicConstant.hh"

using namespace Behaviors::Logics;

LogicConstant::LogicConstant() :
    LogicAtom(),
    _value(lc_false)
{
}

LogicConstant::LogicConstant( bool value ) :
    LogicAtom(),
    _value(lc_false)
{
    if( value ) _value = lc_true;
}

LogicConstant::LogicConstant( logic_constants value ) :
    LogicAtom(),
    _value(value)
{
}

LogicConstant::~LogicConstant()
{
}

std::string LogicConstant::print()
{
    switch( _value )
    {
        case lc_false:
            return std::string("FALSE");
            break;
        case lc_true:
            return std::string("TRUE");
            break;
        default:
            break;
    }
    return std::string("UNVALID_CONSTANT");
}

logic_constants LogicConstant::getValue()
{
    return _value;
}

int LogicConstant::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitLogicConstant(* this);
} 

chase::BaseObject * LogicConstant::copy()
{
    LogicConstant * c = new LogicConstant( _value );
    return c;
}

