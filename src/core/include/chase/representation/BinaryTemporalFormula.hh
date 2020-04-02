/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/LogicFormula.hh"
#include "Chase.hh"

namespace chase {

    /// @brief Class representing a binary temporal formula.
    class BinaryTemporalFormula : public LogicFormula {

    using sptr_bintempform = std::shared_ptr<BinaryTemporalFormula>;
    using sptr_logicform = std::shared_ptr<LogicFormula>;
    
    public:
        /// @brief Constructor.
        BinaryTemporalFormula(
                TemporalOperator op = op_until,
                sptr_logicform op1 = nullptr,
                sptr_logicform op2 = nullptr
                );

        /// @brief Destructor.
        ~BinaryTemporalFormula() override;

        /// @brief Getter of the operator
        /// @return The Temporal Operator of the object.
        TemporalOperator getOp() const;

        /// @brief Setter of the operator.
        /// @param op The Temporal operator to be used.
        void setOp(TemporalOperator op);

        /// @brief Getter for the first formula.
        /// @return The first formula.
        sptr_logicform getFormula1() const;

        /// @brief Setter for the first formula.
        /// @param formula1 the first formula.
        void setFormula1( sptr_logicform formula1);

        /// @brief Getter for the second formula.
        /// @return The second formula.
        sptr_logicform getFormula2() const;

        /// @brief Setter for the second formula.
        /// @param formula2 the second formula.
        void setFormula2( sptr_logicform formula2);

        /// @brief The base class for the visitor pattern.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function producing the text representation of the formula.
        /// @return The string representing the formula.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the object.
        sptr_bintempform clone();

    protected:

        /// @brief The operator.
        TemporalOperator _op;
        /// @brief The first operand.
        sptr_logicform _formula1;
        /// @brief The second operand.
        sptr_logicform _formula2;

    };

}
