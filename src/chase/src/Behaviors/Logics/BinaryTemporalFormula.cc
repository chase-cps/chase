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
 * @file	chase/src/Behaviors/Logics/BinaryTemporalFormula.cc
 * @brief
 */



#include "Behaviors/Logics/BinaryTemporalFormula.hh"

using namespace Behaviors::Logics;

BinaryTemporalFormula::BinaryTemporalFormula(
        temporal_operators op,
        WellFormedFormula * op1,
        WellFormedFormula * op2 ) :
    TemporalFormula(op),
    BinaryFormula(op1, op2)
{
}

BinaryTemporalFormula::~BinaryTemporalFormula()
{
}

std::string BinaryTemporalFormula::print()
{
    std::string ret = std::string("( ");
    ret += _op1->print();
    switch(_op)
    {
        case op_until:
            ret += " U ";
            break;
        case op_release:
            ret += " R ";
            break;
        default:
            break;
    }
    ret += _op2->print();
    ret += " )";
    return ret;
}

int BinaryTemporalFormula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitBinaryTemporalFormula( * this );
}


