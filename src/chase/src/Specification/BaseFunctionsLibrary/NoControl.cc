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
 * @file	chase/src/Specification/BaseFunctionsLibrary/NoControl.cc
 * @brief
 */



#include "Specification/BaseFunctionsLibrary/NoControl.hh"

using namespace Specification;
using namespace Behaviors;
using namespace Behaviors::Logics;

NoControl::NoControl() :
    RequirementFunction("no-control",1)
{
}

NoControl::~NoControl()
{
}

/// \todo
// It is necessary to decide if insert this properties.
// Two possibilities are available:
// - Do not insert these constraints and remove the variables.
// - Keep as it is.

void NoControl::resolve( Requirement * req, Problem * problem )
{
    std::string par0 = req->getParameter(0);
    std::set< Architecture::NetworkComponent * > components =
        Manipulation::findComponents( par0, problem->getNetwork() );

    std::set< Architecture::NetworkComponent * >::iterator it;
    for( it = components.begin();
            it != components.end();
            ++it )
    {
        // Register the component as constant.
        Architecture::NetworkComponent * currcomp = *it;
        problem->noncontrollable_components.insert(currcomp);
    }



}

