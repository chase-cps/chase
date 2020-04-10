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

    /// @brief Class storing a Boolean value.
    class BooleanValue : public NumericValue
    {
        public:
            /// @brief Constructor.
            /// @param value Value of the integer.
            explicit BooleanValue( bool value = false);
            /// @brief Destructor.
            ~BooleanValue() override;

            /// @brief Accessor to the value.
            /// @return the Value of the Boolean.
            bool getValue();

            /// @brief Setter for the value.
            /// @param v value to be set.
            void setValue( bool v );

            /// @brief Copy constructor.
            /// @param o object to copy.
            BooleanValue( const BooleanValue & o );
            
            /// @brief Assignment operator.
            /// @param o the object to compare.
            /// @return true if they are equal.
            BooleanValue & operator=( const BooleanValue & o );

            /// @brief Accept Visitor.
            int accept_visitor(chase::BaseVisitor &v ) final;

            /// @brief Print function.
            std::string getString() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            BooleanValue * clone() override;

        protected:
            /// @brief Stored boolean value.
            bool _value;

    };
}
