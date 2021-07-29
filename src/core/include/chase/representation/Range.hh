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


    /// @deprecated This class has been deprecated as of July 2021.
    /// @brief Class to represent a range of values.
    class Range : public Value
    {
        public:
            /// @brief Constructor.
            Range();

            /// @brief Destructor.
            ~Range() override;

            /// @brief Constructor from Integers.
            /// @param lbound Left bound of the range.
            /// @param rbound Right bound of the range.
            Range(int lbound, int rbound );

            Type *getType() override;


            /// @brief Setter for the left bound.
            /// @param lbound The left bound.
            void setLeftValue(int lbound);
            /// @brief Setter for the right bound.
            /// @param rbound The right bound.
            void setRightValue(int rbound);
            /// @brief Getter of the left Value.
            /// @return The range left bound.
            int getLeftValue();
            /// @brief Getter of the right value.
            /// @brief The range right bound.
            int getRightValue();


            /// @brief Printing function.
            /// @return a string representing the range.
            std::string getString() override;

            /// @brief Visiting function.
            /// @param v Visitor visiting the range.
            /// @return  the return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Range * clone() override;


        protected:
            /// @brief Left bound of the range.
            int _lbound;
            /// @brief Right bound of the range.
            int _rbound;

            /// @brief Procedure checking for the consistency of the range.
            void _checkConsistency();



    };
}
