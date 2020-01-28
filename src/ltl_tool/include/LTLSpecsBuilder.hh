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

        antlrcpp::Any
        visitContract(LTLContractsParser::ContractContext *ctx) override;

        antlrcpp::Any
        visitAssumptions(LTLContractsParser::AssumptionsContext *ctx) override;

        antlrcpp::Any
        visitGuarantees(LTLContractsParser::GuaranteesContext *ctx) override;


        /// @endcond

        Expression *
        createRelation(LTLContractsParser::RelationContext *ctx) ;

        /// @brief Function parsing and building a value.
        /// @param ctx A pointer to a ANTLR4 ValueContext parsed.
        /// @return Pointer to a new CHASE Value object representing the parsed
        /// ValueContext.
        Value * createValue(LTLContractsParser::ValueContext * ctx);

        /// @brief Function parsing and building a Identified.
        /// @param name A name (string) being parsed.
        /// @return Pointer to a new CHASE Identifier object representing
        /// the parsed name.
        Identifier * createIdentifier(std::string name);

        LogicFormula *
        createFormula(LTLContractsParser::FormulaContext * ctx);

        BooleanConstant *
        createLogicConstant(LTLContractsParser::Logic_constantContext * ctx);

        Proposition * createProposition(std::string name);

        DataDeclaration * findDeclaration(std::string name);

    protected:

        /// @brief The system being built.
        System * _system;
        /// @brief The current contract being parsed. If null, the visit is
        /// traversing the global area of the specification.
        Contract * _currContract;

        LogicFormula * _currFormula;

        /// @brief Map of propositions and values.
        std::map< Variable *, Expression * > _map_props_values;





    };

}
