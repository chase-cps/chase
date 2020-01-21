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
#include "representation/DataDeclaration.hh"

namespace chase {

    /// @brief Class describing the instance of a declared object.
    class Identifier : public Value
    {
        public:
            /// @brief Constructor.
            /// @param d Declaration of the identifier.
            explicit Identifier( DataDeclaration * d = nullptr );

            /// @brief Copy constructor.
            Identifier( const Identifier &i );

            /// @brief Destructor.
            ~Identifier() override;

            /// @brief Assignment operator.
            Identifier & operator=( const Identifier &i );

            /// @brief Access function to the declaration.
            /// @return A pointer to the declaration object.
            DataDeclaration * getDeclaration();

            /// @brief Setter for the declaration.
            /// @param d Declaration to be set.
            void setDeclaration( DataDeclaration * d);

            /// @brief Print function.
            /// @return The string representing the Identifier.
            std::string getString() override;

            Type * getType() override;

            /// @brief Main visiting function.
            /// @param v The visitor visiting the identifier.
            /// @return The return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            Identifier * clone() override;

        protected:

            /// @brief Declaration.
            // It MUST be a pointer to a object ALREADY present in the AST.
            // Otherwise, the declaration is not valid.
            DataDeclaration * _declaration;
    };

}
