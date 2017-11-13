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
 * @file	chase/src/Behaviors/Logics/UnaryLogicCombination.cc
 * @brief
 */



#include "Behaviors/Logics/UnaryLogicCombination.hh"

using namespace Behaviors::Logics;

UnaryLogicCombination::UnaryLogicCombination( 
        logic_operators op,
        WellFormedFormula * operand ) :
    BooleanFormula(op),
    UnaryFormula(operand)
{    
}

UnaryLogicCombination::~UnaryLogicCombination()
{
}


std::string UnaryLogicCombination::print()
{
    std::string ret = "(";
    ret += printOperator(_op);
    ret += " ";
    ret += _op1->print();
    ret += ")";
    return ret;
}

formula_type UnaryLogicCombination::getType()
{
    return std::max(ft_propositional, _op1->getType());
}

int UnaryLogicCombination::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitUnaryLogicCombination(* this);
}

