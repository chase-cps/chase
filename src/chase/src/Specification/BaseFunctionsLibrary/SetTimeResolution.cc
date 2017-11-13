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
 * @file	chase/src/Specification/BaseFunctionsLibrary/SetTimeResolution.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/SetTimeResolution.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

SetTimeResolution::SetTimeResolution() :
    RequirementFunction("time-resolution",2)
{
}

SetTimeResolution::~SetTimeResolution()
{
}

void SetTimeResolution::resolve( Requirement * req, Problem * problem )
{
    unsigned int par0 = atoi(req->getParameter(0).c_str());
    std::string par1 = req->getParameter(1);

    Timing::chase_time t(par0, par1);
    problem->setTimeResolution(t);
}

std::list< Behaviors::Timing::chase_time > * 
    SetTimeResolution::getTiming( Requirement * req )
{
    std::list< Behaviors::Timing::chase_time > * ret;
    ret = new std::list< Behaviors::Timing::chase_time >();

    unsigned int par0 = atoi(req->getParameter(0).c_str());
    std::string par1 = req->getParameter(1);

    Timing::chase_time t(par0, par1);
    ret->push_back(t);

    return ret;
}
