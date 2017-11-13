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
 * @file	chase/src/Architecture/NetworkConnection.cc
 * @brief
 */



#include "Architecture/NetworkConnection.hh"

using namespace Architecture;

NetworkConnection::NetworkConnection(
     NetworkComponent * source,
     NetworkComponent * target,
     NetworkComponent * connector ) :
     _source(source),
     _target(target),
     _switch(connector),
     _bidirectional(false)
{
    /// Consistency checks for the connection:
    /// - source and target may not be a switch, while the 
    /// connector must be a switch. Furthermore, source and target
    /// must exist, while switch no.
    assert(_source);
    assert(_target);
    if( connector != NULL )
    {
        component_types connector_type = connector->getType()->getGenericType();
        assert( connector_type == ct_switch );
        connector_type = _target->getType()->getGenericType();
        assert( connector_type != ct_switch );
        connector_type = _source->getType()->getGenericType();
        assert( connector_type != ct_switch );
    }
}

NetworkConnection::~NetworkConnection()
{
}

bool NetworkConnection::setSource( NetworkComponent * source )
{
    component_types connector_type = source->getType()->getGenericType();
    if(connector_type == ct_switch)
        return false;
    _source = source;
    // Add the edge to the node.
    _source->addOutgoingEdge(this);
    return( _source != NULL );
}

bool NetworkConnection::setTarget( NetworkComponent * target )
{
    component_types connector_type = target->getType()->getGenericType();
    if(connector_type == ct_switch)
        return false;
    _target = target;
    // Add the edge to the node.
    _target->addIncomingEdge(this);
    return( _target != NULL );
}

bool NetworkConnection::setSwitch( NetworkComponent * connector )
{
    component_types connector_type = connector->getType()->getGenericType();
    if(connector_type != ct_switch)
        return false;
    _switch = connector;
    return( _switch != NULL );
}

NetworkComponent * NetworkConnection::getSource()
{
    return _source;
}

NetworkComponent * NetworkConnection::getTarget()
{
    return _target;
}

NetworkComponent * NetworkConnection::getSwitch()
{
    return _switch;
}


void NetworkConnection::setBidirectional( bool b )
{
    _bidirectional = b;
}

bool NetworkConnection::getBidirectional()
{
    return _bidirectional;
}



