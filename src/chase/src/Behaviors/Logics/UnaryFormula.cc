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
 * @file	chase/src/Behaviors/Logics/UnaryFormula.cc
 * @brief
 */



#include "Behaviors/Logics/UnaryFormula.hh"

using namespace Behaviors::Logics;

UnaryFormula::UnaryFormula( WellFormedFormula * o ) :
    _op1(o)
{
}

void UnaryFormula::setOp1( WellFormedFormula * f )
{
    _op1 = f;
}

WellFormedFormula * UnaryFormula::getOp1()
{
    return _op1;
}

int UnaryFormula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitUnaryFormula(* this);
}

