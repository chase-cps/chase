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

    /// @brief Class for nodes representing real values.
    class RealValue : public NumericValue
    {
        public:
            /// @brief Constructor.
            /// @param value Value of the integer.
            explicit RealValue( const double value = 0);

            /// @brief Destructor.
            ~RealValue() override;

            /// @brief Accessor to the value.
            /// @return the Value of the Real.
            double getValue();

            /// @brief Setter for the value.
            /// @param v value to be set.
            void setValue( double v );

            /// @brief Copy constructor.
            /// @param o object to copy.
            RealValue( const RealValue & o );
            
            /// @brief Assignment operator.
            /// @param o the object to compare.
            /// @return true if they are equal.
            RealValue & operator=( const RealValue & o );

            /// @brief Accept Visitor.
            int accept_visitor(chase::BaseVisitor &v ) override;

            /// @brief Print function.
            std::string getString() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            RealValue * clone() override;

        protected:
            /// @brief Value stored.
            double _value;

    };


}
