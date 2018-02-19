/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/src/ChaseASTObject.cc
 * @brief
 */



/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0


#include "ChaseASTObject.hh"


using namespace chase;

ChaseASTObject::ChaseASTObject() :
    _parent(nullptr)
{
}

ChaseASTObject::~ChaseASTObject()
{
}

ChaseASTObject::ChaseASTObject( const ChaseASTObject & o ) :
    _parent( o._parent )
{
}

ChaseASTObject & ChaseASTObject::operator=( const ChaseASTObject & )
{
    return *this;
}

ChaseASTObject * ChaseASTObject::getParent()
{
    return _parent;
}

void ChaseASTObject::setParent( ChaseASTObject * p )
{
    _parent = p;
}




