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
 * @file	chase/src/Behaviors/Constant.cc
 * @brief
 */



#include "Behaviors/Constant.hh"

using namespace Behaviors;

Constant::Constant( std::string name, vartype type, double value ) :
    _name(name),
    _type(type),
    _value(value)
{
}

Constant::~Constant()
{
}

std::string Constant::getName()
{
    return _name;
}

vartype Constant::getType()
{
    return _type;
}

double Constant::getValue()
{
    return _value;
}

int Constant::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitConstant( * this );
}

