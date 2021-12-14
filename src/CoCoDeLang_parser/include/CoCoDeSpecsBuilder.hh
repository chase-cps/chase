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

    protected:

    };

}
