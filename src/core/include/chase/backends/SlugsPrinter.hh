/**
 * @author      <a href="mailto:stefano.spellini@univr.it">Stefano Spellini</a>
 * @date        12/06/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation.hh"
#include "GuideVisitor.hh"

#include <fstream>



namespace chase {

    /// @brief Main class for the SLUGS backend.
    class SlugsPrinter : public GuideVisitor {
    public:
        /// @brief Constructor.
        SlugsPrinter();

        /// @brief Destructor.
        ~SlugsPrinter();

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
        /// @brief The current string being built by the visit.
        std::string _curr;

        /// @brief Flag indicating if the visit of the AST reached a section
        /// under a next operator.
        bool _inNext;

        /// @brief Procedure printing the variables declarations.
        void _printDeclarations();

        /// @brief Procedure printing the Initial conditions of the GR1 spec.
        void _printInit();
        /// @brief Procedure printing the Safety conditions of the GR1 spec.
        void _printSafety();
        /// @brief Procedure printing the Liveness conditions of the GR1 spec.
        void _printLiveness();

        /// @brief Function managing the visit when an object is generic.
        /// @param o A pointer to the object to be visited.
        /// @return The return value of the visitor.
        int _continueVisit( ChaseObject *o );

    };

}
