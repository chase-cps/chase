/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation.hh"
#include "GuideVisitor.hh"

namespace chase {

    /// @brief Visitor fixing the declarations after cloning a contract.
    class ClonedDeclarationVisitor : public GuideVisitor {

    public:
        /// @brief Explicit constructor.
        /// @param m Reference to the declaration map.
        explicit ClonedDeclarationVisitor(
                std::map< Declaration *, Declaration * > &m );

        /// @brief Function visiting the identifiers.
        /// @param o The identifier to be visited.
        int visitIdentifier(Identifier &o) override;

    protected:

        /// @brief Declaration map. Generated when cloning a contract. For each
        /// entry: the key is a pointer to the original declaration, the value
        /// is a pointer to the cloned declaration.
        std::map< Declaration *, Declaration * > * _map;
    };

}