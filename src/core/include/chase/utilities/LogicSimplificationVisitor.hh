/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/27/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "GuideVisitor.hh"
#include "representation/LogicFormula.hh"

namespace chase {

    /// @brief Interface for the major Logic simplification. It provides
    /// the visit logic for visiting logic formulas.
    class LogicSimplificationVisitor : public GuideVisitor {
    public:
        /// @brief Constructor.
        LogicSimplificationVisitor();

        /// @brief Destructor.
        ~LogicSimplificationVisitor();

        /// @cond
        int visitBinaryBooleanOperation(BinaryBooleanFormula &formula) override;
        int visitUnaryBooleanOperation(UnaryBooleanFormula &formula) override;
        int visitLargeBooleanFormula(LargeBooleanFormula &formula) override;
        int visitUnaryTemporalOperation(UnaryTemporalFormula &formula) override;
        int
        visitBinaryTemporalOperation(BinaryTemporalFormula &formula) override;
        /// @endcond

    protected:

        /// @brief Interface for the main optimization function.
        /// @param formula The formula to analyze and, eventually, simplify.
        /// @return Pointer to the simplified formula.
        virtual LogicFormula * _analyzeFormula(LogicFormula * formula) = 0;

    };

}
