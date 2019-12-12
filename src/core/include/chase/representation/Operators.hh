/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#pragma once

#include "utilities.hh"

namespace chase {
    /// @brief Enumeration of Mathematical operators.
    /// @todo to be completed with all the possible operators.
    enum Operator
    {
        op_none,
        op_plus,
        op_minus,
        op_multiply,
        op_divide,
        op_mod,
        op_eq,
        op_neq,
        op_lt,
        op_gt,
        op_le,
        op_ge
    };

    /// @brief Enumeration of the Boolean operators.
    enum BooleanOperator
    {
        op_not,
        op_and,
        op_or,
        op_implies,
        op_iff,
        op_xor,
        op_nand,
        op_nor,
        op_xnor
    };

    /// @brief Enumeration of the Modal operators.
    enum ModalOperator
    {
        op_square,
        op_diamond
    };

    /// @brief Enumeration of the temporal operators.
    enum TemporalOperator
    {
        op_globally,
        op_future,
        op_next,
        op_until,
        op_release
    };

    /// @brief Function printing as a string an Operator object.
    /// @param op The operator to convert in string.
    /// @return The string correspondent to the operator.
    std::string to_string(Operator op );

    /// @brief Function printing as a string an BooleanOperator object.
    /// @param op The operator to convert in string.
    /// @return The string correspondent to the operator.
    std::string to_string(BooleanOperator op );

    /// @brief Function printing as a string an ModalOperator object.
    /// @param op The operator to convert in string.
    /// @return The string correspondent to the operator.
    std::string to_string(ModalOperator op );

    /// @brief Function printing as a string an TemporalOperator object.
    /// @param op The operator to convert in string.
    /// @return The string correspondent to the operator.
    std::string to_string(TemporalOperator op );

}

