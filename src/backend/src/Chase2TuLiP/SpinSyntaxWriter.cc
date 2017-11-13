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
 * @file	backend/src/Chase2TuLiP/backend/SpinSyntaxWriter.cc
 * @brief
 */



#include "Chase2TuLiP/SpinSyntaxWriter.hh"
#include <iostream>
#include <cstdlib>

using namespace chase;
using namespace backend;
using namespace Chase2TuLiP;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Manipulation;

SpinSyntaxWriter::SpinSyntaxWriter() :
    GuideVisitor(0),
    _spin("")
{
}

SpinSyntaxWriter::~SpinSyntaxWriter()
{
}

int SpinSyntaxWriter::visitLogicConstant( 
        Behaviors::Logics::LogicConstant & o )
{
    if( o.getValue() == lc_true )
    {
        _spin += "True";
    }
    else if ( o.getValue() == lc_false )
    {
        _spin += "False";
    }
    else
    {
        std::cerr << "[ERROR]" << std::endl
            << "Unsupported value in logic constant" << std::endl;
        exit(-1);
    }
    return 0;
}

int SpinSyntaxWriter::visitLogicProposition( 
        Behaviors::Logics::LogicProposition & o )
{
    _spin += o.print();
    return 0;
}

int SpinSyntaxWriter::visitBinaryTemporalFormula( 
        Behaviors::Logics::BinaryTemporalFormula & o )
{
    _spin += "(";
    o.getOp1()->accept_visitor(*this);
    switch(o.getOp())
    {
        case op_until:
            _spin += " U ";
            break;
        case op_release:
            _spin += " V ";
            break;
        default:
            std::cerr << "[ERROR]" << std::endl 
                << "Unsupported Binary Operator" << std::endl;
            exit(-1);
            break;
    }
    o.getOp2()->accept_visitor(*this);
    _spin += ")";
    return 0;
}

int SpinSyntaxWriter::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    switch(o.getOp())
    {
        case op_globally:
            _spin += "[](";
            o.getOp1()->accept_visitor(*this);
            _spin += ")";
            break;
        case op_future:            
            _spin += "<>(";
            o.getOp1()->accept_visitor(*this);
            _spin += ")";
            break;
        case op_next:
            _spin += "X(";
            o.getOp1()->accept_visitor(*this);
            _spin += ")";
            break;
         default:
            std::cerr << "[ERROR]" << std::endl 
                << "Unsupported Unary Operator" << std::endl;
            exit(-1);
            break;          
    }
    return 0;
}

int SpinSyntaxWriter::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    _spin += "(";
    o.getOp1()->accept_visitor(*this);
    switch(o.getOp())
    {
        case op_land:
            _spin += " && ";
            break;
        case op_lor:
            _spin += " || ";
            break;
        case op_implies:
            _spin += " -> ";
            break;
        case op_iff:
            _spin += " <-> ";
            break;
        default:
            std::cerr << "[ERROR]" << std::endl 
                << "Unsupported Binary Operator" << std::endl;
            exit(-1);
            break;

    }
    o.getOp2()->accept_visitor(*this);
    _spin += ")";
    return 0;
}

int SpinSyntaxWriter::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & o )
{   
    if( o.getOp() == op_lnot )
    {
        _spin += "(! ";
        o.getOp1()->accept_visitor(*this);
        _spin += ")";
    }
    else
    {
        std::cerr << "[ERROR]" << std::endl 
            << "Unsupported Unary Operator" << std::endl;
        exit(-1);
    }
    return 0;
}

std::string SpinSyntaxWriter::getSpinRepresentation(
        Behaviors::Logics::WellFormedFormula * formula )
{
    formula->accept_visitor(*this);
    return _spin;
}
