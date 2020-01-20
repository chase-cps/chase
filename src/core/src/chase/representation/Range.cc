/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Range.hh"

using namespace chase;

Range::Range() :
    Value(),
    _lbound(INT32_MIN),
    _rbound(INT32_MAX)
{
    _node_type = range_node;

    _checkConsistency();
}

Range::~Range()
{
}

Range::Range(int lbound, int rbound ) :
    Value(),
    _lbound(lbound),
    _rbound(rbound)
{
    _node_type = range_node;
    _checkConsistency();
}

void Range::setLeftValue( int lbound )
{
    _lbound = lbound;
    _checkConsistency();
}

void Range::setRightValue( int rbound )
{
    _rbound = rbound;
    _checkConsistency();
}

int Range::getLeftValue()
{
    return _lbound;
}

int Range::getRightValue()
{
    return _rbound;
}  

void Range::_checkConsistency()
{
    if(_lbound > _rbound)
        messageError(
                "Bad Range: " + std::to_string(_lbound) +
                ":" + std::to_string(_rbound));
}


std::string Range::getString()
{
    std::string ret("[");
    ret += std::to_string(_lbound);
    ret += ", ";
    ret += std::to_string(_rbound);
    ret += "]";
    return ret;
}

int Range::accept_visitor( BaseVisitor &v )
{
    return v.visitRange( *this );
}

Type *Range::getType() {
    return new Integer();
}

Range *Range::clone() {
    return new Range(_lbound, _rbound);
}

