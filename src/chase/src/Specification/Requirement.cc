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
 * @file	chase/src/Specification/Requirement.cc
 * @brief
 */



#include "Specification/Requirement.hh"

using namespace Specification;

Requirement::Requirement() :
    _functionName(""),
    _ariety(0),
    _params()
{
}

Requirement::Requirement( std::string functionName, unsigned int ariety ) :
    _functionName(functionName),
    _ariety(ariety),
    _params()
{
}

Requirement::Requirement( 
    std::string functionName, std::vector< std::string > params ) :
    _functionName(functionName),
    _ariety(params.size()),
    _params(params)
{
} 
    
Requirement::Requirement( 
    std::string functionName,
    unsigned int ariety,
    std::vector< std::string > params ) :
    _functionName(functionName),
    _ariety(ariety),
    _params(params)
{
}


Requirement::~Requirement()
{
}

std::string Requirement::getFunctionName()
{
    return _functionName;
}

std::string Requirement::getParameter(unsigned int position)
{
    return _params[position];
}

///\todo Fix the managing of the ariety to be more strong.
unsigned int Requirement::getAriety()
{
    _ariety = _params.size();
    return _ariety;
}

void Requirement::setFunctionName( std::string functionName )
{
    _functionName = functionName;
}


void Requirement::setAriety( unsigned int ariety )
{
    /// Warning: Ariety can be set just once!
    if( _ariety == 0 )
    {
        _ariety = ariety;
        _params.resize(_ariety);
    } 
}

void Requirement::setParameter(unsigned int position, std::string parameter )
{
    /// Warning: the element can be stored if and only if the position is
    /// within the ariety constraint.
    if(position < _ariety)
        _params[position] = parameter;
}





