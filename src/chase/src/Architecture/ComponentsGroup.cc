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
 * @file	chase/src/Architecture/ComponentsGroup.cc
 * @brief
 */



#include "Architecture/ComponentsGroup.hh"

using namespace Architecture;

ComponentsGroup::ComponentsGroup(DomainSpecificType * type) :
    _type(type),
    parameters()
{
}

ComponentsGroup::ComponentsGroup(
        DomainSpecificType * type,
        std::list< TopologyParameter > parameters ) :
    _type(type),
    parameters(parameters)
{
}

ComponentsGroup::~ComponentsGroup()
{
}

DomainSpecificType * ComponentsGroup::getDomainSpecificType()
{
    return _type;
}


bool ComponentsGroup::equals( ComponentsGroup & o )
{
    if( o.getDomainSpecificType() != _type )
        return false;
    std::list< FlowParameter >::iterator tfit;
    std::list< FlowParameter >::iterator ofit;

    if( parameters.size() != o.parameters.size() )
        return false;

    for(tfit = parameters.begin(), ofit = o.parameters.begin();
            tfit != parameters.end() && ofit != o.parameters.end();
            ++tfit, ++ofit )
    {
        FlowParameter t = *tfit;
        FlowParameter o = *ofit;
        if( t != o ) return false;
    }
    return true;
}

