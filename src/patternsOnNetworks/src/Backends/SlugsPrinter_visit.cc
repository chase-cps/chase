/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "Backends/SlugsPrinter.hh"

using namespace chase;


int SlugsPrinter::visitIntegerValue(IntegerValue &o) {
    _curr += std::to_string(o.getValue());
    return 1;
}

int SlugsPrinter::visitExpression(Expression &o) {
    _curr += "(";
    _continueVisit(o.getOp1());
    _curr += to_string(o.getOperator());
    _continueVisit(o.getOp2());
    _curr += ")";
    return 1;
}

int SlugsPrinter::visitIdentifier(Identifier &o) {
    if(_inNext)
    {
        _curr += o.getString();
        _curr +="'";
    } else
        _curr += o.getString();
    return 1;
}

int SlugsPrinter::visitProposition(Proposition &o)
{
    if(o.getValue()->IsA() == identifier_node)
    {
        reinterpret_cast<Identifier*>(o.getValue())->accept_visitor(*this);
    }
    else if(o.getValue()->IsA() == expression_node)
    {
        reinterpret_cast<Expression*>(o.getValue())->accept_visitor(*this);
    }
    return 1;
}

int SlugsPrinter::visitBooleanConstant(BooleanConstant &o) {
    return GuideVisitor::visitBooleanConstant(o);
}

int SlugsPrinter::visitBinaryBooleanOperation(BinaryBooleanFormula &o) {
    _curr += "(";
    _continueVisit(o.getOp1());

    switch(o.getOp())
    {
        case op_not:
            messageError("Not a binary operator.");
            break;
        case op_and:
            _curr += " & ";
            break;
        case op_or:
            _curr += " | ";
            break;
        case op_implies:
            _curr += " -> ";
            break;
        case op_iff:
            _curr += " <-> ";
            break;
        case op_xor:
        case op_nand:
        case op_nor:
        case op_xnor:
            messageError("Operator not supported.");
            break;
    }
    _continueVisit(o.getOp2());
    _curr += ")";
    return 1;
}

int SlugsPrinter::visitUnaryBooleanOperation(UnaryBooleanFormula &o) {
    switch(o.getOp())
    {
        case op_not:
            _curr += "(!";
            _curr += "(";
            _continueVisit(o.getOp1());
            _curr += "))";
            break;
        case op_and:
            messageError("Not an Unary operator.");
            break;
        case op_or:
            messageError("Not an Unary operator.");
            break;
        case op_implies:
            messageError("Not an Unary operator.");
            break;
        case op_iff:
            messageError("Not an Unary operator.");
            break;
        case op_xor:
            messageError("Not an Unary operator.");
            break;
        case op_nand:
            messageError("Not an Unary operator.");
            break;
        case op_nor:
            messageError("Not an Unary operator.");
            break;
        case op_xnor:
            messageError("Not an Unary operator.");
            break;
    }
    return 1;
}

int SlugsPrinter::visitUnaryTemporalOperation(UnaryTemporalFormula &o) {
    switch(o.getOp()) {
        case op_next:
            if (_inNext)
                messageError("Illegal form: double annidated Next.");
            _inNext = true;
            _continueVisit(o.getFormula());
            _inNext = false;
            break;
        case op_globally:
            _continueVisit(o.getFormula());
            break;
        case op_future:
            _continueVisit(o.getFormula());
            break;
        case op_until:
            messageError("Until is not unary.");
            break;
        case op_release:
            messageError("Release operator is not unary.");
            break;
    }
    return 1;
}

int SlugsPrinter::visitBinaryTemporalOperation(BinaryTemporalFormula &o)
{
    switch(o.getOp()) {
        case op_next:
            messageError("Next operator is not binary.");
            break;
        case op_globally:
            messageError("Always operator is not binary.");
            break;
        case op_future:
            messageError("Eventually operator is not binary.");
            break;
        case op_until:
            messageError("Until is not currently supported.");
            break;
        case op_release:
            messageError("Release operator is not currently supported.");
            break;
    }
    return 1;
}

int SlugsPrinter::_continueVisit(ChaseObject *o) {
    switch(o->IsA())
    {
        case integerValue_node:{
            auto v = reinterpret_cast< IntegerValue * >(o);
            v->accept_visitor(*this);
            break;
        }
        case expression_node: {
                auto v = reinterpret_cast< Expression *>(o);
                v->accept_visitor(*this);
                break;
        }
        case identifier_node: {
            auto v = reinterpret_cast< Identifier * > (o);
            v->accept_visitor(*this);
            break;
        }
        case proposition_node: {
            auto v = reinterpret_cast< Proposition * > (o);
            v->accept_visitor(*this);
            break;
        }
        case booleanConstant_node: {
            auto v = reinterpret_cast< BooleanConstant * > (o);
            v->accept_visitor(*this);
            break;
        }
        case binaryBooleanOperation_node: {
            auto v = reinterpret_cast< BinaryBooleanFormula * > (o);
            v->accept_visitor(*this);
            break;
        }
        case unaryBooleanOperation_node: {
            auto v = reinterpret_cast< UnaryBooleanFormula *>(o);
            v->accept_visitor(*this);
            break;
        }
        case unaryTemporalOperation_node: {
            auto v = reinterpret_cast< UnaryTemporalFormula *>(o);
            v->accept_visitor(*this);
            break;
        }
        case binaryTemporalOperation_node: {
            auto v = reinterpret_cast< BinaryTemporalFormula *>(o);
            v->accept_visitor(*this);
            break;
        }
        default:
            messageError("Unsupported formula.");
            break;
    }
    return 1;
}






