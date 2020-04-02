/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "Chase.hh"

namespace chase {

    /// @brief The class represent all the Boolean unary operations.
    class UnaryBooleanFormula : public LogicFormula {

    using sptr_logicform = std::shared_ptr<LogicFormula>;
    using sptr_ubinform = std::shared_ptr<UnaryBooleanFormula>;

    public:
        /// @brief Constructor.
        explicit UnaryBooleanFormula(BooleanOperator op = op_not,
                                     sptr_logicform op1 = nullptr );

        /// @brief Destructor.
        ~UnaryBooleanFormula() override;

        /// @brief Getter for the Operator.
        /// @return The operator.
        BooleanOperator getOp() const;

        /// @brief Setter of the Operator.
        /// @param op The operator to be set.
        void setOp(BooleanOperator op);

        /// @brief Getter of the Operand.
        /// @return the Operand.
        sptr_logicform getOp1() const;

        /// @brief Setter of the Operand.
        /// @param op1 The operand to be set.
        void setOp1(sptr_logicform op1);

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be applied during the visit.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the operation.
        /// @return The operation as a string.
        std::string getString() override;

        /// @brief Clone method.
        /// @return The cloned object.
        sptr_ubinform clone();

    protected:

        /// @brief The temporal operator.
        BooleanOperator _op;

        /// @brief The formula to which apply the operator.
        sptr_logicform _op1;

    };

}