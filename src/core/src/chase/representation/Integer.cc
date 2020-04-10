/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Integer.hh"
#include "representation/Range.hh"

using namespace chase;

Integer::Integer() :
    SimpleType(),
    _signed(true),
    _range(nullptr)
{
    _node_type = integer_node;
    _range = new Range(-2147483647, 2147483647);
}

Integer::Integer(int l, int r) :
    SimpleType(),
    _signed(l < 0),
    _range(new Range(l,r))
{
    _node_type = integer_node;
}



Integer::Integer(Range *r) :
    SimpleType(),
    _range(r)
{
    _node_type = integer_node;
    if( r->getParent() == nullptr ) r->setParent(this);
    if(r->getLeftValue() < 0) _signed = true;
}


Integer::~Integer()
{
    delete _range;
}

bool Integer::isSigned()
{
    return _signed;
}

Range * Integer::getRange()
{
    return _range;
}

int Integer::accept_visitor( BaseVisitor &v )
{
    return v.visitInteger(*this);
}

std::string Integer::getString()
{
    return "integer";
}

Integer *Integer::clone()
{
    return new Integer(_range->clone());
}



