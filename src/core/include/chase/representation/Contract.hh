/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/ChaseObject.hh"
#include "Chase.hh"
#include "Specification.hh"

#include <list>
#include <map>

namespace chase {

    /// @brief Class to represent contracts.
    class Contract : public ChaseObject {
    public:

        /// @brief List of declarations.
        std::list< Declaration * > declarations;

        /// @brief List of assumptions.
        std::map< semantic_domain, Specification * > assumptions;
        /// @brief List of guarantees.
        std::map< semantic_domain, Specification * > guarantees;

        /// @brief Constructor.
        explicit Contract(std::string name = std::string("contract"));

        /// @brief Destructor.
        ~Contract() override;

        /// @brief Getter of the contract's name.
        /// @return The contract name.
        Name * getName() const;

        /// @brief Setter of the contract's name.
        /// @param name The contract name.
        void setName(Name *name);

        /// @brief Function to safely add a declaration to the contract.
        /// The function takes care of managing the parent link to the contract.
        /// @param declaration A pointer to the declaration to add.
        void addDeclaration( Declaration * declaration );
        /// @brief Function to safely add the assumptions for a given semantic
        /// domain. It takes care of managing the parent link to the contract.
        /// @param domain The semantic domain.
        /// @param spec Pointer to the the specification to add.
        void addAssumptions( semantic_domain domain, Specification * spec );
        /// @brief Function to safely add the guarantees for a given semantic
        /// domain. It takes care of managing the parent link to the contract.
        /// @param domain The semantic domain.
        /// @param spec Pointer to the the specification to add.
        void addGuarantees( semantic_domain domain, Specification * spec );

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function printing the contract into a string.
        /// @return The string representing the contract.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the contract.
        Contract * clone() override;


        // -- Methods for the Contract Algebra.

        /// @brief Method to saturate a contract. Note: the contract is
        /// saturated in loco. No copies are produced.
        /// @param c Contract to saturate.
        static void saturate( Contract * c );

        /// @brief Method implementing the composition of two contracts.
        /// @param C1 The first contract.
        /// @param C2 The second contract.
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed.
        /// @return Pointer to a new contract that is the composition of the
        /// two.
        static Contract * composition(
                Contract * C1, Contract * C2,
                std::map< std::string, std::string >& correspondences );



    protected:

        /// @brief Name of the contract. The default value will be "contract".
        /// It will be useful to identify a contract within a system modeled
        /// by multiple contracts.
        Name * _name;

        static void _saturateTemporalLogic( Contract * c );

    };

}
