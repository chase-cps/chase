/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/14/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/Declaration.hh"
#include <list>

namespace chase {

    /// @brief Abstract class for all the object with scope, i.e., system,
    /// contract, component, etc.
    /// \todo Bind it in the Python interface. Remember, also the sub-classes
    /// must be checked.
    class Scope : public ChaseObject {
    public:
        /// @brief List of declarations.
        std::list< Declaration * > declarations;

    protected:

    };
}