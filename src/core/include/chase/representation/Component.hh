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
#include "utilities.hh"

namespace chase {

    class Component : public ChaseObject {
        
    using sptr_compdef = std::shared_ptr<ComponentDefinition>;
    using sptr_name = std::shared_ptr<Name>;
    using sptr_value = std::shared_ptr<Value>;
    using sptr_comp = std::shared_ptr<Component>;

    public:

        /// @brief Constructor.
        /// @param definition
        /// @param name
        Component( sptr_compdef definition, std::string name );

        /// @brief Destructor.
        ~Component();

        sptr_compdef getDefinition() const;
        void setDefinition(sptr_compdef definition);

        sptr_name getName() const;
        void setName(sptr_name name);

        void setParameter(std::string view, std::string param, sptr_value value);
        sptr_value getParameterValue(std::string view, std::string param);
        std::map< std::string, Value * >&
                        getParametersInView(std::string view);

        int accept_visitor(chase::BaseVisitor &v) override;

        std::string getString() override;

        sptr_comp clone();

    protected:

        /// @brief Map of the parameters.
        /// The key of the outer map identifies the view in the
        /// ComponentDefinition; the key of the inner map identifies the name
        /// of the parameter; the value of the inner map is the value to
        /// assign at the parameter.
        std::map<
                std::string,
                std::map< std::string, Value * > > _params;

        sptr_compdef _definition;
        sptr_name _name;

    };

}
