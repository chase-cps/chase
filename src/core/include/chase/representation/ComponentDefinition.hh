/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Declaration.hh"
#include <map>


namespace chase {

    class Component;

    class ComponentDefinition : public Declaration {
    public:

        /// @brief Set of views of the component.
        std::map<std::string, Contract *> views;

        /// @brief Constructor.
        ComponentDefinition();

        /// @brief Constructor.
        /// @param name The name of the component definition.
        ComponentDefinition(Name *name);

        /// @brief Constructor.
        /// @param name The name of the component definition (string).
        ComponentDefinition(std::string name);

        /// @brief Destructor.
        ~ComponentDefinition();

        int accept_visitor(chase::BaseVisitor &v) override;

        std::string getString() override;

        ComponentDefinition *clone() override;

    protected:

    };



}
