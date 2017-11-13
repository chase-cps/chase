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
 * @file	backend/src/Chase2TuLiP/GR1/SafetyFinder.cc
 * @brief
 */



#include "GR1/SafetyFinder.hh"
#include "Behaviors/Logics/Logics.hh"


using namespace chase;
using namespace backend;
using namespace GR1;
using namespace Manipulation;
using namespace Behaviors;
using namespace Behaviors::Logics;

SafetyFinder::SafetyFinder() :
    GuideVisitor(0),
    safety()
{
}

SafetyFinder::~SafetyFinder()
{
}

int SafetyFinder::visitUnaryTemporalFormula(
       Behaviors::Logics::UnaryTemporalFormula & o )
{
    if( o.getOp() == op_globally )
    {
        WellFormedFormula * wff = o.getOp1();
        UnaryTemporalFormula * utf = 
            dynamic_cast< UnaryTemporalFormula * >(wff);
        if( utf != NULL )
        {
            if( utf->getOp() != op_future )
            {
                // Then is a safety property.
                safety.push_back( utf );
            }
        } else {
            safety.push_back( wff );
        }
    }
    return 0;
}


int SafetyFinder::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    if( o.getOp() == op_land )
    {
        return GuideVisitor::visitBinaryLogicCombination(o);
    } 
    else
    {
        return 0;
    }
}

int SafetyFinder::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & )
{
    return 0;
}



