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
 * @file	chase/src/Contracts/AGContract.cc
 * @brief
 */



#include "Contracts/AGContract.hh"
#include <iostream>

using namespace Contracts;
using namespace Behaviors;
using namespace Logics;
using namespace Timing;

AGContract::AGContract() :
    assumptions(NULL),
    guarantees(NULL)
{
    UnaryTemporalFormula * f1 = new UnaryTemporalFormula(
            op_future,
            new LogicConstant(lc_true));
    UnaryTemporalFormula * f2 = new UnaryTemporalFormula(
            op_globally,
            f1 );

    UnaryTemporalFormula * f3 = new UnaryTemporalFormula(
            op_future,
            new LogicConstant(lc_true));
    UnaryTemporalFormula * f4 = new UnaryTemporalFormula(
            op_globally,
            f3 );

    setAssumptions(f2);
    f2->setParent(this);

    setGuarantees(f4);
    f4->setParent(this);

}

AGContract::~AGContract()
{
}

Behaviors::Behavior * AGContract::getAssumptions()
{
    return assumptions;
}

Behaviors::Behavior * AGContract::getGuarantees()
{
    return guarantees;
}

void AGContract::setAssumptions( Behaviors::Behavior * a )
{
    assumptions = a;
}

void AGContract::setGuarantees( Behaviors::Behavior * g )
{
    guarantees = g;
}

std::string AGContract::print()
{
    std::string ret("Declarations:\n");
    std::list< Behaviors::Variable * >::iterator it;
    for( it = environment_variables.begin();
            it != environment_variables.end();
            ++it )
    {
        Behaviors::Variable * var = *it;
        ret += "ENV:\t";
        ret += var->print();
        ret += ";\n";
    }

    for( it = system_variables.begin();
            it != system_variables.end();
            ++it )
    {
        Behaviors::Variable * var = *it;
        ret += "SYS:\t";
        ret += var->print();
        ret += ";\n";
    }


    ret += "\nTiming--------\n"; 

    std::list< Behaviors::Timing::SafetyCounter * >::iterator sit;
    for( sit = safety_counters.begin();
            sit != safety_counters.end();
            ++sit )
    {
        SafetyCounter * counter = *sit;
        ret += counter->print();
    }

    std::list< Behaviors::Timing::LivenessCounter * >::iterator lit;
    for( lit = liveness_counters.begin();
            lit != liveness_counters.end();
            ++lit )
    {
        LivenessCounter * counter = *lit;
        ret += counter->print();
    }


    ret += "\n\nDynamics-------\n\n";
    ret += ("Assumptions:\n");
    ret += assumptions->print();
    ret += "\n\n";
    ret += "Guarantees:\n";
    ret += guarantees->print();
    return ret;
}

int AGContract::accept_visitor( Manipulation::BaseVisitor & v )
{ 
    return v.visitAGContract( * this );
}

