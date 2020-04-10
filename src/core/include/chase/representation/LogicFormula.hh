/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        9/3/2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/Specification.hh"

namespace chase
{
    /// @brief Virtual class for all the atoms for the logics.
    class LogicFormula : public Specification {
    public:
        /// @brief Constructor.
        LogicFormula()= default;

        /// @brief Destructor.
        ~LogicFormula() override= default;

        /// @brief Clone method.
        /// @return Clone of the object.
        LogicFormula * clone() override = 0;

    protected:
    };

}