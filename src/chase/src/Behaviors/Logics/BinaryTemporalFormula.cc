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
 * @file	chase/src/Behaviors/Logics/BinaryTemporalFormula.cc
 * @brief
 */



#include "Behaviors/Logics/BinaryTemporalFormula.hh"

using namespace Behaviors::Logics;

BinaryTemporalFormula::BinaryTemporalFormula(
        temporal_operators op,
        WellFormedFormula * op1,
        WellFormedFormula * op2 ) :
    TemporalFormula(op),
    BinaryFormula(op1, op2)
{
}

BinaryTemporalFormula::~BinaryTemporalFormula()
{
}

std::string BinaryTemporalFormula::print()
{
    std::string ret = std::string("( ");
    ret += _op1->print();
    switch(_op)
    {
        case op_until:
            ret += " U ";
            break;
        case op_release:
            ret += " R ";
            break;
        default:
            break;
    }
    ret += _op2->print();
    ret += " )";
    return ret;
}

int BinaryTemporalFormula::accept_visitor( Manipulation::BaseVisitor & v )
{
    return v.visitBinaryTemporalFormula( * this );
}

formula_type BinaryTemporalFormula::getType()
{
    return std::max(_op1->getType(), _op2->getType());
}


chase::BaseObject * BinaryTemporalFormula::copy()
{
    temporal_operators op = getOp();
    WellFormedFormula * op1 = 
        static_cast< WellFormedFormula * >( getOp1()->copy() );
    WellFormedFormula * op2 = 
        static_cast< WellFormedFormula * >( getOp2()->copy() );

    if( op1 == NULL || op2 == NULL )
    {
        std::cerr << "ERROR: odd configuration." << std::endl;
        exit(-1);
    }

    BinaryTemporalFormula * ret = new BinaryTemporalFormula( op, op1, op2 );
    return ret;
          
}

