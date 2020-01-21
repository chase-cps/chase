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

#include "representation/NumericValue.hh"
#include "utilities.hh"

namespace chase {

    /// @brief Class to represent Integer values.
    class IntegerValue : public NumericValue
    {
        public:
            /// @brief Constructor.
            /// @param value Value of the integer.
            explicit IntegerValue( const int64_t value = 0);
            /// @brief Destructor.
            ~IntegerValue() override;

            /// @brief Accessor to the value.
            /// @return the Value of the Integer.
            int64_t getValue();

            /// @brief Setter for the value.
            /// @param v value to be set.
            void setValue( int64_t v );

            /// @brief Copy constructor.
            /// @param o object to copy.
            IntegerValue( const IntegerValue & o );
            
            /// @brief Assignment operator.
            /// @param o the object to compare.
            /// @return true if they are equal.
            IntegerValue & operator=( const IntegerValue & o );

            /// @brief Accept Visitor.
            int accept_visitor(chase::BaseVisitor &v ) override;

            /// @brief Print function.
            std::string getString() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            IntegerValue * clone() override;

        protected:
            /// @brief Stored value.
            int64_t _value;

    };
}
