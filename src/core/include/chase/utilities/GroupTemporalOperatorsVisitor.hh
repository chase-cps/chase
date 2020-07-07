/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/27/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "LogicSimplificationVisitor.hh"

namespace chase {

    /// @brief Visitor taking care of performing simplifications over temporal
    /// operators. It performs the following simplifications:
    /// - [](f) & [](g) becomes [](f & g)
    /// - <>(f) | <>(g) becomes <>(f | g)
    /// - X(f) op X(g) becomes X(f op g) where op is either | or &.
    class GroupTemporalOperatorsVisitor : public LogicSimplificationVisitor {
    public:
        /// @brief Constructor.
        GroupTemporalOperatorsVisitor();

        /// @brief Destructor.
        ~GroupTemporalOperatorsVisitor() override;

    protected:
        /// @brief Interface for the main optimization function.
        /// @param formula The formula to analyze and, eventually, simplify.
        /// @return Pointer to the simplified formula.
        LogicFormula *_analyzeFormula(LogicFormula *formula) override;

    };

}
