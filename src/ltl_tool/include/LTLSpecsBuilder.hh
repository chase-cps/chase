/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Chase.hh"
#include "parser/LTLContractsLexer.h"
#include "parser/LTLContractsBaseListener.h"
#include "parser/LTLContractsListener.h"
#include "parser/LTLContractsParser.h"

using namespace chase;

namespace ltl_tool {

    /// @brief Main class of the specification builder for the LTL contracts.
    class LTLSpecsBuilder : public LTLContractsBaseListener {
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

        void
        enterSystemSpec(LTLContractsParser::SystemSpecContext *ctx) override;

        /// @endcond


    protected:

        /// @brief The system being built.
        System * _system;



    };

}
