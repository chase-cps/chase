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

#include "representation/Declaration.hh"
#include "representation/Type.hh"

namespace chase {

    /// @brief Abstract class for all the object representing the declaration
    /// of data. E.g., Variables and constants declarations.
    class DataDeclaration : public Declaration
    {
        public:

            /// @brief Constructor.
            /// @param t Type of the declared object.
            /// @param n Name of the declared object.
            explicit DataDeclaration( Type * t = nullptr, Name * n = nullptr);

            /// @brief Getter for the type.
            /// @return The type of the declaration.
            Type * getType();

            /// @brief Setter for the type.
            /// @param t The type of the declared object.
            void setType( Type * t );

            /// @brief Clone method.
            /// @return Clone of the object.
            DataDeclaration * clone() override = 0;

        protected:

            /// @brief The type of the object represented by the declaration.
            Type * _type;

    };
}

