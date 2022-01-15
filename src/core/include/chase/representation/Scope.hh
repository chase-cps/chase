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
    class Scope : public Declaration {
    public:
        /// @brief List of declarations.
        std::list< Declaration * > declarations;

        virtual /// @brief Function to safely add a declaration to the contract.
        /// The function takes care of managing the parent link to the contract.
        /// @param declaration A pointer to the declaration to add.
        void addDeclaration(Declaration * declaration);

        Scope(Name *n);
        Scope(std::string name);

        Scope();

    protected:

    };
}
