/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <chase/representation/ModalFormula.hh>
#include "representation/ChaseObject.hh"

using namespace chase;

ChaseObject::ChaseObject() :
    _parent(nullptr),
    _node_type(object_node)
{
}

ChaseObject::~ChaseObject()
{
}

ChaseObject * ChaseObject::getParent()
{
    return _parent;
}

void ChaseObject::setParent(chase::ChaseObject * parent )
{
    _parent = parent;
}

nodeType ChaseObject::IsA()
{
    return _node_type;
}

std::string ChaseObject::getString() {
    return std::string();
}



