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
 * @file	chase/src/Manipulation/SimplifyVisitor.cc
 * @brief
 */



#include "Manipulation/SimplifyVisitor.hh"
#include <iostream>


using namespace Manipulation;
using namespace Behaviors;
using namespace Logics;

SimplifyVisitor::SimplifyVisitor() :
    GuideVisitor(0)
{
}

SimplifyVisitor::~SimplifyVisitor()
{
}

int SimplifyVisitor::visitBinaryFormula(
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

int SimplifyVisitor::visitUnaryFormula(
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

int SimplifyVisitor::visitBooleanFormula(
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

int SimplifyVisitor::visitTemporalFormula(
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

int SimplifyVisitor::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    int rv = o.getOp1()->accept_visitor(*this);
    return rv;
}

int SimplifyVisitor::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    int rv = o.getOp1()->accept_visitor(*this);
    o.setOp2( _verifyDoubleNot(o.getOp2() ));
    rv |=  o.getOp2()->accept_visitor(*this); 

    return rv;
}

int SimplifyVisitor::visitBinaryTemporalFormula(
        Behaviors::Logics::BinaryTemporalFormula & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    int rv = o.getOp1()->accept_visitor(*this);
    o.setOp2( _verifyDoubleNot(o.getOp2() ));
    rv |=  o.getOp2()->accept_visitor(*this);

    return rv;
}

int SimplifyVisitor::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    int rv = o.getOp1()->accept_visitor(*this);

    return rv;
}

Behaviors::Logics::WellFormedFormula * 
    SimplifyVisitor::_verifyDoubleNot( WellFormedFormula * wff )
{
    UnaryLogicCombination * ulc = dynamic_cast< UnaryLogicCombination * >(wff);
    if( ulc != NULL )
    {
        if( ulc->getOp() == op_lnot )
        {
            UnaryLogicCombination * nulc = 
                dynamic_cast< UnaryLogicCombination * >(ulc->getOp1());
            if( nulc != NULL )
            {
                if( nulc->getOp() == op_lnot )
                {
                    return nulc->getOp1();
                }
            }
        }
    }
    return wff;
}

