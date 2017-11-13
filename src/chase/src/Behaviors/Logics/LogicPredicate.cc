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
 * @file	chase/src/Behaviors/Logics/LogicPredicate.cc
 * @brief
 */



#include "Behaviors/Logics/LogicPredicate.hh"

using namespace Behaviors;
using namespace Logics;

LogicPredicate::LogicPredicate( 
        std::string name,
        unsigned int arity,
        Behaviors::Logics::Term ** args ) :
    arguments(args),
    _arity(arity),
    _name(name)
{
}
                                                     
LogicPredicate::~LogicPredicate()
{
}
                                                     
formula_type LogicPredicate::getType()
{
    return ft_predicative;
}

int LogicPredicate::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitLogicPredicate(* this);
}
                                                     
unsigned int LogicPredicate::getArity()
{
    return _arity;
}

std::string LogicPredicate::getName()
{
    return _name;
}
