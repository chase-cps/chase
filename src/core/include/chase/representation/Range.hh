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

#include "representation/IntegerValue.hh"
#include "representation/RealValue.hh"
#include "Chase.hh"
#include "utilities.hh"

#include <stdint.h>

namespace chase {

    /// @brief Class to represent a range of values.
    class Range : public Value
    {
        public:
            /// @brief Constructor.
            Range();

            /// @brief Destructor.
            ~Range() override;

            /// @brief Specilized constructor.
            /// @param lbound left bound for the range.
            /// @param rbound right bound for the range.
            Range( NumericValue * lbound,  NumericValue * rbound );

            /// @brief Setter for the left value.
            /// @param lbound Left bound to be set for the range.
            void setLeftValue( NumericValue * lbound );

            /// @brief Setter for the left value.
            /// @param rbound Right bound to be set for the range. 
            void setRightValue( NumericValue * rbound );

            /// @brief Getter of the left value.
            /// @return the left value of the range.
            NumericValue * getLeftValue();

            /// @brief Getter of the right value.
            /// @return the right value of the range.
            NumericValue * getRightValue();
           
            /// @brief Constructor from Integers.
            /// @param lbound Left bound of the range.
            /// @param rbound Right bound of the range.
            Range(int lbound, int rbound );

            /// @brief Constructor from Doubles.
            /// @param lbound Left bound of the range.
            /// @param rbound Right bound of the range.
            Range( double lbound, double rbound );

        Type *getType() override;

        /// @brief Printing function.
            /// @return a string representing the range.
            std::string getString() override;

            /// @brief Visiting function.
            /// @param v Visitor visiting the range.
            /// @return  the return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;


        protected:
            /// @brief Left bound of the range.
            NumericValue * _lbound;
            /// @brief Right bound of the range.
            NumericValue * _rbound;

            /// @brief Procedure checking for the consistency of the range.
            void _checkConsistency();
    };
}
