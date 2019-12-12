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

    /// @brief Class representing a Unary Temporal Formula.
    class UnaryTemporalFormula : public LogicFormula {
    public:
        /// @brief Constructor.
        explicit UnaryTemporalFormula(
                TemporalOperator op = op_globally,
                LogicFormula * formula = nullptr
                );

        /// @brief Destructor.
        ~UnaryTemporalFormula() override;

        /// @brief Getter of the operator
        /// @return The Temporal Operator of the object.
        TemporalOperator getOp() const;

        /// @brief Setter of the operator.
        /// @param op The Temporal operator to be used.
        void setOp(TemporalOperator op);

        /// @brief Getter for the first formula.
        /// @return The first formula.
        LogicFormula *getFormula() const;

        /// @brief Setter for the first formula.
        /// @param formula The first formula to be set.
        void setFormula(LogicFormula *formula);

        /// @brief The base class for the visitor pattern.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function producing the text representation of the formula.
        /// @return The string representing the formula.
        std::string getString() override;

    protected:

        /// @brief The temporal operator.
        TemporalOperator _op;

        /// @brief The formula to which apply the operator.
        LogicFormula * _formula;

    };

}
