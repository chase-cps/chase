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

#include "representation/SimpleType.hh"

namespace chase {

    /// @brief Class describing the Boolean type.
    class Boolean : public SimpleType
    {
        public:

            /// @brief Constructor.
            Boolean();
            /// @brief Destructor.
            ~Boolean();

            /// @brief Main function for visit.
            /// @param v The visitor being used.
            /// @return the return value of the visit.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Function printing the type.
            /// @return the type as a string.
            std::string getString() override;


            /// @brief Clone method.
            /// @return A clone of the object.
            Boolean * clone() override;


        protected:

    };
}
