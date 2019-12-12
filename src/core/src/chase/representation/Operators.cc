/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Operators.hh"
#include "utilities.hh"

using namespace chase;

std::string chase::to_string(chase::Operator op )
{
    switch( op )
    {
        case op_none:
            chase::messageWarning("Trying to print a empty operator." );
            return std::string(" NONE ");
            break;
        case op_plus:
            return std::string("+");
            break;
        case op_minus:
            return std::string("-");
            break;
        case op_multiply:
            return std::string("*");
            break;
        case op_divide:
            return std::string("/");
            break;
        case op_mod:
            return std::string("%");
            break;
        case op_eq:
            return std::string("=");
            break;
        case op_neq:
            return std::string("!=");
            break;
        case op_lt:
            return std::string("<");
            break;
        case op_gt:
            return std::string(">");
            break;
        case op_le:
            return std::string("<=");
            break;
        case op_ge:
            return std::string(">=");
            break;
        default:
            messageError("Trying to print a wrong operator.");
            return std::string("");
    }
}

std::string chase::to_string(BooleanOperator op) {
    std::string ret;
    switch(op)
    {
        case op_not:
            ret = std::string("!");
            break;
        case op_and:
            ret = std::string(" /\\ ");
            break;
        case op_or:
            ret = std::string(" \\/ ");
            break;
        case op_implies:
            ret = std::string(" -> ");
            break;
        case op_iff:
            ret = std::string(" <-> ");
            break;
        case op_xor:
            ret = std::string(" XOR ");
            break;
        case op_nand:
            ret = std::string(" NAND ");
            break;
        case op_nor:
            ret = std::string(" NOR ");
            break;
        case op_xnor:
            ret = std::string(" XNOR ");
            break;
        default:
            messageError("Wrong operator (Boolean)");
            break;
    }
    return ret;
}

std::string chase::to_string(ModalOperator op)
{
    std::string ret;
    switch(op)
    {
        case op_square:
            ret = std::string("[]");
            break;
        case op_diamond:
            ret = std::string("<>");
            break;
        default:
            messageError("Invalid Operator (Modal).");
            break;
    }

    return ret;
}

std::string chase::to_string(TemporalOperator op) {
    std::string ret;
    switch(op)
    {
        case op_globally:
            ret = std::string("G");
            break;
        case op_future:
            ret = std::string("F");
            break;
        case op_next:
            ret = std::string("X");
            break;
        case op_until:
            ret = std::string(" U ");
            break;
        case op_release:
            ret = std::string(" R ");
            break;
    }
    return ret;
}
