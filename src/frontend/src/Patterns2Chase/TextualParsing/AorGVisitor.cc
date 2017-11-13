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
 * @file	frontend/src/Patterns2Chase/TextualParsing/AorGVisitor.cc
 * @brief
 */



#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"

#include "Behaviors/Logics/Logics.hh"
#include "Contracts/Contracts.hh"
#include <iostream>

using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Behaviors;
using namespace Behaviors::Logics;

AorGVisitor::AorGVisitor( Contracts::Contract * c ) :
    Manipulation::GuideVisitor(0),
    _contract(c),
    _sysVarUsed(false)
{
}

AorGVisitor::~AorGVisitor()
{
}

bool AorGVisitor::getSysVarUsed()
{
    return _sysVarUsed;
}

int AorGVisitor::visitLogicProposition( Behaviors::Logics::LogicProposition & o )
{

    std::string n = o.GetName();

    // During the tree construction (for a formula), it is already verified that
    // the used propositions are declared within the system xor environment variables.
    // Thus, it is enough to check if it is declared in one of the two. The
    // system variables are checked.
    std::list< Behaviors::Variable * >::iterator vit;
    for( vit =  _contract->system_variables.begin();
            vit != _contract->system_variables.end();
            ++vit )
    {
        std::string vname = (*vit)->GetName();
        if( n == vname )
        {
            // If it is a system variable, set the flag to true and exit the visit
            // of the proposition.
            _sysVarUsed = true;
            return _rv;
        }
    }

    return _rv;
}

