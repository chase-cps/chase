/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Declaration.hh"
#include "Scope.hh"
#include <map>
#include <set>


namespace chase {

    /// @brief Forward declaration of the Component class.
    class Component;

    /// @brief Class representing the definition of a component.
    class ComponentDefinition : public Scope {
    public:

        /// @brief Set of views of the component.
        std::map<std::string, Contract *> views;


        /// @brief Set of subcomponents of the component.
        /// Implements the hierarchy.
        std::set< Component * > subcomponents;

        /// @brief Constructor.
        ComponentDefinition();

        /// @brief Constructor.
        /// @param name The name of the component definition.
        explicit ComponentDefinition(Name *name);

        /// @brief Constructor.
        /// @param name The name of the component definition (string).
        explicit ComponentDefinition(std::string name);

        /// @brief Destructor.
        ~ComponentDefinition() override;

        /// @brief Function to accept a visitor.
        /// @param v The visitor to be accepted.
        /// @return The default return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function transforming the definition into a string.
        /// @return The string representation of the declaration.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A copy of the declaration.
        ComponentDefinition *clone() override;

    protected:

    };



}
