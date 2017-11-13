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
 * @file	chase/src/Specification/RequirementFunction.cc
 * @brief
 */



#include "Specification/RequirementFunction.hh"

using namespace Specification;


RequirementFunction::RequirementFunction( 
        std::string name, unsigned int ariety ) :
    _name(name),
    _ariety(ariety)
{
}


RequirementFunction::RequirementFunction() :
    _name(""),
    _ariety(0)
{
}

RequirementFunction::~RequirementFunction()
{
}

std::string RequirementFunction::getName()
{
    return _name;
}

unsigned int RequirementFunction::getAriety()
{
    return _ariety;
}

std::list< Behaviors::Timing::chase_time > * 
    RequirementFunction::getTiming( Requirement * req )
{
    assert(req);
    return NULL;
}

