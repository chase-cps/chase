/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"
#include "chase/representation/LogicFormula.hh"


namespace chase {

    /// @brief Class representing a logic proposition.
    class Proposition : public LogicFormula {
    public:
        /// @brief Constructor. It is preferable to use the ad-hoc constructor
        /// specifying the value linked to the proposition.
        Proposition();

        /// @brief Constructor with value.
        /// @param v The value to be linked to the proposition.
        /// The value must be boolean.
        explicit Proposition( Value * v );

        /// @brief Destructor.
        ~Proposition() override;

        /// @brief Return the type of the proposition. It always returns
        /// a boolean.
        /// @return The type of the proposition, i.e., Boolean.
        Type * getType();

        /// @brief Returns the value linked to the proposition.
        /// @return Value linked to the proposition.
        Value * getValue();

        /// @brief Returns the name of the proposition.
        /// @return the name of the proposition.
        Name * getName();

        /// @brief Set the value linked to the proposition. It should not be
        /// used. Better using the ad-hoc constructor.
        /// @param v The value to link to the proposition.
        void setValue( Value * v );

        /// @brief Set the name of the Proposition.
        /// @param n The name of the proposition.
        void setName( Name * n );

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be applied during the visit.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the proposition.
        /// @return The name of the proposition as a string.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the object.
        Proposition * clone() override;

    protected:

        /// @brief the Type of the Proposition. I.e., Boolean.
        Type * _type;

        /// @brief The value to which the proposition is connected.
        /// It must be typed boolean. E.g., an expression or a
        /// boolean variable.
        Value * _value;

        /// @brief The name of the proposition.
        Name * _name;

    };
}
