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
 * @file	chase/src/Behaviors/Variable.cc
 * @brief
 */



#include "Behaviors/Variable.hh"

using namespace Behaviors;

Variable::Variable( std::string name ) :
    _name(name),
    _vartype(t_bool)
{

}

Variable::Variable( std::string name, vartype type,
        double min, double max ) :
    _name(name),
    _vartype(type),
    _minValue(min),
    _maxValue(max)
{
}

Variable::~Variable()
{
}

std::string Variable::GetName()
{
    return _name;
}

Behaviors::vartype Variable::GetVarType()
{
    return _vartype;
}

int Variable::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitVariable( * this );
}

double Variable::getMaxValue()
{
    return _maxValue;
}

double Variable::getMinValue()
{
    return _minValue;
}

std::string Variable::print()
{
    std::string toRet;
    toRet = GetName();
    toRet += " : ";

    switch(GetVarType())
    {
        case t_bool:
            toRet += "boolean";
            break;
        case t_int:
            toRet += "integer";
            break;
        case t_real:
            toRet += "real";
            break;
        default:
            break;
    }

    toRet += GetVarType();
    return toRet;
}
