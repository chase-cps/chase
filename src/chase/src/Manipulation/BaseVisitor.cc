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
 * @file	chase/src/Manipulation/BaseVisitor.cc
 * @brief
 */



#include "Manipulation/BaseVisitor.hh"

#include "Behaviors/Behaviors.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Contracts/Contracts.hh"


using namespace Manipulation;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Contracts;

BaseVisitor::BaseVisitor()
{
}

BaseVisitor::~BaseVisitor()
{
}

int BaseVisitor::visitLogicAtom( 
        Behaviors::Logics::LogicAtom &)
{
    return 0;
}

int BaseVisitor::visitLogicConstant( 
        Behaviors::Logics::LogicConstant &)
{
    return 0;
}

int BaseVisitor::visitLogicProposition( 
        Behaviors::Logics::LogicProposition &)
{
    return 0;
}

int BaseVisitor::visitBinaryFormula(
        Behaviors::Logics::BinaryFormula &)
{
    return 0;
}

int BaseVisitor::visitUnaryFormula(
        Behaviors::Logics::UnaryFormula &)
{
    return 0;
}

int BaseVisitor::visitBooleanFormula(
        Behaviors::Logics::BooleanFormula &)
{
    return 0;
}

int BaseVisitor::visitTemporalFormula(
        Behaviors::Logics::TemporalFormula &)
{
    return 0;
}



int BaseVisitor::visitBinaryTemporalFormula( 
        Behaviors::Logics::BinaryTemporalFormula &)
{
    return 0;
}

int BaseVisitor::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula &)
{
    return 0;
}

int BaseVisitor::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination &)
{
    return 0;
}

int BaseVisitor::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination &)
{
    return 0;
}

int BaseVisitor::visitGR1Formula( 
        Behaviors::Logics::GR1Formula &)
{
    return 0;
}

int BaseVisitor::visitLogicPredicate(
        Behaviors::Logics::LogicPredicate &)
{
    return 0;
}

int BaseVisitor::visitVariable( 
        Behaviors::Variable &)
{
    return 0;
}

int BaseVisitor::visitConstant( 
        Behaviors::Constant &)
{
    return 0;
}

int BaseVisitor::visitFunction( 
        Behaviors::Function &)
{
    return 0;
}


int BaseVisitor::visitAGContract( 
        Contracts::AGContract &)
{
    return 0;
}

