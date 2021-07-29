/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "representation/LogicFormula.hh"
#include "representation/Interval.hh"
#include "Chase.hh"

namespace chase {

    /// @brief Class representing a Unary Temporal Formula.
    class UnaryTemporalFormula : public LogicFormula {
    public:
        /// @brief Constructor.
        explicit UnaryTemporalFormula(
                TemporalOperator op = op_globally,
                LogicFormula * formula = nullptr,
                Interval * interval = nullptr
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

        /// @brief Getter of the interval in which the temporal operator holds.
        /// Important: nullptr is to be interpreted as a [0, +Inf) interval.
        /// @return Interval of the temporal operator.
        /// Important: nullptr is to be interpreted as a [0, +Inf) interval.
        Interval *getInterval() const;

        /// @brief Setter of the interval in which the temporal operator holds.
        /// Important: nullptr is to be interpreted as a [0, +Inf) interval.
        /// @param interval Interval of the temporal operator.
        /// Important: nullptr is to be interpreted as a [0, +Inf) interval.
        void setInterval(Interval *interval);

        /// @brief The base class for the visitor pattern.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function producing the text representation of the formula.
        /// @return The string representing the formula.
        std::string getString() override;

        /// @brief Clone method.
        /// @return The cloned object.
        UnaryTemporalFormula * clone() override;

    protected:

        /// @brief The temporal operator.
        TemporalOperator _op;

        /// @brief The formula to which apply the operator.
        LogicFormula * _formula;

        /// @brief Interval in which the temporal operator holds.
        /// Notice: nullptr is to be interpreted as a [0, +Inf) interval.
        Interval * _interval;

    };

}
