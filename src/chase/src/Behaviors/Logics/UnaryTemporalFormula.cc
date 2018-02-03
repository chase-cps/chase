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
 * @file	chase/src/Behaviors/Logics/UnaryTemporalFormula.cc
 * @brief
 */



#include "Behaviors/Logics/UnaryTemporalFormula.hh"

using namespace Behaviors::Logics;


UnaryTemporalFormula::UnaryTemporalFormula(
        temporal_operators op,
        WellFormedFormula * op1 ) :
    TemporalFormula(op),
    UnaryFormula(op1)
{
}

UnaryTemporalFormula::~UnaryTemporalFormula()
{
}

std::string UnaryTemporalFormula::print()
{
    std::string ret = std::string("");
    switch(_op)
    {
        case op_next:
            ret += "X( ";
            break;
        case op_future:
            ret += "F( ";
            break;
        case op_globally:
            ret += "G( ";
            break;
        default:
            break;
    }
    ret += _op1->print();
    ret += " )";
    return ret;
}

formula_type UnaryTemporalFormula::getType()
{
    return std::max(ft_ltl, _op1->getType());
}

int UnaryTemporalFormula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitUnaryTemporalFormula( * this );
}


