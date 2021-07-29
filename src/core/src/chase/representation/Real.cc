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
    _min(-infinity),
    _max(infinity)
{
    _node_type = real_node;
}

Real::Real(double min, double max) :
SimpleType(),
_min(min),
_max(max)
{
    _node_type = real_node;
}

Real::~Real() = default;

int Real::accept_visitor( BaseVisitor &v )
{
    return v.visitReal(*this);
}

std::string Real::getString()
{
    return "real";
}

Real * Real::clone() {
    auto ret = new Real(_min, _max);
    return ret;
}

double Real::getMin() const {
    return _min;
}

double Real::getMax() const {
    return _max;
}


