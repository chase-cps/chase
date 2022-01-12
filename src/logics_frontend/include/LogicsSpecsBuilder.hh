/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Chase.hh"
#include "parser/LogicsContractsLexer.h"
#include "parser/LogicsContractsBaseVisitor.h"
#include "parser/LogicsContractsVisitor.h"
#include "parser/LogicsContractsParser.h"

using namespace chase;

namespace chase {

    /// @brief Main class of the specification builder for the LTL contracts.
    class LogicsSpecsBuilder : public LogicsContractsBaseVisitor {
    public:
        /// @brief Constructor.
        LogicsSpecsBuilder();

        /// @brief Destructor.
        ~LogicsSpecsBuilder();

        /// @brief Function to access the System object being built.
        /// @return A pointer to the system built by the parser.
        System * getSystem() const;

        /// @brief Main parsing function.
        /// @param infile The specification file to be parsed.
        /// @return A pointer to the System object constructed by the parser.
        System * parseSpecificationFile( std::string infile );

        /// @cond
        antlrcpp::Any
        visitSystemSpec(LogicsContractsParser::SystemSpecContext *ctx) override;
        antlrcpp::Any
        visitDeclaration(LogicsContractsParser::DeclarationContext *ctx) override;

        antlrcpp::Any
        visitContract(LogicsContractsParser::ContractContext *ctx) override;

        antlrcpp::Any
        visitAssumptions(LogicsContractsParser::AssumptionsContext *ctx) override;

        antlrcpp::Any
        visitGuarantees(LogicsContractsParser::GuaranteesContext *ctx) override;


        /// @endcond

        Expression *
        createRelation(LogicsContractsParser::RelationContext *ctx) ;

        /// @brief Function parsing and building a value.
        /// @param ctx A pointer to a ANTLR4 ValueContext parsed.
        /// @return Pointer to a new CHASE Value object representing the parsed
        /// ValueContext.
        Value * createValue(LogicsContractsParser::ValueContext * ctx);

        /// @brief Function parsing and building a Identified.
        /// @param name A name (string) being parsed.
        /// @return Pointer to a new CHASE Identifier object representing
        /// the parsed name.
        Identifier * createIdentifier(std::string name, bool primed = false);

        LogicFormula *
        createFormula(LogicsContractsParser::FormulaContext * ctx);

        BooleanConstant *
        createLogicConstant(LogicsContractsParser::Logic_constantContext * ctx);

        Proposition * createProposition(std::string name, bool primed = false);

        DataDeclaration * findDeclaration(std::string name);

        Proposition * createPropositionFromRelation(
                LogicsContractsParser::RelationContext * ctx);

        Interval * createInterval(
                LogicsContractsParser::IntervalContext * ctx);

        NumericValue * createNumericValue(std::string s);



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
