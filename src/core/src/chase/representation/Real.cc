/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Real.hh"
#include "representation/Range.hh"

using namespace chase;

Real::Real() :
    SimpleType(),
    _range(nullptr)
{
    _node_type = real_node;
}

Real::~Real()
{
    if( _range != nullptr ) delete _range;
}

Range * Real::getRange()
{
    return _range;
}

void Real::setRange( Range * r )
{
    _range = r;
}

int Real::accept_visitor( BaseVisitor &v )
{
    return v.visitReal(*this);
}

std::string Real::getString()
{
    return "real";
}
