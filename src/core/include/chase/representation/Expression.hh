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
        using sptr_type = std::shared_ptr<Type>;
        using sptr_value = std::shared_ptr<Value>;
        using sptr_exp = std::shared_ptr<Expression>;
        
        public:

            /// @brief Constructor.
            Expression();

            /// @brief Constructor.
            /// @param op The operator of the expression.
            /// @param op1 The first operand of the expression.
            /// @param op2 The second operand of the expression.
            Expression(Operator op, sptr_value op1, 
                sptr_value op2 = nullptr );

            /// @brief Destructor.
            virtual ~Expression();

            /// @brief Getter of the Operator.
            /// @return Operator of the expression.
            Operator getOperator();

            /// @brief Getter of the first Operand.
            /// @return The first operand.
            sptr_value getOp1();

            /// @brief Getter of the second Operand.
            /// @return The second operand.
            sptr_value getOp2();

            /// @brief Setter of the Operator.
            /// param op Operator to be setted.
            void setOperator(Operator op );

            /// @brief Setter for the first operator.
            /// @param op First operand to be set.
            void setOp1( sptr_value op );

            /// @brief Setter for the second operator.
            /// @param op Second operand to be set.
            void setOp2( sptr_value op );

            /// @brief Print function.
            /// @return The string representing the Expression.
            std::string getString() override;

            /// @brief Main visiting function.
            /// @param v The visitor visiting the identifier.
            /// @return The return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Function returning the type of the expression.
            /// @return The type of the function.
            sptr_type getType() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            sptr_exp clone();

        protected:

            /// @brief Operator.
            Operator _op;

            /// @brief First operand.
            sptr_value _op1;
            /// @brief First operand.
            sptr_value _op2;

    };
}
