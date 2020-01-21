/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/LogicFormula.hh"
#include "representation/Operators.hh"

namespace chase {

    /// @brief Class representing the diamond or the square modal operations.
    class ModalFormula : public LogicFormula {
    public:
        /// @brief Constructor.
        /// @param op The operator of the operation, i.e., diamond or square.
        /// @param formula The logic formula within the Modal Operation.
        explicit ModalFormula(
                ModalOperator op = op_square,
                LogicFormula * formula = nullptr );

        /// @brief Destructor.
        ~ModalFormula() override;

        /// @brief Getter of the operator.
        /// @return The modal operator of the formula.
        ModalOperator getOperator();

        /// @brief Setter of the operator.
        /// @param op The modal operator to set.
        void setOperator(ModalOperator op);

        /// @brief Getter of the formula.
        /// @return Pointer to the formula.
        LogicFormula * getFormula();

        /// @brief Setter of the formula.
        /// @param formula The formula to be set.
        void setFormula(LogicFormula * formula );


        int accept_visitor(chase::BaseVisitor &v) override;
        std::string getString() override;

        /// @brief Clone method.
        /// @return The cloned object.
        ModalFormula * clone() override;

    protected:

        /// @brief The operator.
        ModalOperator _operator;
        /// @brief The formula to which the operator applies.
        LogicFormula * _formula;

    };

}
