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

    /// @brief Class describing the Real type.
    class Real : public SimpleType
    {
        public:

            /// @brief Constructor.
            Real();
            /// @brief Destructor.
            ~Real() override;

            /// @brief Retrieve the range of the Real.
            /// @return a pointer to the range.
            Range * getRange();

            /// @brief Set the range of the Real.
            /// @param r range for the real.
            void setRange( Range * r );

            /// @brief Main function for visit.
            /// @param v The visitor being used.
            /// @return the return value of the visit.
            int accept_visitor( BaseVisitor &v ) override;


            /// @brief Function printing the type.
            /// @return the type as a string.
            std::string getString() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Real * clone() override ;

        protected:
            /// @brief Range of value for the type.
            Range * _range;
    };
}
