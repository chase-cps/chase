/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include <map>

#include "representation/ComponentDefinition.hh"
#include "representation/Name.hh"
#include "representation/Value.hh"
#include "representation/Scope.hh"
#include "utilities.hh"

namespace chase {

    /// @brief Class representing a component instance. The component definition
    /// is represented by object of the ComponentInstance class.
    class Component : public ChaseObject {
    public:

        /// @brief Constructor.
        /// @param definition
        /// @param name
        Component( ComponentDefinition * definition, std::string name );

        /// @brief Destructor.
        ~Component();

        ComponentDefinition *getDefinition() const;
        void setDefinition(ComponentDefinition *definition);

        Name *getName() const;
        void setName(Name *name);

        void setParameter(std::string view, std::string param, Value * value);
        Value * getParameterValue(std::string view, std::string param);
        std::map< std::string, Value * >&
                        getParametersInView(std::string view);

        int accept_visitor(chase::BaseVisitor &v) override;

        std::string getString() override;

        Component *clone() override;

    protected:

        /// @brief Map of the parameters.
        /// The key of the outer map identifies the view in the
        /// ComponentDefinition; the key of the inner map identifies the name
        /// of the parameter; the value of the inner map is the value to
        /// assign at the parameter.
        /// \todo Check whether this managing of parameters is correct.
        std::map<
                std::string,
                std::map< std::string, chase::Value * > > _params;

        ComponentDefinition * _definition;
        Name * _name;

    };

}
