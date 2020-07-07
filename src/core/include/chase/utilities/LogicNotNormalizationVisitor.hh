/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include <chase/representation/LogicFormula.hh>
#include "utilities/LogicSimplificationVisitor.hh"



namespace chase {

    /// @todo Binding for python to be declared for all the methods.
    /// @brief Visitor taking care of visiting the Not logic operations.
    /// It removes the "double not"s, and it moves not operators
    /// as close as possible to atoms.
    class LogicNotNormalizationVisitor : public LogicSimplificationVisitor {
    public:
        /// @brief Constructor.
        LogicNotNormalizationVisitor();

        /// @brief Destructor.
        ~LogicNotNormalizationVisitor();

    protected:

        /// @brief Interface for the main optimization function.
        /// @param formula The formula to analyze and, eventually, simplify.
        /// @return Pointer to the simplified formula.
        LogicFormula * _analyzeFormula(LogicFormula * formula);

    };

}
