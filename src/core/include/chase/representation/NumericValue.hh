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
#include "representation/Type.hh"
#include "utilities.hh"

namespace chase {
    
    /// @brief Abstract class for all the numeric values (e.g., Integers, Doubles, etc.).
    class NumericValue : public Value
    {
        public:

            /// @brief Numericructor.
            NumericValue();
            /// @brief Destructor.
            ~NumericValue() override;

            NumericValue * clone() override = 0;

        protected:
    };


}

