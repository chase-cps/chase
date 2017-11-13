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
 * @file	chase/src/Behaviors/Logics/BooleanFormula.cc
 * @brief
 */



#include "Behaviors/Logics/BooleanFormula.hh"

using namespace Behaviors::Logics;



BooleanFormula::BooleanFormula( logic_operators o ) :
    _op(o)
{
}

BooleanFormula::~BooleanFormula()
{
}

void BooleanFormula::setOp( logic_operators o )
{
    _op = o;
}

logic_operators BooleanFormula::getOp()
{
    return _op;
}

int BooleanFormula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitBooleanFormula( * this );
}


