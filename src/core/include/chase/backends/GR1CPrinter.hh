/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"
#include <fstream>

namespace chase {

    /// @brief Main class for the GR1C backend.
    class GR1CPrinter : public GuideVisitor {
    public:
        /// @brief Constructor.
        GR1CPrinter();

        /// @brief Destructor.
        ~GR1CPrinter();

        /// @brief Function printing on the file.
        /// @param contract The contract to be printed.
        /// @param path The path of the output file.
        void print(Contract * contract, std::string path);

        /// @cond

        // Visit functions.
        int visitIntegerValue(IntegerValue &o) override;
        int visitExpression(Expression &o) override;
        int visitIdentifier(Identifier &o) override;
        int visitProposition(Proposition &o) override;
        int visitBooleanConstant(BooleanConstant &o) override;
        int visitBinaryBooleanOperation(BinaryBooleanFormula &o) override;
        int visitUnaryBooleanOperation(UnaryBooleanFormula &o) override;
        int visitUnaryTemporalOperation(UnaryTemporalFormula &o) override;
        int visitBinaryTemporalOperation(BinaryTemporalFormula &o) override;

        /// @endcond


    protected:

        /// @brief The contract to be printed.
        Contract * _contract;
        /// @brief The file to print.
        std::ofstream fout;

        std::string _curr;

        bool _inNext;

        /// @brief Procedure printing the variables declarations.
        void _printDeclarations();

        void _printInit();
        void _printSafety();
        void _printLiveness();

        int _continueVisit( ChaseObject *o );

    };

}
