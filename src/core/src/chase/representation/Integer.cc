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
    _min(-infinity),
    _max(infinity)
{
    _node_type = integer_node;
}

Integer::Integer(int64_t min, int64_t max) :
    SimpleType(),
    _signed(min < 0),
    _min(min),
    _max(max)
{
    _node_type = integer_node;
}

Integer::~Integer()
= default;

bool Integer::isSigned() const
{
    return _signed;
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
    return new Integer(_min, _max);
}

int64_t Integer::getMin() const {
    return _min;
}

int64_t Integer::getMax() const {
    return _max;
}



