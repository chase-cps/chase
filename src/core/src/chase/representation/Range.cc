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
    _lbound( nullptr ),
    _rbound( nullptr )
{
    _lbound = new IntegerValue(0);
    _rbound = new IntegerValue(INT64_MAX);

    _lbound->setParent( this );
    _rbound->setParent( this );

    _node_type = range_node;

    _checkConsistency();
}

Range::~Range()
{
    delete _lbound;
    delete _rbound;
}

Range::Range( NumericValue * lbound,  NumericValue * rbound ) :
    Value(),
    _lbound(lbound),
    _rbound(rbound)
{
    _lbound->setParent( this );
    _rbound->setParent( this );
    _node_type = range_node;

    _checkConsistency();
}

Range::Range(int lbound, int rbound ) :
    Value(),
    _lbound(nullptr),
    _rbound(nullptr)
{
    _lbound = new IntegerValue(lbound);
    _rbound = new IntegerValue(rbound);

    _node_type = range_node;

    _checkConsistency();
}

Range::Range( double lbound, double rbound ) :
    Value(),
    _lbound(nullptr),
    _rbound(nullptr)
{
    _lbound = new RealValue(lbound);
    _rbound = new RealValue(rbound);

    _node_type = range_node;
    
    _checkConsistency();
}


void Range::setLeftValue( NumericValue * lbound )
{
    _lbound = lbound;
    _checkConsistency();
}

void Range::setRightValue( NumericValue * rbound )
{
    _rbound = rbound;
    _checkConsistency();
}

NumericValue * Range::getLeftValue()
{
    return _lbound;
}

NumericValue * Range::getRightValue()
{
    return _rbound;
}  

void Range::_checkConsistency()
{
    if( _lbound != nullptr && _rbound != nullptr )
    {
        /// @todo Implement here what should happen.
    }
}


std::string Range::getString()
{
    std::string ret("[");
    ret += _lbound->getString();
    ret += ", ";
    ret += _rbound->getString();
    ret += "]";
    return ret;
}

int Range::accept_visitor( BaseVisitor &v )
{
    return v.visitRange( *this );
}

Type *Range::getType() {
    Type * left = _lbound->getType();
    Type * right = _rbound->getType();

    if( left->IsA() != integer_node || right->IsA() != integer_node )
        return new Real();
    else return new Integer();

}
