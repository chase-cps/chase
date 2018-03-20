/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        March 3, 2018
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Manipulation/FixParentsVisitor.hh
 * @brief
 */



#include "Manipulation/FixParentsVisitor.hh"

using namespace Manipulation;
using namespace Behaviors;
using namespace Logics;


FixParentsVisitor::FixParentsVisitor() :
    GuideVisitor(0)
{
}

FixParentsVisitor::~FixParentsVisitor()
{
}

int FixParentsVisitor::visitBinaryFormula(
        Behaviors::Logics::BinaryFormula & o )
{
    int rv = 0;
    BinaryTemporalFormula * btf = dynamic_cast< BinaryTemporalFormula * >(&o);
    if( btf != NULL )
    {
        rv = visitBinaryTemporalFormula(*btf);
        return rv;
    }
    BinaryLogicCombination * blc = dynamic_cast< BinaryLogicCombination * >(&o);
    if( blc != NULL )
    {
        rv = visitBinaryLogicCombination(*blc);
        return rv;
    }
    return rv;
}

int FixParentsVisitor::visitUnaryFormula(
        Behaviors::Logics::UnaryFormula & o )
{
    int rv = 0;
    UnaryTemporalFormula * utf = dynamic_cast< UnaryTemporalFormula * >(&o);
    if( utf != NULL )
    {
        rv = visitUnaryTemporalFormula(*utf);
        return rv;
    }
    UnaryLogicCombination * ulc = dynamic_cast< UnaryLogicCombination * >(&o);
    if( ulc != NULL )
    {
        rv = visitUnaryLogicCombination(*ulc);
        return rv;
    }
    return rv;
}

int FixParentsVisitor::visitBooleanFormula(
        Behaviors::Logics::BooleanFormula & o )
{
    int rv = 0;

    UnaryLogicCombination * ulc = dynamic_cast< UnaryLogicCombination * >(&o);
    if( ulc != NULL )
    {
        rv = visitUnaryLogicCombination(*ulc);
        return rv;
    }
    BinaryLogicCombination * blc = dynamic_cast< BinaryLogicCombination * >(&o);
    if( blc != NULL )
    {
        rv = visitBinaryLogicCombination(*blc);
        return rv;
    }
    return rv;
}

int FixParentsVisitor::visitTemporalFormula(
        Behaviors::Logics::TemporalFormula & o )
{
    int rv = 0;

    BinaryTemporalFormula * btf = dynamic_cast< BinaryTemporalFormula * >(&o);
    if( btf != NULL )
    {
        rv = visitBinaryTemporalFormula(*btf);
        return rv;
    }
    UnaryTemporalFormula * utf = dynamic_cast< UnaryTemporalFormula * >(&o);
    if( utf != NULL )
    {
        rv = visitUnaryTemporalFormula(*utf);
        return rv;
    }
    return rv;
}

int FixParentsVisitor::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & o )
{
    o.getOp1()->setParent(&o);
    int rv = o.getOp1()->accept_visitor(*this);
    return rv;
}

int FixParentsVisitor::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    o.getOp1()->setParent(&o);
    int rv = o.getOp1()->accept_visitor(*this);
    return rv;
}

int FixParentsVisitor::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    o.getOp1()->setParent(&o);
    int rv = o.getOp1()->accept_visitor(*this);
    o.getOp2()->setParent(&o);
    rv |= o.getOp2()->accept_visitor(*this);
    return rv;
}

int FixParentsVisitor::visitBinaryTemporalFormula(
        Behaviors::Logics::BinaryTemporalFormula & o )
{
    o.getOp1()->setParent(&o);
    int rv = o.getOp1()->accept_visitor(*this);
    o.getOp2()->setParent(&o);
    rv |= o.getOp2()->accept_visitor(*this);
    return rv;
}



