/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "representation.hh"


namespace chase
{
    /// @brief Structure of options for the simplifications.
    /// It allows to specify which simplifications should be carried on.
    typedef struct simplify_options {
        /// @brief Simplify not instances. If True, the simplify function
        /// tries to move the not operators as close as possible to atoms
        /// in logic formulas.
        bool nots;
        /// @brief Simplify temporal operators. If True, the simplify function
        /// group temporal operators. That is:
        /// - [](f) & [](g) becomes [](f & g)
        /// - <>(f) | <>(g) becomes <>(f | g)
        /// - X(f) op X(g) becomes X(f op g) where op is either | or &.
        bool temporal_operators;

        /// @brief Constructor.
        /// @param _nots Set the nots option.
        /// @param _temporal_operators Set the temporal_operators option.
        simplify_options(
                bool _nots = true,
                bool _temporal_operators = true );

    } simplify_options;

    /// @brief Main simplify function.
    /// @param object The object to be simplified.
    /// @param options The structure containing the options set for the
    /// simplification.
    void simplify(
            chase::ChaseObject * object,
            simplify_options * options = new simplify_options());
}