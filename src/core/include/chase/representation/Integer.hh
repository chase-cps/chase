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
#include "utilities.hh"

namespace chase {

    /// @brief Class describing the Integer type.
    class Integer : public SimpleType
    {
        public:

            /// @brief Constructor.
            Integer();
            /// @brief Constructor with integer range.
            /// @param l The leff bound of the range.
            /// @param r The right bound of the range.
            Integer( int l, int r );

            explicit Integer( Range * r );

            /// @brief Destructor.
            ~Integer() override;

            /// @brief Verifies whether the type is signed.
            /// @return true if signed, false otherwise.
            bool isSigned();

            /// @brief Retrieve the range of the Integer.
            /// @return a pointer to the range.
            Range * getRange();

            /// @brief Main function for visit.
            /// @param v The visitor being used.
            /// @return the return value of the visit.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Function printing the type.
            /// @return the type as a string.
            std::string getString() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Integer * clone() override;


        protected:
            /// @brief True if it is a signed value, false otherwise.
            bool _signed;

            /// @brief The range of values valid for the Integer.
            Range * _range;
    };
}
