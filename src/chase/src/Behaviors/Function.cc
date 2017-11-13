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
 * @file	chase/src/Behaviors/Function.cc
 * @brief
 */



#include "Behaviors/Function.hh"

using namespace Behaviors;

Function::Function(
        std::string name,
        vartype type,
        unsigned int arity,
        Behaviors::Logics::Term ** args ) :
    arguments(args),
    _name(name),
    _type(type),
    _arity(arity)
{
}

Function::~Function()
{
}

Behaviors::Logics::Term * Function::getArgument(unsigned int position)
{
    if( position < _arity )
        return arguments[position];
    else
        return NULL;
}

void Function::setArgument(
        unsigned int position,
        Behaviors::Logics::Term * term )
{
    if( position < _arity )
    {
        arguments[position] = term;
    }
}
                                                              
std::string Function::getName()
{
    return _name;
}

vartype Function::getType()
{
    return _type;
}

unsigned int Function::getArity()
{
    return _arity;
}

int Function::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitFunction( * this );
}
