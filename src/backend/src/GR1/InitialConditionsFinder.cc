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
 * @file	backend/src/Chase2TuLiP/GR1/InitialConditionsFinder.cc
 * @brief
 */



#include "GR1/InitialConditionsFinder.hh"

#include <iostream>

using namespace chase;
using namespace backend;
using namespace GR1;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Manipulation;

InitialConditionsFinder::InitialConditionsFinder() :
    GuideVisitor(0),
    _negated(false)
{
}

InitialConditionsFinder::~InitialConditionsFinder()
{
}

// I can stop the visit whenever I am in a temporal formula: initial conditions
// are always steady state.
int InitialConditionsFinder::visitTemporalFormula( TemporalFormula & )
{
    return 0;
}

int InitialConditionsFinder::visitUnaryTemporalFormula( 
        UnaryTemporalFormula & )
{
    return 0;
}

int InitialConditionsFinder::visitBinaryTemporalFormula(
        BinaryTemporalFormula & )
{
    return 0;
}

int InitialConditionsFinder::visitUnaryLogicCombination( 
        UnaryLogicCombination & o )
{
    logic_operators op = o.getOp();
    bool local_negated = _negated;
    if( op == op_lnot )
        _negated = true;
    GuideVisitor::visitUnaryLogicCombination(o);
    _negated = local_negated;
    return 0;
}

int InitialConditionsFinder::visitLogicProposition( LogicProposition & o )
{
    if( ! _negated )
    {
        initial_conditions.push_back(&o);
    } else 
    {
        UnaryLogicCombination * lc = new UnaryLogicCombination( op_lnot, &o );
        initial_conditions.push_back(lc);
    }
    return 0;
}
