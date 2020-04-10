/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/6/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Operators.hh"
#include "LogicFormula.hh"
#include "Chase.hh"

#include <vector>

namespace chase {

    /// @brief The class represent the large operators of the boolean algebra.
    /// That is, the operators used to express a sequence of operations. For
    /// instance, the large And/Or operators.
    class LargeBooleanFormula : public LogicFormula {
    public:

        /// @brief The sequence of operands of the big operation.
        std::vector< LogicFormula * > operands;

        /// @brief Constructor.
        LargeBooleanFormula(BooleanOperator op = op_and );

        /// @brief Destructor.
        ~LargeBooleanFormula();

        /// @brief Accessor to the operator.
        /// @return The opeartor.
        BooleanOperator getOp() const;

        /// @brief Setter for the operator.
        /// @param op The operator to set.
        void setOp(BooleanOperator op);

        /// @brief Function to add a operand in the formula. It manages also the
        /// parent link.
        /// @param f The operand to add.
        void addOperand( LogicFormula *f );

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be used.
        /// @return the return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the operation.
        /// @return the string representing the operation.
        std::string getString() override;

        /// @brief Clone method.
        /// @return The cloned object.
        LargeBooleanFormula * clone() override;

    protected:

        /// @brief The operator.
        BooleanOperator _op;

    };

}
