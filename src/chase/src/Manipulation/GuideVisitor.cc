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
 * @file	chase/src/Manipulation/GuideVisitor.cc
 * @brief
 */



#include "Manipulation/GuideVisitor.hh"

#include "Behaviors/Behaviors.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Contracts/Contracts.hh"
#include "Behaviors/Variable.hh"
#include "Behaviors/Constant.hh"
#include "Behaviors/Function.hh"


#include <iostream>

using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Contracts;
using namespace Manipulation;

GuideVisitor::GuideVisitor( int rv ) :
    _rv(rv)
{
}

GuideVisitor::~GuideVisitor()
{
}

int GuideVisitor::visitLogicAtom( Behaviors::Logics::LogicAtom & )
{
    return _rv;//o.accept_visitor(*this);
}

int GuideVisitor::visitLogicConstant( Behaviors::Logics::LogicConstant & )
{
    return _rv;//o.accept_visitor( * this );
}

int GuideVisitor::visitLogicProposition( Behaviors::Logics::LogicProposition & )
{
    return _rv;//o.accept_visitor( * this );
}

int GuideVisitor::visitBinaryFormula( 
        Behaviors::Logics::BinaryFormula & o )
{
    int rv = (o.getOp1())->accept_visitor(*this);
    rv |= (o.getOp2())->accept_visitor(*this);
    return rv;
}

int GuideVisitor::visitUnaryFormula(
        Behaviors::Logics::UnaryFormula & o )
{
    int rv = (o.getOp1())->accept_visitor(*this);
    return rv;
}

int GuideVisitor::visitBooleanFormula(
        Behaviors::Logics::BooleanFormula & o )
{
    return o.accept_visitor( * this );
}

int GuideVisitor::visitTemporalFormula(
        Behaviors::Logics::TemporalFormula & o )
{
    return o.accept_visitor( * this );
}

int GuideVisitor::visitBinaryTemporalFormula( 
        Behaviors::Logics::BinaryTemporalFormula & o )
{
    int rv = (o.getOp1())->accept_visitor(*this);
    rv |= (o.getOp2())->accept_visitor(*this);
    return rv;
}

int GuideVisitor::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    int rv = (o.getOp1())->accept_visitor(*this);
    return rv;
}

int GuideVisitor::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    int rv = (o.getOp1())->accept_visitor(*this);
    rv |= (o.getOp2())->accept_visitor(*this);
    return rv;
}

int GuideVisitor::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & o )
{
    int rv = (o.getOp1())->accept_visitor(*this);
    return rv;
}

int GuideVisitor::visitVariable( Behaviors::Variable & )
{
    return _rv;//o.accept_visitor( * this );
}

int GuideVisitor::visitConstant( Behaviors::Constant & )
{
    return _rv;//o.accept_visitor( * this );
}

int GuideVisitor::visitFunction( Behaviors::Function & o )
{
    int ret = 0;
    for( unsigned int i = 0; i < o.getArity(); ++i )
    {
        ret |= o.arguments[i]->accept_visitor( *this );
    }
    return ret;
}

int GuideVisitor::visitLogicPredicate( Behaviors::Logics::LogicPredicate & o )
{
    int ret = 0;
    for( unsigned int i = 0; i < o.getArity(); ++i )
    {
        ret |= o.arguments[i]->accept_visitor( *this );
    }
    return ret;
}
int GuideVisitor::visitAGContract( Contracts::AGContract & o )
{
    int ret = 0;

    std::list< Behaviors::Variable * >::iterator vit;
    for( vit = o.environment_variables.begin();
            vit != o.environment_variables.end();
            ++vit )
    {
        Behaviors::Variable * var = *vit;
        ret |= var->accept_visitor( * this );
    }
    for( vit = o.system_variables.begin();
            vit != o.system_variables.end();
            ++vit )
    {
        Behaviors::Variable * var = *vit;
        ret |= var->accept_visitor( * this );
    }

    ret |= o.assumptions->accept_visitor( * this );
    ret |= o.guarantees->accept_visitor( * this );

    return ret;

}

int GuideVisitor::visitGR1Formula(
        Behaviors::Logics::GR1Formula & o )
{
    int ret = 0;
    if( o.getInitial() )
        ret |= o.getInitial()->accept_visitor( *this );
    if( o.getSafety() )
        ret |= o.getSafety()->accept_visitor( *this );
    if( o.getProgress() )
        ret |= o.getProgress()->accept_visitor( *this );

    return ret;
}

