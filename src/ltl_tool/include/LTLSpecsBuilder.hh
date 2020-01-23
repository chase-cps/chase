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


namespace ltl_tool {

    /// @brief Main class of the specification builder for the LTL contracts.
    class LTLSpecsBuilder : public LTLContractsListener {
    public:
        /// @brief Constructor.
        LTLSpecsBuilder();

        /// @brief Destructor.
        ~LTLSpecsBuilder();



    protected:

        /// @brief The system being built.
        System * _system;



    };

}
