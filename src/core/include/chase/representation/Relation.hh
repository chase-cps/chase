/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/Value.hh"
#include "chase/representation/Expression.hh"

namespace chase {

    /// @brief Main class to represent a relation.
    /// @todo Enable relation definition (acting on the operator).
    /// Operators shound be divided in operational operators, and
    //  relational operators to enable relation definition.
    class Relation : public Value {
    public:

        /// @brief
        std::vector< Value * > elements;

        /// @brief Constructor.
        Relation();

        /// @brief Destructor.
        ~Relation();

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        Relation *clone() override;

        /// @brief Getter of the Operator.
        /// @return The operator.
        Operator getOperator() const;
        /// @brief Setter of the operator.
        /// @param op The operator.
        void setOperator(Operator op);


    protected:

        /// @brief Operator of the Relation.
        Operator _operator;


    };

}