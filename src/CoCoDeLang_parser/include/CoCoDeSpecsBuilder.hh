/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/10/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"

#include "parser/CoCoDeLangLexer.h"
#include "parser/CoCoDeLangBaseVisitor.h"
#include "parser/CoCoDeLangVisitor.h"
#include "parser/CoCoDeLangParser.h"

namespace chase {

    /// @brief Main class of the specification builder for the CHASE Contracts
    /// and Components specification language.
    class CoCoDeSpecsBuilder : public CoCoDeLangBaseVisitor {
    public:
        /// @brief Constructor.
        CoCoDeSpecsBuilder();

        /// @brief Destructor.
        ~CoCoDeSpecsBuilder() override;

        /// @brief Main parsing function.
        /// @param infile The specification file to be parsed.
        /// @return A pointer to the System object constructed by the parser.
        System * parseSpecificationFile( std::string infile );

        /// @cond
        //------------------- Declarations.
        antlrcpp::Any visitConstant(
                CoCoDeLangParser::ConstantContext *ctx) override;
        antlrcpp::Any visitVariable(
                CoCoDeLangParser::VariableContext *ctx) override;
        antlrcpp::Any visitDistribution(
                CoCoDeLangParser::DistributionContext *ctx) override;

        //------------------- Design Problem
        antlrcpp::Any visitDesign(CoCoDeLangParser::DesignContext *ctx) override;
        /// @endcond

        static
        chase::Type * analyzeBaseType(CoCoDeLangParser::BasetypeContext *ctx);

    protected:

    };

}
