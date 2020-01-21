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

#include "representation/DataDeclaration.hh"

namespace chase {

    /// @brief Class representing a Constant declaration.
    class Constant : public DataDeclaration
    {
        public:
            /// @brief Constructor.
            /// @param type The name of the declared constant.
            /// @param name The name of the declared constant.
            /// @param value The value of the declared constant.
            explicit Constant(
                    Type * type = nullptr,
                    Name * name = nullptr,
                    Value * value = nullptr );
            
            /// @brief Destructor.
            ~Constant() override;

            /// @brief Copy constructor.
            /// @param o Object to be copied.
            Constant( const Constant &o );

            /// @brief Assignment operator.
            /// @param o Object to be assigned.
            /// @return
            Constant & operator=( const Constant &o );

            /// @brief Getter for the value.
            /// @return A pointer to the value.
            Value * getValue();

            /// @brief Setter for the value.
            /// @param value A pointer to the value.
            void setValue(Value * value);

            /// @brief Function to print the constant.
            /// @return The textual representation of the constant.
            std::string getString() override;

            /// @brief Main function for the visit.
            /// @param v The visitor visiting the constant.
            /// @return The return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Constant * clone() override;

        protected:
            /// @brief Value of the constant.
            Value * _value;
    };
}
