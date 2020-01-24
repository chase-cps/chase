/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Chase.hh"
#include "parser/LTLContractsLexer.h"
#include "parser/LTLContractsBaseVisitor.h"
#include "parser/LTLContractsVisitor.h"
#include "parser/LTLContractsParser.h"

using namespace chase;

namespace ltl_tool {

    /// @brief Main class of the specification builder for the LTL contracts.
    class LTLSpecsBuilder : public LTLContractsBaseVisitor {
    public:
        /// @brief Constructor.
        LTLSpecsBuilder();

        /// @brief Destructor.
        ~LTLSpecsBuilder();

        /// @brief Function to access the System object being built.
        /// @return A pointer to the system built by the parser.
        System * getSystem() const;

        /// @brief Main parsing function.
        /// @param infile The specification file to be parsed.
        /// @return A pointer to the System object constructed by the parser.
        System * parseSpecificationFile( std::string infile );

        /// @cond
        antlrcpp::Any
        visitSystemSpec(LTLContractsParser::SystemSpecContext *ctx) override;
        antlrcpp::Any
        visitDeclaration(LTLContractsParser::DeclarationContext *ctx) override;

        Expression *
        buildRelation(LTLContractsParser::RelationContext *ctx) ;

        /// @endcond


    protected:

        /// @brief The system being built.
        System * _system;
        /// @brief The current contract being parsed. If null, the visit is
        /// traversing the global area of the specification.
        Contract * _currContract;

        /// @brief Map of propositions and values.
        std::map< Variable *, Expression * > _map_props_values;





    };

}
