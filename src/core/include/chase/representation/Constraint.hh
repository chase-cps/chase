/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/Specification.hh"
#include "chase/representation/Expression.hh"

namespace chase {

    /// @brief Constraint as a specification object.
    class Constraint : public Specification {
    public:
        /// @brief Constructor.
        Constraint();

        /// @brief Destructor.
        ~Constraint() override;

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        Constraint *clone() override;

        /// @brief Getter of the expression specifying the constraint.
        /// @return Pointer to the expression.
        Expression *getExpression() const;

        /// @brief Setter of the expression specifying the constraint.
        /// @param expression Pointer to the expression specifying the constraint.
        void setExpression(Expression *expression);


    protected:

        /// @brief Expression
        Expression * _expression{};

    };

}