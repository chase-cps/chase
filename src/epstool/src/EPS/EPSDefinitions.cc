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
 * @file	chase/src/Architecture/EPS/EPSDefinitions.cc
 * @brief
 */



#include "EPS/EPSDefinitions.hh"

using namespace chase::epstool;
using namespace EPS;
using namespace Architecture;

Contactor::Contactor( 
        unsigned int n,
        EPSComponent * n1,
        EPSComponent * n2,
        Behaviors::Logics::LogicProposition * sv ) :
    number(n),
    node_1(n1),
    node_2(n2),
    stateVariable(sv)
{
}

Contactor::~Contactor()
{
}

PhysicalContactor::PhysicalContactor(
        unsigned int n,
        EPSComponent * n1,
        EPSComponent * n2,
        Behaviors::Logics::LogicProposition * sv,
        Behaviors::Logics::LogicProposition * iv,
        Behaviors::Timing::Counter * oc,
        Behaviors::Timing::Counter * cc,
        unsigned int ot,
        unsigned int ct ) :
    Contactor(n, n1, n2, sv),
    intentionVariable(iv),
    openingCounter(oc),
    closingCounter(cc),
    openingTime(ot),
    closingTime(ct)
{
}

PhysicalContactor::~PhysicalContactor()
{
}

