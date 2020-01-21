/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/LogicFormula.hh"
#include "representation/Operators.hh"

namespace chase {

    /// @brief The class represent all the Boolean binary operations.
    class BinaryBooleanFormula : public LogicFormula {
    public:
        /// @brief Constructor.
        explicit BinaryBooleanFormula(BooleanOperator op = op_and,
                                      LogicFormula * op1 = nullptr,
                                      LogicFormula * op2 = nullptr );

        /// @brief Destructor.
        ~BinaryBooleanFormula() override;

        /// @brief Getter for the Operator.
        /// @return The operator.
        BooleanOperator getOp() const;

        /// @brief Setter of the Operator.
        /// @param op The operator to be set.
        void setOp(BooleanOperator op);

        /// @brief Getter of the first operand.
        /// @return the first operand.
        LogicFormula *getOp1() const;

        /// @brief Setter of the first operand.
        /// @param op1 The first operand to be set.
        void setOp1(LogicFormula *op1);

        /// @brief Getter of the second operand.
        /// @return the second operand.
        LogicFormula *getOp2() const;

        /// @brief Setter of the second operand.
        /// @param op2 The second operand to be set.
        void setOp2(LogicFormula *op2);

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be applied during the visit.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the operation.
        /// @return The operation as a string.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the object.
        BinaryBooleanFormula * clone() override;

    protected:

        /// @brief The operator.
        BooleanOperator _op;
        /// @brief The first operand.
        LogicFormula * _op1;
        /// @brief The second operand.
        LogicFormula * _op2;

    };

}