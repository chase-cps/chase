/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Expression.hh"

using namespace chase;
using namespace std;

using sptr_type = std::shared_ptr<Type>;
using sptr_value = std::shared_ptr<Value>;
using sptr_exp = std::shared_ptr<Expression>;

Expression::Expression() :
    Value(),
    _op(op_none),
    _op1(nullptr),
    _op2(nullptr)
{
    _node_type = expression_node;
    _op1->setParent(this);
    _op2->setParent(this);
}

Expression::Expression(Operator op, sptr_value op1, 
    sptr_value op2 ) :
    Value(),
    _op(op),
    _op1(op1),
    _op2(op2)
{
    _node_type = expression_node;
    _op1->setParent(this);
    _op2->setParent(this);
}

Expression::~Expression()
{
}


Operator Expression::getOperator()
{
    return _op;
}

sptr_value Expression::getOp1()
{
    return _op1;
}

sptr_value Expression::getOp2()
{
    return _op2;
}

void Expression::setOperator(Operator op )
{
    _op = op;
}

void Expression::setOp1(sptr_value op )
{
    _op1 = op;
}

void Expression::setOp2(sptr_value op )
{
    _op2 = op;
}

std::string Expression::getString()
{
    std::string ret = _op1->getString();
    ret += to_string(_op);
    ret += _op2->getString();
    return ret;
}

int Expression::accept_visitor(BaseVisitor &v )
{
    return v.visitExpression(*this);
}

sptr_type Expression::getType()
{
    if( _op >= op_eq && _op <= op_ge )
        return make_shared<Boolean>();
    else
        return nullptr;
    /// \todo Fix this to find the actual type.
}

sptr_exp Expression::clone() {
    return make_shared<Expression>(_op, _op1, _op2);
}

