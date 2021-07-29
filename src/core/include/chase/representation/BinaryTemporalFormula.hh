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

    /// @brief Class representing a binary temporal formula.
    class BinaryTemporalFormula : public LogicFormula {
    public:
        /// @brief Constructor.
        BinaryTemporalFormula(
                TemporalOperator op = op_until,
                LogicFormula * op1 = nullptr,
                LogicFormula * op2 = nullptr,
                Interval * interval = nullptr
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
        LogicFormula *getFormula1() const;

        /// @brief Setter for the first formula.
        /// @param formula1 the first formula.
        void setFormula1(LogicFormula *formula1);

        /// @brief Getter for the second formula.
        /// @return The second formula.
        LogicFormula *getFormula2() const;

        /// @brief Setter for the second formula.
        /// @param formula2 the second formula.
        void setFormula2(LogicFormula *formula2);

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
        /// @return A clone of the object.
        BinaryTemporalFormula * clone() override;

    protected:

        /// @brief The operator.
        TemporalOperator _op;
        /// @brief The first operand.
        LogicFormula * _formula1;
        /// @brief The second operand.
        LogicFormula * _formula2;

        /// @brief Interval in which the temporal operator holds.
        /// Notice: nullptr is to be interpreted as a [0, +Inf) interval.
        Interval * _interval;

    };

}
