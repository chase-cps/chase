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
 * @file	backend/src/Chase2TuLiP/GR1/ProgressFinder.cc
 * @brief
 */



#include "GR1/ProgressFinder.hh"
#include "Behaviors/Logics/Logics.hh"
#include <iostream>

using namespace chase;
using namespace backend;
using namespace GR1;
using namespace Manipulation;
using namespace Behaviors;
using namespace Behaviors::Logics;

ProgressFinder::ProgressFinder() :
    GuideVisitor(0),
    progress()
{
}

ProgressFinder::~ProgressFinder()
{
}

int ProgressFinder::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    if( o.getOp() == op_globally )
    {
        WellFormedFormula * wff = o.getOp1();
        UnaryTemporalFormula * utf = 
            dynamic_cast< UnaryTemporalFormula * >(wff);
        if( utf != NULL )
        {
            if( utf->getOp() == op_future )
            {
                // Then is a progress property
                progress.push_back(utf->getOp1());
            }
        }
    }
    return 0;
}


int ProgressFinder::visitBinaryLogicCombination(
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

int ProgressFinder::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & )
{
    return 0;
}



