/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include <chase/representation/LogicFormula.hh>

namespace chase {

    /// @brief Class representing a boolean constant.
    class BooleanConstant : public LogicFormula {
    public:
        /// @brief Constructor.
        explicit BooleanConstant(bool value = true);

        /// @brief Destructor.
        ~BooleanConstant() override;

        /// @brief Getter of the value.
        /// @return The truth value of the constant.
        bool getValue();

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be used for the visit.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function printing the constant.
        /// @return the string TRUE if true, FALSE otherwise.
        std::string getString() override;

        /// @brief Clone method.
        /// @return The cloned object.
        BooleanConstant * clone() override;

    protected:

        /// @brief The value of the constant.
        bool _value;

    };

}
