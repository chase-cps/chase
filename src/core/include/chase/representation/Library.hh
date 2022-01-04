/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/3/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/Scope.hh"
#include <set>

namespace chase {

    class Library : public Scope {
    public:

        /// @brief Constructor.
        Library();
        /// @brief Constructor.
        /// @param name The name as a string.
        Library(const std::string &name);
        /// @brief Constructor.
        /// @param name The name as an object.
        Library(Name *n);

        /// @brief Destructor.
        ~Library() override;

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        Library *clone() override;

    protected:

    };

}