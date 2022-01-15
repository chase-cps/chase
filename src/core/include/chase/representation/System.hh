/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "representation/Scope.hh"
#include "representation/Contract.hh"
#include "representation/Component.hh"

#include "chase/utilities.hh"

namespace chase {

    /// @brief Class representing a System composed by multiple contracts.
    class System : public Scope {
    public:

        /// @brief Constructor.
        explicit System( std::string name = std::string("System") );

        /// @brief Destructor.
        ~System() override;

        /// @brief Method to safely add a declaration to the system.
        /// It manages the parent link of the declaration.
        /// @param declaration Pointer to the declaration to add.
        void addDeclaration( Declaration * declaration ) override;

        /// @brief Method to safely add a contract to the system.
        /// It manages the parent link of the contract.
        /// @param contract Pointer to the contract to add.
        void addContract( Contract * contract );

        /// @brief Method to safely add a component to the system.
        /// It manages the parent link of the component.
        /// @param component Pointer to the component to add.
        void addComponent( Component * component );

        /// @brief Method to access the declaration set. It should be used
        /// only to access the declaration set for reading it. Use the
        /// safe method addDeclaration for add a new declaration.
        /// @return a reference to the set of declarations.
        std::list< Declaration * >& getDeclarationsSet();

        /// @brief Method to access the contracts set. It should be used
        /// only to access the contracts set for reading it. Use the
        /// safe method addContract. for add a new contract.
        /// @return a reference to the set of contracts.
        std::set< Contract * >& getContractsSet();

        /// @brief Method to access the set of component instances composing
        /// the system. It should be used only to access the components set for
        /// reading it. Use the safe addComponent method for add new components.
        /// @return a reference to the set of components.
        std::set< Component * >& getComponentsSet();

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be applied during the visit.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the system.
        /// @return The name of the system as a string.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the object.
        System * clone() override;



    protected:

        /// Set of contracts describing the system's requirements.
        std::set< Contract * > _contracts;
        /// Set of components of the system.
        std::set< Component * > _components;


    };

}
