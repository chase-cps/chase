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
 * @file	chase/src/Architecture/DomainSpecificGenericType.cc
 * @brief
 */



#include "Architecture/DomainSpecificGenericType.hh"

using namespace Architecture;

DomainSpecificGenericType::DomainSpecificGenericType( std::string name ) :
    DomainSpecificType( name, ct_generic )
{
}

DomainSpecificGenericType::DomainSpecificGenericType( 
        std::string name,
        unsigned int params) :
    DomainSpecificType( name, ct_generic )
{
    _flow_params = params;
    if( _flow_params > 0 )
    {
        _flow_parameters = new FlowParameter[_flow_params];
    }
}



DomainSpecificGenericType::~DomainSpecificGenericType()
{
}
