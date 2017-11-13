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
 * @file	chase/src/Behaviors/Logics/BinaryFormula.cc
 * @brief
 */



#include "Behaviors/Logics/BinaryFormula.hh"

using namespace Behaviors::Logics;

BinaryFormula::BinaryFormula(
        WellFormedFormula * op1,
        WellFormedFormula * op2 ) :
    _op1(op1),
    _op2(op2)
{
}

void BinaryFormula::setOp1( WellFormedFormula * f )
{
    _op1 = f;
}

void BinaryFormula::setOp2( WellFormedFormula * f )
{
    _op2 = f;
}

WellFormedFormula * BinaryFormula::getOp1()
{
    return _op1;
}

WellFormedFormula * BinaryFormula::getOp2()
{
    return _op2;
}

int BinaryFormula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitBinaryFormula(* this);
}

