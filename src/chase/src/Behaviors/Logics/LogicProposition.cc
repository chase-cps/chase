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
 * @file	chase/src/Behaviors/Logics/LogicProposition.cc
 * @brief
 */



#include "Behaviors/Logics/LogicProposition.hh"

using namespace Behaviors::Logics;

LogicProposition::LogicProposition() :
    Variable("anonimous", t_bool){
}

LogicProposition::LogicProposition( std::string name ) :
    Variable(name, t_bool)
{
}

LogicProposition::~LogicProposition()
{
}

std::string LogicProposition::print()
{
    return _name;
}

int LogicProposition::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitLogicProposition(* this);
}

