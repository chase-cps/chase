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
 * @file	backend/src/Chase2GR1C/backend/GR1CSyntaxWriter.cc
 * @brief
 */



#include "Chase2GR1C/GR1CSyntaxWriter.hh"
#include <iostream>
#include <cstdlib>

using namespace chase;
using namespace backend;
using namespace Chase2GR1C;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Manipulation;

GR1CSyntaxWriter::GR1CSyntaxWriter() :
    GuideVisitor(0),
    _gr1c(""),
    _next(0)
{
}

GR1CSyntaxWriter::~GR1CSyntaxWriter()
{
}

int GR1CSyntaxWriter::visitLogicConstant( 
        Behaviors::Logics::LogicConstant & o )
{
    if( o.getValue() == lc_true )
    {
        _gr1c += "True";
    }
    else if ( o.getValue() == lc_false )
    {
        _gr1c += "False";
    }
    else
    {
        std::cerr << "[ERROR]" << std::endl
            << "Unsupported value in logic constant" << std::endl;
        exit(-1);
    }
    return 0;
}

int GR1CSyntaxWriter::visitLogicProposition( 
        Behaviors::Logics::LogicProposition & o )
{
    std::string prop( o.print() );

    /// @todo Fix this horrible stuff!
    std::size_t pos = prop.find('=');
    if( pos == std::string::npos )
    {
        pos = prop.length();
    }
    for( unsigned int i = 0; i < _next; ++i )
    {
        prop.insert(pos, "'");
    }

    _gr1c += prop;
    return 0; 
}

int GR1CSyntaxWriter::visitBinaryTemporalFormula( 
        Behaviors::Logics::BinaryTemporalFormula & o )
{
    ///@todo Find a solution to avoid this message: may be a
    /// transformation.
    std::cerr << "[ERROR]" << std::endl
        << "Unsupported Binary Logic operator in GR1C syntax."
        << std::endl
        << o.print()
        << std::endl;
    exit(-1);
    return -1;
}

int GR1CSyntaxWriter::visitUnaryTemporalFormula(
        Behaviors::Logics::UnaryTemporalFormula & o )
{
    switch(o.getOp())
    {
        case op_globally:
            _gr1c += "[](";
            o.getOp1()->accept_visitor(*this);
            _gr1c += ")";
            break;
        case op_future:            
            _gr1c += "<>(";
            o.getOp1()->accept_visitor(*this);
            _gr1c += ")";
            break;
        case op_next:
            // Uses a stack kind of structure to annotate the
            // levels of innested next. Only one should be
            // supported by gr1c, still, it may be possible to
            // have a multiple next function (think about a PSL
            // formula using the next[] construct).
            ++_next;
            o.getOp1()->accept_visitor(*this);
            --_next;
            break;
         default:
            std::cerr << "[ERROR]" << std::endl 
                << "Unsupported Unary Operator" << std::endl;
            exit(-1);
            break;          
    }
    return 0;

}

int GR1CSyntaxWriter::visitBinaryLogicCombination(
        Behaviors::Logics::BinaryLogicCombination & o )
{
    _gr1c += "(";
    o.getOp1()->accept_visitor(*this);
    switch(o.getOp())
    {
        case op_land:
            _gr1c += " & ";
            break;
        case op_lor:
            _gr1c += " | ";
            break;
        case op_implies:
            _gr1c += " -> ";
            break;
        case op_iff:
            _gr1c += " <-> ";
            break;
        default:
            std::cerr << "[ERROR]" << std::endl 
                << "Unsupported Binary Operator" << std::endl;
            exit(-1);
            break;

    }
    o.getOp2()->accept_visitor(*this);
    _gr1c += ")";
    return 0;

}

int GR1CSyntaxWriter::visitUnaryLogicCombination(
        Behaviors::Logics::UnaryLogicCombination & o )
{
    if( o.getOp() == op_lnot )
    {
        _gr1c += "(! ";
        o.getOp1()->accept_visitor(*this);
        _gr1c += ")";
    }
    else
    {
        std::cerr << "[ERROR]" << std::endl 
            << "Unsupported Unary Operator" << std::endl;
        exit(-1);
    }
    return 0;
}

std::string GR1CSyntaxWriter::getGR1CRepresentation( 
        Behaviors::Logics::WellFormedFormula * formula )
{
    _next = 0;
    _gr1c = std::string("");
    formula->accept_visitor(*this);
    return _gr1c;
}


