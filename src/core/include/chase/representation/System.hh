/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "representation/ChaseObject.hh"
#include "representation/Declaration.hh"
#include "representation/Contract.hh"
#include "representation/Component.hh"

#include "chase/utilities.hh"

namespace chase {

    /// @brief Class representing a System composed by multiple contracts.
    class System : public ChaseObject {
    
    using sptr_decl = std::shared_ptr<Declaration>;
    using sptr_contract = std::shared_ptr<Contract>;
    using sptr_comp = std::shared_ptr<Component>;
    using sptr_name = std::shared_ptr<Name>;
    using sptr_sys = std::shared_ptr<System>;

    public:

        /// @brief Constructor.
        System( std::string name = std::string("System") );

        /// @brief Destructor.
        ~System();

        /// @brief Method to safely add a declaration to the system.
        /// It manages the parent link of the declaration.
        /// @param declaration Pointer to the declaration to add.
        void addDeclaration( sptr_decl declaration );

        /// @brief Method to safely add a contract to the system.
        /// It manages the parent link of the contract.
        /// @param contract Pointer to the contract to add.
        void addContract( sptr_contract contract );

        /// @brief Method to safely add a component to the system.
        /// It manages the parent link of the component.
        /// @param component Pointer to the component to add.
        void addComponent( sptr_comp component );

        /// @brief Method to access the declaration set. It should be used
        /// only to access the declaration set for reading it. Use the
        /// safe method addDeclaration for add a new declaration.
        /// @return a reference to the set of declarations.
        std::set< sptr_decl >& getDeclarationsSet();

        /// @brief Method to access the contracts set. It should be used
        /// only to access the contracts set for reading it. Use the
        /// safe method addContract. for add a new contract.
        /// @return a reference to the set of contracts.
        std::set< sptr_contract >& getContractsSet();

        /// @brief Method to access the set of component instances composing
        /// the system. It should be used only to access the components set for
        /// reading it. Use the safe addComponent method for add new components.
        /// @return a reference to the set of components.
        std::set< sptr_comp >& getComponentsSet();

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be applied during the visit.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the system.
        /// @return The name of the system as a string.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the object.
        sptr_sys clone();

        /// @brief Getter of the name of the system.
        /// @return A pointer to the name object.
        sptr_name getName();

        /// @brief Setter of the name of the system.
        /// @return name The pointer to the name object.
        void setName(sptr_name name);

    protected:

        /// Set of global declarations of the system.
        std::set< sptr_decl > _declarations;

        /// Set of contracts describing the system's requirements.
        std::set< sptr_contract > _contracts;


        std::set< sptr_comp > _components;

        /// @brief Name of the system.
        sptr_name _name;

    };

}
