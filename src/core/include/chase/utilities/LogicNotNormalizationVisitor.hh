/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/26/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include <chase/representation/LogicFormula.hh>
#include "GuideVisitor.hh"


namespace chase {

    /// @todo Binding for python to be declared for all the methods.
    /// @brief Visitor taking care of visiting the Not logic operations
    /// in order to remove the "double not"s.
    class LogicNotNormalizationVisitor : public GuideVisitor {
    public:
        /// @brief Constructor.
        LogicNotNormalizationVisitor();

        /// @brief Destructor.
        ~LogicNotNormalizationVisitor();


        int visitBinaryBooleanOperation(BinaryBooleanFormula &formula) override;
        int visitUnaryBooleanOperation(UnaryBooleanFormula &formula) override;
        int visitLargeBooleanFormula(LargeBooleanFormula &formula) override;
        int visitUnaryTemporalOperation(UnaryTemporalFormula &formula) override;
        int
        visitBinaryTemporalOperation(BinaryTemporalFormula &formula) override;


    protected:

        LogicFormula * _analyzeFormula(LogicFormula * formula);

    };

}
