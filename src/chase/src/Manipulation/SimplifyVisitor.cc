/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/src/Manipulation/SimplifyVisitor.cc
 * @brief
 */



#include "Manipulation/SimplifyVisitor.hh"
#include "Contracts/AGContract.hh"

#include <iostream>


using namespace Manipulation;
using namespace Behaviors;
using namespace Logics;
using namespace chase;

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
    o.getOp1()->setParent( &o );

    int rv = o.getOp1()->accept_visitor(*this);
    return rv;
}

int SimplifyVisitor::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    o.getOp1()->setParent( &o );

    int rv = o.getOp1()->accept_visitor(*this);

    o.setOp2( _verifyDoubleNot(o.getOp2() ));
    o.getOp2()->setParent( &o );

    rv |=  o.getOp2()->accept_visitor(*this);

    return rv;
}

int SimplifyVisitor::visitBinaryTemporalFormula(
        Behaviors::Logics::BinaryTemporalFormula & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    o.getOp1()->setParent( &o );

    int rv = o.getOp1()->accept_visitor(*this);

    o.setOp2( _verifyDoubleNot(o.getOp2() ));
    o.getOp2()->setParent( &o );

    rv |=  o.getOp2()->accept_visitor(*this);

    return rv;
}

int SimplifyVisitor::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    o.setOp1( _verifyDoubleNot(o.getOp1() ));
    o.getOp1()->setParent( &o );

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

int SimplifyVisitor::visitLogicConstant( Behaviors::Logics::LogicConstant & o )
{
    BaseObject * parent = o.getParent();

    BinaryLogicCombination * pbf = 
        dynamic_cast< BinaryLogicCombination * >(parent);
    if( pbf == NULL )
    {
        return 1;
    }

    if(! ((o.getValue() == lc_true && pbf->getOp() == op_land) ||
            ( o.getValue() == lc_false && pbf->getOp() == op_lor )))
    {
        return 1;
    }

    // FIX BY REMOVING THE FORMULA!
    
    // Manage the grandparent to understand if the parent is its
    // first or second operand.
    BaseObject * grandparent = parent->getParent();

    BinaryFormula * gpbf = 
        dynamic_cast< BinaryFormula * >( grandparent );
    UnaryFormula * gpuf =
        dynamic_cast< UnaryFormula * >( grandparent );

    Contracts::AGContract * cp = 
        dynamic_cast< Contracts::AGContract * >( grandparent );

    if( gpbf == nullptr && gpuf == nullptr && cp == nullptr )
    {
        std::cerr << "ERROR: Unknown condition during simplification" << std::endl;
        exit(-1);
    }

    WellFormedFormula * sibling;

    if( pbf->getOp1() == &o )
    {
        sibling = pbf->getOp2();
    }
    else
    {
        sibling = pbf->getOp1();
    }

    // replace the parent in the grandparent with the sibling.

    sibling->setParent(grandparent);    
    if( cp != nullptr )
    {
        if( cp->getAssumptions() == parent )
        {
            cp->setAssumptions( sibling );
        }
        else
        {
            cp->setGuarantees( sibling );
        }
    }

    if( gpbf != nullptr )
    {
        if( gpbf->getOp1() == parent )
        {
            gpbf->setOp1( sibling );
        } 
        else if( gpbf->getOp2() == parent )
        {
            gpbf->setOp2( sibling );
        }
        else
        {
            std::cerr << 
                "ERROR: Unknown condition during simplification" 
                << std::endl;
            exit(-1);
        }
    }

    if( gpuf != nullptr )
    {
        if( gpbf->getOp1() == parent )
        {
            gpbf->setOp1( sibling );
        } 
        else
        {
            std::cerr << 
                "ERROR: Unknown condition during simplification" 
                << std::endl;
            exit(-1);
        }
    }

    return 1;
}



