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
 * @file	chase/src/Specification/FunctionsLibrary.cc
 * @brief
 */



#include "Specification/FunctionsLibrary.hh"

using namespace Specification;

FunctionsLibrary::FunctionsLibrary() :
    _functions()
{
}

FunctionsLibrary::~FunctionsLibrary()
{
}

void FunctionsLibrary::resolveRequirement(
        Requirement * req,
        Problem * problem )
{
    unsigned int req_ariety = req->getAriety();
    std::string req_name = req->getFunctionName();


    std::list< RequirementFunction * >::iterator it;

    bool matched = false;
    for( it = _functions.begin();
            it != _functions.end() && !matched ;
            ++it )
    {
        RequirementFunction * currfun = *it;

        if( 
                req_ariety <= currfun->getAriety() && 
                req_name == currfun->getName() 
          )
        {
            matched = true;
            currfun->resolve(req, problem);
            return;

        }
    }

    if( matched == false )
    {
        std::cerr << "WARNING: Impossible to match the requirement"
            << std::endl << "\t Requirement ignored" << std::endl;
    }
}
