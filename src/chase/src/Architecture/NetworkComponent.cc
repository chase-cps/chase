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
 * @file	chase/src/Architecture/NetworkComponent.cc
 * @brief
 */



#include "Architecture/NetworkComponent.hh"

using namespace Architecture;

NetworkComponent::NetworkComponent( std::string name, 
        DomainSpecificType * dst ) :
    _name(name),
    _type(dst),
    _abbreviation(""),
    _incomingEdges(),
    _outgoingEdges()
{
}

NetworkComponent::NetworkComponent(
        std::string name,
        DomainSpecificType * dst,
        std::string abbreviation ) :
    _name(name),
    _type(dst),
    _abbreviation(abbreviation),
    _incomingEdges(),
    _outgoingEdges()
{
}

NetworkComponent::~NetworkComponent()
{
}

bool NetworkComponent::addIncomingEdge( NetworkConnection * edge )
{
    _incomingEdges.insert(edge);
    size_t setsize = _incomingEdges.size();
    return(
            (setsize >= _type->getInDegreeRange()->min) && 
            (setsize <= _type->getInDegreeRange()->max)
            ); 
}

bool NetworkComponent::addOutgoingEdge( NetworkConnection * edge )
{
    _outgoingEdges.insert(edge);
    size_t setsize = _outgoingEdges.size();
    return(
            (setsize >= _type->getOutDegreeRange()->min) && 
            (setsize <= _type->getOutDegreeRange()->max)
            ); 
}

std::set< NetworkConnection * > NetworkComponent::getIncomingEdges()
{
    return _incomingEdges; 
}

std::set< NetworkConnection * > NetworkComponent::getOutgoingEdges()
{
    return _outgoingEdges;
}

DomainSpecificType * NetworkComponent::getType()
{
    return _type;
}

void NetworkComponent::setType( DomainSpecificType * type )
{
    _type = type;
}

std::string NetworkComponent::getName()
{
    return _name;
}

void NetworkComponent::setName( std::string name )
{
    _name = name;
}

std::string NetworkComponent::getAbbreviation()
{
    return _abbreviation;
}

void NetworkComponent::setAbbreviation( std::string name )
{
    _abbreviation = name;
}


