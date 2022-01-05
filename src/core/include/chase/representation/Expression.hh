/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#pragma once

#include "representation/Value.hh"
#include "representation/Operators.hh"
#include "representation/Type.hh"
#include "representation/Boolean.hh"
#include "utilities.hh"


namespace chase {

    /// @brief Unary or binary mathematical expression.
    class Expression : public Value
    {
        public:

            /// @brief Constructor.
            Expression();

            /// @brief Constructor.
            /// @param op The operator of the expression.
            /// @param op1 The first operand of the expression.
            /// @param op2 The second operand of the expression.
            Expression(Operator op, Value *op1, Value *op2 = nullptr );

            /// @brief Destructor.
            virtual ~Expression();

            /// @brief Getter of the Operator.
            /// @return Operator of the expression.
            Operator getOperator();

            /// @brief Getter of the first Operand.
            /// @return The first operand.
            Value * getOp1();

            /// @brief Getter of the second Operand.
            /// @return The second operand.
            Value * getOp2();

            /// @brief Setter of the Operator.
            /// param op Operator to be setted.
            void setOperator(Operator op );

            /// @brief Setter for the first operator.
            /// @param op First operand to be set.
            void setOp1( Value * op );

            /// @brief Setter for the second operator.
            /// @param op Second operand to be set.
            void setOp2( Value * op );

            /// @brief Print function.
            /// @return The string representing the Expression.
            std::string getString() override;

            /// @brief Main visiting function.
            /// @param v The visitor visiting the identifier.
            /// @return The return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Function returning the type of the expression.
            /// @return The type of the function.
            Type * getType() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Expression * clone() override;

        protected:

            /// @brief Operator.
            /// @todo Operators shound be divided in operational operators, and
            /// relational operators to enable relation definition.
            Operator _op;

            /// @brief First operand.
            Value * _op1;
            /// @brief First operand.
            Value * _op2;

    };
}
