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

#include "chase/representation/Type.hh"

namespace chase {

    /// @brief Abstract class for simple types, i.e., all those types which
    /// definition does not require other types.
    class SimpleType : public Type
    {
        public:
            /// @brief Constructor.
            SimpleType();

            /// @brief Destructor.
            virtual ~SimpleType();

            /// @brief Clone method.
            /// @return Clone of the object.
            SimpleType * clone() override = 0;
    };
}
