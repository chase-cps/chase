/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        7/27/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "chase/representation/Interval.hh"
#include "chase/representation/Integer.hh"
#include "chase/representation/Real.hh"

using namespace chase;

Interval::~Interval()
{
    delete _leftBound;
    delete _rightBound;
}

Interval::Interval(Value *lbound, Value *rbound, bool leftOpen, bool rightOpen) :
    Value(),
    _leftBound(lbound),
    _rightBound(rbound),
    _leftOpen(leftOpen),
    _rightOpen(rightOpen)
{
    _node_type = interval_node;
    if(_leftBound->getParent() != nullptr)
        messageWarning("Already used value in left bound.", _leftBound);
    else _leftBound->setParent(this);

    if(_rightBound->getParent() != nullptr)
        messageWarning("Already used value in right bound.", _rightBound);
    else _rightBound->setParent(this);
}

Value *Interval::getLeftBound() const {
    return _leftBound;
}

Value *Interval::getRightBound() const {
    return _rightBound;
}

bool Interval::isLeftOpen() const {
    return _leftOpen;
}

bool Interval::isRightOpen() const {
    return _rightOpen;
}

Type *Interval::getType()
{
    auto ltype = _leftBound->getType()->IsA();
    auto rtype = _rightBound->getType()->IsA();

    if(ltype == integer_node ) {
        if (rtype == integer_node)
            return new Integer();
        else if (rtype == real_node)
            return new Real();
    }
    else if( ltype == real_node ) {
        if (rtype == real_node || rtype == integer_node)
            return new Real();
    }
    return nullptr;
}

Interval *Interval::clone() {
    return new Interval(
            _leftBound->clone(),
            _rightBound->clone(),
            isLeftOpen(),
            isRightOpen()
            );
}

int Interval::accept_visitor(BaseVisitor &v) {
    return v.visitInterval( *this );;
}

std::string Interval::getString() {
    std::string ret("");
    if(_leftOpen) ret += "]";
    else ret += "[";
    ret += _leftBound->getString()
            + ", " + _rightBound->getString();
    if(_rightOpen) ret += "[";
    else ret += "]";
    return ret;
}

